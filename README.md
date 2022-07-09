# GF256-Based Erasure Code 
《大数据存储》课程项目
## 说明
本项目完成简单的基于伽罗瓦域GF256的纠删码实验。将一个文件平均分为`N`段，编码成`N+K`个shards（碎片）并保存。当随机删除不超过`K`个碎片时，解码器都可以将数据恢复出来。

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
|   ├── utils.hpp       # 声明一些常用函数
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
    ├── utils.cpp       # 定义常用函数
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
* **编码**<br>
其中`./data/file/file0`为原始数据的相对路径，本项目提供了3个示例；`5`为分段数量，`3`为纠错容量。`encode`会把编码后的shards根据时间存到`./data/shards/"%Y%m%d_%H%M%S"`目录中。
```
$ ./encode ./data/file/file0 5 3

Input file path:
./data/file/file0

Get input string:
========
This is a GF256-based erasure-code project!
========

Data size = 43, pad size = 2, shards size [N, B] = [5, 9]

Raw data GFM:
  col   0   1   2   3   4   5   6   7   8
row -------------------------------------
  0 |  84 104 105 115  32 105 115  32  97
  1 |  32  71  70  50  53  54  45  98  97
  2 | 115 101 100  32 101 114  97 115 117
  3 | 114 101  45  99 111 100 101  32 112
  4 | 114 111 106 101  99 116  33   0   0

Vandermonde [N, K] = [5, 3] matrix:
  col   0   1   2   3   4
row ---------------------
  0 |   1   0   0   0   0
  1 |   0   1   0   0   0
  2 |   0   0   1   0   0
  3 |   0   0   0   1   0
  4 |   0   0   0   0   1
  5 |   1   1   1   1   1
  6 |   1   2   4   8  16
  7 |   1   3   5  15  17

Encoded shards:
  col   0   1   2   3   4   5   6   7   8
row -------------------------------------
  0 |  84 104 105 115  32 105 115  32  97
  1 |  32  71  70  50  53  54  45  98  97
  2 | 115 101 100  32 101 114  97 115 117
  3 | 114 101  45  99 111 100 101  32 112
  4 | 114 111 106 101  99 116  33   0   0
  5 |   7  64  12 103 124  61 123  17   5
  6 |   1 222 243 182 226 196 149  40 205
  7 |  99 181 120 245 193 213 222 217 148

Write shards to path:
./data/shards/20220708_200314
```

* **随机删除shards**<br>
其中`./data/shards/20220708_200314`为shards的目录，`3`为删除的shards数量。`rmshards`会随机删除指定数量的shards。
```
$ ./rmshards ./data/shards/20220708_200314 3
Will remove 3 shards!
Reading shards dir:
./data/shards/20220708_200314

Found 8 shards
Deleting 5_3_3 ==> Finished
Deleting 5_3_4 ==> Finished
Deleting 5_3_5 ==> Finished
```

* **解码**<br>
其中`./data/shards/20220708_200314`为shards目录。`decode`从指定目录读取shards，并求出剩余shards对应的Vandermonde矩阵的逆矩阵，用该逆矩阵恢复/解码数据，最后存到`./data/recover/20220708_200314`中。
```
$ ./decode ./data/shards/20220708_200314
Reading Shards...
./data/shards/20220708_200314

Vandermonde [N, K] = [5, 3] matrix:
  col   0   1   2   3   4
row ---------------------
  0 |   1   0   0   0   0
  1 |   0   1   0   0   0
  2 |   0   0   1   0   0
  3 |   0   0   0   1   0
  4 |   0   0   0   0   1
  5 |   1   1   1   1   1
  6 |   1   2   4   8  16
  7 |   1   3   5  15  17

Remain shards:
  col   0   1   2   3   4   5   6   7   8
row -------------------------------------
  0 |   1 222 243 182 226 196 149  40 205
  1 |  99 181 120 245 193 213 222 217 148
  2 |  84 104 105 115  32 105 115  32  97
  3 | 115 101 100  32 101 114  97 115 117
  4 |  32  71  70  50  53  54  45  98  97

Remain shards M:
  col   0   1   2   3   4
row ---------------------
  0 |   1   2   4   8  16
  1 |   1   3   5  15  17
  2 |   1   0   0   0   0
  3 |   0   0   1   0   0
  4 |   0   1   0   0   0

Remain shards M inverse:
  col   0   1   2   3   4
row ---------------------
  0 |   0   0   1   0   0
  1 |   0   0   0   0   1
  2 |   0   0   0   1   0
  3 | 234  49 219 122 154
  4 | 173 150  59 122 224

Recover:
  col   0   1   2   3   4   5   6   7   8
row -------------------------------------
  0 |  84 104 105 115  32 105 115  32  97
  1 |  32  71  70  50  53  54  45  98  97
  2 | 115 101 100  32 101 114  97 115 117
  3 | 114 101  45  99 111 100 101  32 112
  4 | 114 111 106 101  99 116  33   0   0

To string:
This is a GF256-based erasure-code project!

Write recover to:
./data/recover/20220708_200314
```

* **比较**<br>
在Linux下采用`hexdump -C FILE`命令输出文件的十六进制。原始数据与恢复数据除了补齐数据外完全一致，说明恢复成功。
```
# raw data
$ hexdump -C ./data/file/file0
00000000  54 68 69 73 20 69 73 20  61 20 47 46 32 35 36 2d  |This is a GF256-|
00000010  62 61 73 65 64 20 65 72  61 73 75 72 65 2d 63 6f  |based erasure-co|
00000020  64 65 20 70 72 6f 6a 65  63 74 21                 |de project!|
0000002b

# recover data
$ hexdump -C ./data/recover/20220708_200314
00000000  54 68 69 73 20 69 73 20  61 20 47 46 32 35 36 2d  |This is a GF256-|
00000010  62 61 73 65 64 20 65 72  61 73 75 72 65 2d 63 6f  |based erasure-co|
00000020  64 65 20 70 72 6f 6a 65  63 74 21 00 00           |de project!..|
0000002d
```

* **运行FILE1的过程**<br>
在较大文件[./run/data/file/file1](./run/data/file/file1)上运行的过程请查看[RUN_FILE1.md](./RUN_FILE1.md)。