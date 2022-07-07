#include <iostream>
#include <assert.h>
#include <dirent.h>
#include <regex>
#include <cstdlib>
#include <ctime>
#include <list>

using namespace std;

int main(int argc, char** argv){
    if(argc!=3){
        cout << "Usage: ./rmshards ./data/shards/20220707_105758 5";
        return 0;
    }
    int removenum=stoi(argv[2]); assert(removenum>=0);
    cout << "Will remove " << removenum << " shards!" << endl;;
    string shardsdir(argv[1]);
    cout << "Reading shards dir: " << endl << shardsdir << endl;

    DIR *dir; struct dirent *diread;
    // 定义shards的文件名正则表达式为 "%d_%d_%d"
    regex shards_pattern("\\d+_\\d+_\\d+");
    int shardsnum = 0;
    list<string> shardsname;

    // 打开目录统遍文件
    assert((dir = opendir(shardsdir.c_str())) != nullptr);
    while ((diread = readdir(dir)) != nullptr) {;
        // 获取文件名
        string shardname(diread->d_name);
        // 检查文件名是否为shards的格式
        bool ifmatch = regex_match(shardname, shards_pattern);
        if(ifmatch){
            shardsnum+=1;
            shardsname.push_back(shardname);
        } 
    }
    closedir(dir);

    cout << endl << "Found " << shardsnum << " shards" << endl;
    if(removenum >= shardsnum){
        cout << "Error: remove nums >= shardsnum" << endl;
        return 0;
    }

    int remainshardsnum = shardsnum, remainnr = removenum;
    
    // 随机记录删除shards
    list<string> deleteshards;
    srand(int(time(0)));
    for(list<string>::iterator iter = shardsname.begin() ; iter != shardsname.end() ; iter++){
        // cout << *iter << " ";
        if((rand()%remainshardsnum) < remainnr){
            deleteshards.push_back(*iter);
            // cout << " will be deleted!";
            remainnr -= 1;
        }
        remainshardsnum -= 1; 
        // cout << endl;
    }
    
    // 开始删除
    for(list<string>::iterator iter = deleteshards.begin() ; iter != deleteshards.end() ; iter++){
        cout << "Deleting " << *iter << " ==> ";
        string cmd("rm " + shardsdir + string("/") + *iter);
        int ret = system(cmd.c_str());
        assert(ret==0);
        cout << "Finished" << endl;
    }

    return 0;
}