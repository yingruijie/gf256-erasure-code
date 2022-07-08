# GF256-Based Erasure Code 

## 说明
本项目完成简单的基于伽罗瓦域GF256的纠删码实验。将一个`N`字节的文件编码成`N+K`个1字节的shards（碎片）并保存。当随机删除不超过`K`个碎片时，解码器都可以将数据恢复出来。

## 环境
```
Ubuntu18.04
GNU Make 4.1
cmake version 3.10.2
``` 

## 文件结构
```
$ tree
.
├── CMakeLists.txt
├── example
│   ├── decode.cpp      # 解码器源码
│   ├── encode.cpp      # 编码器源码
│   ├── rmshards.cpp    # 随机删除shards程序源码
│   └── testec.cpp      # 测试
├── inc
│   ├── ec.hpp          # 声明EC(Erasure Code)类
│   ├── gf.hpp          # 声明GF(Galois Field)类
│   ├── gfm.hpp         # 声明GFM(Galois Field Matirx)类
│   └── table.hpp       # 声明GF指数对数表的操作函数
├── LICENSE
├── README.md
├── run                            # 运行过程文件夹
│   ├── data                       # 数据文件夹
│   │   ├── file                   # 原始数据
│   │   │   ├── file0
│   │   │   ├── file1
│   │   │   └── file2
│   │   ├── recover                # 解码/恢复数据
│   │   │   ├── 20220707_165208
│   │   │   └── 20220707_204207
│   │   └── shards                 # Shards数据
│   │       └── 20220707_165208
│   │           ├── 43_15_0
│   │           └── 43_15_9
│   ├── decode                     # 解码器
│   ├── encode                     # 编码器
│   ├── rmshards                   # 随机删除
│   ├── scripts
│   │   └── clean.sh               # 清除recover和shards
│   └── testec                     # 测试程序
└── src
    ├── ec.cpp          # 定义EC类及相关操作
    ├── gf.cpp          # 定义GF类及相关操作
    ├── gfm.cpp         # 定义GFM类及相关操作
    └── table.cpp       # 定义GF指数对数表相关函数
```

## 运行过程示例
* 下载&编译<br>
```
$ git clone git@github.com:jieyr3Rui/gf256-erasure-code.git
$ cd gf256-erasure-code
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ cd ..
```
CMake会把可执行文件输出到`./run/`下，一共四个可执行文件`decode`、`encode`、`rmshards`和`testec`。
* 编码<br>
其中`./data/file/file0`为原始数据的相对路径，本项目提供了3个示例；`5`为纠错容量。`encode`会把编码后的shards根据时间存到`./data/shards/"%Y%m%d_%H%M%S"`目录中。
```
$ cd ./run
$ ./encode ./data/file/file0 5

Input file path:
./data/file/file0

Get input string:
========
Hello World!
========

Raw data GFM:
  col   0
row -----
  0 |  72
  1 | 101
  2 | 108
  3 | 108
  4 | 111
  5 |  32
  6 |  87
  7 | 111
  8 | 114
  9 | 108
 10 | 100
 11 |  33

Vandermonde [N, K] = [12, 5] matrix:
  col   0   1   2   3   4   5   6   7   8   9  10  11
row -------------------------------------------------
  0 |   1   0   0   0   0   0   0   0   0   0   0   0
  1 |   0   1   0   0   0   0   0   0   0   0   0   0
  2 |   0   0   1   0   0   0   0   0   0   0   0   0
  3 |   0   0   0   1   0   0   0   0   0   0   0   0
  4 |   0   0   0   0   1   0   0   0   0   0   0   0
  5 |   0   0   0   0   0   1   0   0   0   0   0   0
  6 |   0   0   0   0   0   0   1   0   0   0   0   0
  7 |   0   0   0   0   0   0   0   1   0   0   0   0
  8 |   0   0   0   0   0   0   0   0   1   0   0   0
  9 |   0   0   0   0   0   0   0   0   0   1   0   0
 10 |   0   0   0   0   0   0   0   0   0   0   1   0
 11 |   0   0   0   0   0   0   0   0   0   0   0   1
 12 |   1   1   1   1   1   1   1   1   1   1   1   1
 13 |   1   2   4   8  16  32  64 128  29  58 116 232
 14 |   1   3   5  15  17  51  85 255  28  36 108 180
 15 |   1   4  16  64  29 116 205  19  76  45 180 234
 16 |   1   5  17  85  28 108 193 226  77 100 233 106

Encoded shards:
  col   0
row -----
  0 |  72
  1 | 101
  2 | 108
  3 | 108
  4 | 111
  5 |  32
  6 |  87
  7 | 111
  8 | 114
  9 | 108
 10 | 100
 11 |  33
 12 |   1
 13 |  47
 14 | 131
 15 |  78
 16 | 166

Write shards to path:
./data/shards/20220708_105349
```

* 随机删除shards<br>
其中`./data/shards/20220708_105349`为shards的目录，`5`为删除的shards数量。`rmshards`会随机删除指定数量的shards。
```
$ ./rmshards ./data/shards/20220708_105349 5
Will remove 5 shards!
Reading shards dir:
./data/shards/20220708_105349

Found 17 shards
Deleting 12_5_15 ==> Finished
Deleting 12_5_2 ==> Finished
Deleting 12_5_10 ==> Finished
Deleting 12_5_3 ==> Finished
Deleting 12_5_5 ==> Finished
```

