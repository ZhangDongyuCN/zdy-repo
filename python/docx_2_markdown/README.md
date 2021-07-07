# 简介

**功能：**把word文档转为markdown文档

**环境：**

- python == 3.8.5
- python-docx == 0.8.10

**参考资料：**
- [python-docx官方文档](https://python-docx.readthedocs.io/en/latest/index.html)
- [Python顺序读取word文档中的文本与表格](https://blog.csdn.net/qq_39600166/article/details/101537368)
- [kmrambo/Python-docx-Reading-paragraphs-tables-and-images-in-document-order-](https://github.com/kmrambo/Python-docx-Reading-paragraphs-tables-and-images-in-document-order-)
- [Python使用标准库zipfile+re提取docx文档中超链接文本和链接地址](https://cloud.tencent.com/developer/article/1703178)
- [Python如何提取docx中的超链接](https://blog.csdn.net/s1162276945/article/details/102919305)
- [第105天：Python操作Word](http://www.ityouknow.com/python/2019/12/31/python-word-105.html)
- [python-docx处理word文档](https://zhuanlan.zhihu.com/p/61340025)

# 运行模式

## 模式1：my_style

**简介：**我的专属写作风格

**原理：**

- 根据Word文字颜色判断是否为标题、行代码
- 根据Word文字是否加粗、倾斜进行加粗和倾斜判定
- 根据Word“项目符号/编号”判断是否为列表
- 根据Word 1x1的表格判断是否为块代码

- 可以处理Word文档中的图片
- 可以处理Word文档中的超链接
- 只能处理docx文档，不能处理doc文档

## 模式2：stand_word_style

**简介：**标准Word写作风格

**原理：**

- 根据Word标题判断是否为标题
- 根据Word文字是否加粗进行加粗
- 根据Word“项目符号/编号”判断是否为列表
- 根据Word文字是否倾斜判断是否为行代码
- 根据Word 1x1的表格判断是否为块代码
- 可以处理Word文档中的图片
- 可以处理Word文档中的超链接
- 只能处理docx文档，不能处理doc文档

# 配置参数说明

## 所有配置项说明

- head_1_color：一级标题颜色
- head_2_color：二级标题颜色
- head_3_color：三级标题颜色
- head_4_color：四级标题颜色
- head_5_color：五级标题颜色
- line_code_color：行代码颜色
- head_1\_#_num：一级标题几个#号，后续标题#个数依次递增
- word_mode：
  - 1：处理单个docx文档
  - 2：处理目录下的所有docx文档
- img_mode：
  - 1：word文档图片保存在同文档名的文件夹下
  - 2：word文档图片保存在指定文件夹下
- src_path：docx文档路径（模式1），或目录路径（模式2）
-       模式1：e.g. `D:\下载\test.docx`
-       模式2：e.g. `D:\下载\test`
- save_path：markdown文件保存路径
-       模式1：e.g. `D:\下载\test.md`
-       模式2：e.g. `D:\下载\test_md`
- imgs_path：
-       模式1：填写`Null`值
- 模式2：填写图片保存路径，e.g. `D:\下载\imgs`

## 颜色对照表

- 7030A0：紫色
- 0070C0：蓝色
- 00B050：绿色
- C55A11：橙色
- FF66CC：粉色
- C00000：红色

# 示例配置

## 模式1：my_style

**说明：**此模式下仅需要以下配置项。

**e.g. 1：单docx转markdown & 图片保存在同文档名的文件夹下**

```yaml
head_1_color: 7030A0
head_2_color: 0070C0
head_3_color: 00B050
head_4_color: C55A11
head_5_color: FF66CC
line_code_color: C00000
head_1_#_num: 1
word_mode: 1
img_mode: 1
src_path: D:\下载\linux-manual.docx
save_path: D:\下载\linux-manual.md
imgs_path: Null
```

**e.g. 2：单docx转markdown & 图片保存在指定文件夹下**

```yaml
head_1_color: 7030A0
head_2_color: 0070C0
head_3_color: 00B050
head_4_color: C55A11
head_5_color: FF66CC
line_code_color: C00000
head_1_#_num: 1
word_mode: 1
img_mode: 2
src_path: D:\下载\test.docx
save_path: D:\下载\test.md
imgs_path: D:\下载\imgs
```

**e.g. 3：目录下的所有docx转markdown & 图片保存在同文档名的文件夹下**

```yaml
head_1_color: 7030A0
head_2_color: 0070C0
head_3_color: 00B050
head_4_color: C55A11
head_5_color: FF66CC
line_code_color: C00000
head_1_#_num: 1
word_mode: 2
img_mode: 1
src_path: D:\下载\test
save_path: D:\下载\test_md
imgs_path: Null
```

**e.g. 4：目录下的所有docx转markdown & 图片保存在指定文件夹下**

```yaml
head_1_color: 7030A0
head_2_color: 0070C0
head_3_color: 00B050
head_4_color: C55A11
head_5_color: FF66CC
line_code_color: C00000
head_1_#_num: 1
word_mode: 2
img_mode: 2
src_path: D:\下载\test
save_path: D:\下载\test_md
imgs_path: D:\下载\imgs
```

## 模式2：stand_word_style

**说明：**此模式下仅需要以下配置项。

**e.g. 1：单docx转markdown & 图片保存在同文档名的文件夹下**

```yaml
head_1_#_num: 1
word_mode: 1
img_mode: 1
src_path: test_docx\linux-manual.docx
save_path: test_docx\linux-manual.md
imgs_path: Null
```

# 如何使用？

根据自己的需求和示例配置修改`config.yaml`文件，然后命令行输入：`python main.py`运行程序，根据引导提示输入运行模式，等待处理结果。