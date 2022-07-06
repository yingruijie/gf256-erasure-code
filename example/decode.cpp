#include <string>
#include <iostream>
#include <vector>
#include <dirent.h>
#include <regex>


using namespace std;

int main() {
    DIR *dir; struct dirent *diread;

    regex r("\\d+_\\d+_\\d+");

    if ((dir = opendir("../run/data/shards/")) != nullptr) {
        while ((diread = readdir(dir)) != nullptr) {;
            string filename(diread->d_name);
            bool ifmatch = regex_match(filename, r);
            cout << filename << " " << ifmatch << endl;
            if(ifmatch){
                regex pattern("\\d+");
                smatch result;
                string::const_iterator iter_begin = filename.cbegin();
                string::const_iterator iter_end = filename.cend();
                while (regex_search(iter_begin, iter_end, result, pattern)){
                    cout << "查找成功：" << result[0] << endl;
                    // cout << "查找结果子串的在源串中的迭代器位置" << *result[0].first << endl;
                    // cout << "查找结果子串的在源串后面的位置" << *result[0].second << endl;
                    iter_begin = result[0].second;	//更新迭代器位置
                }
            }
        }
        closedir (dir);
    } else {
        perror("opendir");
        return EXIT_FAILURE;
    }

    // for (auto file : files) cout << file << "| ";
    // cout << endl;

    return EXIT_SUCCESS;
}