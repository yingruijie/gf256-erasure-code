#include <gfm.hpp>


GFM::GFM(){}

GFM::~GFM(){
    if(created){
        for(int i=0; i<R; i++) delete [] M[i];
        delete [] M;
    }
}

void GFM::create(int r, int c){
    R = r; C = c;
    M = new GF* [R];
    for(int i=0; i<R; i++){
        M[i] = new GF [C];
    }
    created = true;
}


void GFM::show(){
    assert(created);
    for(int i=0; i<R; i++){
        for(int j=0; j<C; j++){
            cout << setw(3) << M[i][j].get_value() << " "; 
        }
        cout << endl;
    }
    return;
}

GFM GFM::select_rows(const int* selected_rows, const int nums){
    assert(created);
    for(int i=0; i<nums; i++){
        for(int j=i+1; j<nums; j++){
            assert(i != j);
        }
        assert(selected_rows[i] < R && selected_rows[i] >= 0);
    }
    GFM res; res.create(nums, C);
    for(int i=0; i<nums; i++){
        for(int j=0; j<C; j++){
            res.M[i][j] = M[selected_rows[i]][j];
        }
    }
    return res;
}

void GFM::add_row(const int row, const GFM* add){
    assert(add->R==1 && add->C==C);
    assert(row < R && row >=0);
    for(int j=0; j<C; j++){
        M[row][j] = M[row][j] + add->M[0][j];
    }
    return;
}

void GFM::mul_row(const int row, const GF mul){
    assert(row < R && row >=0);
    for(int j=0; j<C; j++){
        M[row][j] = M[row][j] * mul;
    }
    return;
}

void GFM::inverse(){
    assert(created);
    if(R!=C){cout << "R!=C"; return;}
    int N=R;
    // 伴随矩阵
    GFM adjoint; adjoint.create(N, 2*N);
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            adjoint.M[i][j]  = M[i][j];
        }
        for(int j=0; j<N; j++){
            if(i==j){GF one(1); adjoint.M[i][N+j] = one;}
        }
    }

    cout << "adjoint" << endl;
    adjoint.show();

    GF zero(0); GF one(1);

    for(int i=0; i<N; i++){
        cout << endl << "====Enter i = " << i << "=====" << endl;
        if(adjoint.M[i][i] == zero){
            cout << "adjoint.M[" << i << "][ " << i << " ] == zero" << endl;
            for(int k=i+1; k<N; k++){
                if(k==i) continue;
                if(adjoint.M[k][i] != zero){
                    cout << "found adjoint.M[" << k << "][" << i << "] != zero" << endl;
                    int sl[1] = {k};
                    GFM sgfm = adjoint.select_rows(sl, 1);
                    sgfm.mul_row(0, one / adjoint.M[k][i]);
                    cout << "padding sgfm = " << endl;
                    sgfm.show();
                    adjoint.add_row(i, &sgfm);
                    break;
                }
            }
        }
        if(adjoint.M[i][i] == zero){
            cout << "error padding" << endl;
            assert(adjoint.M[i][i] == zero);
        }
        cout << "after padding" << endl;
        adjoint.show();
        for(int k=0; k<N; k++){
            if(k==i) continue;
            if(adjoint.M[k][i] == zero) continue;
            int sl[1] = {i};
            GFM sgfm = adjoint.select_rows(sl, 1);
            sgfm.mul_row(0, adjoint.M[k][i] / adjoint.M[i][i]);
            // cout << "sgfm k=" << k << endl;
            // sgfm.show();
            adjoint.add_row(k, &sgfm);

        }

        adjoint.mul_row(i, one/adjoint.M[i][i]);

        cout << "i = " << i << " final" << endl;
        adjoint.show();
        cout << "=====end of i = " << i << "=====" << endl << endl;

    }

    cout << "final" << endl;
    adjoint.show();
}