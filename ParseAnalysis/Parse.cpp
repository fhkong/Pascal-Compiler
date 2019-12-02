#include<bits/stdc++.h>
#include "Define.h"
using namespace std;

/**********************************
 * FileName : Parse.cpp
 * Author   : fhkong
 * Time     : 2019.10.3
 * Version  : 1.1.0
 ********************************/


const int MAX_PROC = 100;
const int MAX_LENGTH = 100;
const int MAX_NORTOR = 50;
const int MAX_TOR = 50;

int PROC_LINE = 0;  //PROC数组中的行数,从1开始


int PROC[MAX_PROC][MAX_LENGTH];  //存放产生式
int FIRST[MAX_PROC][MAX_LENGTH];//first 集
int FOLLOW[MAX_PROC][MAX_LENGTH]; //follow 集
int empty[MAX_NORTOR];  //能推出空的非终结符集合
int firstVisit[MAX_NORTOR]; //求解FIRST集时，非终结符是否已求过
int emptyRecu[MAX_NORTOR];//在求可推出空的非终结符的编号集时使用的防治递归的集合  
int first[MAX_NORTOR][MAX_LENGTH];
int followRecu[MAX_NORTOR];//在求Follow集时使用的防治递归的集合  
int follow[MAX_NORTOR][MAX_LENGTH];
int followVisit[MAX_PROC];//记录某非终结符的Follow集是否已经求过 
int connectFirst[MAX_LENGTH];//将某些First集结合起来的集合  
int M[MAX_NORTOR][MAX_TOR][MAX_LENGTH];//分析预测表
int select[MAX_PROC][MAX_LENGTH];  


vector< pair<string,int> > KeyMap;   //
vector< pair<string,int> > NorTerMap;
vector< pair<string,int> > TerMap;
vector< pair<string,int> > SpecialMap;
vector< pair<string,int> > OperMap;
vector< pair<string,int> > LimitMap;

void InitKeyMap()
{
    KeyMap.clear();
    KeyMap.push_back(make_pair("program",PROGRAM)); 
    KeyMap.push_back(make_pair("const",CONST)); 
    KeyMap.push_back(make_pair("var",VAR)); 
    KeyMap.push_back(make_pair("procedure",PROCEDURE)); 
    KeyMap.push_back(make_pair("begin",BEGIN)); 
    KeyMap.push_back(make_pair("end",END)); 
    KeyMap.push_back(make_pair("if",IF)); 
    KeyMap.push_back(make_pair("else",ELSE)); 
    KeyMap.push_back(make_pair("while",WHILE)); 
    KeyMap.push_back(make_pair("do",DO)); 
    KeyMap.push_back(make_pair("call",CALL)); 
    KeyMap.push_back(make_pair("read",READ)); 
    KeyMap.push_back(make_pair("write",WRITE)); 
    KeyMap.push_back(make_pair("odd",ODD)); 
    KeyMap.push_back(make_pair("then",THEN));
    cout<<"*****************KeyMap initial is ok!***************"<<endl;
}

void InitOperMap()
{
    OperMap.clear();
    OperMap.push_back(make_pair(":=",ASSIGN));
    OperMap.push_back(make_pair("<",LESS_THAN));
    OperMap.push_back(make_pair("<=",LESS_EQUA));
    OperMap.push_back(make_pair(">",GRT_THAN));
    OperMap.push_back(make_pair(">=",GRT_EQUA));
    OperMap.push_back(make_pair("+",ADD));
    OperMap.push_back(make_pair("-",SUB));
    OperMap.push_back(make_pair("*",MUL));
    OperMap.push_back(make_pair("/",DIV));
    OperMap.push_back(make_pair("^",POWER));
    OperMap.push_back(make_pair("<>",NOREQUA));
    OperMap.push_back(make_pair("=",EQUAL));

    cout<<"***************OperMap initial is ok!****************"<<endl; 
}
void InitBoundary()
{
    LimitMap.clear();
    LimitMap.push_back(make_pair("(",LEFT_SBRA));   
    LimitMap.push_back(make_pair(")",RIGHT_SBRA));
    LimitMap.push_back(make_pair("[",LEFT_MBRA));
    LimitMap.push_back(make_pair("]",RIGHT_MBRA));
    LimitMap.push_back(make_pair(".",POINTER));
    LimitMap.push_back(make_pair(",",COMMA));
    LimitMap.push_back(make_pair(";",SEMI));
    

    cout<<"*************BoundaryMap initial is ok!*****************"<<endl;
}

