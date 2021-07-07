#!/bin/bash
function z-apt-uninstall() {
    if [ ! $1 ]; then
        echo "z-apt-uninstall error: software name not input: z-apt-uninstall usage: z-apt-uninstall <software_name> ..."
    else
        sudo apt purge $*
        sudo apt autoremove
    fi
}
