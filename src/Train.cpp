#include "Train.hpp"
#include "priority_queue.hpp"
using sjtu::priority_queue;
// Train 实现

Train::Train(TrainID trainID,int stationNum, Station stations[maxStationNum], int seatNum, int prices[maxStationNum], 
    Time startTime, int travelTimes[maxStationNum], int stopoverTimes[maxStationNum], Date sales, Date salet, char type){
    this->trainID=trainID;
    this->stationNum=stationNum;
    this->seatNum=seatNum;
    this->startTime=startTime;
    this->sales=sales,this->salet=salet;
    this->type=type;
    for(int i=0;i<stationNum;i++){
        this->stations[i]=stations[i];
        this->prices[i]=prices[i];
        this->travelTimes[i]=travelTimes[i];
        this->stopoverTimes[i]=stopoverTimes[i];
    }
}
Train& Train::operator=(const Train& other){
    if(this==(&other))return *this;
    this->trainID=other.trainID;
    this->stationNum=other.stationNum;
    this->seatNum=other.seatNum;
    this->startTime=other.startTime;
    this->sales=other.sales,this->salet=other.salet;
    this->type=other.type;
    for(int i=0;i<stationNum;i++){
        this->stations[i]=other.stations[i];
        this->prices[i]=other.prices[i];
        this->travelTimes[i]=other.travelTimes[i];
        this->stopoverTimes[i]=other.stopoverTimes[i];
    }
    return *this;
}

bool Train::operator <(const Train& other)const{
    return trainID<other.trainID;
}
bool Train::operator ==(const Train& other)const{
    return trainID==other.trainID;
}

// Tickets 实现

Tickets::Tickets(const Train& train){
    this->train=train;
    this->datelen=train.salet-train.sales+1;
    for(int i=0;i<this->datelen;i++)
        for(int j=0;j<train.stationNum-1;j++)
            this->seat[i][j]=train.seatNum;
}
Tickets::Tickets(const Tickets& other){
    this->train=other.train;
    this->datelen=other.datelen;
    for(int i=0;i<other.datelen;i++)
        for(int j=0;j<other.train.stationNum-1;j++)
            this->seat[i][j]=other.seat[i][j];
}
Tickets& Tickets::operator =(const Tickets& other){
    if(this==(&other))return *this;
    train=other.train;
    datelen=other.datelen;
    for(int i=0;i<other.datelen;i++)
        for(int j=0;j<other.train.stationNum-1;j++)
            seat[i][j]=other.seat[i][j];
    return *this;
}

bool Tickets::operator <(const Tickets& other){
    return train<other.train;
}
bool Tickets::operator ==(const Tickets& other){
    return train==other.train;
}

//TrainManager 实现

TrainManager::TrainManager(BPT<TrainID,Train,50> *bpt3,MemoryRiver<Tickets,1> *mem1,BPT<TrainID,int,100> *bpt4,BPT<pair<Station,Station>,int,100> *bpt5,BPT<Station,int,100> *bpt6,BPT<Station,Info2,100> *bpt){
    this->bpt3=bpt3;
    this->mem1=mem1;
    this->bpt4=bpt4;
    this->bpt5=bpt5;
    this->bpt6=bpt6;
    this->bpt=bpt;
}
void TrainManager::clear(){
    this->bpt3->clear();
    this->mem1->clear();
    this->bpt4->clear();
    this->bpt5->clear();
    this->bpt6->clear();
    return ;
}

void TrainManager::add_train(TrainID trainID, int stationNum, int seatNum, string stations, string prices, Time startTime, string travelTimes, string stopoverTimes, Date sales, Date salet, char type){
    if(bpt3->find(trainID).size()){cout<<-1<<endl;return ;}
    Station _stations[maxStationNum];
    int _prices[maxStationNum]={0};
    int _travelTimes[maxStationNum]={0};
    int _stopoverTimes[maxStationNum]={0};
    string aux="";int j=0,sum=0;
    for(int i=0;i<stations.size();i++){
        if(stations[i]=='|')_stations[j++]=aux,aux="";
        else aux+=stations[i];
    }
    if(aux.size())_stations[j++]=aux;
    aux="",j=1,sum=0;
    for(int i=0;i<prices.size();i++){
        if(prices[i]=='|')_prices[j++]=(sum+=str_to_int(aux)),aux="";
        else aux+=prices[i];
    }
    if(aux.size())_prices[j++]=(sum+=str_to_int(aux));

    aux="",j=1,sum=0;
    for(int i=0;i<travelTimes.size();i++){
        if(travelTimes[i]=='|')_travelTimes[j++]=(sum+=str_to_int(aux)),aux="";
        else aux+=travelTimes[i];
    }
    if(aux.size())_travelTimes[j++]=(sum+=str_to_int(aux));
    aux="",j=1,sum=0;
    for(int i=0;i<stopoverTimes.size();i++){
        if(stopoverTimes[i]=='|')_stopoverTimes[j++]=(sum+=str_to_int(aux)),aux="";
        else aux+=stopoverTimes[i];
    }
    if(aux.size())_stopoverTimes[j++]=(sum+=str_to_int(aux));

    Train train(trainID,stationNum,_stations,seatNum,_prices,startTime,_travelTimes,_stopoverTimes,sales,salet,type);
    bpt3->ins(trainID,train);
    cout<<0<<endl;
    return ;
}

