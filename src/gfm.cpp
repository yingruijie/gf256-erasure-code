#include <gfm.hpp>

/**
* @brief 创建GFM
*/
GFM::GFM(void){

}

/**
* @brief 析构GFM 释放M的内存
*/
GFM::~GFM(){
    if(created){
        for(int i=0; i<R; i++) delete [] M[i];
        delete [] M;
    }
}

/**
* @brief 为矩阵GFM::M开辟二维空间
* @param r 行数
* @param c 列数
* @exception r->(0, ) c->(0, )
*/
void GFM::create(int r, int c){
    R = r; C = c;
    assert(R>0 && C>0);
    M = new GF* [R];
    for(int i=0; i<R; i++){
        M[i] = new GF [C];
    }
    created = true;
}

/**
* @brief 可视化GFM
* @exception 必须先create
*/
void GFM::show(void){
    assert(created);
    cout << "  col ";
    for(int j=0; j<C; j++){
        cout << setw(3) << j << " ";
    }
    cout << endl;
    cout << "row -";
    for(int j=0; j<C; j++){
        cout << "----";
    }
    cout << endl;
    for(int i=0; i<R; i++){
        cout << setw(3) << i << " | ";
        for(int j=0; j<C; j++){
            cout << setw(3) << M[i][j].get_value() << " ";
        }
        cout << endl;
    }
    return;
}

/**
* @brief 选择GFM中的某几行
* @param selected_rows 选择的行索引数组
* @param nums 数组长度
* @exception 必须先create
* @exception selected_rows[i] -> [0, R) 且无重复
*/
GFM GFM::select_rows(const int* selected_rows, const int nums){
    assert(created);
    for(int i=0; i<nums; i++){
        for(int j=i+1; j<nums; j++){
            assert(i != j);
        }
        assert(selected_rows[i] < R && selected_rows[i] >= 0);
    }
    // 创建新的GFM大小为(nums, C)
    GFM res; res.create(nums, C);
    for(int i=0; i<nums; i++){
        for(int j=0; j<C; j++){
            res.M[i][j] = M[selected_rows[i]][j];
        }
    }
    return res;
}

/**
* @brief 选择GFM中的某几列
* @param selected_rows 选择的列索引数组
* @param nums 数组长度
* @exception 必须先create
* @exception selected_rows[j] -> [0, C) 且无重复
*/
GFM GFM::select_cols(const int* selected_cols, const int nums){
    assert(created);
    for(int i=0; i<nums; i++){
        for(int j=i+1; j<nums; j++){
            assert(i != j);
        }
        assert(selected_cols[i] < C && selected_cols[i] >= 0);
    }
    // 创建新的GFM大小为(R, nums)
    GFM res; res.create(R, nums);
    for(int i=0; i<R; i++){
        for(int j=0; j<nums; j++){
            res.M[i][j] = M[i][selected_cols[j]];
        }
    }
    return res;
}

/**
* @brief GFM中的某一行加上一个行向量 add
* @param row 行索引 [0, R)
* @param add 行向量 GFM且大小为(1, C)
* @exception 必须先create
*/
void GFM::add_row(const int row, const GFM& add){
    assert(created);
    assert(add.R==1 && add.C==C);
    assert(row < R && row >=0);
    for(int j=0; j<C; j++){
        M[row][j] = M[row][j] + add.M[0][j];
    }
    return;
}

/**
* @brief GFM中的某一行乘上一个值 mul
* @param row 行索引 [0, R)
* @param mul 乘数 GF
* @exception 必须先create
*/
void GFM::mul_row(const int row, const GF mul){
    assert(created);
    assert(row < R && row >=0);
    for(int j=0; j<C; j++){
        M[row][j] = M[row][j] * mul;
    }
    return;
}

