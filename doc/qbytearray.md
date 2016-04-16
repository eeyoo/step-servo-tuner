## QByteArray

一句话
> The QByteArray class provides an array of bytes.

如上所述，`QByteArray`类用于操作字节数组。

`QByteArray`用于操作字符串，可以是以`\0`结尾的原生字节(raw bytes)或者传统意义的8位字节字符串。
用起来比`const char *`指针方便太多(没体验过，持怀疑态度)。

`QString`类也是Qt用来管理字符串，不同的是采用16位Unicode字符，可以存放non-ASCII/non-Latin-1
字符。`QByteArray`有两个优势：a) 用于管理原生二进制数据；b) 内存资源稀缺情况下(如嵌入式Linux
环境)。

### 初始化

`QByteArray ba("Hello")` 传人`const char *`初始化`QByteArray`数据类型。

`resize()`方法
```cpp
QByteArray ba;
ba.resize(5);
ba[0] = 0x3c;
ba[1] = 0xb8;
ba[2] = 0x64;
ba[3] = 0x18;
ba[4] = 0xca;
```

### QByteArray::fromHex(), QByteArray::toHex()
```
QByteArray ba("Hello");
ba.toHex();  //
```

### 思路
QtGui界面上字符串`QString`进行组装为指令结构体数据类型，然后转换为`QByteArray`数据类型，便于串口进行传送。

- 从交互界面获取字符数据
- 数据组装为指令数据
- 转成`QByteArray`数据格式
- 通过串口发送出去
