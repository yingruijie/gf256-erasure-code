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


void EC::write_shards(const GFM& shards, const char* shardsroot){
    assert(shards.R>0 && shards.C==1);

    // 获取当前时间
    time_t t; 
    tm* local;
    char buf[128]= {0};  
    t = time(NULL);
    local = localtime(&t);
    strftime(buf, 64, "%Y%m%d_%H%M%S", local);  
    string shardsdir = string(shardsroot) + string(buf) ;

    // 通过命令的方式创建文件夹 
    // 例如./data/shards/20220706_195635
	string cmd("mkdir -p " + shardsdir);
	int ret = system(cmd.c_str());
	assert(ret==0);

    // 写到目标文件夹
    cout << "Write to Path: " << shardsdir << endl;
    fstream f;
    for(int i=0; i<shards.R; i++){
        string filename(shardsdir);
        filename += string("/") + to_string(N) + "_" + to_string(K) + "_" + to_string(i);
        f.open(filename.c_str(),ios::out);
        f << (char)shards.M[i][0].get_value();
        f.close();
    }
}

void EC::encode(const GFM& raw, int k, const char* shardsroot){
    N = raw.R; K = k;
    assert(0<N && N<100 && 0<K && K<50);
    create_m(N, K);
    GFM shards = M.rdot(raw);
    cout << "Encoded shards: " << endl;
    for(int i=0; i<N+K; i++){
        cout << setw(3) << i << " " << setw(3) <<  shards.M[i][0].get_value() << endl;
    }
    // shards.show();
    write_shards(shards, shardsroot);
}


void EC::read_shards(const char* shardsroot, const char* shardsname){
    cout << "Readding Shards... " << endl 
         << "Shardsroot: "  << shardsroot << endl
         << "Shardsname: " << shardsname << endl;
    // 路径拼接
    string shardsdir = string(shardsroot) + string(shardsname) + string("/");
    DIR *dir; struct dirent *diread;
    // 定义shards的文件名正则表达式为 "%d_%d_%d"
    regex shards_pattern("\\d+_\\d+_\\d+");
    // 打开目录
    assert((dir = opendir(shardsdir.c_str())) != nullptr);

    // 定义读取记录数组
    int shardsN = 0, shardsK = 0, i=0, j=0, indices_read[BUFSIZE]; 
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
            // 记录N K I
            // cout << "n_k_i " << n_k_i[0] << " " << n_k_i[1] << " " << n_k_i[2] << endl;
            if(i==0){
                shardsN = n_k_i[0]; shardsK = n_k_i[1];
            }
            else{
                assert(shardsN==n_k_i[0] && shardsK==n_k_i[1]);
            }
            indices_read[i] = n_k_i[2];

            // 读取这个shard
            ifstream fin;
            // cout << "Read shard path: " << (shardsdir+filename).c_str()<< endl;
            fin.open((shardsdir+filename).c_str());
            assert(fin.is_open());
            int thishardlen=0; char c[BUFSIZE];
            while((c[thishardlen++]=fin.get())!=EOF){
            }
            fin.close();
            // cout << "thishardlen " << thishardlen << endl;
            assert(thishardlen == 2); // 一次字符，一次EOF   
            

            // 记录到GF中
            cout << "here: c = " << (uint8_t)c[0] << endl;
            GF me((uint8_t)c[0]);
            buffer[i] = me;

            i++;
        } 
    }
    closedir(dir);

    int shardslen=i; N = shardsN; K = shardsK;
    assert(N>0 && K>0 && shardslen>=N);
    int* indices = new int [N];
    GFM shards; shards.create(N, 1);

    for(i=0; i<N; i++){
        indices[i] = indices_read[i];
        shards.M[i][0] = buffer[i];
    }


    create_m(N, K);

    // 根据索引选编码矩阵的行sgfm
    GFM shardsgfm = M.select_rows(indices, N);

    
    cout << "ndx   sha  shardsgfm" << endl;
    for(i=0; i<N; i++){
        cout << setw(3) << indices[i] << "   " << setw(3) << shards.M[i][0].get_value();
        for(j=0; j<N; j++){
            cout << setw(3) << shardsgfm.M[i][j].get_value() << " ";
        }
        cout << endl;
    }
    

    cout << "shardsgfm = " << endl; 
    shardsgfm.show();
    cout << "shardsgfmi = " << endl; 
    GFM shardsgfmi = shardsgfm.inverse();
    shardsgfmi.show();

    GFM res = shardsgfmi.rdot(shards);
    cout << "decode result:" << endl;
    res.show();
    cout << "to string: " << endl;
    for(i=0; i<N; i++){
        cout << (char)res.M[i][0].get_value();
    }
    cout << endl;
}

EC::~EC(){
}