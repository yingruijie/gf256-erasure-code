#include <iostream>
#include <table.hpp>
#include <gf.hpp>
#include <ec.hpp>


using namespace std;

int main(int argc, char **argv){
    if(argc!=2){
        cout << "Usage: ./decode 20220707_105758";
        return 0;
    }
    get_table_256();
    
    string shardsdir("./data/shards/");
    EC ec;
    ec.read_shards(shardsdir.c_str(), argv[1]);
    
    destory_table_256();
    return 0;
}