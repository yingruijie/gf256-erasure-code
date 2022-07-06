#include <gf.hpp>


/**
* @brief 伽罗瓦域的构造函数
* @param v 初始值
* @param x 域值 默认为8
* @exception 0 <= V < T, X == 8
*/
GF::GF(){
    V = 0; X = 8; T = 1 << X;
}

/**
* @brief 伽罗瓦域的构造函数
* @param v 初始值
* @param x 域值 默认为8
* @exception 0 <= V < T, X == 8
*/
GF::GF(int v){
    V = v; X = 8; T = 1 << X;
    // 创建时V [0, T)
    assert(V < T && V >= 0);
    // 本项目目前只实现GF8
    assert(X == 8);
}

/**
* @brief 伽罗瓦域的构造函数
* @param v 初始值
* @param x 域值 默认为8
* @exception 0 <= V < T, X == 8
*/
GF::GF(int v, int x){
    V = v; X = x; T = 1 << X;
    // 创建时V [0, T)
    assert(V < T && V >= 0);
    // 本项目目前只实现GF8
    assert(X == 8);
}

/**
* @brief 伽罗瓦域的析构函数
*/
GF::~GF(){
}

/**
* @brief 获取GF的值V
* @return V 
*/
int GF::get_value(void)const{
    return V;
}

/**
* @brief 获取GF的阈值
* @return 域值X
*/
int GF::get_x_field(void)const{
    return X;
}

/**
* @brief 重定义GF的操作符"+"  实现a+b=c
* @param b 操作的另外一个GF类
* @return 操作结果 c
*/
GF GF::operator+(const GF& b){   
    GF c(
        (this->V ^ b.get_value()) % (this->T-1)
    ); 
    return c;
}

/**
* @brief 重定义GF的操作符"-"  实现a-b=c
* @param b 操作的另外一个GF类
* @return 操作结果 c
*/
GF GF::operator-(const GF& b){   
    GF c(
        (this->V ^ b.get_value()) % (this->T-1)
    ); 
    return c;
}

/**
* @brief 重定义GF的操作符"*"  实现a*b=c
* @param b 操作的另外一个GF类
* @return 操作结果 c
*/
GF GF::operator*(const GF& b){
    if(b.get_value() == 0 || V == 0){
        GF zero(0);
        return zero;
    }   
    GF c(
        P[(L[this->V] + L[b.get_value()]) % (this->T-1)] 
    );
    return c;
}

/**
* @brief 重定义GF的操作符"/"  实现a/b=c
* @param b 操作的另外一个GF类
* @return 操作结果 c
* @exception 除数b为0时报错，即a/0无意义
*/
GF GF::operator/(const GF& b){
    assert(b.get_value() != 0);
    if(V == 0){
        GF zero(0);
        return zero;
    }
    GF c(
        P[(L[this->V] - L[b.get_value()] + (this->T-1)) % (this->T-1)] 
    ); 
    return c;
}

/**
* @brief 实现指数函数
* @param p 指数，大于等于0
* @return 操作结果 c
* @exception 除数b为0时报错，未实现负指数
*/
GF GF::pow(const int p){
    assert(p >= 0);
    GF c(1); GF me(this->V);
    for(int i=0; i<p; i++){
        c = c * me;
    }
    return c;
}

/**
* @brief 重定义GF的操作符"=="  实现a==b
* @param b 操作的另外一个GF类
* @return 操作结果 ture or fasle
*/
bool GF::operator==(const GF& b){
    return b.V == this->V;
}

/**
* @brief 重定义GF的操作符"!="  实现a!=b
* @param b 操作的另外一个GF类
* @return 操作结果 ture or fasle
*/
bool GF::operator!=(const GF& b){
    return b.V != this->V;
}
