#ifndef ORDER_HPP
#define ORDER_HPP

#include "User.hpp"
#include "Train.hpp"

enum Status{
    success,pending,refunded
};

class Order{
public:
    Status status;
    TrainID trainID;
    Station s,t;
    int l,r;
    Date d0;// 偏移量
    Date leavingDate, arrivingDate;
    Time leavingTime, arrivingTime;
    int price;
    int num;
};

class OrderManager{
private:
    BPT<Username,bool,100> *bpt2;// 登录用户
    MemoryRiver<Tickets,1> *mem1;// 所有发布的火车的售票情况
    BPT<TrainID,int,100> *bpt4;// 所有发布火车的 mem1 id

    MemoryRiver<Order,1> *mem2;
    BPT<Username,int,100> *bpt8;// 每个用户的订单 mem2 id
    BPT<pair<TrainID,Date>,int,50> *bpt9;// 处于 pending 状态的订单队列。
public:
    OrderManager(BPT<Username,bool,100>*, MemoryRiver<Tickets,1>*, BPT<TrainID,int,100>*, MemoryRiver<Order,1>*, BPT<Username,int,100>*, BPT<pair<TrainID,Date>,int,50>*);
    void clear();
    void buy_ticket(Username, TrainID, Date, int, Station, Station, bool);
    void query_order(Username);
    void refund_ticket(Username, int);
};

#endif