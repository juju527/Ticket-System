#ifndef TRAIN_HPP
#define TRAIN_HPP
#include "utility.hpp"
#include "BPT.hpp"

constexpr int maxStationNum=35;
using TrainID=String<20>;
using Station=String<30>;

class Train{
public:
    TrainID trainID;
    int stationNum;
    Station stations[maxStationNum];
    int seatNum;
    int prices[maxStationNum];//前缀和
    Time startTime;
    int travelTimes[maxStationNum];//前缀和
    int stopoverTimes[maxStationNum];//前缀和
    Date sales,salet;
    char type;
    Train(){
        stationNum=0;
    }
    Train(TrainID, int, Station[maxStationNum], int, int[maxStationNum], Time, int[maxStationNum], int[maxStationNum], Date, Date, char);
    Train& operator=(const Train&);
    bool operator <(const Train&)const;
    bool operator ==(const Train&)const;
};

constexpr int maxDate=100;

class Tickets{
public:
    Train train;
    int datelen;
    int seat[maxDate][maxStationNum];
    Tickets()=default;
    Tickets(const Train&);
    Tickets(const Tickets&);
    Tickets& operator=(const Tickets&);
    bool operator <(const Tickets&);
    bool operator ==(const Tickets&);
};

struct Info2{
    Date Ld,Ad;
    Time Lt,At;
    int t,c,seat;
    TrainID trainID;
    Info2()=default;
    Info2(Date,Time,Date,Time,int,int,int,TrainID);
    bool operator <(const Info2& other)const{return trainID<other.trainID;}
    bool operator ==(const Info2& other)const{return trainID==other.trainID;}
};

class TrainManager{
private:
    BPT<TrainID,Train,50> *bpt3;// 所有火车信息
    MemoryRiver<Tickets,1> *mem1;// 所有发布的火车的售票情况
    BPT<TrainID,int,100> *bpt4;// 所有发布火车的 mem1 id
    BPT<pair<Station,Station>,int,100> *bpt5;// 包含站 (s,t) 的火车 mem1 id
    BPT<Station,int,100> *bpt6;// 包含站 s 的火车 mem id
    BPT<Station,Info2,100> *bpt;
public:
    TrainManager(BPT<TrainID,Train,50>*,MemoryRiver<Tickets,1>*,BPT<TrainID,int,100>*,BPT<pair<Station,Station>,int,100>*,BPT<Station,int,100>*,BPT<Station,Info2,100>*);
    void clear();
    void add_train(TrainID, int, int, string, string, Time, string, string, Date, Date, char);
    void delete_train(TrainID);
    void release_train(TrainID);
    void query_train(TrainID, Date);
    void query_tickets(Station, Station, Date, bool);// 0:time, 1:cost
    void query_transfer(Station, Station, Date, bool);
};

#endif