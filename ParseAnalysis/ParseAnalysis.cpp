#include<bits/stdc++.h>
//#include "errortype.h"
using namespace std;


/********************************
FileName: Parse Analysis
Author : fhkong
Time : 2019.9.26
Version: 1.0.0
********************************/


struct Word   //�ʷ������������м���
{
    int row;
    int col;
    string type;
    string value;
};

/*********************** 
Word.type
1.$operator  ���������ϵ��
2.$const    ����
3.$keyword  �ؼ���
4.$identifier   ��ʶ��
5.$#     ������
6.$error     �ʷ����������
7.$boundary  �߽��
************************/

vector<Word> SourceWords;

vector<Word>::iterator iter;


ostream& operator<<(ostream &out,Word &W)
{
    out<<"["<<W.row<<","<<W.col<<"] "<<W.type<<" "<<W.value<<endl;
    return out;
}
istream& operator>>(istream &in,Word W)
{
    in>>W.row>>W.col>>W.type>>W.value;
    return in;
}


const string LexAlyRstURL = "LexAnalysisOutFile.txt";   //Lexical Analysis result input file url
const string ParseAlyRstURL = "output.txt" ;//Parse Analysis result output file url

bool ReadFile(ifstream &InputFile)
//read Lexical analysis file to vector 
{
    Word word;
    string line;    //read a line from Lexical Analysis result file each time
    stringstream stream;
   
    while(!InputFile.eof())
    {
        getline(InputFile,line);
        //cout<<line<<endl;
        if(line[0] == '*')  //Lexical Analysis error
        {
            line = "-1 -1 $error NULL";
        }
        stream<<line;
        stream>>word.row>>word.col>>word.type>>word.value;
        stream.clear();
        SourceWords.push_back(word);
    }

    line = "-1 -1 $# NULL";  //end of analysis add '#'
    stream<<line;
    stream>>word.row>>word.col>>word.type>>word.value;
    SourceWords.push_back(word);
    InputFile.close();
    return true;
}

void ADVANCE()
{
    iter++;
}

Word SYM()
{
    return *iter;
}


bool PROG()
//<prog>->program<id>;<block>
{
    Word curword = SYM();
    if(curword.type == "$keyword" && curword.value == "program" )
    {
        curword = SYM();
        if(curword.type == "$identifier")
        {
            curword = SYM();
            if(curword.type == "$operator" && curword.value == ";")
            {
                BLOCK();
            }
            else
            {
                error(); //ȱ��;
            }
            
        }
    }
    else 
    {
        
    }
}


bool BLOCK()
{
    Word curword = SYM();
    if(curword.type == "$keyword" && curword.value == "const")
    {
        CONDECL();
    }
}


bool CONDECL()
{
    Word curword = SYM();
    if(curword.type == "identifier")
}
int main()
{
    ifstream InputFile(LexAlyRstURL);
    if(InputFile.fail())
    {
        cout<<"Can't open the "<<LexAlyRstURL<<" file!"<<endl;
        return 0;
    }

    ofstream OutputFile(ParseAlyRstURL);
    if(OutputFile.fail())
    {
        cout<<"Can't open the "<<ParseAlyRstURL<<" file!"<<endl;
        return 0;
    }

    ReadFile(InputFile);

    iter = SourceWords.begin();

    PROG();

}
