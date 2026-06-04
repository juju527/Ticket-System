#include "Order.hpp"

Order::Order(Status status, TrainID trainID, Station s, Station t, Date d0, Date leavingDate, Time leavingTime, Date arrivingDate, Time arrivingTime, int price, int num){
    this->status=status;
    this->trainID=trainID;
    this->s=s;this->t=t;
    this->d0=d0;
    this->leavingDate=leavingDate;this->leavingTime=leavingTime;
    this->arrivingDate=arrivingDate;this->arrivingTime=arrivingTime;
    this->price=price;
    this->num=num;
}

bool Order::operator <(const Order&)const{
    
}