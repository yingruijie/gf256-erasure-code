#ifndef _UTILS_HPP_
#define _UTILS_HPP_
#include <time.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <dirent.h>
#include <regex>
#include <time.h>

using namespace std;

string get_datetime(void);


string readFileIntoString(const string& path);

#endif