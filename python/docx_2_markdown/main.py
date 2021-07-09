# -*- coding: UTF-8 -*-

import my_style
import stand_word_style

if '__main__' == __name__:
    msg = '''
功能：word和markdown互转

运行模式：
    1 -> my_style -> docx 2 markdown
    
    2 -> stand_word_style -> docx 2 markdown
    
    3 -> my_style -> markdown 2 docx
    '''

    print(msg)

    run_mode = input('输入运行模式：\n')

    if '1' == run_mode:
        my_style.docx_2_markdown.enter_fun()
    elif '2' == run_mode:
        stand_word_style.docx_2_markdown.enter_fun()
    elif '3' == run_mode:
        my_style.markdown_2_docx.enter_fun()
    else:
        print('运行模式输入错误，请退出程序，并重新运行')

    input('Press any key to exit!')
