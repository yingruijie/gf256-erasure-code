#include <utils.hpp>

using namespace std;

/**
 * @brief 获取时间字符串
 * @return "%Y%m%d_%H%M%S"格式的string
 */
string get_datetime(void){
    time_t t; 
    tm* local;
    char buf[128]= {0};  
    t = time(NULL);
    local = localtime(&t);
    strftime(buf, 64, "%Y%m%d_%H%M%S", local); 
    return string(buf);
}

/**
 * @brief 从文件中读取并存为string
 * @param path 文件路径
 * @return string
 * @exception path必须是可访问的
 */
string readFileIntoString(const string& path) {
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}