void TrainManager::delete_train(TrainID trainID){
    vector<Train> tmp=bpt3->find(trainID);
    if(!tmp.size()||bpt4->find(trainID).size()){cout<<-1<<endl;return ;}
    bpt3->del(trainID,tmp[0]);
    cout<<0<<endl;
    return ;
}

void TrainManager::release_train(TrainID trainID){
    vector<Train> tmp=bpt3->find(trainID);
    if(!tmp.size()||bpt4->find(trainID).size()){cout<<-1<<endl;return ;}
    Train train=tmp[0];
    Tickets t(train);
    int id=0;
    mem1->get_info(id,1);id++;
    mem1->writeorder(t,id);
    bpt4->ins(trainID,id);
    for(int i=0;i<train.stationNum;i++){
        bpt6->ins(train.stations[i],id);
        for(int j=i+1;j<train.stationNum;j++)
            bpt5->ins(pair(train.stations[i],train.stations[j]),id);
    }
    cout<<0<<endl;
    return ;
}

void TrainManager::query_train(TrainID trainID, Date date){
    vector<Train> tmp=bpt3->find(trainID);
    if(!tmp.size()){cout<<-1<<endl;return ;}
    Train train=tmp[0];
    if(train.sales>date||train.salet<date){cout<<-1<<endl;return ;}
    cout<<trainID<<" "<<train.type<<endl;
    vector<int> aux=bpt4->find(trainID);
    if(aux.size()){
        Tickets tickets;
        mem1->readorder(tickets,aux[0]);
        int D=date-train.sales;
        cout<<train.stations[0]<<" xx-xx xx:xx -> "<<date_time_to_str(date,train.startTime)<<" 0 "<<tickets.seat[D][0]<<endl;
        for(int i=1;i<train.stationNum-1;i++){
            cout<<train.stations[i]<<" "<<date_time_calc(date,train.startTime,train.travelTimes[i]+train.stopoverTimes[i-1])
            <<" -> "<<date_time_calc(date,train.startTime,train.travelTimes[i]+train.stopoverTimes[i])<<" "<<train.prices[i]<<" "<<tickets.seat[D][i]<<endl;
        }
        cout<<train.stations[train.stationNum-1]<<" "<<date_time_calc(date,train.startTime,train.travelTimes[train.stationNum-1]+train.stopoverTimes[train.stationNum-2])
        <<" -> xx-xx xx:xx "<<train.prices[train.stationNum-1]<<" x"<<endl;
        return ;
    }
    cout<<train.stations[0]<<" xx-xx xx:xx -> "<<date_time_to_str(date,train.startTime)<<" 0 "<<train.seatNum<<endl;
    for(int i=1;i<train.stationNum-1;i++){
        cout<<train.stations[i]<<" "<<date_time_calc(date,train.startTime,train.travelTimes[i]+train.stopoverTimes[i-1])
        <<" -> "<<date_time_calc(date,train.startTime,train.travelTimes[i]+train.stopoverTimes[i])<<" "<<train.prices[i]<<" "<<train.seatNum<<endl;
    }
    cout<<train.stations[train.stationNum-1]<<" "<<date_time_calc(date,train.startTime,train.travelTimes[train.stationNum-1]+train.stopoverTimes[train.stationNum-2])
    <<" -> xx-xx xx:xx "<<train.prices[train.stationNum-1]<<" x"<<endl;
    return ;
}

struct Info1{
    TrainID trainID;
    Date Ld,Ad;
    Time Lt,At;
    int price,time,seat;
    Info1(TrainID _trainID,Date _Ld,Time _Lt,Date _Ad,Time _At,int _price,int _time,int _seat){
        trainID=_trainID;
        Ld=_Ld,Lt=_Lt,Ad=_Ad,At=_At;
        price=_price,time=_time,seat=_seat;
    }
};