void InitSpecialMap()
{
    SpecialMap.clear();
    SpecialMap.push_back(make_pair("->",GRAMMAR_ARROW));
    SpecialMap.push_back(make_pair("|",GRAMMAR_OR));
    SpecialMap.push_back(make_pair("$",GRAMMAR_NULL));
    SpecialMap.push_back(make_pair("#",GRAMMAR_END));

    cout<<"****************SpecialMap initial is ok!*************"<<endl;
}

int DynamicNorTer(string word)  
{  
    int i = 0;  
    int dynamicNum;  
    for(i=0; i<NorTerMap.size(); i++)  
    {  
        if(word == NorTerMap[i].first)  
        {  
            return NorTerMap[i].second;  
        }  
    }  
    if(i == NorTerMap.size())  
    {  
        if(i == 0)  
        {  
            dynamicNum = NorTerBase;  
            NorTerMap.push_back(make_pair(word,dynamicNum));  
        }  
        else  
        {  
            dynamicNum = NorTerMap[NorTerMap.size()-1].second + 1;  
            NorTerMap.push_back(make_pair(word,dynamicNum));  
        }  
    }  
    return dynamicNum;  
}  

int SeekCodeNum(string word)  
{  
    //处理文法中的特殊符号  
    for(int i = 0; i<SpecialMap.size(); i++)  
    {  
        if(word == SpecialMap[i].first)  
        {  
            return SpecialMap[i].second;  
        }  
    }  
    //先搜索终结符映射表中有没有此终结符  
    for(int i=0; i<TerMap.size(); i++)  
    {  
        if(word == TerMap[i].first)  
        {  
            return TerMap[i].second;  
        }  
    }  
    for(int i = 0; i<KeyMap.size(); i++)  
    {  
        if(word == KeyMap[i].first)  
        {  
            TerMap.push_back(make_pair(word,KeyMap[i].second));  
            return KeyMap[i].second;  
        }  
    }  
  
    for(int i = 0; i<OperMap.size(); i++)  
    {  
        if(word == OperMap[i].first)  
        {  
            TerMap.push_back(make_pair(word,OperMap[i].second));  
            return OperMap[i].second;  
        }  
    }  
  
    for(int i = 0; i<LimitMap.size(); i++)  
    {  
        if(word == LimitMap[i].first)  
        {  
            TerMap.push_back(make_pair(word,LimitMap[i].second));  
            return LimitMap[i].second;  
        }  
    }  
  
    if( word == "id" )  
    {  
        //处理标志符  
        TerMap.push_back(make_pair(word,IDENTIFIER));  
        return IDENTIFIER;  
    }  
    else  
    {  
        //处理完关键字、运算符、限界符表，即非终结符  
        return DynamicNorTer(word);  
    }  
}  

bool SplitPROC(int p[][MAX_LENGTH],int &line,int orNum)  
{  
    if(p[line][1] == -1 || orNum == 0)  
    {  
        return false;  
    }  
    int head = p[line][1];  
    int push = p[line][2];  
    int length = 0;  
    int right,left;  
    int lineTrue = line + orNum;  
    for(length = 3;;length++)  
    {  
        if(p[line][length] == -1)  
        {  
            break;  
        }  
    }  
    length--;  
    for(left = length,right = length;left>=2;)  
    {  
        if(p[line][left] == GRAMMAR_OR || left == 2)  
        {  
            p[line + orNum][1] = head;  
            p[line + orNum][2] = push;  
            for(int i=left+1;i<=right;i++)  
            {  
                p[line+orNum][i-left+2] = p[line][i];  
            }  
            p[line+orNum][right-left+3] = -1;  
            right = left = left-1;  
            orNum--;  
        }  
        else  
        {  
            left--;  
        }  
    }  
    line = lineTrue;  

    return true;
}  

