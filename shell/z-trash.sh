#!/bin/bash
function z-trash() {
    # 判断参数是否为空
    if [ ! $1 ]; then
        echo "z-trash error: file name of dir name not input: z-trash usage: z-trash <file_or_dir_name> ..."
    else
        sourceDir=$(pwd)
        # 判断回收站是否存在
        trashDir="/root/.trash"
        if [ ! -d $trashDir ]; then
            mkdir $trashDir
        fi
        # 移动文件
        mv -f -t $trashDir $* >/dev/null 2>&1
        # 在文件前边加上时间前缀
        cd ${trashDir}
        currentTime=$(date +%Y%m%d-%H%M%S)
        for value in $*; do
            newName="${currentTime}-${value}"
            mv $value $newName >/dev/null 2>&1
        done
        cd $sourceDir
    fi
}
