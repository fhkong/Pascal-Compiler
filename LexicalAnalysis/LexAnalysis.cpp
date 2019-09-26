#include<bits/stdc++.h>
using namespace std;

/********************************
FileName: Lexical Analysis
Author : fhkong
Time : 2019.9.22
Version: 1.1.0
********************************/
/******************************
1.֧�ֳ�ǰ����
2.֧�������Լ��Ȳ���
3.��ÿ���ַ������д������ǵ����ַ�
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
    for(auto k:KeyWords)
        if(k == s)
            return true;
    return false;
}



//�Զ�ȡ��һ���ַ��������ж�
void Judge(string strToken,int row,int col,ofstream &OutputFile)  
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
                OutputFile<<"*****Error: ��"<<row<<"�У���"<<col-len<<"�г��ַǷ���ʶ��: "<<strToken<<"*****"<<endl;
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
                    OutputFile<<"< $�ؼ���   "<<setw(10)<<setiosflags(ios::left)<<strToken<<" >"<<endl;
                    iskey = true;
                    break;
                }                
            }
            if(!iskey)
            {
                OutputFile<<"< $��ʶ��   "<<setw(10)<<setiosflags(ios::left)<<strToken<<" >"<<endl;
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
                OutputFile<<"*****Error: ��"<<row<<"�У���"<<col-len<<"�г��ַǷ���ʶ��: "<<strToken<<"*****"<<endl;
                isvaild = 0;
                break;
            }            
        }
        if(isvaild)
        {
            OutputFile<<setw(10)<<"< $����      "<<setw(10)<<setiosflags(ios::left)<<strToken<<" >"<<endl;
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
        getline(InputFile,LineCode);        //��ȡһ��
        row++;                              //�к�+1
        col = 0;                            //�кô�0��ʼ
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
               Judge(strToken,row,col,OutputFile);
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
                        OutputFile<<"$�ؼ���  "<<strToken<<endl;
                        strToken.clear();
                    }
                    else
                    {
                        OutputFile<<"$��ʶ�� "<<strToken<<endl;
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
                    OutputFile<<"$����  "<<strToken<<endl; 
                */
            }   
            else if(curch == '/')  //����ע�ͺͳ���������û��ע�ͣ�
            {
                if(nextch == '/')
                    while(LineCode[col]!='\0')
                        col++;
                else
                {
                    OutputFile<<"< $����      "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
                }                
            }
            else if(curch == '+')
            {
                OutputFile<<"< $�ӷ�      "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;                
            }
            else if(curch == '-')
            {
                OutputFile<<"< $����        "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == '*')  
            {
                OutputFile<<"< $�˷�        "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;               
            }
            else if(curch == '^')
            {
                OutputFile<<"< $�˷�      "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == ':')
            {
                if(nextch == '=')
                {
                    OutputFile<<"< $��ֵ      "<<curch<<setw(9)<<setiosflags(ios::left)<<nextch<<" >"<<endl;
                    col++;
                }
                else
                {
                    OutputFile<<"********Error: ��"<<row<<"��,��"<<col+1<<"��ȱ�� =  *******"<<endl;
                }                
            }
            else if(curch == '<')
            {
                if(nextch == '=')
                {
                    OutputFile<<"< $С�ڵ���  "<<curch<<setw(9)<<setiosflags(ios::left)<<nextch<<" >"<<endl;
                    col++;
                }
                else if(nextch == '>')
                {
                    OutputFile<<"< $������  "<<curch<<setw(9)<<setiosflags(ios::left)<<nextch<<" >"<<endl;
                    col++;
                }
                else
                {
                    OutputFile<<"< $С��    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
                }                
            }
            else if(curch == '>')
            {
                if(nextch == '=')
                {
                    OutputFile<<"< $���ڵ���    "<<curch<<setw(9)<<setiosflags(ios::left)<<nextch<<" >"<<endl;
                    col++;
                }
                else
                {
                    OutputFile<<"< $����    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
                }                
            }
            else if(curch == '"')
            {
                OutputFile<<"< $�ַ�����Ƿ�    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == ',')
            {
                OutputFile<<"< $�ֽ��    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == ';')
            {
                OutputFile<<"< $�ֽ��    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == '(')
            {
                OutputFile<<"< $�ֽ��    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            
            else if(curch == ')')
            {
                OutputFile<<"< $�ֽ��    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }
            else if(curch == '\'')
            {
                OutputFile<<"< $�ֽ��    "<<setw(10)<<setiosflags(ios::left)<<curch<<" >"<<endl;
            }          
            else
            {        
                if(curch == '=')
                {
                    OutputFile<<"*****Error: ��"<<row<<"�У���"<<col<<"�г��ַǷ��ַ� "<<curch<<"  ***** ";
                    OutputFile<<"����ǰ��ȱ��:"<<endl;
                }
                else
                {
                    OutputFile<<"*****Error: ��"<<row<<"�У���"<<col<<"�г��ַǷ��ַ� "<<curch<<"  ***** "<<endl;
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