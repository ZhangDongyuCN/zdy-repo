# -*- coding: UTF-8 -*-
# python 3.9.0 64bit

from functions import *

if '__main__' == __name__:
    dir = input('Please input directory path: ')
    format = input(r'Please input new file name format: (input 1 to use default format: IMG_%Y%m%d_%H%M%S, e.g. IMG_20102029_202550): ')
    mode = input('Rename file by modify time or create time (1: modiry time, 2: create time): ')

    if '1' == format:
        format = r'IMG_%Y%m%d_%H%M%S'

    if '1' == mode:
        renameFileByModifyTime(dir, format)
    elif '2' == mode:
        renameFileByCreateTime(dir, format)
    else:
        print('Input error!')

    print('Done!')
    input('Press any key to exit!')
