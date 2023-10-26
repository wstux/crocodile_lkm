#!/bin/bash

DEVICE="croc"
DEST_DEVICE="/dev/$DEVICE"
MOD_DIR="build_release/drivers/croc"

function create_nodes
{
    local major_ver=$1
    local rc=$( mknod ${DEST_DEVICE} c $major_ver 0 )
    if [[ "$rc" -eq "0" ]]; then
        chmod o+w ${DEST_DEVICE}
        chmod o+r ${DEST_DEVICE}
    fi
    echo $rc
}

function major_version
{
    local major_ver=$( awk "\$2==\"$DEVICE\" {print \$1}" /proc/devices )
    echo $major_ver
}

function remove_nodes
{
    rm -f $DEST_DEVICE
}

function load_device
{
	local dev_path="$MOD_DIR/$DEVICE.ko"

    local rc=$( insmod $dev_path )
    if [[ "$rc" -eq "0" ]]; then
        local major_ver=$( major_version )
        echo "Major version '$major_ver'"

        remove_nodes
        rc=$( create_nodes $major_ver )
        if [[ "$rc" -ne "0" ]]; then
            echo "Failed to create device node"
            rmmod $DEVICE
            exit 1
        fi
    else
        echo "Failed to install $DEVICE"
        exit 1
    fi
}

function unload_device () {
    rmmod $DEVICE
    remove_nodes $FILES
}

case "$1" in
  --load|-l)
     echo "Loading $DEVICE"
     load_device
     ;;
  --unload|-u)
     echo "Unloading $DEVICE"
     unload_device
     ;;
  --reload|-r)
     echo "Reloading $DEVICE"
     unload_device
     load_device
     ;;
  *)
     echo "Usage: $0 {load|unload|reload}"
     exit 1
esac

exit 0
