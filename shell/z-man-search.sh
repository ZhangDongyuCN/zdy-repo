#!/bin/bash
function z-man-search() {
    if test $# -eq 2; then
        displayItem=1
    elif test $# -eq 3; then
        displayItem=$3
    else
        echo "z-man-search error: usage: z-man-search <cmd> <key_words> [display_item]"
        return 1
    fi
    currPath=$(pwd)
    cd ~
    fileName='.man-search-temp'
    man $1 >$fileName
    cat $fileName | grep -A5 -w -m$displayItem -e " \{4,\}$2\| $2"
    rm -rf $fileName
    cd $currPath
    return 0
}
