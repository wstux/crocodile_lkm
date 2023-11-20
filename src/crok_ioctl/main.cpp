/*
 * crok_lkm
 * Copyright (C) 2023  Chistyakov Alexander
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <iostream>

#include "progopts/progopts.h"

#define MODULE_NAME         "crok"
#define DEVICE_PATH         "/dev/" MODULE_NAME
#define CROK_IOC_HIDE_PID   0x40046B05
#define CROK_IOC_SHOW_PID   0x40046B06

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
        cmd = CROK_IOC_HIDE_PID;
        arg = po.value<int>("--pid");
    } else if (po.value<bool>("--show") && po.has_value("--pid")) {
        cmd = CROK_IOC_SHOW_PID;
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

