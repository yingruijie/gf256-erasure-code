#include <iostream>
#include <table.hpp>
#include <gf.hpp>
#include <ec.hpp>
#include <string>
using namespace std;



GFM read_file(const char * filename){
    cout << "Get input file: " << filename << endl;
    ifstream fin;
    fin.open(filename);
	assert(fin.is_open());
	
	int buffer[BUFSIZE], len=0; char c;
    memset(buffer, 0, sizeof(int)*BUFSIZE);

    // 按char读取并存到buffer
    cout << "Get input: " << endl << "========" << endl;
	while ((c=fin.get())!=EOF){
        buffer[len++] = (int)c;
		cout << c;
	}
    cout << endl << "========" << endl;
    assert(len > 0);
    fin.close();
    // 生成原始输入的GFM 列向量
    GFM raw; raw.create(len, 1);
    for(int i=0; i< len; i++){
        GF me(buffer[i]); raw.M[i][0] = me;
    }
    cout << "Raw data GFM:" << endl;
    raw.show();
    return raw;
}


int main(int argc, char** argv){
    if(argc != 3){
        cout << "Usage: ./encode abcd 3" << endl;
        return 0;
    }
    get_table_256();

    GF a(3);
    GF b(3);
     
    cout << a.get_value() << " + " << b.get_value() << " = " << (a+b).get_value() << endl;
    cout << a.get_value() << " - " << b.get_value() << " = " << (a-b).get_value() << endl;
    cout << a.get_value() << " * " << b.get_value() << " = " << (a*b).get_value() << endl;
    cout << a.get_value() << " / " << b.get_value() << " = " << (a/b).get_value() << endl;

    EC ec;
    string filepath("./data/file");
    filepath += string("/") + string(argv[1]);
    GFM raw = read_file(filepath.c_str());
    int k=stoi(argv[2]);
    assert(k>0);
    ec.encode(raw, k, "./data/shards/");

    destory_table_256();
    return 0;
}