#include "Parse.cpp"
#include "LexAnalysis.cpp"
using namespace std;

//stack<int> Words;  //输入串
stack<int> S;  //分析栈


void PrintStack(stack<int> S)
{
    int n;
    while(!S.empty())
    {
        n = S.top();
        cout<<SearchMapping(n)<<" ";
        S.pop();
    }
}
bool Anlysis()
{
    Word EndWord;
    EndWord.row = EndWord.col = -1;
    EndWord.type = GRAMMAR_END;
    EndWord.value = GRAMMAR_END;
    EndWord.content = "#";

    SourceWords.push_back(EndWord);//将结束符圧进输入串
    vector<Word> :: iterator iter = SourceWords.begin(); //开始时指向第一个单词


    S.push(GRAMMAR_END); //# 压栈
    S.push(PROC[1][1]); //开始符号压栈 

    int STop;
    while(!S.empty())
    {
        PrintStack(S);
        cout<<endl;
        cout<<*iter<<endl;
        STop = S.top();

        if(STop == GRAMMAR_END)
        {
            if((*iter).type == GRAMMAR_END)
            {
                cout<<endl;
                cout<<"/*********************Analysis successfully!********************/"<<endl;
                return true;
            }
            else
            {
                //error(1)
                cout<<"Not Matching #"<<endl;
                return false;
            }            
        }
        else if(inTer(STop))  //栈顶是终结符
        {
            if((*iter).type == IDENTIFIER && STop == IDENTIFIER)  //标识符的value == -1 无意义
                {
                    iter++; S.pop();
                    cout<<"[OK!]  "<<SearchMapping(STop)<<endl;
                    cout<<endl;
                }
            else if((*iter).value == STop)  //其余的终结符都有value值都有意义
                {
                    iter++;
                    cout<<"[OK!]  "<<SearchMapping(S.top())<<endl;
                    S.pop();
                    cout<<endl;
                }
            else  //不匹配则进入错误处理程序
            {
                //error(2);
                cout<<"Not Match :"<<SearchMapping(STop)<<":"<<SearchMapping((*iter).value)<<endl;
            }         
        }
        else //栈顶元素为非终结符
        {
            int TerNum;  //记录当前输入单词的标号
            if((*iter).type == IDENTIFIER)
                TerNum = (*iter).type;
            else
            {
                TerNum = (*iter).value;
            }

            int t = 0;  //记录当前终结符在Map中的序列号
            for(int i=0;i<TerMap.size();i++)
            {
                if(TerMap[i].second == TerNum)
                {
                    t = i;
                    break;
                }                
            }

            int n = 0;
            for(int i=0;i<NorTerMap.size();i++)
            {
                if(NorTerMap[i].second == STop)
                {
                    n = i;
                    break;
                }
            }

            if(M[n][t][0] == -1)  //查表
            {
                //error(1);   //分析表中不存在对应的项，进入错误处理程序
                cout<<"M == -1  Not Found!"<<endl;
                return false;
            }
            else
            {
                cout<<"[OK!]  ";
                int Length=0;
                for(; ;Length++)
                {
                    if(M[n][t][Length] == -1)
                        break;
                    cout<<SearchMapping(M[n][t][Length]);
                }
                cout<<endl;
                cout<<endl;
                Length--;
                
                if(!S.empty())
                    S.pop();  //将当前栈顶的非终结符弹出
                else
                {
                    cout<<"Stack is empty! error"<<endl;    
                }

                for(; Length>=2;Length--)
                {
                    if(M[n][t][Length] != GRAMMAR_NULL)  //只将非空的入栈，出错了，呜呜呜
                        S.push(M[n][t][Length]);  //反序压栈
                }
            }
        }      
    }
    

    return true;
}
int main()
{
    LexAnaly();  //返回词法分析的结果
    Parse();
    int  i = 1;
    //for(auto a : SourceWords)
     //   cout<<i++<<" "<<a<<endl;
    Anlysis();
    return 0;
}