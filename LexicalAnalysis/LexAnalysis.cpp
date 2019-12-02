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
 *          1.�޸����ļ��������ʽ�������﷨�������ӿ�
 *          2.ɾ���˲���PL/0������û�е��ַ�
*******************************/
/******************************
1.֧�ֳ�ǰ����
2.֧�������Լ��Ȳ���
3.��ÿ���ַ������д������ǵ����ַ�
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
1.$operator     ���������ϵ��
2.$const        ����
3.$keyword      �ؼ���
4.$identifier   ��ʶ��
5.$#            ������
6.$error        �ʷ����������
7.$boundary     �߽��
************************/

//�ض����׼���<<
ostream& operator<<(ostream& out ,const Word& w)
{
    out<<"["<<w.row<<","<<w.col<<"] "<<w.type<<": "<<w.value<<" "<<w.content;
    return out;
}

Word word;
vector<Word> SourceWords;                //Դ�ļ�����
vector< pair<string,int> > KeyWordsMap;  //�ؼ���ӳ���


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

bool IsBoundary(char ch)   //�Ƿ�Ϊ�߽����
{
    return  (ch=='('||ch==')' ||ch==',' ||ch==';' ||ch=='"'||ch=='\''  //'������Ҫת��һ��
           ||ch==' '||ch=='\t'||ch=='\0'||ch=='\n'); //�ո�tab�����С��س�
}

bool IsNumber(char ch)  //�Ƿ�Ϊ����
{
    return ch>='0' && ch<='9';
}

bool IsLetter(char ch) //�Ƿ�Ϊ��ĸ
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A'&& ch <= 'Z');
}

bool IsOperator(char ch)  //�Ƿ�Ϊ�����
{
    return   (ch == '+' || ch == '-' || ch == '*'|| ch == '/'  //�Ӽ��˳� 
            ||ch == '<' || ch == '>' || ch == ':'   //С�ڵ��ڣ����ڵ��ڣ����ڣ�������
            ||ch == '^');  //�˷�    
}


bool IsKeyWord(string s)   //�ж��Ƿ�Ϊ�ؼ���
{
    for(auto k:KeyWordsMap)
        if(k.first == s)
            return true;
    return false;
}



//�Զ�ȡ��һ���ַ��������ж�
void Judge(string strToken,int row,int col)  
{
    int len = strToken.size();
    int isvaild = 1;
    if(IsLetter(strToken[0])) //��ĸ��ͷ
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

    else if(IsNumber(strToken[0])) // ���ֿ�ͷ
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

//�ʷ��������
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
        getline(InputFile,LineCode);        //��ȡһ��
        row++;                              //�к�+1
        col = 0;                            //�д�0��ʼ
        while(LineCode[col] != '\0')        //û�е���β
        {
            curch = LineCode[col++];        //��ǰ�жϵ��ַ�
            nextch = LineCode[col];         //��һ���ַ���ʵ�ֳ�ǰ����

            if(curch == ' '||curch == '\t'||curch == '\n')
            {
                   //cout<<"0"<<endl;                         //�����༭���ַ�                
            }
            else if(IsLetter(curch) || IsNumber(curch))        //��ĸ��ͷ�������ֿ�ͷ
            {
               strToken.clear();           //���strToken
               while(!IsBoundary(curch) && !IsOperator(curch)) //���Ǳ߽�����Ҳ��������ַ�����ȡһ���ַ���
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

                    col--;  //����һ��
                    // xy4����ʶ��xy4$������
                    if(IsKeyWord(strToken))
                    {
                        //OutputFile<<"$�ؼ���  "<<strToken<<endl;
                        strToken.clear();
                    }
                    else
                    {
                        //OutputFile<<"$��ʶ�� "<<strToken<<endl;
                        strToken.clear();
                    } 
                

                }
                else if(IsNumber(curch)) //���ֿ�ͷ   9c87�����ʷ������������
                {
                    strToken.clear();
                    while(IsNumber(curch))
                    {
                        strToken += curch;
                        curch = LineCode[col++];
                    }
                    col--;   //����һ��
                    //OutputFile<<"$����  "<<strToken<<endl; 
                */
            }   
            else if(curch == '/')  //����ע�ͺͳ���������û��ע�ͣ�
            {
                if(nextch == '/')
                    while(LineCode[col]!='\0')
                        col++;
                else  //����
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
            else if(curch == '+')  //�ӷ�
            {
                //OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;                
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = ADD;
                word.content = curch;
                SourceWords.push_back(word);  
            }
            else if(curch == '-') //����
            {
                //OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = SUB;
                word.content = curch;
                SourceWords.push_back(word);  
            }
            else if(curch == '*')  //�˷�
            {
                //OutputFile<<row<<" "<<col<<" $operator "<<curch<<endl;
                word.row = row;
                word.col = col;
                word.type = Operator;
                word.value = MUL;
                word.content = curch;
                SourceWords.push_back(word);  
            }
            else if(curch == '^')  //�˷�
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
                if(nextch == '=')  //��ֵ
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
                else //����
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
                if(nextch == '=')  //С�ڵ���
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
                else if(nextch == '>') //������
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
                else  //С��
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
                if(nextch == '=') //���ڵ���
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
                else  //����
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