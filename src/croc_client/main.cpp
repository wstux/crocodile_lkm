/*
 * The MIT License
 *
 * Copyright 2023 Chistyakov Alexander.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <iostream>

#include "progopts/progopts.h"

#define MODULE_NAME         "croc"
#define DEVICE_PATH         "/dev/" MODULE_NAME
#define CROC_IOC_HIDE_PID   0x40046B05
#define CROC_IOC_SHOW_PID   0x40046B06

int main(int argc, char* argv[])
{
    wstux::po::prog_opts po;
    po.insert<int>("-p,--pid", "Perform an action on the specified process.");
    po.insert("-d,--hide", false, "Hide process or module.");
    po.insert("-s,--show", false, "Show process or module.");
    po.insert("-h,--help", false, "Print this message.");

    if (! po.parse(argc, argv)) {
        std::cerr << po.error_msg() << std::endl;
        std::cout << po.usage() << std::endl;
        return EXIT_FAILURE;
    }

    if (po.value<bool>("--help")) {
        std::cout << po.usage() << std::endl;
        return EXIT_SUCCESS;
    }

    unsigned int cmd;
    unsigned long arg;
    if (po.value<bool>("--hide") && po.has_value("--pid")) {
        cmd = CROC_IOC_HIDE_PID;
        arg = po.value<int>("--pid");
    } else if (po.value<bool>("--show") && po.has_value("--pid")) {
        cmd = CROC_IOC_SHOW_PID;
        arg = po.value<int>("--pid");
    } else {
        std::cerr << "Invalid input parameters." << std::endl;
        std::cout << po.usage() << std::endl;
        return EXIT_FAILURE;
    }

    const int fd = ::open(DEVICE_PATH, O_RDWR);
    if (fd == -1) {
        std::cerr << "Could not open '" DEVICE_PATH "'. Reason: '" << ::strerror(errno) << "'." << std::endl;
        return EXIT_FAILURE;
    }

    int rc = EXIT_SUCCESS;
    if (::ioctl(fd, cmd, arg) < 0) {
        std::cerr << "Failed to perform command. Reason: '" << ::strerror(errno) << "'." << std::endl;
        rc = EXIT_FAILURE;
    }
    ::close(fd);
    return rc;
}

