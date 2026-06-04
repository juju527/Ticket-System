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
    mem1.initialise("tickets");
    mem2.initialise("order");
    
    return 0;
}