#include<bits/stdc++.h>
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

#define NoUse -1

/**************KeyWord********************/
#define    KeyWord      0   //type

#define    PROGRAM      1
#define    CONST        2
#define    VAR          3
#define    PROCEDURE    4
#define    BEGIN        5
#define    END          6
#define    IF           7
#define    ELSE         8    
#define    WHILE        9
#define    DO           10
#define    CALL         11
#define    READ         12
#define    WRITE        13
#define    ODD          14
#define    THEN         15


/**************Operator*******************/
#define     Operator    99      //type

#define     ASSIGN      100     // :=
#define     LESS_THAN   101     // <
#define     LESS_EQUA   102     // <=
#define     GRT_THAN    103     // >
#define     GRT_EQUA    104     // >=
#define     ADD         105     // +
#define     SUB         106     // -
#define     MUL         107     // *
#define     DIV         108     // /
#define     POWER       109     // ^
#define     NOREQUA     110     // <>
#define     EQUAL       111     // =

/**************Boundary*******************/
#define     Boundary     200     //type

#define     LEFT_SBRA    201     // (
#define     RIGHT_SBRA   202     // )
#define     LEFT_MBRA    203     // [
#define     RIGHT_MBRA   204     // ]
#define     POINTER      205     // .
#define     COMMA        206     // ,
#define     SEMI         206     // ;


/*************Identifier*******************/
#define     IDENTIFIER          500     //type,include id and const number
   
/************NorTerMap start num**********/
#define     NorTerBase      1000  //非终结符的起始数

/************Grammer Special char************/
#define     GRAMMAR_ARROW   2000  // ->
#define     GRAMMAR_OR      2001  // |
#define     GRAMMAR_NULL    2002  // $
#define     GRAMMAR_END     2003  // #




vector<string> KeyWords;

string ProgramURL = "Program1.txt";  //Program file url
string KeyWordsURL = "KeyWords.txt"; //keywords file url
string OutFileURL = "LexAnalysisOutFile.txt"; //output file url


struct Word
{
    int row,col;//record the location of word
    int type;   //record the type of word
    int value;  //record the value of word
};
ofstream& operator<<(ofstream &out , Word &w)
{
    out<<"["<<w.row<<","<<w.col<<"] "<<w.type<<": "<<w.value;
    return out;
}

Word word;
vector<Word> SourceWords;

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
                OutputFile<<"*****Error: ["<<row<<","<<col-len<<"] illegal identifier : "<<strToken<<"*****"<<endl;
                word.row = row;
                word.col = col;
                word.type = IDENTIFIER;
                word.value = NoUse;
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
                    OutputFile<<row<<" "<<col<<" $keyword   "<<strToken<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = KeyWord;
                    transform(strToken.begin(),strToken.end(),strToken.begin(),::toupper);
                    word.value = strToken;
                    iskey = true;
                    break;
                }                
            }
            if(!iskey)
            {
                OutputFile<<row<<" "<<col<<" $identifier   "<<strToken<<endl;
                word.row = row;
                word.col = col;
                word.type = IDENTIFIER;
                word.value = NoUse;
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
                OutputFile<<"*****Error: ["<<row<<","<<col-len<<"] illegal identifier : "<<strToken<<"*****"<<endl;
                word.row = row;
                word.col = col;
                word.type = IDENTIFIER;
                word.value = NoUse;
                isvaild = 0;
                break;
            }            
        }
        if(isvaild)
        {
            OutputFile<<row<<" "<<col<<" $const "<<strToken<<endl;
            word.row = row;
            word.col = col;
            word.type = IDENTIFIER;
            word.value = NoUse;
        }
    }
    SourceWords.push_back(word);
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
                else  //除法
                {
                    OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = DIV;
                }                
            }
            else if(curch == '+')  //加法
            {
                OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;                
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = ADD;
            }
            else if(curch == '-') //减法
            {
                OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = SUB;
            }
            else if(curch == '*')  //乘法
            {
                OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = MUL;
            }
            else if(curch == '^')  //乘方
            {
                OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = POWER ;
            }
            else if(curch == ':')  //
            {
                if(nextch == '=')  //赋值
                {                    
                    OutputFile<<row<<" "<<col<<" $operator "<<curch<<nextch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = ASSIGN;
                    col++;
                }
                else //出错
                {
                    OutputFile<<"********Error: ["<<row<<","<<col+1<<"] Maybe miss '='  *******"<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = ASSIGN;
                }                
            }
            else if(curch == '<')
            {
                if(nextch == '=')  //小于等于
                {
                    OutputFile<<row<<" "<<col<<" $operator "<<curch<<nextch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = LESS_EQUA; //<=
                    col++;
                }
                else if(nextch == '>') //不等于
                {
                    OutputFile<<row<<" "<<col<<" $operator "<<curch<<nextch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = NOREQUA; //<>
                    col++;
                }
                else  //小于
                {
                    OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = LESS_THAN;
                }                
            }
            else if(curch == '>')
            {
                if(nextch == '=') //大于等于
                {
                    OutputFile<<row<<" "<<col<<" $operator "<<curch<<nextch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = GRT_EQUA;
                    col++;
                }
                else  //大于
                {
                    OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                    word.row = row;
                    word.col = col;
                    word.type = Operator;
                    word.value = GRT_THAN;
                }                
            }
            else if(curch == ',')
            {
                OutputFile<<row<<" "<<col<<" $boundary "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Boundary;
                word.value = COMMA;
            }
            else if(curch == ';')
            {
                OutputFile<<row<<" "<<col<<" $boundary "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Boundary;
                word.value = SEMI;
            }
            else if(curch == '(')
            {
                OutputFile<<row<<" "<<col<<" $boundary "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Boundary;
                word.value = LEFT_SBRA;
            }
            
            else if(curch == ')')
            {
                OutputFile<<row<<" "<<col<<" $boundary "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Boundary;
                word.value = RIGHT_SBRA;
            }         
            else if(curch == '=')
            {
                 OutputFile<<row<<" "<<col<<" $Operator "<<curch<<endl;
                 word.row = row;
                 word.col = col;
                 word.type = Operator;
                 word.value = EQUAL;                    
            }
            else
            {
                OutputFile<<"*****Error: ["<<row<<","<<col<<"] illegal char "<<curch<<"  ***** "<<endl;             
            } 
            SourceWords.push_back(word);           
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
    SourceWords.clear();

    if(!InitKeyWords())
    {
        cout<<"Init KeyWords table Fail !"<<endl;
        return 0;
    }
    LexAnalysis();

    return 0;
}