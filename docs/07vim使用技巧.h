/*******************************
 * vim高级使用技巧
 * Author: zht
 * Date: 2017-12-28
 *******************************/


1.在vim中直接打开头文件
==========================
将光标放到代码开头的include语句处，如<stdio.h>上;
按g,f(先按g再按f)会跳转到该头文件，按Ctrl+o(同时按Ctrl和o)跳回去；

如果要跳转到内核代码的头文件，需要在.vimrc中增加头文件路径：
$>vim ~/.vimrc
set path+=/srv/ftp/class-e6818/X6818_B/kernel/include/
set path+=/srv/ftp/class-e6818/X6818_B/kernel/arch/arm/plat-s5p6818/x6818/include/
set path+=/srv/ftp/class-e6818/X6818_B/kernel/arch/arm/mach-s5p6818/include/
set path+=/srv/ftp/class-e6818/X6818_B/kernel/arch/arm/plat-s5p6818/common/

可以测试一下：
#include <stdio.h>
#include <linux/module.h>



2.设置vim的Tab宽度和缩进
========================
$>vim ~/.vimrc
写入如下内容：
set tabstop=4
set shiftwidth=4
set autoindent
set cindent

这些设置的含义如下：
tabstop=4		//将一个Tab显示为4个字符
shiftwidth=4	//代码每一级缩进4个字符
autoindent		//自动缩进
cindent			//针对C语言字段缩进



3.在vim中直接打开man手册
==========================
将光标放到函数上，按shift+k；
打开该函数对应的man手册
	socket();



4.将命令的执行结果写入文件
==========================
(1):r !pwd
将命令pwd的执行结果写入当前文件

(2):r /home/xx.h
打开给定的文件，将其内容写入当前文件



5.在vim中开双屏工作
=========================
在vim中，可以用快捷键打开两个窗口：
Ctrl+w,n	//上下分屏,打开空白窗口
Ctrl+w,s	//上下分屏,打开本文件

Ctrl+w,v	//左右分屏,打开本文件

Ctrl+w,w	//在两个窗口间切换

Ctrl+w,c	//如果开启了多个窗口，则关闭当前窗口
Ctrl+w,o	//如果开启了多个窗口，关闭其他窗口，只保留当前窗口

可以在新打开的窗口中编辑新文件
:e /home/xxx.h



6.同时编辑当前目录下的多个文件
=========================
$>vim -p *.c
用Ctrl+Alt+Pageup/Pagedown切换文件;
不是特别推荐，因为有些笔记本没有Pageup/Pagedown...



7.文本替换/删除/复制
=====================
(1)文本替换
-------------------
:s/Hello/abc/
将当前行的Hello替换为abc
:44s/Hello/abc/
替换44行的第一个Hello为abc
:1,$s/Hello/abc/g
检索全文，将所有的Hello都替换为abc
:1,$s/Hello/abc/gc
检索全文，将所有的Hello都替换为abc，每次替换时都询问


(2)文本删除
-------------------
:55,100d	//删除55到100行
:1,$d		//从第一行删除到最后一行
dd			//删除本行
100dd		//从光标行开始向下删除100行(含本行)
D			//删除光标后面的所有字符(本行)


(3)文本复制
--------------------
:1,20y		//复制1到20行
:1,$y		//全文复制
yy			//复制当前行
20yy		//从光标所处行开始复制20行
p			//复制后要粘贴



8.杂项
====================
(1)存盘退出
---------------------
:w			//保存当前文件
:w abc.txt	//将本文件另存为abc.txt
:wq			//保存文件并退出
ZZ			//两个Z都大写，保存退出

(2)显示当前文件的信息
---------------------
Ctrl+g

(3)显示行号
---------------------
:set nu		//显示行号
:set nonu	//不显示行号

(4)取消高亮
---------------------
:noh	//取消高亮

(5)undo/reundo
---------------------
u		//undo
Ctrl+r	//reundo


(6)重复上一动作
---------------------
.		//重复上一动作



/*******************************
 * 使用ctags+vim工具查看源代码
 *******************************/

1.创建tags文件
========================
在要检索的源代码目录下创建tags文件:
$>cd /srv/ftp/class-x6818/X6818_B/kernel/
$>ctags -R
命令会执行一会儿，完毕后，会在目录下生成一个名字为tags的文件；
tags文件很大，包含了整个内核代码的符号信息；



2.设置vim的配置文件
========================
将tags文件的位置告诉vim：
$>vim ~/.vimrc
set tag=/srv/ftp/class-e6818/X6818_B/kernel/tags

可以同时观察多个tags文件，如:
set tag+=/home/zht/Qt5.8.0/5.8/Src/tags

如果系统中有多个内核，都要创建tags文件，但不要让多个tags文件同时起作用；
例如:
系统中有x6818开发板用的linux3.4.39版内核，也有ubuntu用到的linux4.4.0内核，都放在srv/ftp目录下，可以这样:
"set tag=/srv/ftp/linux-3.4.39/tags
set tag=/srv/ftp/linux-4.4.0/tags
不用的tags文件用一个单一的双引号注释



3.符号检索
=========================
准备好tags文件，并将路径写入.vimrc后，就可以开始阅读linux内核源代码了；

(1)在启动vim的同时检索符号
------------------------
$>vim -t file_operations
//vim会检索tags文件，打开包含定义file_operations结构体的文件


(2)先进入vim然后再检索
-------------------------
$>vim xx.c
在vim中进入命令行模式，键入要搜索的内容：
:ta memset
此时，vim会自动跳转到memset定义的位置;

:ts memset
如果memset有多个定义，则换用ts命令，按照列出的数字选择定义;

Ctrl+o
按快捷键回到原来的位置;


(3)利用光标位置检索
-------------------------
在用vim编辑文件时，还可以把光标定位到要检索的符号，然后按如下快捷键：
  Ctrl + ]  	//查看定义，作用同:ta
  g + ]		//查看一组定义中的某个，作用同:ts
  Ctrl + o  	//返回
上述都支持命令支持递归操作


