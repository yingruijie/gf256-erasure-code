#ifndef _EC_HPP_
#define _EC_HPP_

#include <assert.h>
#include <gfm.hpp>
#include <string>
#include <iostream>

#include <dirent.h>
#include <regex>
#include <time.h>

#define BUFSIZE 256

class EC{
private:
    int N=0, K=0;
    GFM M;
    GFM raw, shards, remain_shards, recover;
    int *remain_indices;
    bool encoded=false, decoded=false;
public:
    EC();
    ~EC();
    void create_m(int n, int k);
    // 编码过程
    void read_file(const char* filepath);
    void encode(int k);
    void write_shards(const char* shardsroot);
    // 解码过程
    void read_shards(const char* shardsdir);
    void decode(void);
    void write_recover(const char* recoverpath);
};

#endif