string SearchMapping(int num)  
{  
    //标志符  
    if(num == IDENTIFIER)  
    {  
        return "id";  
    }  
    //处理文法中的特殊符号  
    for(int i = 0; i<SpecialMap.size(); i++)  
    {  
        if(SpecialMap[i].second == num)  
        {  
            return SpecialMap[i].first;  
        }  
    }  
    //处理非终结符  
    for(int i=0; i<NorTerMap.size(); i++)  
    {  
        if(NorTerMap[i].second == num)  
        {  
            return NorTerMap[i].first;  
        }  
    }  
    //处理终结符  
    for(int i=0; i<TerMap.size(); i++)  
    {  
        if(TerMap[i].second == num)  
        {  
            return TerMap[i].first;  
        }  
    }  
    
    return " ";
}  
bool InitGrammar()
{
    ifstream InputFile;
    InputFile.open("Pascal.txt");
    if(InputFile.fail())
    {
        cout<<"Can't open the wenfa.txt"<<endl;
        return false;
    }


    char ch = '\0';
    string word = "";
    int line = 1;
    int orNum = 0;
    int count = 0;
    int codeNum = 0;

    InputFile.get(ch);
    while(ch != '\n')
    {
        word.clear();
        while(ch == ' '||ch == '\t')
        {
            InputFile.get(ch);
        }
        while(ch != ' ' && ch != '\n')
        {
            word+=ch;
            InputFile.get(ch);
        }
        while(ch == ' '||ch == '\t')  //过滤掉无用字符
        {
            InputFile.get(ch);
        }
        codeNum = 0;
       // cout<<word<<endl;
        codeNum = SeekCodeNum(word);
        if(codeNum != 0)
        {
            count++;
            if(codeNum == GRAMMAR_OR)
            {
                orNum++;
            }
            PROC[line][count] = codeNum;
        }

        if(ch == '\n')
        {
            SplitPROC(PROC,line,orNum);
            count  = 0;
            orNum = 0;
            line++;
            InputFile.get(ch);
        }
    }

    cout<<"****************Pascal Grammar*******************"<<endl;
    for(int i=1;i<line;i++)
    {
        for(int j=1;j<MAX_LENGTH;j++)
        {
            if(PROC[i][j] != -1)
            {
                cout<<SearchMapping(PROC[i][j])<<" ";
            }
            else
            {
                break;
            }
        }
        cout<<endl;
    }

    PROC_LINE = line-1;  //PROC数组中的行数，从1开始

    cout<<"***************Terminal char******************"<<endl;
    for(int i=0;i<TerMap.size();i++)
    {
        cout<<TerMap[i].first<<" ";
    }
    cout<<endl;
    cout<<"**************NorTerminal char****************"<<endl;
    //cout<<NorTerMap.size()<<endl;
    for(int i=0;i<NorTerMap.size();i++)
    {
        cout<<NorTerMap[i].first<<" ";
    }
    cout<<endl;
    return true;
}

//将s集合合并至d集合中，type = 1代表包括空（$）,type = 2代表不包括空  
void merge(int *d,int *s,int type)  
{  
    int flag = 0;  
    for(int i = 0;; i++)  
    {  
        flag = 0;  
        if(s[i] == -1)  
        {  
            break;  
        }  
        int j = 0;  
        for(j = 0;; j++)  
        {  
            if(d[j] == -1)  
            {  
                break;  
            }  
            if(d[j] == s[i])  
            {  
                flag = 1;  
                break;  
            }  
        }  
        if(flag == 1)  
        {  
            continue;  
        }  
        if(type == 1)  
        {  
            d[j] = s[i];  
        }  
        else  
        {  
            if(s[i] != GRAMMAR_NULL)  
            {  
                d[j] = s[i];  
            }  
        }  
        d[j + 1] = -1;  
    }  
}  


