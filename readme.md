***
* Filename:readme.md
* Author:fhkong
* Time: 2019-9-22
* Version:1.0.0
***

## Pascal Compiler
* 一款由C++语言写的 `Pascal/0`语言的编译器前端（只包括词法分析和语法分析）  
* 该项目为专业课中编译原理的实验课内容  
* 目前仅仅实现了前端的简单部分，后面有时间会继续完善

## 文件夹介绍
* LexicalAnalysis: 词法分析的代码部分
* ParseAnalysis:语法分析的代码部分 语法分析要用到词法分析的结果

## 词法分析
* 输入：`Pascal/0`源代码
* 输出：源代码的单词串  
* 文件路径
    *  LexAnalysis.cpp中的输出路径 OutFileURL="LexAnalysisOutFile.txt"  
* 编译运行LexAnalysis.cpp即可

## 语法分析LL(1)
* 输入：PL/0文法和词法分析的结果
* 输出：逐步分析程序的结果  
* 文件路径
    * 文法输入路径：Parse.cpp中InputFile.open("Pascal.txt")
    * 词法分析结果路径:LexAnalysis中 ProgramURL = "1.txt"
* 编译Main.cpp运行即可

## 预测分析法
语法分析采用预测分析法，构建预测分析表和符号栈进行逐步分析  
求出每个非终结符的first集和follow集，构造出预测分析表MTable
然后将文法开始符号压入栈中，逐步进行分析

## 错误处理
错误处理采用同步符号集处理，对构造出来的预测分析表扩充，将空的项目填入同步符号
当a属于follow(A)且a不属于first(A)时，将MTable[A][a]填入sync
所以在分析时，只要遇到未匹配的情况，看看是否时sync,假如是，就从栈中弹出当前的非终结符继续分析
如果栈顶的终结符和当前的终结符不匹配，弹出栈中的终结符和跳过当前的终结符可以看情况选择

## 未来工作
- [ ] 继续完善语法分析中的错误处理的能力
- [ ] 添加语义分析、产生中间代码、优化、生成目标代码
- [ ] 尝试着做出一个完整的编译程序
