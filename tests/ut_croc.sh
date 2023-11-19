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

function hide_pid
{
    local pid="$1"
    cat << EOF > "$( get_device )"
IOC_PID_HIDE_${pid}
EOF
}

function is_process_running
{
    local pid="$1"
    pid="[${pid:0:1}]${pid:1}" # pid => [p]id
    if ps ax | grep "${pid}" 2>&1 > /dev/null; then
        echo "true"  # process is running
    else
        echo "false" # process is not running
    fi
}

function run_process
{
    local cmd="$1"
    $cmd > /dev/null & #watch ls -l > /dev/null &
    echo "$!" # return pid
}

function read_log_level
{
    local proc_fs_file="/proc/$( get_device_name )"
    local lvl="$( cat "${proc_fs_file}" | grep "Log level" | awk '{ print $3 }' )"
    echo "${lvl}"
}

function show_pid
{
    local pid="$1"
    cat << EOF > "$( get_device )"
IOC_PID_SHOW_${pid}
EOF
}

function stop_process
{
    local pid="$1"
    local is_running="$( is_process_running "${pid}" )"

    if [[ "${is_running}" == "true" ]]; then
        kill -9 "${pid}"
    fi
}

##########################################################################
# Public functions                                                       #
##########################################################################

function croc__proc_fs
{
    local proc_fs_file="/proc/$( get_device_name )"

    local etalon="Log level: 6
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
    local etalon="6"

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

function croc__hide_proc
{
    local pid="$( run_process "sleep 3600" )"

    local etalon="Log level: 6
Count of hidden processes: 1
Hidden pids:
  -- ${pid};"

    load_device

    # check that process exists
    local is_proc_exist="$( is_process_running "${pid}" )"
    expect_true "croc__hide_proc::running_process" is_proc_exist

    if [[ "${is_proc_exist}" == "true" ]]; then
        # hide process
        hide_pid "${pid}"
        is_proc_exist="$( is_process_running "${pid}" )"
        expect_false "croc__hide_proc::hidden_process" is_proc_exist

        # read from proc fs
        local read_data="$( cat "/proc/$( get_device_name )" )"
        expect_qe "croc__hide_proc::pid_in_proc_fs" etalon read_data

        # show process
        show_pid "${pid}"
        is_proc_exist="$( is_process_running "${pid}" )"
        expect_true "croc__hide_proc::showed_process" is_proc_exist
        if [[ "${is_proc_exist}" == "true" ]]; then
            stop_process "${pid}"
        fi
    fi

    unload_device
}

##########################################################################
# Main                                                                   #
##########################################################################

croc__proc_fs
croc__log_level
croc__hide_proc

exit $( rc )

