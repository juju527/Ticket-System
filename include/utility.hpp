#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include "vector.hpp"
using std::string;
using sjtu::vector;


template<int N>
class String{
private:
    int len;
    char s[N+1];
public:
    String(){len=0;}
    String(string st){
        len=st.length();
        for(int i=0;i<len;i++)s[i]=st[i];
    }
    
    int getlen()const{return len;}
    char operator[](int i)const{return s[i]}
    
    bool operator <(const String& tmp)const{
        for(int i=0;i<len&&i<tmp.len;i++){
            if(s[i]<tmp[i])return 1;
            if(s[i]>tmp[i])return 0;
        }
        return len<tmp.len;
    }
    bool operator ==(const String& tmp)const{
        if(len!=tmp.len)return 0;
        for(int i=0;i<len;i++)if(s[i]!=tmp[i])return 0;
        return 1;
    }

    friend std::ostream &operator<<(std::ostream &out, const String &st) {
        for(int i=0;i<st.len;i++)out<<st[i];
        return out;
    }
};

template<class T1,class T2>
class pair{
public:
    T1 first;
    T2 second;
    Pair(const T1& f,const T2&s):first(f),second(s){}
    
    Pair& operator =(const pair& other){
        first=other.first;
        second=other.second;
        return *this;
    }

    bool operator <(const pair& other)const{
        if(first==other.first)return second<other.second;
        return first<other.first;
    }
    bool operator ==(const pair& other)const{
        return first==other.first&&second==other.second;
    }
};

int str_to_int(string st){
    int res=0;
    for(int i=0;i<st.size();i++)res=res*10+st[i]-'0';
    return res;
}

vector<string> Parse(string st){
    vector<string> res;
    string cur="";
    for(int i=0;i<st.size();i++){
        if(st[i]==' ')res.push_back(cur),cur="";
        else cur+=st[i];
    }
    if(cur.size())res.push_back(cur);
    return res;
}

#endif