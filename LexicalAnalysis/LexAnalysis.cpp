#include<bits/stdc++.h>
#include "Define.h"
using namespace std;

/********************************
FileName: Lexical Analysis
Author : fhkong
Time : 2019.9.22
Version: 1.1.1
********************************/
/******************************
 * updata
 * time:2019.09.26
 * author:fhkong
 * content:
 *          1.修改了文件的输出格式，留给语法分析做接口
 *          2.删除了部分PL/0语言中没有的字符
*******************************/
/******************************
1.支持超前搜索
2.支持自增自减等操作
3.对每个字符串进行处理，而非单个字符
*******************************/


string ProgramURL = "1.txt";  //Program file url
//string KeyWordsURL = "KeyWords.txt"; //keywords file url
string OutFileURL = "LexAnalysisOutFile.txt"; //output file url


struct Word
{
    int row,col;//record the location of word
    int type;   //record the type of word
    int value;  //record the value of word
    string content;
};

/*********************** 
Word.type
1.$operator     运算符，关系符
2.$const        常数
3.$keyword      关键字
4.$identifier   标识符
5.$#            结束符
6.$error        词法分析错误符
7.$boundary     边界符
************************/

//重定向标准输出<<
ostream& operator<<(ostream& out ,const Word& w)
{
    out<<"["<<w.row<<","<<w.col<<"] "<<w.type<<": "<<w.value<<" "<<w.content;
    return out;
}

Word word;
vector<Word> SourceWords;                //源文件数组
vector< pair<string,int> > KeyWordsMap;  //关键字映射表


bool InitKeyWords(void)
{
    KeyWordsMap.clear();
    KeyWordsMap.push_back(make_pair("program",PROGRAM)); 
    KeyWordsMap.push_back(make_pair("const",CONST)); 
    KeyWordsMap.push_back(make_pair("var",VAR)); 
    KeyWordsMap.push_back(make_pair("procedure",PROCEDURE)); 
    KeyWordsMap.push_back(make_pair("begin",BEGIN)); 
    KeyWordsMap.push_back(make_pair("end",END)); 
    KeyWordsMap.push_back(make_pair("if",IF)); 
    KeyWordsMap.push_back(make_pair("else",ELSE)); 
    KeyWordsMap.push_back(make_pair("while",WHILE)); 
    KeyWordsMap.push_back(make_pair("do",DO)); 
    KeyWordsMap.push_back(make_pair("call",CALL)); 
    KeyWordsMap.push_back(make_pair("read",READ)); 
    KeyWordsMap.push_back(make_pair("write",WRITE)); 
    KeyWordsMap.push_back(make_pair("odd",ODD)); 
    KeyWordsMap.push_back(make_pair("then",THEN));

    cout<<"*****************KeyWordsMap initial is ok!***************"<<endl;
    return true;
}

bool IsBoundary(char ch)   //是否为边界符号
{
    return  (ch=='('||ch==')' ||ch==',' ||ch==';' ||ch=='"'||ch=='\''  //'符号需要转义一下
           ||ch==' '||ch=='\t'||ch=='\0'||ch=='\n'); //空格、tab、换行、回车
}

bool IsNumber(char ch)  //是否为数字
{
    return ch>='0' && ch<='9';
}

bool IsLetter(char ch) //是否为字母
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A'&& ch <= 'Z');
}

bool IsOperator(char ch)  //是否为运算符
{
    return   (ch == '+' || ch == '-' || ch == '*'|| ch == '/'  //加减乘除 
            ||ch == '<' || ch == '>' || ch == ':'   //小于等于，大于等于，等于，不等于
            ||ch == '^');  //乘方    
}


bool IsKeyWord(string s)   //判断是否为关键字
{
    for(auto k:KeyWordsMap)
        if(k.first == s)
            return true;
    return false;
}



