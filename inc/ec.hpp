#ifndef _EC_HPP_
#define _EC_HPP_

#include <assert.h>
#include <gfm.hpp>

class EC{
private:
    int N, K;
    GFM M;
public:
    EC(int n, int k);
    ~EC();
};

#endif