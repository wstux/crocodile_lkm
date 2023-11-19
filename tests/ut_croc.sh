#!/bin/bash

##########################################################################
# Includes                                                               #
##########################################################################

__test_dir="$( dirname "$( realpath "$0" )" )"

test -f "${__test_dir}/test_fixture.sh" || { echo "Subscript '${__test_dir}/test_fixture.sh' does not exist"; exit 1; }
source "${__test_dir}/test_fixture.sh"

##########################################################################
# Variables declaration                                                  #
##########################################################################

##########################################################################
# Private functions                                                      #
##########################################################################

function change_log_level
{
    local log_level="$1"

    cat << EOF > "$( get_device )"
IOC_LOG_${log_level}
EOF
}

function read_log_level
{
    local proc_fs_file="/proc/$( get_device_name )"
    local lvl="$( cat "${proc_fs_file}" | grep "Log level" | awk '{ print $3 }' )"
    echo "${lvl}"
}

##########################################################################
# Public functions                                                       #
##########################################################################

function croc__proc_fs
{
    local proc_fs_file="/proc/$( get_device_name )"

    local etalon="Log level: 4
Count of hidden processes: 0
There are not hidden processes"

    load_device

    # read from proc fs
    local read_data="$( cat "${proc_fs_file}" )"

    # check data
    expect_qe "croc__proc_fs" etalon read_data

    unload_device
}

function croc__log_level
{
    local etalon="4"

    load_device

    # read from proc fs
    local log_level="$( read_log_level )"

    # check data
    expect_qe "croc__log_level::default" etalon log_level

    # Check debug log level
    change_log_level "DEBUG"
    etalon="7"
    log_level="$( read_log_level )"
    expect_qe "croc__log_level::debug" etalon log_level

    # Check critical log level
    change_log_level "CRIT"
    etalon="2"
    log_level="$( read_log_level )"
    expect_qe "croc__log_level::crit" etalon log_level

    unload_device
}

##########################################################################
# Main                                                                   #
##########################################################################

croc__proc_fs
croc__log_level

