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
    mem1.initialise("tickets");
    mem2.initialise("order");
    UserManager UM(&bpt1,&bpt2);
    TrainManager TM(&bpt3,&mem1,&bpt4,&bpt5,&bpt6,&bpt);
    OrderManager OM(&bpt2,&mem1,&bpt4,&mem2,&bpt8,&bpt9);
    while(1){
        string s;
        getline(cin,s);
        vector<string> v=Parse(s);
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
    }
    return 0;
}