class cmp0{
public:
    bool operator()(const Info1 &i,const Info1 &j)const{
        if(i.time==j.time)return j.trainID<i.trainID;
        return j.time<i.time;
    }
};
class cmp1{
public:
    bool operator()(const Info1 &i,const Info1 &j)const{
        if(i.price==j.price)return j.trainID<i.trainID;
        return j.price<i.price;
    }
};

// 0:time, 1:cost
void TrainManager::query_tickets(Station s, Station t, Date date, bool op){
    vector<int> tmp=bpt5->find(pair(s,t));
    vector<Info1> res;
    for(int i=0;i<tmp.size();i++){
        Tickets tickets;
        mem1->readorder(tickets,tmp[i]);
        Train train=tickets.train;
        Date sd,td;
        Time st,tt;
        int l,r,lt,rt;
        for(int j=0;j<train.stationNum;j++){
            int delta;
            if(train.stations[j]==s){
                delta=train.travelTimes[j]+train.stopoverTimes[j];
                sd=date_calc(train.sales,train.startTime,delta);
                st=time_calc(train.sales,train.startTime,delta);
                l=j;lt=delta;
            }
            if(train.stations[j]==t){
                delta=train.travelTimes[j]+train.stopoverTimes[j-1];
                td=date_calc(train.sales,train.startTime,delta);
                tt=time_calc(train.sales,train.startTime,delta);
                r=j;rt=delta;
            }
        }
        if(sd>date||sd+train.salet-train.sales<date)continue;
        int p=date-sd;
        td+=p;sd=date;
        int seat=1e9;
        for(int j=l;j<r;j++)seat=std::min(seat,tickets.seat[p][j]);
        Info1 aux(train.trainID,sd,st,td,tt,train.prices[r]-train.prices[l],rt-lt,seat);
        res.push_back(aux);
    }
    if(!op){
        priority_queue<Info1,cmp0> q;
        cout<<res.size()<<endl;
        for(int i=0;i<res.size();i++)q.push(res[i]);
        while(q.size()){
            Info1 cur=q.top();
            q.pop();
            cout<<cur.trainID<<" "<<s<<" "<<date_time_to_str(cur.Ld,cur.Lt)<<" -> "<<t<<" "<<date_time_to_str(cur.Ad,cur.At)<<" "<<cur.price<<" "<<cur.seat<<endl;
        }
    }
    else{
        priority_queue<Info1,cmp1> q;
        cout<<res.size()<<endl;
        for(int i=0;i<res.size();i++)q.push(res[i]);
        while(q.size()){
            Info1 cur=q.top();
            q.pop();
            cout<<cur.trainID<<" "<<s<<" "<<date_time_to_str(cur.Ld,cur.Lt)<<" -> "<<t<<" "<<date_time_to_str(cur.Ad,cur.At)<<" "<<cur.price<<" "<<cur.seat<<endl;
        }
    }
    return ;
}

Info2::Info2(Date _Ld,Time _Lt,Date _Ad,Time _At,int _t,int _c,int _seat,TrainID _trainID){
    this->Ld=_Ld;
    this->Lt=_Lt;
    this->Ad=_Ad;
    this->At=_At;
    this->t=_t;
    this->c=_c;
    this->seat=_seat;
    this->trainID=_trainID;
}

struct Info3{
    int time,cost;
    TrainID trainID1,trainID2;
    Station transfer;
    Date Ld1,Ad1,Ld2,Ad2;
    Time Lt1,At1,Lt2,At2;
    int price1,price2;
    int seat1,seat2;
    Info3(int _t,int _c){
        time=_t,cost=_c;
    }
    Info3(int _t,int _c,TrainID t1,TrainID t2,Station _transfer,Date _Ld1,Time _Lt1,Date _Ad1,Time _At1,Date _Ld2,Time _Lt2,Date _Ad2,Time _At2,int p1,int p2,int s1,int s2){
        time=_t,cost=_c;
        trainID1=t1,trainID2=t2;
        transfer=_transfer;
        Ld1=_Ld1,Lt1=_Lt1;Ad1=_Ad1,At1=_At1;
        Ld2=_Ld2,Lt2=_Lt2;Ad2=_Ad2,At2=_At2;
        price1=p1,price2=p2;
        seat1=s1,seat2=s2;
    }
};
bool Cmp0(const Info3 &i,const Info3 &j){
    if(i.time!=j.time)return i.time<j.time;
    if(i.cost!=j.cost)return i.cost<j.cost;
    if(!(i.trainID1==j.trainID1))return i.trainID1<j.trainID1;
    return i.trainID2<j.trainID2;
}
bool Cmp1(const Info3 &i,const Info3 &j){
    if(i.cost!=j.cost)return i.cost<j.cost;
    if(i.time!=j.time)return i.time<j.time;
    if(!(i.trainID1==j.trainID1))return i.trainID1<j.trainID1;
    return i.trainID2<j.trainID2;
}

