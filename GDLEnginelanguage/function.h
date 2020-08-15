#pragma once
#include <iostream>
#include <vector>
#include <ctype.h>
#include "utils.h"
#include <unordered_map>
#include <algorithm>
#include "expressions.h"
using namespace std;
class Functions {
public:
    string name;
    vector<string> args;
    vector<int> argument_types;
    int function_type;
    string player_name;
    int function_index = 0;
    static bool func_eval(string element);
    static Functions* get_function(const string &funct);
    static string get_simple_arguments(const string &arge, int &index);
    static string get_function_argument(const string &arge, int &index);
    void show_message();
    static void remove_spaces(const string &element, int &index);
    void add_function_type();
    static bool is_function_recursion(const string &input, int &index, int depth);
    static bool is_function(const string &input);
    static bool is_argument(const string &input, int &index, int depth);
    static string get_argument(string input, int index);
    static Functions *get_function_object(const string &input, int &index);
    static int argument_types_function(string &argument);
    static void process_line(string input);
    static bool processor(string init);
    static bool search_inits(Functions *current_funct);
    static bool evaluate(const string &param);
    string to_string();
    static bool get_responses(const string &param, int &index);
};
int get_string_response(const string &input, int &index);
string modifier(unordered_map<string, string> &elems, string &function_name, string &to_modify, int index);