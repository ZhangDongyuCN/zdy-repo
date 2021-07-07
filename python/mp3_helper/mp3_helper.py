# -*- coding: UTF-8 -*-

import os
import time

if __name__ == '__main__':
    dir_path = input('输入音频目录：\n')
    mp3_files = os.listdir(dir_path)
    mp3_files.sort()

    cur_time = time.time()
    for mp3_file in mp3_files:
        mp3_file = os.path.join(dir_path, mp3_file)
        stinfo = os.stat(mp3_file)
        os.utime(mp3_file, (stinfo.st_atime, cur_time))
        cur_time += 2

    input('已完成，输入任意键退出！')
