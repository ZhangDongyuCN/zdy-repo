# 简介

**功能：**word和markdown互转

**环境：**

- python==3.8.5
- python-docx==0.8.10
- pyyaml==5.4.1

**参考资料：**
- [python-docx官方文档](https://python-docx.readthedocs.io/en/latest/index.html)
- [Python顺序读取word文档中的文本与表格](https://blog.csdn.net/qq_39600166/article/details/101537368)
- [kmrambo/Python-docx-Reading-paragraphs-tables-and-images-in-document-order-](https://github.com/kmrambo/Python-docx-Reading-paragraphs-tables-and-images-in-document-order-)
- [Python使用标准库zipfile+re提取docx文档中超链接文本和链接地址](https://cloud.tencent.com/developer/article/1703178)
- [Python如何提取docx中的超链接](https://blog.csdn.net/s1162276945/article/details/102919305)
- [第105天：Python操作Word](http://www.ityouknow.com/python/2019/12/31/python-word-105.html)
- [python-docx处理word文档](https://zhuanlan.zhihu.com/p/61340025)
- [使用python-docx在MSWord中添加超链接](https://qa.1r1g.com/sf/ask/3336664971/#)
- [python-docx设置表格填充底色以及切分单元格(仅使用python-docx)](https://blog.csdn.net/starnet2010/article/details/102754664)

# 配置参数说明

- head_1_color：一级标题颜色
- head_2_color：二级标题颜色
- head_3_color：三级标题颜色
- head_4_color：四级标题颜色
- line_code_color：行代码颜色
- word_mode：
  - 1：处理单个docx文档
  - 2：处理目录下的所有docx文档
- img_mode：
  - 1：word文档图片保存在同文档名的文件夹下
  - 2：word文档图片保存在指定文件夹下
- src_path：docx文档路径（模式1），或目录路径（模式2）
  - 模式1：e.g. `D:\下载\test.docx`
  - 模式2：e.g. `D:\下载\test`
- save_path：markdown文件保存路径
  - 模式1：e.g. `D:\下载\test.md`
  - 模式2：e.g. `D:\下载\test_md`
- imgs_path：
  - 模式1：填写`Null`值
  - 模式2：填写图片保存路径，e.g. `D:\下载\imgs`

# 运行模式

## 模式1：my_style -> docx 2 markdown

**简介：**我的专属写作风格，docx转markdown

**原理：**

- 根据Word文字颜色判断是否为标题、行代码
- 根据Word文字是否加粗、倾斜进行加粗和倾斜判定
- 根据Word“项目符号/编号”判断是否为列表
- 根据Word 1x1的表格判断是否为块代码

- 可以处理Word文档中的图片
- 可以处理Word文档中的超链接
- 只能处理docx文档，不能处理doc文档

**示例配置：**

**说明：**此模式下仅需要以下配置项。

**e.g. 1：单docx转markdown & 图片保存在同文档名的文件夹下**

```yaml
head_1_color: 7030A0
head_2_color: 0070C0
head_3_color: 00B050
head_4_color: C55A11
line_code_color: C00000
word_mode: 1
img_mode: 1
src_path: test_document\git常用命令.docx
save_path: test_document\git常用命令-temp.md
imgs_path: Null
```

**颜色说明：**

- 7030A0：紫色
- 0070C0：蓝色
- 00B050：绿色
- C55A11：橙色
- C00000：红色

**e.g. 2：单docx转markdown & 图片保存在指定文件夹下**

```yaml
head_1_color: 7030A0
head_2_color: 0070C0
head_3_color: 00B050
head_4_color: C55A11
line_code_color: C00000
word_mode: 1
img_mode: 2
src_path: test_document\git常用命令.docx
save_path: test_document\git常用命令-temp.md
imgs_path: D:\zdy-repo\tools\docx_2_markdown\test_document\imgs
```

**e.g. 3：目录下的所有docx转markdown & 图片保存在同文档名的文件夹下**

```yaml
head_1_color: 7030A0
head_2_color: 0070C0
head_3_color: 00B050
head_4_color: C55A11
line_code_color: C00000
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
line_code_color: C00000
word_mode: 2
img_mode: 2
src_path: D:\下载\test
save_path: D:\下载\test_md
imgs_path: D:\下载\imgs
```

## 模式2：stand_word_style -> docx 2 markdown

**简介：**标准Word写作风格，docx转markdown

**原理：**

- 根据Word标题判断是否为标题
- 根据Word文字是否加粗进行加粗
- 根据Word“项目符号/编号”判断是否为列表
- 根据Word文字是否倾斜判断是否为行代码
- 根据Word 1x1的表格判断是否为块代码
- 可以处理Word文档中的图片
- 可以处理Word文档中的超链接
- 只能处理docx文档，不能处理doc文档

**示例配置：**

**说明：**此模式下仅需要以下配置项。

**e.g. 1：单docx转markdown & 图片保存在同文档名的文件夹下**

```yaml
word_mode: 1
img_mode: 1
src_path: test_document\linux-manual.docx
save_path: test_document\linux-manual-temp.md
imgs_path: Null
```

## 模式3：my_style -> markdown 2 docx

**简介：**我的专属写作风格，markdown转docx

**原理：** 模式1的逆变换

**示例配置：**

**说明：**此模式下仅需要以下配置项。

**e.g. 1：单markdown转docx**

```yaml
word_mode: 1
src_path: test_document\linux-manual.md
save_path: test_document\linux-manual-temp.docx
```

# 如何使用？

根据自己的需求和示例配置修改`docx_2_markdown_config.yaml`文件，然后命令行输入：`python main.py`运行程序，根据引导提示输入运行模式，等待处理结果。