void TrainManager::query_transfer(Station s, Station t, Date date, bool op){
    vector<int> A=bpt6->find(s),B=bpt6->find(t);
    bpt->clear();
//    std::cerr<<A.size()<<" "<<B.size()<<endl;
    for(int i=0;i<A.size();i++){
        Tickets tickets;
        mem1->readorder(tickets,A[i]);
        Train train1=tickets.train;
//        std::cerr<<train1.trainID<<endl;
        Date sd;
        Time st;
        int l,d0;
        for(int j=0;j<train1.stationNum;j++)
            if(train1.stations[j]==s){
                int delta=train1.travelTimes[j]+train1.stopoverTimes[j];
                sd=date_calc(train1.sales,train1.startTime,delta);
                st=time_calc(train1.sales,train1.startTime,delta);
                d0=delta;l=j;
                break;
            }
        if(sd>date||sd+train1.salet-train1.sales<date)continue;
        int p=date-sd,seat=tickets.seat[p][l];
        for(int j=l+1;j<train1.stationNum;j++){
            int delta=train1.travelTimes[j]+train1.stopoverTimes[j-1];
            int cost=train1.prices[j]-train1.prices[l];
            Date d=date_calc(train1.sales,train1.startTime,delta);
            Time t=time_calc(train1.sales,train1.startTime,delta);
            Info2 info(date,st,d+p,t,delta-d0,cost,seat,train1.trainID);
            bpt->ins(train1.stations[j],info);
            seat=std::min(seat,tickets.seat[p][j]);
//            string st="北京市";
//            if(train1.stations[j]==st)std::cerr<<"nice!"<<endl;
        }
    }
    Info3 res((int)1e9,(int)1e9);
    for(int i=0;i<B.size();i++){
        Tickets tickets;
        mem1->readorder(tickets,B[i]);
        Train train2=tickets.train;
//        std::cerr<<train2.trainID<<endl;
        int r,arrivet;
        for(int j=0;j<train2.stationNum;j++)
            if(train2.stations[j]==t){
                if(j)arrivet=train2.travelTimes[j]+train2.stopoverTimes[j-1];
                r=j;
                break;
            }
        for(int j=r-1;j>=0;j--){
            int delta=train2.travelTimes[j]+train2.stopoverTimes[j];
            Date td=date_calc(train2.sales,train2.startTime,delta);
            Time tt=time_calc(train2.sales,train2.startTime,delta);
            vector<Info2> aux=bpt->find(train2.stations[j]);
//            string st="北京市";
//            if(train2.stations[j]==st)std::cerr<<aux.size()<<endl;
            for(auto info:aux){
                if(info.trainID==train2.trainID)continue;
                if(td+train2.salet-train2.sales<info.Ad)continue;
                if(td+train2.salet-train2.sales==info.Ad&&tt<info.At)continue;//= 如何？
                Date Td=td;Time Tt=tt;
                if(td<=info.Ad){
                    if(tt<info.At)Td=info.Ad+1;
                    else Td=info.Ad;
                }
                int t0=info.t+calc_interval(info.Ad,info.At,Td,Tt)+arrivet-delta;
                int c0=info.c+train2.prices[r]-train2.prices[j];
                int seat=1e9;
                for(int k=j;k<r;k++)seat=std::min(seat,tickets.seat[Td-td][k]);
                Info3 cur(t0,c0,info.trainID,train2.trainID,train2.stations[j],info.Ld,info.Lt,info.Ad,info.At,Td,Tt,date_calc(Td,Tt,arrivet-delta),time_calc(Td,Tt,arrivet-delta),info.c,train2.prices[r]-train2.prices[j],info.seat,seat);
                if(!op){
                    if(Cmp0(cur,res))res=cur;
                }
                else{
                    if(Cmp1(cur,res))res=cur;
                }
            }
        }
    }
    if(res.time==(int)1e9){cout<<0<<endl;return ;}
    cout<<res.trainID1<<" "<<s<<" "<<date_time_to_str(res.Ld1,res.Lt1)<<" -> "<<res.transfer<<" "<<date_time_to_str(res.Ad1,res.At1)<<" "<<res.price1<<" "<<res.seat1<<endl;
    cout<<res.trainID2<<" "<<res.transfer<<" "<<date_time_to_str(res.Ld2,res.Lt2)<<" -> "<<t<<" "<<date_time_to_str(res.Ad2,res.At2)<<" "<<res.price2<<" "<<res.seat2<<endl;
    return ;
}
