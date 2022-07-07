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
    cout << endl << "Vandermonde [N, K] = [" << N << ", " << K << "] matrix: " << endl;
    M.show();
}

void EC::read_file(const char * filename){
    cout << endl << "Input file path: " << endl << filename << endl;
    ifstream fin;
    fin.open(filename);
	assert(fin.is_open());
	
	int buffer[BUFSIZE], len=0; char c;
    memset(buffer, 0, sizeof(int)*BUFSIZE);

    // 按char读取并存到buffer
    cout << endl << "Get input string: " << endl << "========" << endl;
	while ((c=fin.get())!=EOF){
        buffer[len++] = (int)c;
		cout << c;
	}
    cout << endl << "========" << endl;
    assert(len > 0);
    fin.close();
    // 生成原始输入的GFM 列向量
    raw.create(len, 1);
    for(int i=0; i< len; i++){
        GF me(buffer[i]); raw.M[i][0] = me;
    }
    cout << endl << "Raw data GFM:" << endl;
    raw.show();
}



void EC::encode(int k){
    N = raw.R; K = k;
    assert(0<N && N<100 && 0<K && K<50);
    create_m(N, K);
    shards = M.rdot(raw);
    cout << endl << "Encoded shards: " << endl;
    shards.show();
    encoded = true;
}

void EC::write_shards(const char* shardsroot){
    assert(encoded = true && shards.R>0 && shards.C==1);
    // 获取当前时间
    time_t t; 
    tm* local;
    char buf[128]= {0};  
    t = time(NULL);
    local = localtime(&t);
    strftime(buf, 64, "%Y%m%d_%H%M%S", local);  
    string shardsdir = string(shardsroot) + string("/") + string(buf) ;

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
        f << (char)shards.M[i][0].get_value();
        f.close();
    }
}


void EC::read_shards(const char* shardsdir){
    cout << "Reading Shards... " << endl << shardsdir << endl;
    // 路径拼接
    DIR *dir; struct dirent *diread;
    // 定义shards的文件名正则表达式为 "%d_%d_%d"
    regex shards_pattern("\\d+_\\d+_\\d+");
    // 打开目录
    assert((dir = opendir(shardsdir)) != nullptr);

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
            fin.open((string(shardsdir) + string("/") + filename).c_str());
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
    remain_indices = new int [N];

    remain_shards.create(N, 1);

    for(i=0; i<N; i++){
        remain_indices[i] = indices_read[i];
        remain_shards.M[i][0] = buffer[i];
    }

}

void EC::decode(){
    create_m(N, K);

    // 根据索引选编码矩阵的行sgfm
    GFM shardsgfm = M.select_rows(remain_indices, N);

    
    cout << endl << "Remain shards: " << endl;
    remain_shards.show();

    cout << endl << "Remain shards M: " << endl; 
    shardsgfm.show();
    cout << endl << "Remain shards M inverse: " << endl; 
    GFM shardsgfmi = shardsgfm.inverse();
    shardsgfmi.show();

    recover = shardsgfmi.rdot(remain_shards);
    cout << endl << "Recover: " << endl;
    recover.show();


}

void EC::write_recover(const char* recoverpath){
    cout << endl << "To string: " << endl;
    char* recover_string = new char [N];
    for(int i=0; i<N; i++){
        recover_string[i] = (char)recover.M[i][0].get_value();
        cout << recover_string[i];
    }
    cout << endl;
    cout << endl << "Write recover to: " << endl << recoverpath << endl;;
    fstream f;
    f.open(recoverpath, ios::out);
    for(int i=0; i<N; i++) f << recover_string[i];
    f.close();
}

EC::~EC(){
}