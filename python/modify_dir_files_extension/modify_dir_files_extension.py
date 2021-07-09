# -*- coding: UTF-8 -*-
# python 3.9.0 64bit

import os


def changeFileExt(dir, newExt):
    for root, dirs, files in os.walk(dir):
        for name in files:
            baseName, ext = os.path.splitext(name)
            oldName = os.path.join(root, name)
            newName = os.path.join(root, baseName + newExt)
            try:
                os.rename(oldName, newName)
                print('{} --> {}'.format(os.path.basename(oldName), os.path.basename(newName)))
            except Exception as e:
                pass


def toLowerCaseExt(dir):
    for root, dirs, files in os.walk(dir):
        for name in files:
            baseName, ext = os.path.splitext(name)
            ext = ext.lower()
            oldName = os.path.join(root, name)
            newName = os.path.join(root, baseName + ext)
            try:
                os.rename(oldName, newName)
                print('{} --> {}'.format(os.path.basename(oldName), os.path.basename(newName)))
            except Exception as e:
                pass


def toUpperCaseExt(dir):
    for root, dirs, files in os.walk(dir):
        for name in files:
            baseName, ext = os.path.splitext(name)
            ext = ext.upper()
            oldName = os.path.join(root, name)
            newName = os.path.join(root, baseName + ext)
            try:
                os.rename(oldName, newName)
                print('{} --> {}'.format(os.path.basename(oldName), os.path.basename(newName)))
            except Exception as e:
                pass


if '__main__' == __name__:
    runMode = \
'''
Run mode:
    1 --> change ext to a new one, e.g. .jpg --> .png
    2 --> change ext to lower case, e.g. .JPG --> .jpg
    3 --> change ext to upper case, e.g. .jpg --> .JPG
'''
    print(runMode)
    mode = input('Please input run mode: ')

    if '1' == mode:
        dir = input('Please input the file directory path which you want to change extension: ')
        ext = input('Please input the new extension (for example: .txt): ')
        changeFileExt(dir, ext)
    elif '2' == mode:
        dir = input('Please input the file directory path which you want to change extension: ')
        toLowerCaseExt(dir)
    elif '3' == mode:
        dir = input('Please input the file directory path which you want to change extension: ')
        toUpperCaseExt(dir)
    else:
        print('Input error!')

    print('Done!')
    input('Press any key to exit!')
