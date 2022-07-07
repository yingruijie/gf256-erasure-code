#include <iostream>
#include <table.hpp>
#include <gf.hpp>
#include <ec.hpp>
#include <string>
using namespace std;


int main(int argc, char** argv){
    if(argc != 3){
        cout << "Usage: ./encode ./data/file/abcd 3" << endl;
        return 0;
    }
    int k=stoi(argv[2]); assert(k>0);

    get_table_256();

    EC ec;
    ec.read_file(argv[1]);
    ec.encode(k);
    ec.write_shards("./data/shards");

    destory_table_256();
    return 0;
}