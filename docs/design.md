### 数据管理
由 MemoryRiver 与 BPT 两个部分完成。


### utility

包括实现的 String[maxlen] 类，和 pair 类。

### User

bpt1[Username,User]：储存了全部用户信息。
bpt2[Username,bool]：储存了登录用户信息。

### Train

bpt3[TrainID,Train]：储存了所有火车信息。
mem1[Tickets]：储存了发布的火车的售票情况。
bpt4[TrainID,int]：储存了发布的火车对应的 mem1 编号。
bpt5[pair[Station,Station],int]：储存了能从 s 到 t 的发布了的火车的 mem1 编号。
bpt6[Station,int]：储存了包含 s 的发布了的火车 mem1 编号。
bpt[Station,info]：处理 query_transfer 中转的 bpt，info 里有时间、花费等信息。

### Order

mem2[Order]：储存了全部订单。
bpt7[Username,int]：储存了全部用户的全部订单的 mem2 编号。
bpt8[pair[TrainId,Date],int]：储存了候补序列的全部订单的 mem2 编号。