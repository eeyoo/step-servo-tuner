# Qt框架下串口通讯分析

Qt框架特点
C++语言实现，面向对象编程思想，注重跨平台。
信号和槽的设计模式
信号对外开放，
槽分为公有和私有
信号可以与信号连接
信号可以与槽连接

串口通讯特点
全双工，接收同时可以发送数据
串行通讯，通讯速率单位为波特率，即每秒比特位数。
按位收发数据。

通讯协议特点
双方定义的通讯数据格式。分为头部、数据段、校验。
收发数据本质上二进制0101，经过操作系统应用软件转换，通讯双方理解传送数据。


## 串口实例化及配置
串口名称
win: com+数字
unix: /dev/ttyXXX
配置
设置波特率
设置数据位
设置校验
0 - PAR_NONE 无校验
1 - PAR_ODD 奇校验
2 - PAR_EVEN 偶校验
设置停止位
0 - STOP_1 1个停止位
1 - win STOP_1_5是 1.5个停止位，unix无
2 - STOP_2 2个停止位
设置数据流控制
FLOW_OFF
设置延时
串口读写延时


# QextSerialPort 模板设计（QT第三方串口通讯实现类）

## 常数 采用宏定义方式

1. 行状态常数 line status constants
```
#define LS_CTS 0x01
...
```
2. 异常常数 error constants
```
#define E_NO_ERROR 0
...
```
## 枚举类型 enum
1. BaudRateType 波特率
```
enum BaudRateType
{
    BAUD50 = 50,
    ...
    BAUD9600 = 9600,
    ...
};
```
2. DataBitsType 数据位数， 分为5、6、7、8位串行通讯
3. ParityType 校验类型
PAR_NONE PAR_ODD PAR_EVEN
4. StopBitsType 停止位类型
5. FlowType 流控制类型

## 结构体 struct
1. 配置类型结构
```
struct PortSettings
{
    BaudRateType BaudRate; //波特率
    DataBitsType DataBits; //数据位数
    ParityType Parity;     //校验类型
    StopBitsType StopBits; //停止位
    FlowType FlowControl;  //流控制
    long Timeout_Millisec; //延时毫秒级
};
```
## 类
```
class QextSerialPortPrivate;
```


## QextSerialPort 类设计
1. public QIODevice
继承父类属性和函数

2. public 公有成员
枚举类型成员
```
enum QueryMode {
    Polling,
    EventDriven
};
```
构造函数
`explicit` 构造函数的修饰符，只允许显式转换
构造函数亦可重构，不同传人参数区分
析构函数
`~QextSerialPort();`

一般接口方法
```
//返回串口属性，类似getter()方法
QString portName() const; //返回串口名称
QueryMode queryMode() const; //串口轮询方式
BaudRateType baudRate() const; //返回波特率
DataBitsType dataBits() const; //返回数据位
ParityType parity() const; //返回校验类型
StopBitsType stopBits() const; //返回停止位类型
FLowType FlowControl() const; //返回流控制类型

//串口操作
bool open(OpenMode mode); //mode 表示文件操作类型在QIODevice类定义
bool isSequential() const;
void close(); //关闭串口
void flush(); //清空串口缓存
qint64 bytesAvailable() const; //返回缓存可读字节数
bool canReadLine() const;
QByteArray readAll(); //读取所有数据

ulong lastError() const;

ulong lineStatus(); //行状态
QString errorString(); //异常类型名称
```
公共槽 public Q_SLOTS 与公用方法调用相同，且支持信号触发
类似setter()方法
```
void setPortName(const QString &name);
void setQueryMode(QueryMode mode);
void setBaudRate(BaudRateType);
void setDataBits(DataBitsType);
void setParity(ParityType);
void setStopBits(StopBitsType);
void setFlowControl(FlowType);
void setTimeout(long);

void setDtr(bool set=true);
void setRts(bool set=true);
```
信号 Q_SIGNALS
```
void dsrChanged(bool status);
```
3. 继承成员 protected
```
qint64 readData(char *data, qint64 maxSize);
qint64 writeData(char *data, qint64 maxSize);
```
