/*
 * 定义一个串口通讯协议基类模板
 * 包括基本的数据格式，指令类型，收发操作
 * 由简及繁
 */

 class Command
 {
//对外接口
public:
    void readData(); // 读取串口缓存数据，存在哪里？
    void writeData(); // 向串口缓存写入数据


};