//对读取的一个字符串进行判断
void Judge(string strToken,int row,int col)  
{
    int len = strToken.size();
    int isvaild = 1;
    if(IsLetter(strToken[0])) //字母开头
    {
        for(int i=0;i<len;i++)
        {
            if(IsLetter(strToken[i]) ||IsNumber(strToken[i]) || strToken[i] == '_')
                continue;
            else
            {
                //OutputFile<<"*****Error: ["<<row<<","<<col-len<<"] illegal identifier : "<<strToken<<"*****"<<endl;
                word.row = row;
                word.col = col;
                word.type = IDENTIFIER;
                word.value = NoUse;
                word.content = strToken;
                isvaild = 0;
                break;
            }           
        }
        if(isvaild)  
        {
            bool iskey = false;
            for(auto a : KeyWordsMap)
            {
                if(a.first == strToken)
                {
                    //OutputFile<<row<<" "<<col<<" $keyword   "<<strToken<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = KeyWord;                    
                    word.value = a.second;
                    word.content = strToken;
                    iskey = true;
                    break;
                }                
            }
            if(!iskey)
            {
                //OutputFile<<row<<" "<<col<<" $identifier   "<<strToken<<endl;
                word.row = row;
                word.col = col;
                word.type = IDENTIFIER;
                word.value = NoUse;
                word.content = strToken;
            }
        }
    }

    else if(IsNumber(strToken[0])) // 数字开头
    {
        for(int i=0;i<len;i++)
        {
            if(IsNumber(strToken[i]))
                continue;
            else
            {
                //OutputFile<<"*****Error: ["<<row<<","<<col-len<<"] illegal identifier : "<<strToken<<"*****"<<endl;
                word.row = row;
                word.col = col;
                word.type = IDENTIFIER;
                word.value = NoUse;
                word.content = strToken;
                isvaild = 0;
                break;
            }            
        }
        if(isvaild)
        {
            //OutputFile<<row<<" "<<col<<" $const "<<strToken<<endl;
            word.row = row;
            word.col = col;
            word.type = IDENTIFIER;
            word.value = NoUse;
            word.content = strToken;
        }
    }
    SourceWords.push_back(word);  
}

