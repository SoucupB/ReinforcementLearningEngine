#pragma once
#include <iostream>
#include <vector>
#include <ctype.h>
#include "utils.h"
using namespace std;
class Functions {
public:
    string name;
    vector<string> args;
    vector<vector<Functions*> > childs;
    vector<int> signs;
    vector<int> argument_types;
    Functions *parent;
    vector<string> permited_strings = {"init", "next", "does", "legal", "true"};
    int function_type;
    string player_name;
    int function_index = 0;
    static Functions* get_function(string funct);
    static string get_simple_arguments(string arge, int &index);
    static string get_function_argument(string arge, int &index);
    void show_message();
    static int get_function_index(string mess, int index);
    static bool regex_function(string mess);
    static void remove_spaces(string element, int &index);
    void add_function_type();
    static bool is_function_recursion(string input, int &index);
    static bool is_function(string input);
    static bool is_argument(string input, int &index);
    static string get_argument(string input, int index);
    static Functions *get_function_object(string input, int &index);
    static string get_argument(string arge, int &index);
    static string get_arguments(string arge, int &index);
    static int argument_types_function(string argument);
};