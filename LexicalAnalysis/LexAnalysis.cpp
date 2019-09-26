#include<bits/stdc++.h>
using namespace std;

/********************************
FileName: Lexical Analysis
Author : fhkong
Time : 2019.9.22
Version: 1.1.0
********************************/
/******************************
1.支持超前搜索
2.支持自增自减等操作
3.对每个字符串进行处理，而非单个字符
*******************************/


vector<string> KeyWords;

string ProgramURL = "Program1.txt";  //Program file url
string KeyWordsURL = "KeyWords.txt"; //keywords file url
string OutFileURL = "LexAnalysisOutFile.txt"; //output file url


bool InitKeyWords(void)
{
    ifstream Inputfile(KeyWordsURL);
    if(Inputfile.fail())
    {
        cout<<"Can't open the KeyWords file!"<<endl;
        return false;
    }

    string TempStr;
    while(!Inputfile.eof())
    {
        Inputfile>>TempStr;
        KeyWords.push_back(TempStr);
    }

    //for(auto s : KeyWords)
    //{
    //    OutputFile<<s<<endl;
    //}
    //OutputFile<<KeyWords.size()<<endl;

    cout<<"******************************************"<<endl;
    cout<<"Init the KeyWords table Successfully!"<<endl;
    cout<<"******************************************"<<endl;
    Inputfile.close();
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
    for(auto k:KeyWords)
        if(k == s)
            return true;
    return false;
}



//对读取的一个字符串进行判断
void Judge(string strToken,int row,int col,ofstream &OutputFile)  
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
                OutputFile<<"*****Error: 第"<<row<<"行，第"<<col-len<<"列出现非法标识符: "<<strToken<<"*****"<<endl;
                isvaild = 0;
                break;
            }           
        }
        if(isvaild)  
        {
            bool iskey = false;
            for(auto a : KeyWords)
            {
                if(a == strToken)
                {
                    OutputFile<<"< $关键字   "<<setw(10)<<setiosflags(ios::left)<<strToken<<" >"<<endl;
                    iskey = true;
                    break;
                }                
            }
            if(!iskey)
            {
                OutputFile<<"< $标识符   "<<setw(10)<<setiosflags(ios::left)<<strToken<<" >"<<endl;
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
                OutputFile<<"*****Error: 第"<<row<<"行，第"<<col-len<<"列出现非法标识符: "<<strToken<<"*****"<<endl;
                isvaild = 0;
                break;
            }            
        }
        if(isvaild)
        {
            OutputFile<<setw(10)<<"< $常数      "<<setw(10)<<setiosflags(ios::left)<<strToken<<" >"<<endl;
        }
    }
}


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
    
    ofstream OutputFile(OutFileURL);
    if(OutputFile.fail())
    {
        cout<<"Can't open the OutputFile!"<<endl;
        return false;
    }

    string  LineCode;                       //read one line from source code file
    string  strToken;                       //temp string
    //getline(InputFile,LineCode);
    //OutputFile<<LineCode<<endl;

    int row = 0;                            //row number
    int col = 0;                            //col number
    char curch,nextch;                      //pre search
    while(!InputFile.eof())
    {
        getline(InputFile,LineCode);        //读取一行
        row++;                              //行号+1
        col = 0;                            //列好从0开始
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
               Judge(strToken,row,col,OutputFile);
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
                        OutputFile<<"$关键字  "<<strToken<<endl;
                        strToken.clear();
                    }
                    else
                    {
                        OutputFile<<"$标识符 "<<strToken<<endl;
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
                    OutputFile<<"$常数  "<<strToken<<endl; 
                */
            }   
            else if(curch == '/')  //处理注释和除法，可能没有注释？
            {
                if(nextch == '/')
                    while(LineCode[col]!='\0')
                        col++;
                else
                {
                    OutputFile<<"< $除法      "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
                }                
            }
            else if(curch == '+')
            {
                OutputFile<<"< $加法      "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;                
            }
            else if(curch == '-')
            {
                OutputFile<<"< $减法        "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == '*')  
            {
                OutputFile<<"< $乘法        "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;               
            }
            else if(curch == '^')
            {
                OutputFile<<"< $乘方      "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == ':')
            {
                if(nextch == '=')
                {
                    OutputFile<<"< $赋值      "<<curch<<setw(9)<<setiosflags(ios::left)<<nextch<<" >"<<endl;
                    col++;
                }
                else
                {
                    OutputFile<<"********Error: 第"<<row<<"行,第"<<col+1<<"列缺少 =  *******"<<endl;
                }                
            }
            else if(curch == '<')
            {
                if(nextch == '=')
                {
                    OutputFile<<"< $小于等于  "<<curch<<setw(9)<<setiosflags(ios::left)<<nextch<<" >"<<endl;
                    col++;
                }
                else if(nextch == '>')
                {
                    OutputFile<<"< $不等于  "<<curch<<setw(9)<<setiosflags(ios::left)<<nextch<<" >"<<endl;
                    col++;
                }
                else
                {
                    OutputFile<<"< $小于    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
                }                
            }
            else if(curch == '>')
            {
                if(nextch == '=')
                {
                    OutputFile<<"< $大于等于    "<<curch<<setw(9)<<setiosflags(ios::left)<<nextch<<" >"<<endl;
                    col++;
                }
                else
                {
                    OutputFile<<"< $大于    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
                }                
            }
            else if(curch == '"')
            {
                OutputFile<<"< $字符串标记符    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == ',')
            {
                OutputFile<<"< $分界符    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == ';')
            {
                OutputFile<<"< $分界符    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == '(')
            {
                OutputFile<<"< $分界符    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            
            else if(curch == ')')
            {
                OutputFile<<"< $分界符    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == '\'')
            {
                OutputFile<<"< $分界符    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }          
            else
            {        
                if(curch == '=')
                {
                    OutputFile<<"*****Error: 第"<<row<<"行，第"<<col<<"列出现非法字符 "<<curch<<"  ***** ";
                    OutputFile<<"或许前面缺少:"<<endl;
                }
                else
                {
                    OutputFile<<"*****Error: 第"<<row<<"行，第"<<col<<"列出现非法字符 "<<curch<<"  ***** "<<endl;
                }              
            }            
        }
    }

    InputFile.close();
    OutputFile.close();
    cout<<"Lexical Analysis is finished!"<<endl;
    cout<<"Output file is generated successfully!"<<endl;

    return true;

}

int main()
{
   if(!InitKeyWords())
    {
        cout<<"Init KeyWords table Fail !"<<endl;
        return 0;
    }
    LexAnalysis();
    return 0;
}