/**
* @brief GFM用增广矩阵的方法取逆
* @exception 必须先create
* @exception R==C
* @exception GFM为可逆矩阵
*/
GFM GFM::inverse(void){
    assert(created);
    if(R!=C){cout << "R!=C"; assert(R==C);}
    int N=R;

    // 生成增广矩阵
    GFM augment; augment.create(N, 2*N);
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            augment.M[i][j]  = M[i][j];
        }
        for(int j=0; j<N; j++){
            if(i==j){GF one(1); augment.M[i][N+j] = one;}
        }
    }

    // cout << "augment" << endl;
    // augment.show();
    // 两个常用的数 0 1
    GF zero(0); GF one(1);

    // 按行开始将增广矩阵左边化为单位矩阵
    for(int i=0; i<N; i++){
        // cout << endl << "====Enter i = " << i << "=====" << endl;
        // M[i][i]为0要向其它行找非0元素补上
        if(augment.M[i][i] == zero){
            // cout << "augment.M[" << i << "][ " << i << " ] == zero" << endl;
            for(int k=i+1; k<N; k++){
                if(k==i) continue;
                // 发现非0元素
                if(augment.M[k][i] != zero){
                    // cout << "found augment.M[" << k << "][" << i << "] != zero" << endl;
                    int sl[1] = {k};
                    GFM sgfm = augment.select_rows(sl, 1);
                    sgfm.mul_row(0, one / augment.M[k][i]);
                    // cout << "padding sgfm = " << endl;
                    // sgfm.show();
                    // 整行补上
                    augment.add_row(i, sgfm);
                    break;
                }
            }
        }
        // 找不到非0元素说明初始矩阵不可逆
        if(augment.M[i][i] == zero){
            cout << "error padding" << endl;
            assert(augment.M[i][i] == zero);
        }
        // cout << "after padding" << endl;
        // augment.show();
        // 消去同i列的非0元素
        for(int k=0; k<N; k++){
            if(k==i) continue;
            if(augment.M[k][i] == zero) continue;
            int sl[1] = {i};
            GFM sgfm = augment.select_rows(sl, 1);
            // 高斯消元法
            sgfm.mul_row(0, augment.M[k][i] / augment.M[i][i]);
            // cout << "sgfm k=" << k << endl;
            // sgfm.show();
            augment.add_row(k, sgfm);

        }
        // 对角元素归一
        augment.mul_row(i, one/augment.M[i][i]);

        // cout << "i = " << i << " final" << endl;
        // augment.show();
        // cout << "=====end of i = " << i << "=====" << endl << endl;

    }

    // cout << "final" << endl;
    // augment.show();
    // 取处理过后的增广矩阵的右半边 即为原矩阵的逆
    int cols[N];
    for(int j=0; j<N;j++) cols[j] = N+j;
    GFM res = augment.select_cols(cols, N);
    // cout << "Inverse = " << endl;
    // res.show();

    // 与原矩阵相乘，检查是否为单位阵
    GFM testinverse = rdot(res);
    // cout << "testinverse = " << endl;
    // testinverse.show();
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if(i==j) assert(testinverse.M[i][j] == one);
            else if(i==j) assert(testinverse.M[i][j] == zero);
        }
    }
    // 检验通过,结束
    return res;
}

/**
* @brief GFM*b 左乘一个矩阵
* @param b GFM 
* @exception 必须先create
* @exception C == b.R
* @exception GFM为可逆矩阵
*/
GFM GFM::rdot(const GFM& b){
    assert(created && b.created);
    assert(R>0 && C>0 && b.C>0 && b.R>0);
    // 保证两个矩阵可以相乘
    assert(C == b.R);
    GFM res; res.create(R, b.C);
    for(int i=0; i<R; i++){
        for(int j=0; j<b.C; j++){
            GF me(0);
            for(int k=0; k<C; k++){
                me = me + M[i][k] * b.M[k][j];
            }
            res.M[i][j] = me;
        }
    }
    return res;
}

/**
* @brief 重定义"="操作为内存拷贝 避免同时指向同一内存区域
* @param b GFM 
*/
void GFM::operator=(const GFM& b){
    if(b.created && created){
        for(int i=0; i<R; i++) delete [] M[i];
        delete [] M;
        created = false;
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                M[i][j] = b.M[i][j];
            }
        }
        return;
    }
    else if(!b.created && created){
        for(int i=0; i<R; i++) delete [] M[i];
        delete [] M;
        R = 0; C=0;
        return;
    }
    else if(b.created && !created){
        create(b.R, b.C);
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                M[i][j] = b.M[i][j];
            }
        }
        return;
    }
    else{
        return;
    }
}