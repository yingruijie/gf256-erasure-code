#include <ec.hpp>

EC::EC(int n, int k){
    N=n; K=k;
    assert(N<100 && K<50);
    // 分配内存
    M.create(N+K, N);
    // 初始化前N行为单位矩阵
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if(i==j){GF one(1); M.M[i][j] = one;}
        }
    }
    // 初始化后K行为范德蒙德矩阵
    for(int i=0; i<K; i++){
        for(int j=0; j<N; j++){
            GF alpha(i+1);
            M.M[N+i][j] = alpha.pow(j);
        }
    }
    cout << "M = " << endl;
    M.show();
    int sl[12] = {0,1,3,4,6,7,8,10,11, 12,14,15};
    GFM sgfm = M.select_rows(sl, 12);
    cout << "sgfm = " << endl;
    sgfm.show();
    
    // M.add_row(27, &sgfm);
    // cout << "add sgfm to row[27]"<< endl;
    // M.show();
    // GF a(2);
    // M.mul_row(27, a);
    // cout << "row[27]*=2"<< endl;
    // M.show();
    cout << "inverse" << endl;
    sgfm.inverse();
}

EC::~EC(){
}