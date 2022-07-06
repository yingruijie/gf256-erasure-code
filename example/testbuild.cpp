#include <iostream>
#include <table.hpp>
#include <gf.hpp>
#include <ec.hpp>

using namespace std;

int main(){
    get_table_256();

    // GF a(253);
    // GF b(2);
     
    // cout << a.get_value() << " + " << b.get_value() << " = " << (a+b).get_value() << endl;
    // cout << a.get_value() << " - " << b.get_value() << " = " << (a-b).get_value() << endl;
    // cout << a.get_value() << " * " << b.get_value() << " = " << (a*b).get_value() << endl;
    // cout << a.get_value() << " / " << b.get_value() << " = " << (a/b).get_value() << endl;

    EC ec(12, 5);
    destory_table_256();
    return 0;
}