#include <ec.hpp>

/**
* @brief 创建EC
*/
EC::EC(void){

}

/**
* @brief 析构EC
*/
EC::~EC(){
    // 释放remain_indices内存
    if(decoded){
        delete [] remain_indices;
    }
}


/**
* @brief 创建EC的Vandermonde矩阵
* @param n 数据量 N
* @param k 纠错容量 K
* @exception N: (0, MAXN) 否则报错
* @exception K: (0, MAXK)  否则报错
*/
void EC::create_m(int n,int k){
    N=n; K=k;
    // 限制N K大小
    assert(0<N && N<=MAXN && 0<K && K<=MAXK);
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
    cout << endl << "Vandermonde [N, K] = [" << N << ", " << K << "] matrix: " << endl;
    M.show();
}


/**
* @brief 读取原始数据并存在EC::raw中
* @param filename 文件路径 例如 ./data/file/file0
* @exception 路径存在且不为空文件
*/
void EC::read_file(const char * filename, int n){
    cout << endl << "Input file path: " << endl << filename << endl;
	
	int len=0;

    string rawstring = readFileIntoString(string(filename));

    cout << endl << "Get input string: " << endl; 
    cout << "========" << endl << rawstring << endl << "========" << endl;
    len = rawstring.length();

    assert(len > 0); 

    // 生成原始输入的GFM 列向量
    int padsize, shardsize;
    if(len % n) {padsize=n-(len%n); shardsize=(len+padsize)/n;}
    cout << endl << "Data size = " << len << ", pad size = " << padsize
    << ", shards size [N, B] = ["<< n << ", " << shardsize << "]" << endl;
    
    raw.create(n, shardsize);
    int k = 0;
    for(int i=0; i< n; i++){
        for(int j=0; j<shardsize; j++){
            if(k<len){GF me((uint8_t)rawstring[k++]); raw.M[i][j] = me;}
            else {GF zero(0); raw.M[i][j] = zero;}
        }
    }
    cout << endl << "Raw data GFM:" << endl;
    raw.show();
}


/**
* @brief 编码过程 M * raw = shards
* @param k 纠错容量K 
* @exception N: (0, 100) 否则报错
* @exception K: (0, 50)  否则报错
*/
void EC::encode(int k){
    N = raw.R; K = k;
    assert(0<N && N<MAXN && 0<K && K<MAXK);
    // 创建矩阵
    create_m(N, K);
    // 编码
    shards = M.rdot(raw);
    cout << endl << "Encoded shards: " << endl;
    shards.show();
    encoded = true;
}


/**
* @brief 写入shards到指定目录 
* @details 自动创建"%Y%m%d_%H%M%S"格式时间作为目录名
* @details 保存时按照("%d_%d_%d", N_K_i)的格式对每个shard命名
* @param shardsroot shards根目录
* @exception 保证shardsroot可访问
* @exception 必须先encode否则shards为空
*/
void EC::write_shards(const char* shardsroot){
    assert(encoded = true && shards.R>0 && shards.C>0);
    string shardsdir = string(shardsroot) + string("/") + get_datetime();

    // 通过命令的方式创建文件夹 
    // 例如./data/shards/20220706_195635
	string cmd("mkdir -p " + shardsdir);
	int ret = system(cmd.c_str());
	assert(ret==0);

    // 写到目标文件夹
    cout << endl << "Write shards to path: " << endl << shardsdir << endl;
    fstream f;
    for(int i=0; i<shards.R; i++){
        string filename(shardsdir);
        filename += string("/") + to_string(N) + "_" + to_string(K) + "_" + to_string(i);
        f.open(filename.c_str(), ios::out);
        for(int j=0; j<shards.C; j++){
            f << (char)shards.M[i][j].get_value();
        }
        f.close();
    }
}

