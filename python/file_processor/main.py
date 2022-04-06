# -*- coding: UTF-8 -*-

import time

import encode_file
import modify_ext
import rename_file
from logger import init_log, logger_info

if "__main__" == __name__:
    # 初始化日志
    init_log(log_path="./file_processor.log")

    # 任务处理
    try:
        while True:
            msg = """
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
功能：文件处理器

运行模式：
    ****************************** 重命名 ******************************

    1.1 --> 更改文件后缀

    1.2 --> 文件后缀改为小写

    1.3 --> 文件后缀改为大写

    1.4 --> 驼峰文件名转下划线

    1.5 --> 下划线文件名转驼峰

    1.6 --> 根据文件修改时间重命名文件

    1.7 --> 根据文件创建时间重命名文件（仅限Windows）

    1.8 --> 根据正则规则重命名文件

    1.9 --> 根据照片拍摄时间重命名照片

    1.10 --> 针对目录下（包含子目录）下的微信照片进行名称规范化处理
    
    1.11 --> 针对目录下（包含子目录）下的美图秀秀照片进行名称规范化处理
    
    1.12 --> 针对目录下（包含子目录）下的百度网盘照片进行名称规范化处理

    ****************************** 编解码 ******************************

    2.1 --> base64方式编码单个文件

    2.2 --> base64方式解码单个文件

    2.3 --> 分割txt文件

    2.4 --> 合并txt文件
                    """

            print(msg)

            run_mode = input("\n输入运行模式: \n")

            if "1.1" == run_mode:
                dir = input("请输入文件所在目录: \n")
                ext = input("请输入新后缀（例如：.txt）: \n")
                logger_info(f"运行模式：{run_mode}，目录：{dir}，新后缀：{ext}")
                modify_ext.change_ext(dir, ext)

            elif "1.2" == run_mode:
                dir = input("请输入文件所在目录: \n")
                logger_info(f"运行模式：{run_mode}，目录：{dir}")
                modify_ext.to_lower_case_ext(dir)

            elif "1.3" == run_mode:
                dir = input("请输入文件所在目录: \n")
                logger_info(f"运行模式：{run_mode}，目录：{dir}")
                modify_ext.to_upper_case_ext(dir)

            elif "1.4" == run_mode:
                dir = input("请输入文件所在目录: \n")
                logger_info(f"运行模式：{run_mode}，目录：{dir}")
                rename_file.hump_2_underline_for_dir(dir)

            elif "1.5" == run_mode:
                dir = input("请输入文件所在目录: \n")
                logger_info(f"运行模式：{run_mode}，目录：{dir}")
                rename_file.underline_2_hump_for_dir(dir)

            elif "1.6" == run_mode:
                dir = input("请输入文件所在目录: \n")
                format = input("请输入命名格式: (1：默认格式：%Y%m%d_%H%M%S, e.g. 20102029_202550.txt): \n")
                if "1" == format:
                    format = "%Y%m%d_%H%M%S"
                logger_info(f"运行模式：{run_mode}，目录：{dir}，命名格式：{format}")
                rename_file.rename_file_by_modify_time(dir, format)

            elif "1.7" == run_mode:
                dir = input("请输入文件所在目录: \n")
                format = input("请输入命名格式: (1：默认格式：%Y%m%d_%H%M%S, e.g. 20102029_202550.txt): \n")
                if "1" == format:
                    format = "%Y%m%d_%H%M%S"
                logger_info(f"运行模式：{run_mode}，目录：{dir}，命名格式：{format}")
                rename_file.rename_file_by_create_time(dir, format)

            elif "1.8" == run_mode:
                dir = input("请输入文件所在目录: \n")
                pattern = input("请输入匹配规则: \n")
                repl = input("请输入替换规则（用\g<1>反向引用第一个匹配组）: \n")
                logger_info(f"运行模式：{run_mode}，目录：{dir}，匹配规则：{pattern}，替换规则：{repl}")
                rename_file.rename_file_by_regex(dir, pattern, repl)

            elif "1.9" == run_mode:
                dir = input("请输入照片所在目录: \n")
                logger_info(f"运行模式：{run_mode}，目录：{dir}")
                rename_file.rename_pic_by_shooting_time(dir)

            elif "1.10" == run_mode:
                dir = input("请输入照片所在目录: \n")
                logger_info(f"运行模式：{run_mode}，目录：{dir}")
                rename_file.rename_pic_with_weixin_format(dir)

            elif "1.11" == run_mode:
                dir = input("请输入照片所在目录: \n")
                logger_info(f"运行模式：{run_mode}，目录：{dir}")
                rename_file.rename_pic_with_meituxiuxiu_format(dir)

            elif "1.12" == run_mode:
                dir = input("请输入照片所在目录: \n")
                logger_info(f"运行模式：{run_mode}，目录：{dir}")
                rename_file.rename_pic_with_baiduwangpan_format(dir)

            elif "2.1" == run_mode:
                file = input("请输入要编码的文件路径（e.g. D:\\xx.zip）：\n")
                txt = input("请输入编码文件的保存路径（e.g. D:\\xx.txt）：\n")
                logger_info(f"运行模式：{run_mode}，文件路径：{file}，保存路径：{txt}")
                encode_file.encode_file(file, txt)

            elif "2.2" == run_mode:
                txt = input("请输入编码文件的路径（e.g. D:\\xx.txt）：\n")
                file = input("请输入解码后的文件保存路径（e.g. D:\\xx.zip）：\n")
                logger_info(f"运行模式：{run_mode}，文件路径：{txt}，保存路径：{file}")
                encode_file.decode_file(txt, file)

            elif "2.3" == run_mode:
                txt = input("请输入要分割的txt文件路径（e.g. D:\\xx.txt）：\n")
                each_file_mb = input("请输入分割大小（单位：MB，e.g. 1）：\n")
                save_dir = input("请输入分割后的txt文件保存目录（e.g. D:\\dir）：\n")
                logger_info(f"运行模式：{run_mode}，文件路径: {txt}，分割大小：{each_file_mb}，保存目录：{save_dir}")
                encode_file.split_txt_file(txt, int(each_file_mb), save_dir)

            elif "2.4" == run_mode:
                txt_dir = input("请输入要合并的txt文件目录（e.g. D:\\dir）：\n")
                txt = input("请输入合并后的txt文件保存路径（e.g. D:\\xx.txt）：\n")
                logger_info(f"运行模式：{run_mode}，文件目录：{txt_dir}，保存路径：{txt}")
                encode_file.merge_txt_file(txt_dir, txt)

            else:
                logger_info(f"运行模式：{run_mode}，运行模式输入错误，请等待2秒后重新输入！")
                time.sleep(2)
                print("\n\n")
                continue

            logger_info("任务处理完毕，等待2秒后进入下一次循环！")
            time.sleep(2)
            print("\n\n")
    except Exception as e:
        logger_info(f"捕获到异常，代码内部逻辑错误，请及时修复！异常信息: {e}")
