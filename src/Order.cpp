#include "Order.hpp"

OrderManager::OrderManager(BPT<Username,bool,100> *bpt2, MemoryRiver<Tickets,1> *mem1, BPT<TrainID,int,100> *bpt4, MemoryRiver<Order,1> *mem2, BPT<Username,int,100> *bpt8, BPT<pair<TrainID,Date>,int,50> *bpt9){
    this->bpt2=bpt2;
    this->mem1=mem1;
    this->bpt4=bpt4;
    this->mem2=mem2;
    this->bpt8=bpt8;
    this->bpt9=bpt9;
}
void OrderManager::clear(){
    mem2->clear();
    bpt8->clear();
    bpt9->clear();
    return ;
}

void OrderManager::buy_ticket(Username username, TrainID trainID, Date date, int num, Station s, Station t, bool op){
    vector<int> aux=bpt4->find(trainID);
    if(!bpt2->find(username).size()||!aux.size()){cout<<-1<<endl;return ;}
    Tickets tickets;
    mem1->readorder(tickets,aux[0]);
    Train train=tickets.train;
    int l,r,p;
    Order od;
    od.trainID=trainID;
    od.s=s,od.t=t;
    for(int i=0;i<train.stationNum;i++){
        if(train.stations[i]==s){
            int delta=train.travelTimes[i]+train.stopoverTimes[i];
            Date sd=date_calc(train.sales,train.startTime,delta);
            Time st=time_calc(train.sales,train.startTime,delta);
            if(sd>date||sd+train.salet-train.sales<date){cout<<-1<<endl;return ;}
            p=date-sd;l=i;od.d0=p;od.l=l;
            od.leavingDate=date;od.leavingTime=st;
        }
        else if(train.stations[i]==t){
            int delta=train.travelTimes[i]+train.stopoverTimes[i-1];
            Date td=date_calc(train.sales,train.startTime,delta);
            Time tt=time_calc(train.sales,train.startTime,delta);
            r=i;od.r=r;
            od.arrivingDate=td+p;od.arrivingTime=tt;
        }
    }
    int seat=1e9;
    for(int i=l;i<r;i++)seat=std::min(seat,tickets.seat[p][i]);
    if(seat>=num){
        int cost=(train.prices[r]-train.prices[l])*num;
        for(int i=l;i<r;i++)tickets.seat[p][i]-=num;
        mem1->writeorder(tickets,aux[0]);
        od.price=cost;od.num=num;
        od.status=Status::success;
        int tot;
        mem2->get_info(tot,1);
        mem2->writeorder(od,tot+1);
        bpt8->ins(username,tot+1);
        cout<<cost<<endl;
    }
    else{
        if(!op){cout<<-1<<endl;return ;}
        int cost=(train.prices[r]-train.prices[l])*num;
        od.price=cost;od.num=num;
        od.status=Status::pending;
        int tot;
        mem2->get_info(tot,1);
        mem2->writeorder(od,tot+1);
        bpt8->ins(username,tot+1);
        bpt9->ins(pair(username,p),tot+1);
        cout<<"queue"<<endl;
    }
    return ;
}

const string STATUS[]={"success","pending","refunded"};

void OrderManager::query_order(Username username){
    if(!bpt2->find(username).size()){cout<<-1<<endl;return ;}
    vector<int> aux=bpt8->find(username);
    cout<<aux.size()<<endl;
    for(int i=aux.size()-1;i>=0;i--){
        Order od;
        mem2->readorder(od,aux[i]);
        cout<<'['<<STATUS[od.status]<<"] "<<od.trainID<<" "<<od.s<<" "<<date_time_to_str(od.leavingDate,od.leavingTime)<<" -> "<<od.t<<" "<<date_time_to_str(od.arrivingDate,od.arrivingTime)<<" "<<od.price<<" "<<od.num<<endl;
    }
    return ;
}

void OrderManager::refund_ticket(Username username,int num){
    if(!bpt2->find(username).size()){cout<<-1<<endl;return ;}
    vector<int> aux=bpt8->find(username);
    if(aux.size()<num){cout<<-1<<endl;return ;}
    Order od;
    mem2->readorder(od,aux[aux.size()-num]);
    if(od.status==Status::refunded){cout<<-1<<endl;return ;}
    if(od.status==Status::pending){
        od.status=Status::refunded;
        mem2->writeorder(od,aux[aux.size()-num]);
        bpt9->del(pair(od.trainID,od.d0),aux[aux.size()-num]);
        cout<<0<<endl;
        return ;
    }
    od.status=Status::refunded;
    mem2->writeorder(od,aux[aux.size()-num]);
    Tickets tickets;
    int id=bpt4->find(od.trainID)[0];
    mem1->readorder(tickets,id);
    Train train=tickets.train;
    for(int i=od.l;i<od.r;i++)tickets.seat[od.d0][i]+=od.num;
    vector<int> tmp=bpt9->find(pair(od.trainID,od.d0));
    for(int i=0;i<tmp.size();i++){
        Order od1;
        mem2->readorder(od1,tmp[i]);
        int seat=1e9;
        for(int j=od1.l;j<od1.r;j++)seat=std::min(seat,tickets.seat[od1.d0][j]);
        if(seat>=od1.num){
            for(int j=od1.l;j<od1.r;j++)tickets.seat[od1.d0][j]-=od1.num;
            od1.status=Status::success;
            mem2->writeorder(od1,tmp[i]);
            bpt9->del(pair(od.trainID,od.d0),tmp[i]);
        }
    }
    mem1->writeorder(tickets,id);
    cout<<0<<endl;
    return ;
}