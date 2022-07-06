#ifndef _GFM_HPP_
#define _GFM_HPP_

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <gf.hpp>

using namespace std;

/**
* @class GFM gfm.hpp
* @brief GFM伽罗瓦域类组成的矩阵
*/
class GFM{
public:
    int R, C;
    GF **M;
    bool created=false;

    GFM(void);
    ~GFM();
    void create(int r, int c);
    void show(void);
    GFM select_rows(const int* selected_rows, const int nums);
    void add_row(const int row, const GFM& add);
    void mul_row(const int row, const GF mul);
    void inverse(void);
    GFM rdot(const GFM& b);
};

#endif