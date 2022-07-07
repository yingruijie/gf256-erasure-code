#include <iostream>
#include <table.hpp>
#include <gf.hpp>
#include <ec.hpp>

using namespace std;

int main(int argc, char **argv){
    if(argc!=2){
        cout << "Usage: ./decode ./data/shards/20220707_105758";
        return 0;
    }
    // 生成GF256的指数表和对数表
    get_table_256();
    
    string shardsdir(argv[1]);
    // 去除最后的"/"
    if(shardsdir[shardsdir.length()-1] == '/') shardsdir.pop_back();
    
    int pos=shardsdir.find_last_of('/');
    string recoverdir("./data/recover");
    
    EC ec;
    // 读取shards
    ec.read_shards(argv[1]);
    // 解码过程
    ec.decode();
    // 写入recover
    ec.write_recover((recoverdir + string("/") + shardsdir.substr(pos+1)).c_str());
    
    destory_table_256();
    return 0;
}