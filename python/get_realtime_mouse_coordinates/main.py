# -*- coding: UTF-8 -*-
# python 3.9.0 64bit

import time
import pyautogui


def getRealtimeMouseCoordinates():
    try:
        xOld = 0
        yOld = 0
        while True:
            xNew, yNew = pyautogui.position()
            if xOld != xNew and yOld != yNew:
                xOld = xNew
                yOld = yNew
                screenshot = pyautogui.screenshot()
                color = screenshot.getpixel((xNew, yNew))
                print('X:', '{:>4}'.format(xNew), ', Y:', '{:>4}'.format(yNew), ', RGB:',
                      '({:>3}, {:>3}, {:>3})'.format(color[0], color[1], color[2]))
            time.sleep(0.1)
    except KeyboardInterrupt:
        print('Exit')


if __name__ == '__main__':
    getRealtimeMouseCoordinates()
