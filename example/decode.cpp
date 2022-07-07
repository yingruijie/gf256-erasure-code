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
    get_table_256();
    
    string shardsdir(argv[1]);
    if(shardsdir[shardsdir.length()-1] == '/') shardsdir.pop_back();
    
    int pos=shardsdir.find_last_of('/');
    string recoverdir("./data/recover");
	recoverdir + string("/") + shardsdir.substr(pos+1);
    
    EC ec;
    ec.read_shards(argv[1]);
    ec.decode();
    ec.write_recover((recoverdir + string("/") + shardsdir.substr(pos+1)).c_str());
    
    destory_table_256();
    return 0;
}