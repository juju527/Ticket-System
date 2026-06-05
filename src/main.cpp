#include<iostream>
#include "utility.hpp"
#include "User.hpp"
#include "Train.hpp"
#include "Order.hpp"
BPT<Username,User,100> bpt1("user");
BPT<Username,bool,100> bpt2("user_login");
BPT<TrainID,Train,50> bpt3("train");
MemoryRiver<Tickets,1> mem1;
BPT<TrainID,int,100> bpt4("train_mem1_id");
BPT<pair<Station,Station>,int,100> bpt5("train_s_t_mem1_id");
BPT<Station,int,100> bpt6("train_s_mem1_id");
BPT<Station,Info2,100> bpt("tmp");
MemoryRiver<Order,1> mem2;
BPT<Username,int,100> bpt8("user_mem2_id");
BPT<pair<TrainID,Date>,int,50> bpt9("pending_queue");
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    mem1.initialise("tickets");mem1.open();
    mem2.initialise("order");mem2.open();
    UserManager UM(&bpt1,&bpt2);
    TrainManager TM(&bpt3,&mem1,&bpt4,&bpt5,&bpt6,&bpt);
    OrderManager OM(&bpt2,&mem1,&bpt4,&mem2,&bpt8,&bpt9);
    while(1){
        string s;
        getline(cin,s);
        vector<string> v=Parse(s);
        cout<<v[0]<<" ";
        if(v[1]=="add_user"){
            Username cur,username;
            Password password;
            Name name;
            MailAddress mailaddress;
            int privilege;
            for(int i=2;i<v.size();i+=2){
                if(v[i]=="-c")cur=v[i+1];
                else if(v[i]=="-u")username=v[i+1];
                else if(v[i]=="-p")password=v[i+1];
                else if(v[i]=="-n")name=v[i+1];
                else if(v[i]=="-m")mailaddress=v[i+1];
                else if(v[i]=="-g")privilege=str_to_int(v[i+1]);
            }
            UM.add_user(cur,username,password,name,mailaddress,privilege);
        }
        else if(v[1]=="login"){
            Username username;
            Password password;
            for(int i=2;i<v.size();i+=2){
                if(v[i]=="-u")username=v[i+1];
                else if(v[i]=="-p")password=v[i+1];
            }
            UM.login(username,password);
        }
        else if(v[1]=="logout"){
            Username username=v[3];
            UM.logout(username);
        }
        else if(v[1]=="query_profile"){
            Username cur,username;
            for(int i=2;i<v.size();i+=2){
                if(v[i]=="-c")cur=v[i+1];
                else if(v[i]=="-u")username=v[i+1];
            }
            UM.query_profile(cur,username);
        }
        else if(v[1]=="modify_profile"){
            Username cur,username;
            for(int i=2;i<v.size();i+=2){
                if(v[i]=="-c")cur=v[i+1];
                else if(v[i]=="-u")username=v[i+1];
            }
            UM.modify_profile(cur,username,s);
        }
        else if(v[1]=="add_train"){
            TrainID trainID;
            int stationNum,seatNum;
            string stations;
            string prices;
            Time startTime;
            string travelTimes;
            string stopoverTimes;
            Date sales,salet;
            char type;
            for(int i=2;i<v.size();i+=2){
                if(v[i]=="-i")trainID=v[i+1];
                else if(v[i]=="-n")stationNum=str_to_int(v[i+1]);
                else if(v[i]=="-m")seatNum=str_to_int(v[i+1]);
                else if(v[i]=="-s")stations=v[i+1];
                else if(v[i]=="-p")prices=v[i+1];
                else if(v[i]=="-x")startTime=str_to_time(v[i+1]);
                else if(v[i]=="-t")travelTimes=v[i+1];
                else if(v[i]=="-o")stopoverTimes=v[i+1];
                else if(v[i]=="-d"){
                    string s="",t="";
                    int p=0;
                    for(int j=0;j<v[i+1].size();j++)if(v[i+1][j]=='|'){p=j+1;break;}else s+=v[i+1][j];
                    for(int j=p;j<v[i+1].size();j++)t+=v[i+1][j];
                    sales=str_to_date(s),salet=str_to_date(t);
                }
                else if(v[i]=="-y")type=v[i+1][0];
            }
            TM.add_train(trainID,stationNum,seatNum,stations,prices,startTime,travelTimes,stopoverTimes,sales,salet,type);
        }
        else if(v[1]=="delete_train"){
            TrainID trainID;
            for(int i=2;i<v.size();i+=2)
                if(v[i]=="-i")trainID=v[i+1];
            TM.delete_train(trainID);
        }
        else if(v[1]=="release_train"){
            TrainID trainID;
            for(int i=2;i<v.size();i+=2)
                if(v[i]=="-i")trainID=v[i+1];
            TM.release_train(trainID);
        }
        else if(v[1]=="query_train"){
            TrainID trainID;
            Date date;
            for(int i=2;i<v.size();i+=2){
                if(v[i]=="-i")trainID=v[i+1];
                else if(v[i]=="-d")date=str_to_date(v[i+1]);
            }
            TM.query_train(trainID,date);
        }
        else if(v[1]=="query_ticket"){
            Station s,t;
            Date date;
            bool op;
            for(int i=2;i<v.size();i+=2){
                if(v[i]=="-s")s=v[i+1];
                else if(v[i]=="-t")t=v[i+1];
                else if(v[i]=="-d")date=str_to_date(v[i+1]);
                else if(v[i]=="-p")op=(v[i+1]=="cost");
            }
            TM.query_tickets(s,t,date,op);
        }
        else if(v[1]=="query_transfer"){
            Station s,t;
            Date date;
            bool op=0;
            for(int i=2;i<v.size();i+=2){
                if(v[i]=="-s")s=v[i+1];
                else if(v[i]=="-t")t=v[i+1];
                else if(v[i]=="-d")date=str_to_date(v[i+1]);
                else if(v[i]=="-p")op=(v[i+1]=="cost");
            }
            TM.query_transfer(s,t,date,op);
        }
        else if(v[1]=="buy_ticket"){
            Username username;
            TrainID trainID;
            Date date;
            int num;
            Station from,to;
            bool op=0;
            for(int i=2;i<v.size();i+=2){
                if(v[i]=="-u")username=v[i+1];
                else if(v[i]=="-i")trainID=v[i+1];
                else if(v[i]=="-d")date=str_to_date(v[i+1]);
                else if(v[i]=="-n")num=str_to_int(v[i+1]);
                else if(v[i]=="-f")from=v[i+1];
                else if(v[i]=="-t")to=v[i+1];
                else if(v[i]=="-q")op=(v[i+1]=="true");
            }
            OM.buy_ticket(username,trainID,date,num,from,to,op);
        }
        else if(v[1]=="query_order"){
            Username username;
            for(int i=2;i<v.size();i+=2)
                if(v[i]=="-u")username=v[i+1];
            OM.query_order(username);
        }
        else if(v[1]=="refund_ticket"){
            Username username;
            int num=1;
            for(int i=2;i<v.size();i+=2){
                if(v[i]=="-u")username=v[i+1];
                else if(v[i]=="-n")num=str_to_int(v[i+1]);
            }
            OM.refund_ticket(username,num);
        }
        else if(v[1]=="clean"){
            UM.clear();
            TM.clear();
            UM.clear();
            cout<<0<<endl;
        }
        else if(v[1]=="exit"){
            cout<<"bye"<<endl;
            return 0;
        }
/*        if(v[1]=="buy_ticket"||v[1]=="refund_ticket"){
            string st="LeavesofGrass";
            if(bpt4.find(st).size()){
                int id=bpt4.find(st)[0];
                Tickets tickets;
                mem1.readorder(tickets,id);
                Train train=tickets.train;
                std::cerr<<v[0]<<" ";
                for(int i=0;i<train.stationNum-1;i++)std::cerr<<tickets.seat[0][i]<<" ";
                std::cerr<<endl;
            }
        }*/
    }
    return 0;
}