//判断某个标号是不是终结符的标号，1代表是，0代表否  
int inTer(int n)  
{  
    for(int i=0; i<TerMap.size(); i++)  
    {  
        if(TerMap[i].second == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
}  

//判断某个标号是不是非终结符的标号,1代表是，0代表否  
int inNonTer(int n)  
{  
    for(int i=0; i<NorTerMap.size(); i++)  
    {  
        if(NorTerMap[i].second == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
}  

//判断某个标号在不在此时的emptyRecu集中，1代表是，0代表否  
int inEmptyRecu(int n)  
{  
    //当前Empty集的长度  
    int emptyLength = 0;  
    for(emptyLength = 0;; emptyLength++)  
    {  
        if(emptyRecu[emptyLength] == -1)  
        {  
            break;  
        }  
    }  
    for(int i = 0; i<emptyLength; i++)  
    {  
        if(emptyRecu[i] == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
}  

//判断某个标号在不在此时的empty集中，1代表是，0代表否  
//empty集中存放能够推出空的非终结符
int inEmpty(int n)  
{  
    //当前Empty集的长度  
    int emptyLength = 0;  
    for(emptyLength = 0;; emptyLength++)  
    {  
        if(empty[emptyLength] == -1)  
        {  
            break;  
        }  
    }  
    for(int i = 0; i<emptyLength; i++)  
    {  
        if(empty[i] == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
  
}  

//判断某个标号是不是在产生式的右边  
int inProcRight(int n,int * p)  
{  
    //注意这里默认是从3开始  
    for(int i=3;; i++)  
    {  
        if(p[i] == -1)  
        {  
            break;  
        }  
        if(p[i] == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
}  

//判断某个标号在不在此时的followRecu集中，1代表是，0代表否  
int inFollowRecu(int n)  
{  
    int followLength = 0;  
    for(followLength = 0;; followLength++)  
    {  
        if(followRecu[followLength] == -1)  
        {  
            break;  
        }  
    }  
    for(int i = 0; i<followLength; i++)  
    {  
        if(followRecu[i] == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
}  


//判断该非终结符是否能推出空，但终结符也可能传入，但没关系  
int reduNull(int currentNon)  
{  
    int temp[2];  
    int result = 1;  
    int mark = 0;  
    temp[0] = currentNon;  
    temp[1] = -1;  
    merge(emptyRecu,temp,1);//先将此符号并入防递归集合中  
    if(inEmpty(currentNon) == 1)  
    {  
        return 1;  
    }  
  
    for(int j = 1; j<=PROC_LINE; j++)  
    {  
        if(PROC[j][1] == currentNon)  
        {  
            int rightLength = 0;  
            //先求出右部的长度  
            for(rightLength = 3;; rightLength++)  
            {  
                if(PROC[j][rightLength] == -1)  
                {  
                    break;  
                }  
            }  
            rightLength--;  
            //如果长度为1，并且已经求过  
            if(rightLength - 2 == 1 && inEmpty(PROC[j][rightLength]))  
            {  
                return 1;  
            }  
            //如果长度为1，并且是终结符  
            else if(rightLength -2 == 1 && inTer(PROC[j][rightLength]))  
            {  
                return 0;  
            }  
            //如果长度超过了2  
            else  
            {  
                for(int k=3; k<=rightLength; k++)    //A->BA
                {  
                    if(inEmptyRecu(PROC[j][k]))  
                    {  
                        mark = 1;  
                    }  
                }  
                if(mark == 1)  
                {  
                    continue;  
                }  
                else  
                {  
                    for(int k=3; k<=rightLength; k++)  
                    {  
                        result*= reduNull(PROC[j][k]);   //有一个推不出来就推不出来
                        temp[0] = PROC[j][k];  
                        temp[1] = -1;  
                        merge(emptyRecu,temp,1);//先将此符号并入防递归集合中  
                    }  
                }  
            }  
            if(result == 0)  
            {  
                continue;  
            }  
            else if(result == 1)  
            {  
                return 1;  
            }  
        }  
    }  
    return 0;  
}  


void nullSet(int currentNum)  
{  
    int temp[2];  
    for(int j = 1; j<=PROC_LINE; j++)  
    {  
        //如果右边的第一个是该字符，并且长度只有1  
        if(PROC[j][3] == currentNum && PROC[j][4] == -1)  
        {  
            temp[0] = PROC[j][1];  
            temp[1] = -1;  
            merge(empty,temp,1);  
            nullSet(PROC[j][1]);  //B->A A->$的情况
        }  
    }  
}  

void firstSet(int i)  
/*************************************************************
 * step1. A->β if β == $ | β == Terminal char add First(β) to First(A)
 * step2. A->α... add First(α) to First(A)
 * **********************************************************/
{  
    int k = 0;  
    int currentNon = NorTerMap[i].second;//当前的非终结符标号  
    //依次遍历全部产生式  
    for(int j = 1; j<=PROC_LINE; j++) //j代表第几个产生式  
    {  
        //找到该非终结符的产生式  
        if(currentNon == PROC[j][1])//注意从1开始  
        {  
            //当右边的第一个是终结符或者空的时候  
            if(inTer(PROC[j][3]) == 1 || PROC[j][3] == GRAMMAR_NULL)  
            {  
                //并入当前非终结符的first集中  
                int temp[2];  
                temp[0] = PROC[j][3];  
                temp[1] = -1;//其实是模拟字符串操作的手段  
                merge(first[i],temp,1);  
            }  
            //当右边的第一个是非终结符的时候  
            else if(inNonTer(PROC[j][3]) == 1)  
            {  
                //如果遇到左递归形式的，直接放过 
                if(PROC[j][3] == currentNon)  
                {  
                    continue;  
                }  
                //记录下右边第一个非终结符的位置  
                for(k=0;; k++)  
                {  
                    if(NorTerMap[k].second == PROC[j][3])  
                    {  
                        break;  
                    }  
  
                }  
                //当右边第一个非终结符还未访问过的时候  
                if(firstVisit[k] == 0)  
                {  
                    firstSet(k);  
                    firstVisit[k] = 1;  
                }  
                merge(first[i],first[k],2);//如果first[k]此时有空值的话，暂时不把空值并入first[i]中  
                int rightLength = 0;  
                //先求出右部的长度  
  
                for(rightLength = 3;; rightLength++)  
                {  
                    if(PROC[j][rightLength] == -1)  
                    {  
                        break;  
                    }  
                }  
                //到目前为止，只求出了右边的第一个(还不包括空的部分)，For循环处理之后的  
                for(k = 3; k<rightLength; k++)  
                {  
                    emptyRecu[0] = -1;//相当于初始化这个防递归集合  
  
                    //如果右部的当前字符能推出空并且还不是最后一个字符，就将之后的一个字符并入First集中  
                    if(reduNull(PROC[j][k]) == 1 && k<rightLength -1)  
                    {  
                        int u = 0;  
                        for(u=0;; u++)  
                        {  
                            //注意是记录下一个符号的位置  
                            if(NorTerMap[u].second == PROC[j][k+1])  
                            {  
                                break;  
                            }  
                        }  
                        if(firstVisit[u] == 0)  
                        {  
                            firstSet(u);  
                            firstVisit[u] = 1;  
                        }  
                        merge(first[i],first[u],2);  
                    }  
                    //到达最后一个字符，并且产生式右部都能推出空,将$并入First集中  
                    else if(reduNull(PROC[j][k]) == 1 && k == rightLength -1)  
                    {  
                        int temp[2];  
                        temp[0] = GRAMMAR_NULL;  
                        temp[1] = -1;//其实是模拟字符串操作的手段  
                        merge(first[i],temp,1);  
                    }  
                    else  
                    {  
                        break;  
                    }  
                }  
            }  
        }  
    }  
    firstVisit[i] = 1;  
}  

void First()  
{  
    //先求出能直接推出空的非终结符集合  
    nullSet(GRAMMAR_NULL);  
    for(int i=0; i<NorTerMap.size(); i++)  
    {  
        firstSet(i);  
    }  
    cout<<"************************************First******************************"<<endl;  
    for(int i=0; i<NorTerMap.size(); i++)  
    {  
        cout<<"FIRST["<<NorTerMap[i].first<<"] = ";  
        for(int j=0;; j++)  
        {  
            if(first[i][j] == -1)  
            {  
                break;  
            }  
            cout<<SearchMapping(first[i][j])<<" ";  
        }  
        cout<<endl;
    }  
}  

void connectFirstSet(int *p)
/*************************
 * connect first(i) first(j) ... to connectFirst Array
 * step1. if len(p) == 1 then merge First(p[0]) to connectFirst
 * step2. if len(p) > 1  then merge First(p[0]) First(p[1]) ... to connectFirst
*************************/ 
{  
    int i = 0;  
    int flag = 0;  
    int temp[2];    
    if(p[1] == -1)  //if len(p) == 1  
    {  
        if(p[0] == GRAMMAR_NULL)  //p[0] is $
        {  
            connectFirst[0] = GRAMMAR_NULL;  
            connectFirst[1] = -1;  
        }  
        else  
        {  
            for(i=0; i<NorTerMap.size(); i++)  //p[0] is norterminal char  
            {  
                if(NorTerMap[i].second == p[0])  
                {  
                    flag = 1;  
                    merge(connectFirst,first[i],1);  
                    break;  
                }  
            }  
            //p[0] is terminal char  
            if(flag == 0)  
            {  
                for(i=0; i<TerMap.size(); i++)  
                {  
                    if(TerMap[i].second == p[0])  
                    {  
                        temp[0] = TerMap[i].second;  
                        temp[1] = -1;  
                        merge(connectFirst,temp,2);//add this termianl char to connectFirst  
                        break;  
                    }  
                }  
            }  
        }  
    }  
    //else len(p) > 1  
    else  
    {  
        //p[0] is norterminal char
        for(i=0; i<NorTerMap.size(); i++)  
        {  
            if(NorTerMap[i].second == p[0])  
            {  
                flag = 1;  
                merge(connectFirst,first[i],2);  
                break;  
            }  
        }  
        //p[0] is terminal  char
        if(flag == 0)  
        {  
            for(i=0; i<TerMap.size(); i++)  
            {  
                if(TerMap[i].second == p[0])  
                {  
                    temp[0] = TerMap[i].second;  
                    temp[1] = -1;  
                    merge(connectFirst,temp,2);  
                    break;  
                }  
            }  
        }  
        flag = 0;  
        int length = 0;  

        //cal the length of p
        for(length = 0;; length++)  
        {  
            if(p[length] == -1)  
            {  
                break;  
            }  
        }

        for(int k=0; k<length; k++)  
        {  
            emptyRecu[0] = -1;//Init the Array  
  
            //如果右部的当前字符能推出空并且还不是最后一个字符，就将之后的一个字符并入First集中  
            if(reduNull(p[k]) == 1 && k<length -1)  
            {  
                int u = 0;  
                for(u=0; u<NorTerMap.size(); u++)  
                {  
                    //记录下一个符号的位置  
                    if(NorTerMap[u].second == p[k+1])  
                    {  
                        flag = 1;  
                        merge(connectFirst,first[u],2);  
                        break;  
                    }  
                }

                //也可能是终结符  
                if(flag == 0)  
                {  
                    for(u=0; u<TerMap.size(); u++)  
                    {  
                        //注意是记录下一个符号的位置  
                        if(TerMap[u].second == p[k+1])  
                        {  
                            temp[0] = TerMap[i].second;  
                            temp[1] = -1;  
                            merge(connectFirst,temp,2);  
                            break;  
                        }  
                    }  
                }  
                flag = 0;  
            }  
            //到达最后一个字符，并且产生式右部都能推出空,将$并入First集中  
            else if(reduNull(p[k]) == 1 && k == length -1)  
            {  
                temp[0] = GRAMMAR_NULL;  
                temp[1] = -1;         
                merge(connectFirst,temp,1);  
            }  
            else  
            {  
                break;  
            }  
        }  
    }  
}  


void followSet(int i) 
/*****************************************
 * if S , add # to follow(S)
 * if S->...A , then add follow(S) to follow(A)
 * if S->αAβ , then if $ in First(β) then add follow(S) to follow(A)
 *             add First(β) to follow(A)
 * **************************************/
{  
    int currentNon = NorTerMap[i].second;  //current norterminal char's number in pair<string,int>  
    int temp[2];  
    int result = 1;  
    temp[0] = currentNon;  
    temp[1] = -1;  
    merge(followRecu,temp,1);    //将当前标号加入防递归集合中  
  
    //current char is start char S,add #  to follow(s)
    if(PROC[1][1] == currentNon)  
    {  
        temp[0] = GRAMMAR_END;       //add # to FOLLOW(S)  
        temp[1] = -1;  
        merge(follow[i],temp,1);     //merge temp to follow(i)
    }  

    for(int j = 1; j<=PROC_LINE; j++)   
    {  
        //if current norterminal char exist in the right of proc . S->Currentchar  
        if(inProcRight(currentNon,PROC[j]) == 1)  
        {  
            int rightLength = 1;              
            int k = 0;  //k为该非终结符在产生式右部的序号  
            int flag = 0;  
            int leftNum = PROC[j][1];       //the proc[j]'left char  
            int h = 0;                      //leftnum in the vector<pari<string,int> 's id
            int kArray[MAX_LENGTH];         //temp array
            memset(kArray,-1,sizeof(kArray));  

            //calculate the h          
            for(h = 0; h < NorTerMap.size(); h++)  
            {  
                if(NorTerMap[h].second == leftNum)  
                {  
                    break;  
                }  
            }  
  
            for(rightLength = 1; ; rightLength++)  
            {  
                if(currentNon == PROC[j][rightLength+2])  
                {  
                    kArray[k++] = rightLength;  //record the currentNon in the proc right location
                }  
                if(PROC[j][rightLength+2] == -1)  
                {  
                    break;  
                }  
            }  
            rightLength--;  

            for(int y=0;; y++)  
            {  
                if(kArray[y] == -1)  
                {  
                    break;  
                }  
                //currentNon in the end of proc[j]  
                if(kArray[y] == rightLength)  
                {  
                    if(inFollowRecu(leftNum) == 1)   //A->BA
                    {  
                        merge(follow[i],follow[h],1);  
                        continue;  
                    }  
                    if(followVisit[h] == 0)  //proc[j]'left follow hasn't known
                    {  
                        followSet(h);       //Recu to cal follow(h)  
                        followVisit[h] = 1;  
                    }  
                    merge(follow[i],follow[h],1);  
                }  
                //else current isn't in the end of proc[j]  S->BACD..  (cal follow(A))  
                /**********************************
                 * S->BACD...   to cal follow(A)
                 * step1. check CD...->$ ? if can then add follow(S) to follow(A)
                 * step2. add first(CD...) to follow(A)
                 * *********************************/
                else  
                {  
                    int n = 0;  
                    result = 1;     //this step is very important 
                    for(n=kArray[y]+1; n<=rightLength; n++)  
                    {  
                        emptyRecu[0] = -1;  
                        result *= reduNull(PROC[j][n+2]); //check left chars of currentNon can get $ or not 
                    }  
                    if(result == 1)  //if all of left chars of currentNon can get $. CD...->$
                    {  
                        if(inFollowRecu(leftNum) == 1)  
                        {  
                            merge(follow[i],follow[h],1);  
                            continue;  
                        }  
                        if(followVisit[h] == 0)  
                        {  
                            followSet(h);  
                            followVisit[h] = 1;  
                        }  
                        merge(follow[i],follow[h],1);  //add follow(left) to follow(currentNon)
                    }  

                    int temp2[MAX_LENGTH];  
                    memset(temp2,-1,sizeof(temp2));  
                    for(n=kArray[y]+1; n<=rightLength; n++)  
                    {  
                        temp2[n-kArray[y]-1] = PROC[j][n+2];  
                    }  
                    temp2[rightLength-kArray[y]] = -1;  
                    connectFirst[0] = -1;//Initial connectFirst Array  
                    connectFirstSet(temp2); //Cal first(CD...)  
                    merge(follow[i],connectFirst,2);  //merge first(CD...) to follow(i) without $
                }  
            }  
        }  
    }  
    followVisit[i] = 1;  
}  
  
//求所有非终结符的Follow集  
void Follow()  
{  
    for(int i=0; i<NorTerMap.size(); i++)  
    {  
        followRecu[0] = -1;  //对防递归的数组进行初始化  
        followSet(i);  
    }  

    cout<<"************************************Follow******************************"<<endl;
    
    for(int i=0; i<NorTerMap.size(); i++)  
    {  
        cout<<"FOLLOW["<<NorTerMap[i].first<<"] = ";  
        for(int j=0;; j++)  
        {  
            if(follow[i][j] == -1)  
            {  
                break;  
            }  
            cout<<SearchMapping(follow[i][j])<<" ";  
        }  
        cout<<endl;
    }  
}  

//求已经分解的产生式对应的Select集,注意Select集中不能含有空($),因而Type=2  
void Select()  
/******************************************************************
一个产生式的选择符号集SELECT。
给定上下文无关文法的产生式A→α,A∈VN,α∈V∗A→α,A∈VN,α∈V∗
若α⇏∗ε ,则SELECT(A→α) = FIRST(α)
若α⇒∗ε ,则SELECT(A→α)=(FIRST(α)−{ε})⋃ FOLLOW(A)

用于生成分析表时，当前产生式填在哪，相当于综合了FIRST和FOLLOW
*******************************************************************/
{  
    for(int i = 1; i<=PROC_LINE; i++) //i代表第几个产生式  
    {  
        int leftNum = PROC[i][1];//产生式的左边  
        int h = 0;   //产生式左边的非终结符在数组中的位置
        int result = 1;  
        for(h = 0; h < NorTerMap.size(); h++)  
        {  
            if(NorTerMap[h].second == leftNum)  
            {  
                break;  
            }  
        }  
  
        int rightLength = 1; //产生式右边的长度
        for(rightLength = 1;; rightLength++)  
        {  
            if(PROC[i][rightLength+2] == -1)  
            {  
                break;  
            }  
        }  
        rightLength--;  
        //如果右部推出式的长度为1并且是空,select[i-1] = follow[左边]  
        if(rightLength == 1 && PROC[i][rightLength + 2] == GRAMMAR_NULL)  
        {  
            merge(select[i-1],follow[h],2);  
        }  
        //如果右部不是空的时候,select[i-1] = first[右部全部]  
        //如果右部能够推出空，select[i-1] = first[右部全部] ^ follow[左边]  
        else  
        {  
            int temp2[MAX_LENGTH];  
            int n = 0;  
            memset(temp2,-1,sizeof(temp2));  
            for(n=1; n<=rightLength; n++)  
            {  
                temp2[n-1] = PROC[i][n+2];  
            }  
            temp2[rightLength] = -1;  
            connectFirst[0] = -1;//应该重新初始化一下  
            connectFirstSet(temp2);  
            merge(select[i-1],connectFirst,2);  
            for(n=1; n<=rightLength; n++)  
            {  
                emptyRecu[0] = -1;  
                result *= reduNull(PROC[i][n+2]);  
            }  
            //如果右部能推出空，将follow[左边]并入select[i-1]中  
            if(result == 1)  
            {  
                merge(select[i-1],follow[h],2);  
            }  
        }  
    }  
    printf("\n************************************Select******************************\n\n");  
    for(int i=0; i<PROC_LINE; i++)  
    {  
        cout<<"Select["<<i+1<<"] = ";  
        for(int j=0;; j++)  
        {  
            if(select[i][j] == -1)  
            {  
                break;  
            }  
            cout<<SearchMapping(select[i][j])<<" ";  
        }  
        cout<<endl;  
    }  
}  
//输出预测分析表  
void MTable()  
{  
    //将句子结束符#加入最后
    //int temp_num = TerMap[TerMap.size()-1].second + 1 ;
    TerMap.push_back(make_pair("#",GRAMMAR_END));  


    fstream outfile;  
    outfile.open("MTable.txt",ios::out);  
  
    for(int i=0; i<PROC_LINE; i++)   //select index from 0 ,while proc index from 1
    {  
        int m = 0;//非终结符的序号 ,找到该非终结符在vector 中的序列号
        for(int t=0; t<NorTerMap.size(); t++)  
        {  
            if(NorTerMap[t].second == PROC[i+1][1])  
            {  
                m = t;  
                break;  
            }  
        }  
  
        for(int j=0; ; j++)  
        {  
            if(select[i][j] == -1)  
            {  
                break;  
            }  
            for(int k=0; k<TerMap.size(); k++)  
            {  
                if(TerMap[k].second == select[i][j])  
                {  
                    int n = 0;  
                    for(n=1; n<=MAX_LENGTH; n++)  
                    {  
                        M[m][k][n-1] = PROC[i+1][n];   //把产生式放进分析表中
                        if(PROC[i+1][n] == -1)  
                        {  
                            break;  
                        }  
                    }  
                    break;  
                }  
            }  
        }  
    }  
    //printf("\n*********************************预测分析表******************************\n\n");  
    outfile<<endl<<"*********************************预测分析表******************************"<<endl;  
    for(int i=0; i<NorTerMap.size(); i++)  
    {  
        for(int j=0; j<TerMap.size(); j++)  
        {  
            outfile<<"M["<<NorTerMap[i].first<<"]["<<TerMap[j].first<<"] = ";  
            //printf("M[%s][%s] = ",nonTerMap[i].first,terMap[j].first);  
            for(int k=0;; k++)  
            {  
                if(M[i][j][k] == -1)  
                {  
                    break;  
                }  
                outfile<<SearchMapping(M[i][j][k]);  
                //printf("%s ",searchMapping(M[i][j][k]));  
            }  
            outfile<<endl;  
            //printf("\n");  
        }  
        outfile<<endl<<endl;  
        //printf("\n\n");  
    }  
    outfile.close();  
}  
int Parse()
{
    //freopen("FFS.txt","w",stdout);
    memset(PROC,-1,sizeof(PROC));
    memset(empty,-1,sizeof(empty));
    memset(firstVisit,0,sizeof(firstVisit));
    memset(emptyRecu,-1,sizeof(emptyRecu));
    memset(first,-1,sizeof(first));
    memset(followRecu,-1,sizeof(followRecu));
    memset(follow,-1,sizeof(follow));
    memset(followVisit,0,sizeof(followVisit));
    memset(connectFirst,-1,sizeof(connectFirst));
    memset(M,-1,sizeof(M));
    memset(select,-1,sizeof(select));

    InitKeyMap();
    InitOperMap();
    InitSpecialMap();
    InitBoundary();
    InitGrammar();
    First();
    Follow();
    Select();
    MTable();


    return 0;
}