/**
* @brief 读取指定目录的shards并保存到EC::remain_shards
* @details 读取时按照("%d_%d_%d", N_K_i)的格式对每个shard命名
* @details 读取同时记录索引i到EC::remain_indices
* @param shardsdir shards根目录
* @exception 保证shardsdir可访问
* @exception 每个shard的(N,K)和文件大小必须相同 i->[0,N)且不重复
* @exception 剩余的数据量>=N 最终只读取N个shard
*/
void EC::read_shards(const char* shardsdir){
    cout << "Reading Shards... " << endl << shardsdir << endl;
    // 路径拼接
    DIR *dir; struct dirent *diread;
    // 定义shards的文件名正则表达式为 "%d_%d_%d"
    regex shards_pattern("\\d+_\\d+_\\d+");
    // 打开目录
    assert((dir = opendir(shardsdir)) != nullptr);

    // 定义读取记录数组
    int shardsN=0, shardsK=0, shardSize=0, i=0, j=0, k=0, indices_read[BUFSIZE]; 
    GF buffer[BUFSIZE];
    memset(indices_read, 0, sizeof(int)*BUFSIZE);

    // 遍历目录
    while ((diread = readdir(dir)) != nullptr) {;
        // 获取文件名
        string filename(diread->d_name);
        // 检查文件名是否为shards的格式
        bool ifmatch = regex_match(filename, shards_pattern);
        // cout << filename << " " << ifmatch << endl;
        if(ifmatch){
            regex integer_pattern("\\d+");
            smatch result;
            string::const_iterator iter_begin = filename.cbegin();
            string::const_iterator iter_end = filename.cend();
            
            int n_k_i[3]; j=0; memset(n_k_i, 0, 3*sizeof(int));
            
            while (regex_search(iter_begin, iter_end, result, integer_pattern)){
                string temp(result[0]); 
                n_k_i[j++] = stoi(temp.c_str());
                //更新迭代器位置
                iter_begin = result[0].second;
            }
            
            // 读取这个shard
            string shardstring = readFileIntoString(string(shardsdir) + string("/") + filename);

            assert(shardstring.length() > 0); // 字符数量+一次EOF 

            // 记录N K I shardsize  
            if(i == 0){
                shardsN = n_k_i[0]; shardsK = n_k_i[1];
                shardSize = shardstring.length();
            }   
            else{
                // 保证所有shard都一致
                // cout <<  "Shardsize: " << shardSize << " and " << shardstring.length() << endl;
                assert(shardSize == shardstring.length());
                assert(shardsN==n_k_i[0] && shardsK==n_k_i[1]);
            }
            indices_read[i] = n_k_i[2];

            // 记录到GF中
            for(j=0; j<shardSize; j++){
                GF me((uint8_t)shardstring[j]);
                buffer[k++] = me;
            }
            i++;
        } 
    }
    closedir(dir);

    // 保证读取整块shard
    assert(k%shardSize==0);

    int shardslen=i; 
    // 对 N K 赋值
    N = shardsN; K = shardsK;
    // 保证剩余的数据量shardslen>=N
    assert(N>0 && K>0 && shardslen>=N);

    remain_indices = new int [N];

    remain_shards.create(N, shardSize);

    // 记录到EC中的数据结构
    k=0;
    for(i=0; i<N; i++){
        remain_indices[i] = indices_read[i];
        for(j=0; j<shardSize; j++){
            remain_shards.M[i][j] = buffer[k++];
        }
    }
    decoded = true;
}

/**
* @brief 解码过程 Mi = 1/M, Mi * remain_shards = recover 
* @details 从remain_indices作为行索引选出对应的M
*/
void EC::decode(){
    create_m(N, K);

    // remain_indices作为行索引选编码矩阵的行sgfm
    GFM shardsgfm = M.select_rows(remain_indices, N);

    cout << endl << "Remain shards: " << endl;
    remain_shards.show();

    cout << endl << "Remain shards M: " << endl; 
    shardsgfm.show();
    cout << endl << "Remain shards M inverse: " << endl; 
    // 取逆矩阵
    GFM shardsgfmi = shardsgfm.inverse();
    shardsgfmi.show();

    // 解码得到recover
    recover = shardsgfmi.rdot(remain_shards);
    cout << endl << "Recover: " << endl;
    recover.show();
}

/**
* @brief 把EC::revcover写到recoverpath
* @param recoverpath shards根目录
*/
void EC::write_recover(const char* recoverpath){
    // 转化为string可视化
    cout << endl << "To string: " << endl;
    char* recover_string = new char [recover.R * recover.C];
    int k=0;
    for(int i=0; i<recover.R; i++){
        for(int j=0; j<recover.C; j++){
            recover_string[k] = (char)recover.M[i][j].get_value();
            cout << recover_string[k++];
        }
    }
    cout << endl;
    cout << endl << "Write recover to: " << endl << recoverpath << endl;;
    // 写入recoverpath
    fstream f;
    f.open(recoverpath, ios::out);
    for(int i=0; i<recover.R * recover.C; i++) f << recover_string[i];
    f.close();
}
