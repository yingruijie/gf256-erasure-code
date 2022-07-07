#include <table.hpp>

using namespace std;

int *P; /** 指数表 在GF256里面是256数组 */
int *L; /** 对数表 在GF256里面是256数组 */

bool inited_table=false; /** 表是否已经被初始化 */

/**
* @brief 生成GF256的指数表和对数表
*/
void get_table_256(void){
    int T = 256, PX=285;
    P = new int [T]; L = new int [T];
    memset(P, 0, sizeof(int)*T);
    memset(L, 0, sizeof(int)*T);
    int n = 1; 
    for(int i=0; i<T; i++){
        P[i] = n; L[n] = i;
        n*=2;
        if(n >= T) n = n ^ PX;
    }
    L[1] = 0;
    inited_table=true;
    return;
}

/**
* @brief 删除GF256的指数表和对数表
*/
void destory_table_256(void){
    if(inited_table){
        delete [] P;
        delete [] L;
        inited_table=false;
    }
    return;
}

/**
* @brief 打印GF256的指数表和对数表
*/
void print_table_256(void){
    int X = 8;
    int sqrtT=1<<(X/2);
    cout << "power" << endl;
    for(int i=0; i<sqrtT; i++){
        for(int j=0; j<sqrtT; j++){
            cout << setw(2) << hex << P[i*sqrtT+j] << " ";
        }
        cout << endl;
    }
    cout << "log" << endl;
    for(int i=0; i<sqrtT; i++){
        for(int j=0; j<sqrtT; j++){
            cout << setw(3) << hex << L[i*sqrtT+j] << " ";
        }
        cout << endl;
    }
}
