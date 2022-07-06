#ifndef _GF_HPP_
#define _GF_HPP_

#include <assert.h>
#include <table.hpp>

/**
* @class GF gf.hpp
* @brief GF伽罗瓦域类
*/
class GF{
private:
    int V;  /** 值 */
    int X;  /** 域值，默认为8 */
    int T;  /** 总值大小，在GF256里面是256 */ 

public:
    GF();
    GF(int v);
    GF(int v, int x);
    ~GF();
    int get_value(void)const;
    int get_x_field(void)const;

    // 定义操作符
    GF operator+(const GF& b);
    GF operator-(const GF& b);
    GF operator*(const GF& b);
    GF operator/(const GF& b);
    GF pow(const int p);
    bool operator==(const GF& b);
    bool operator!=(const GF& b);
};

#endif
