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
```c++
//string ProgramURL = "Program1.txt";             //Program file url
//string KeyWordsURL = "KeyWords.txt";            //keywords file url
string OutFileURL = "LexAnalysisOutFile.txt";   //output file url
```
* 编译运行LexAnalysis.cpp即可