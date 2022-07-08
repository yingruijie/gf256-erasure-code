#ifndef _EC_HPP_
#define _EC_HPP_

#include <assert.h>

#include <string>
#include <iostream>

#include <dirent.h>
#include <regex>
#include <time.h>
#include <fstream>
#include <gfm.hpp>
#include <utils.hpp>

// 一些buffer的size
#define BUFSIZE 8192
#define MAXK    50
#define MAXN    100
/**
* @class EC ec.hpp
* @brief 基于GF256的Erasure Code类
*/
class EC{
private:
    int N=0, K=0;
    GFM M;
    GFM raw, shards, remain_shards, recover;
    int *remain_indices;
    bool encoded=false, decoded=false;
public:
    EC(void);
    ~EC();
    void create_m(int n, int k);

    void read_file(const char* filepath, int n);
    void encode(int k);
    void write_shards(const char* shardsroot);

    void read_shards(const char* shardsdir);
    void decode(void);
    void write_recover(const char* recoverpath);
};

#endif