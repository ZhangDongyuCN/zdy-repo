# -*- coding: UTF-8 -*-
# python 3.9.0 64bit

import os


def hump2Underline(text):
    res = []
    for index, char in enumerate(text):
        if char.isupper() and index != 0:
            res.append("_")
        res.append(char)
    return ''.join(res).lower()


def underline2Hump(text):
    arr = text.lower().split('_')
    res = []
    for i in arr:
        res.append(i[0].upper() + i[1:])
    return ''.join(res)


def hump2UnderlineDir(dir):
    for root, dirs, files in os.walk(dir):
        for file in files:
            baseName, ext = os.path.splitext(file)
            oldName = os.path.join(root, file)
            newName = os.path.join(root, hump2Underline(baseName) + ext)
            os.rename(oldName, newName)
        for dir in dirs:
            oldName = os.path.join(root, dir)
            newName = os.path.join(root, hump2Underline(dir))
            os.rename(oldName, newName)


def underline2HumpDir(dir):
    for root, dirs, files in os.walk(dir):
        for file in files:
            baseName, ext = os.path.splitext(file)
            oldName = os.path.join(root, file)
            newName = os.path.join(root, underline2Hump(baseName) + ext)
            os.rename(oldName, newName)
        for dir in dirs:
            oldName = os.path.join(root, dir)
            newName = os.path.join(root, underline2Hump(dir))
            os.rename(oldName, newName)


if '__main__' == __name__:
    runMode = \
'''
Run mode:
    1 --> hump to underline
    2 --> underline to hump
'''
    print(runMode)
    mode = input('Please input run mode: ')
    if int(mode) not in range(1, 3, 1):
        print('Input error!')

    dir = input('Please input the file directory path which you want to change: ')
    if '1' == mode:
        hump2UnderlineDir(dir)
    elif '2' == mode:
        underline2HumpDir(dir)

    print('Done!')
    input('Press any key to exit!')