//词法分析入口
bool LexAnalysis()
{
    //ifstream InputFile(ProgramURL);
    string IO;
    cout<<"Please input the source code file url : ";
    getline(cin,IO);
    ifstream InputFile(IO);
    
    if(InputFile.fail())
    {
        cout<<"Can't open the program file!"<<endl;
        return false;
    }
    
    //ofstream //OutputFile(OutFileURL);
    //if(OutputFile.fail())
    //{
    //    cout<<"Can't open the //OutputFile!"<<endl;
    //    return false;
    //}

    string  LineCode;                       //read one line from source code file
    string  strToken;                       //temp string
    //getline(InputFile,LineCode);
    ////OutputFile<<LineCode<<endl;

    int row = 0;                            //row number
    int col = 0;                            //col number
    char curch,nextch;                      //nextch use for pre search

    while(!InputFile.eof())
    {
        getline(InputFile,LineCode);        //读取一行
        row++;                              //行号+1
        col = 0;                            //列从0开始
        while(LineCode[col] != '\0')        //没有到行尾
        {
            curch = LineCode[col++];        //当前判断的字符
            nextch = LineCode[col];         //下一个字符，实现超前搜索

            if(curch == ' '||curch == '\t'||curch == '\n')
            {
                   //cout<<"0"<<endl;                         //跳过编辑性字符                
            }
            else if(IsLetter(curch) || IsNumber(curch))        //字母开头或者数字开头
            {
               strToken.clear();           //清空strToken
               while(!IsBoundary(curch) && !IsOperator(curch)) //不是边界符并且不是运算字符，读取一个字符串
               {
                   strToken += curch;
                   curch = LineCode[col++];
               }
               col--;
               Judge(strToken,row,col);
                    /*
                    while(IsLetter(curch)||IsNumber(curch) || curch == '_') 
                    {
                        strToken += curch;
                        curch = LineCode[col++];
                    }

                    col--;  //回退一个
                    // xy4可以识别，xy4$不可以
                    if(IsKeyWord(strToken))
                    {
                        //OutputFile<<"$关键字  "<<strToken<<endl;
                        strToken.clear();
                    }
                    else
                    {
                        //OutputFile<<"$标识符 "<<strToken<<endl;
                        strToken.clear();
                    } 
                

                }
                else if(IsNumber(curch)) //数字开头   9c87交给词法分析出错检验
                {
                    strToken.clear();
                    while(IsNumber(curch))
                    {
                        strToken += curch;
                        curch = LineCode[col++];
                    }
                    col--;   //回退一个
                    //OutputFile<<"$常数  "<<strToken<<endl; 
                */
            }   
            else if(curch == '/')  //处理注释和除法，可能没有注释？
            {
                if(nextch == '/')
                    while(LineCode[col]!='\0')
                        col++;
                else  //除法
                {
                    //OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = DIV;
                    word.content = curch;
                    SourceWords.push_back(word);  
                }                
            }
            else if(curch == '+')  //加法
            {
                //OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;                
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = ADD;
                word.content = curch;
                SourceWords.push_back(word);  
            }
            else if(curch == '-') //减法
            {
                //OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = SUB;
                word.content = curch;
                SourceWords.push_back(word);  
            }
            else if(curch == '*')  //乘法
            {
                //OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = MUL;
                word.content = curch;
                SourceWords.push_back(word);  
            }
            else if(curch == '^')  //乘方
            {
                //OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = POWER ;
                word.content = curch;
                SourceWords.push_back(word);  
            }
            else if(curch == ':')  //
            {
                if(nextch == '=')  //赋值
                {                    
                    //OutputFile<<row<<" "<<col<<" $operator "<<curch<<nextch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = ASSIGN;
                    word.content = ":=";
                    SourceWords.push_back(word);  
                    col++;
                }
                else //出错
                {
                    //OutputFile<<"********Error: ["<<row<<","<<col+1<<"] Maybe miss '='  *******"<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = ASSIGN;
                    word.content = curch;
                    SourceWords.push_back(word);  
                }                
            }
            else if(curch == '<')
            {
                if(nextch == '=')  //小于等于
                {
                    //OutputFile<<row<<" "<<col<<" $operator "<<curch<<nextch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = LESS_EQUA; //<=
                    word.content = "<=";
                    SourceWords.push_back(word);  
                    col++;
                }
                else if(nextch == '>') //不等于
                {
                    //OutputFile<<row<<" "<<col<<" $operator "<<curch<<nextch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = NOREQUA; //<>
                    word.content = "<>";
                    SourceWords.push_back(word);  
                    col++;
                }
                else  //小于
                {
                    //OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = LESS_THAN;
                    word.content = curch; 
                    SourceWords.push_back(word);                     
                }                
            }
            else if(curch == '>')
            {
                if(nextch == '=') //大于等于
                {
                    //OutputFile<<row<<" "<<col<<" $operator "<<curch<<nextch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = GRT_EQUA;
                    word.content = ">=";
                    SourceWords.push_back(word);  
                    col++;
                }
                else  //大于
                {
                    //OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = GRT_THAN;
                    word.content = ">=";
                    SourceWords.push_back(word);  
                }                
            }
            else if(curch == ',')
            {
                //OutputFile<<row<<" "<<col<<" $boundary "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Boundary;
                word.value = COMMA;
                word.content = curch;
                SourceWords.push_back(word);  
            }
            else if(curch == ';')
            {
                //OutputFile<<row<<" "<<col<<" $boundary "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Boundary;
                word.value = SEMI;
                word.content = curch;
                SourceWords.push_back(word);  
            }
            else if(curch == '(')
            {
                //OutputFile<<row<<" "<<col<<" $boundary "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Boundary;
                word.value = LEFT_SBRA;
                word.content = curch;
                SourceWords.push_back(word);  
            }
            
            else if(curch == ')')
            {
                //OutputFile<<row<<" "<<col<<" $boundary "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Boundary;
                word.value = RIGHT_SBRA;
                word.content = curch;
                SourceWords.push_back(word);  
            }         
            else if(curch == '=')
            {
                 //OutputFile<<row<<" "<<col<<" $Operator "<<curch<<endl;
                 word.row = row;
                 word.col = col;
                 word.type = Operator;
                 word.value = EQUAL;
                 word.content = curch;
                 SourceWords.push_back(word);                      
            }
            else
            {
                //OutputFile<<"*****Error: ["<<row<<","<<col<<"] illegal char "<<curch<<"  ***** "<<endl;             
            }           
        }
    }

    InputFile.close();
    //OutputFile.close();
    cout<<"Lexical Analysis is finished!"<<endl;
    cout<<"Output file is generated successfully!"<<endl;

    return true;

}

int main()
{
    SourceWords.clear();

    if(!InitKeyWords())
    {
        cout<<"Init KeyWords table Fail !"<<endl;
        return 0;
    }
    LexAnalysis();

    //cout<<word;
    //int i = 1;
    //for(auto a : SourceWords)
    //   cout<<i++<<" "<<a<<endl;
    return 0;
}