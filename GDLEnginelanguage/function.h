#pragma once
#include <iostream>
#include <vector>
#include <ctype.h>
using namespace std;
enum {
    NUMBER,
    STRING,
    VARIABLE
};
class Functions {
public:
    string name;
    vector<string> args;
    vector<Functions*> childs;
    Functions *parent;
    static Functions get_function(string funct);
    static string get_argument(string arge, int &index);
    static string get_arguments(string arge, int &index);
    void show_message();
    static int get_function_index(string mess, int index);
    static bool regex_function(string mess);
    static void remove_spaces(string element, int &index);
};