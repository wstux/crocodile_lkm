#!/bin/bash

##########################################################################
# Variables declaration                                                  #
##########################################################################

__device_name="croc"
__device="/dev/${__device_name}"
__repository_root_dir="$( realpath "$( dirname "$( realpath "$0" )" )/.." )"
__loader_script="${__repository_root_dir}/croc_load.sh"

##########################################################################
# Logging                                                                #
##########################################################################

declare -A __severity_levels=([TRACE]=0 [DEBUG]=1 [INFO]=2 [WARN]=3 [ERROR]=4)
__severity_level="WARN"

function log_level { echo "${__severity_level}"; }
function logging_set_severity_level { if [[ ${__severity_levels[${1}]} ]]; then __severity_level="${1}"; fi; }

function log
{
    local log_lvl=$1
    local log_msg=$2

    # Check if level exists.
    if [[ ! ${__severity_levels[${log_lvl}]} ]]; then return; fi
    # Check if level is enough.
    if (( ${__severity_levels[${log_lvl}]} < ${__severity_levels[${__severity_level}]} )); then
        return
    fi

    echo "[${log_lvl}] ${log_msg}"
}

function log_trace { log "TRACE" "$1"; }
function log_debug { log "DEBUG" "$1"; }
function log_info  { log "INFO"  "$1"; }
function log_warn  { log "WARN"  "$1"; }
function log_error { log "ERROR" "$1"; }

##########################################################################
# Public functions                                                      #
##########################################################################

function get_device { echo "${__device}"; }
function get_device_name { echo "${__device_name}"; }
function get_repository_root_dir { echo "${__repository_root_dir}"; }

##########################################################################
# Constructor                                                            #
##########################################################################

function load_device
{
    local loader_path="${__loader_script}"

    sudo /bin/bash "${loader_path}" -l --loglevel "$( log_level )"
    local rc="$?"
    if [[ "$rc" != "0" ]]; then
        log_error "Failed to load device '"${DEVICE_NAME}"'"
        exit 1
    fi
}

##########################################################################
# Destructor                                                             #
##########################################################################

function unload_device
{
    local loader_path="${__loader_script}"

    sudo /bin/bash "${loader_path}" -u --loglevel "$( log_level )"
    local rc="$?"
    if [[ "$rc" != "0" ]]; then
        log_error "Failed to unload device '"${DEVICE_NAME}"'"
        exit 1
    fi
}

##########################################################################
# Tests interface                                                        #
##########################################################################

function expect_qe
{
    local prefix="$1"
    local arg1="$2"
    local arg2="$3"

    if [[ "${!arg1}" == "${!arg2}" ]]; then
        echo "[ OK ] ${prefix}"
    else
        echo "[FAIL] ${prefix}: ${arg1} ('${!arg1}') != ${arg2} ('${!arg2}')"
    fi
}

