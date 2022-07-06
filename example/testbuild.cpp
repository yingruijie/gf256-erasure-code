#include <iostream>
#include <table.hpp>
#include <gf.hpp>
#include <ec.hpp>

using namespace std;

#define BUFSIZE 256

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

    // 生成原始输入的GFM 列向量
    GFM raw; raw.create(len, 1);
    for(int i=0; i< len; i++){
        GF me(buffer[i]); raw.M[i][0] = me;
    }
    cout << "Raw data GFM:" << endl;
    raw.show();
    return raw;
}


int main(){
    get_table_256();

    // GF a(253);
    // GF b(2);
     
    // cout << a.get_value() << " + " << b.get_value() << " = " << (a+b).get_value() << endl;
    // cout << a.get_value() << " - " << b.get_value() << " = " << (a-b).get_value() << endl;
    // cout << a.get_value() << " * " << b.get_value() << " = " << (a*b).get_value() << endl;
    // cout << a.get_value() << " / " << b.get_value() << " = " << (a/b).get_value() << endl;

    EC ec;
    GFM raw = read_file("./data/file/example");
    
    ec.encode(raw, 5, "./data/shards/");

    destory_table_256();
    return 0;
}