#define __DEBUG
#include <iostream>
#include "bcdnum.hh"
using namespace std;
constexpr int calculate(int start,int end)
{
    int ret=0;
    for(int i=start;i<end;++i) ret+=i;
    return ret;
}
int main()
{
    cout<<"please enter a number:";
    string num;
    cin>>num;
    cout<<"the string is :"<<num<<endl;
    bcdcode code=num;
    cout<<"the bcdcode string is:"<<(string)code<<endl;
    long long lnum;
    cout<<"please enter a long long:";
    cin>>lnum;
    bcdcode tc=lnum;
    cout<<"the number is :"<<lnum<<endl;
    cout<<"the bcdcode is :"<<(string)tc<<endl;
    bcdcode result=tc+code;
    cout<<"the result is:"<<(string)result;
    cin.get();
}