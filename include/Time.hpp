#ifndef TIME_HPP
#define TIME_HPP
#include<string>
using std::string;

using Time=int;
using Date=int;

Time str_to_time(string st){
    int h=(st[0]-'0')*10+st[1]-'0';
    int m=(st[3]-'0')*10+st[4]-'0';
    return h*60+m;
}
string time_to_str(Time time){
    int h=time/60,m=time%60;
    string st="";
    st+=(char)('0'+h/10);st+=(char)('0'+h%10);st+=':';
    st+=(char)('0'+m/10);st+=(char)('0'+m%10);
    return st;
}

const int Month_dates[]={0,31,28,31,30,31,30,31,31,30,31,30,31};

Date str_to_date(string st){
    int m=(st[0]-'0')*10+st[1]-'0';
    int d=(st[3]-'0')*10+st[4]-'0';
    Date date=d;
    for(int i=1;i<m;i++)date+=Month_dates[i];
    return date;
}
string date_to_str(Date date){
    int m,d;
    for(int i=1;i<=12;i++){
        if(date<=Month_dates[i]){m=i,d=date;break;}
        date-=Month_dates[i];
    }
    string st="";
    st+=(char)('0'+m/10);st+=(char)('0'+m%10);st+='-';
    st+=(char)('0'+d/10);st+=(char)('0'+d%10);
    return st;
}
string date_time_to_str(Date date,Time time){
    return date_to_str(date)+" "+time_to_str(time);
}

Date date_calc(Date date,Time time,int delta){
    return date+(time+delta)/1440;
}
Time time_calc(Date date,Time time,int delta){
    return (time+delta)%1440;
}

string date_time_calc(Date date,Time time,int delta){
    date+=(time+delta)/1440;
    time=(time+delta)%1440;
    return date_to_str(date)+" "+time_to_str(time);
}

int calc_interval(Date d1,Time t1,Date d2,Time t2){
    return (d2-d1)*1440+t2-t1;
}
#endif