#include <iostream>
#include <table.hpp>
#include <gf.hpp>
#include <ec.hpp>


using namespace std;

int main(int argc, char **argv){
    get_table_256();
    
    string shardsdir("./data/shards/");
    EC ec;
    ec.read_shards(shardsdir.c_str(), "20220706_195644");
    
    destory_table_256();
    return 0;
}