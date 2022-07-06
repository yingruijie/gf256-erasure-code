#include <ec.hpp>

EC::EC(){}

void EC::create_m(int n,int k){
    N=n; K=k;
    // 限制N K大小
    assert(0<N && N<100 && 0<K && K<50);
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
    cout << "Erasure Code Matrix = " << endl;
    M.show();
    // int sl[12] = {0,1,3,4,6,7,8,10,11, 12,14,15};
    // GFM sgfm = M.select_rows(sl, 12);
    // cout << "sgfm = " << endl;
    // sgfm.show();
    // M.add_row(27, &sgfm);
    // cout << "add sgfm to row[27]"<< endl;
    // M.show();
    // GF a(2);
    // M.mul_row(27, a);
    // cout << "row[27]*=2"<< endl;
    // M.show();
    // cout << "inverse" << endl;
    // sgfm.inverse();
}


void EC::write_shards(const GFM& shards, const char* shardsdir){
    assert(shards.R>0 && shards.C==1);
    cout << "Write to Path: " << shardsdir << endl;
    fstream f;
    for(int i=0; i<shards.R; i++){
        string filename(shardsdir);
        filename += to_string(N) + "_" + to_string(K) + "_" + to_string(i);
        f.open(filename.c_str(),ios::out);
        f << (char)shards.M[i][0].get_value();
        f.close();
    }
}

void EC::encode(const GFM& raw, int k, const char* shardsdir){
    N = raw.R; K = k;
    assert(0<N && N<100 && 0<K && K<50);
    create_m(N, K);
    GFM shards = M.rdot(raw);
    cout << "Encoded shards: " << endl;
    shards.show();
    write_shards(shards, shardsdir);
}

void EC::read_shards(const char* shardsdir){
    DIR *dir; struct dirent *diread;
    regex r("\\d+_\\d+_\\d+");
    assert((dir = opendir(shardsdir)) != nullptr);
    while ((diread = readdir(dir)) != nullptr) {;
        string filename(diread->d_name);
        bool ifmatch = regex_match(filename, r);
        cout << filename << " " << ifmatch << endl;
        if(ifmatch){
            regex pattern("\\d+");
            smatch result;
            string::const_iterator iter_begin = filename.cbegin();
            string::const_iterator iter_end = filename.cend();
            while (regex_search(iter_begin, iter_end, result, pattern)){
                cout << "查找成功：" << result[0] << endl;
                // cout << "查找结果子串的在源串中的迭代器位置" << *result[0].first << endl;
                // cout << "查找结果子串的在源串后面的位置" << *result[0].second << endl;
                iter_begin = result[0].second;	//更新迭代器位置
            }
        }
    }
    closedir(dir);

}

EC::~EC(){
}