* 解码<br>
其中`./data/shards/20220708_105349`为shards目录。`decode`从指定目录读取shards，并求出剩余shards对应的Vandermonde矩阵的逆矩阵，用该逆矩阵恢复/解码数据，最后存到`./data/recover/20220708_105349`中。
```
$ ./decode ./data/shards/20220708_105349
Reading Shards...
./data/shards/20220708_105349
here: c = l
here: c = !
here: c = �
here: c = o
here: c = �
here: c = H
here: c = /
here: c = 
here: c = r
here: c = W
here: c = e
here: c = o

Vandermonde [N, K] = [12, 5] matrix:
  col   0   1   2   3   4   5   6   7   8   9  10  11
row -------------------------------------------------
  0 |   1   0   0   0   0   0   0   0   0   0   0   0
  1 |   0   1   0   0   0   0   0   0   0   0   0   0
  2 |   0   0   1   0   0   0   0   0   0   0   0   0
  3 |   0   0   0   1   0   0   0   0   0   0   0   0
  4 |   0   0   0   0   1   0   0   0   0   0   0   0
  5 |   0   0   0   0   0   1   0   0   0   0   0   0
  6 |   0   0   0   0   0   0   1   0   0   0   0   0
  7 |   0   0   0   0   0   0   0   1   0   0   0   0
  8 |   0   0   0   0   0   0   0   0   1   0   0   0
  9 |   0   0   0   0   0   0   0   0   0   1   0   0
 10 |   0   0   0   0   0   0   0   0   0   0   1   0
 11 |   0   0   0   0   0   0   0   0   0   0   0   1
 12 |   1   1   1   1   1   1   1   1   1   1   1   1
 13 |   1   2   4   8  16  32  64 128  29  58 116 232
 14 |   1   3   5  15  17  51  85 255  28  36 108 180
 15 |   1   4  16  64  29 116 205  19  76  45 180 234
 16 |   1   5  17  85  28 108 193 226  77 100 233 106

Remain shards:
  col   0
row -----
  0 | 108
  1 |  33
  2 | 131
  3 | 111
  4 | 166
  5 |  72
  6 |  47
  7 |   1
  8 | 114
  9 |  87
 10 | 101
 11 | 111

Remain shards M:
  col   0   1   2   3   4   5   6   7   8   9  10  11
row -------------------------------------------------
  0 |   0   0   0   0   0   0   0   0   0   1   0   0
  1 |   0   0   0   0   0   0   0   0   0   0   0   1
  2 |   1   3   5  15  17  51  85 255  28  36 108 180
  3 |   0   0   0   0   0   0   0   1   0   0   0   0
  4 |   1   5  17  85  28 108 193 226  77 100 233 106
  5 |   1   0   0   0   0   0   0   0   0   0   0   0
  6 |   1   2   4   8  16  32  64 128  29  58 116 232
  7 |   1   1   1   1   1   1   1   1   1   1   1   1
  8 |   0   0   0   0   0   0   0   0   1   0   0   0
  9 |   0   0   0   0   0   0   1   0   0   0   0   0
 10 |   0   1   0   0   0   0   0   0   0   0   0   0
 11 |   0   0   0   0   1   0   0   0   0   0   0   0

Remain shards M inverse:
  col   0   1   2   3   4   5   6   7   8   9  10  11
row -------------------------------------------------
  0 |   0   0   0   0   0   1   0   0   0   0   0   0
  1 |   0   0   0   0   0   0   0   0   0   0   1   0
  2 |  35 138 115 167  89 238 153  93  95 133 199  90
  3 |  66 193 184 216 204 177 169 108 156  14  45 188
  4 |   0   0   0   0   0   0   0   0   0   0   0   1
  5 |  96 183  38  76 149 140  37  26 146  68 193  27
  6 |   0   0   0   0   0   0   0   0   0   1   0   0
  7 |   0   0   0   1   0   0   0   0   0   0   0   0
  8 |   0   0   0   0   0   0   0   0   1   0   0   0
  9 |   1   0   0   0   0   0   0   0   0   0   0   0
 10 |   0 253 237  50   0 210  21  42  80 206  42 252
 11 |   0   1   0   0   0   0   0   0   0   0   0   0

Recover:
  col   0
row -----
  0 |  72
  1 | 101
  2 | 108
  3 | 108
  4 | 111
  5 |  32
  6 |  87
  7 | 111
  8 | 114
  9 | 108
 10 | 100
 11 |  33

To string:
Hello World!

Write recover to:
./data/recover/20220708_105349
```

* 比较
在Linux下采用`hexdump -C FILE`命令输出文件的十六进制。原始数据与恢复数据完全一致。
```
# raw data
$ hexdump -C ./data/file/file0
00000000  48 65 6c 6c 6f 20 57 6f  72 6c 64 21              |Hello World!|
0000000c

# recover data
$ hexdump -C ./data/recover/20220708_105349
00000000  48 65 6c 6c 6f 20 57 6f  72 6c 64 21              |Hello World!|
0000000c
```
