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
public:
    EC();
    ~EC();
    void create_m(int m, int k);

    void encode(const GFM& raw, int k, const char* shardsroot);
    void write_shards(const GFM& shards, const char* shardsroot);

    void read_shards(const char* shardsroot, const char* shardsname);
    void decode(const GFM& shards);
    
};

#endif