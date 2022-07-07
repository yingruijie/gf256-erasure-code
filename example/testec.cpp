#include <iostream>
#include <table.hpp>
#include <gf.hpp>
#include <ec.hpp>
#include <string>
using namespace std;



int main(int argc, char** argv){
    get_table_256();

    GF a(3);
    GF b(3);
     
    cout << a.get_value() << " + " << b.get_value() << " = " << (a+b).get_value() << endl;
    cout << a.get_value() << " - " << b.get_value() << " = " << (a-b).get_value() << endl;
    cout << a.get_value() << " * " << b.get_value() << " = " << (a*b).get_value() << endl;
    cout << a.get_value() << " / " << b.get_value() << " = " << (a/b).get_value() << endl;

    // EC ec;
    // ec.create_m(12, 5);
    // int sl[12] = {0,1,3,4,6,7,8,10,11, 12,14,15};
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

    destory_table_256();
    return 0;
}