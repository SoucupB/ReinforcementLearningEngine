#pragma once
#include <iostream>
#include <vector>
#include <ctype.h>
#include "utils.h"
#include <unordered_map>
using namespace std;
class Functions {
public:
    string name;
    vector<string> args;
    vector<vector<Functions*> > childs; //To be removed
    vector<int> signs; // To be removed
    vector<int> argument_types;
    class Definitions {
        public:
            unordered_map<string, vector<Functions *> > definitions;
            vector<Functions *> definition_vector;
            vector<char> definition_vector_signs;
         //   unordered_map<string, vector<int> > definitions_signs;
    };
    Functions *parent;
    Definitions *def = NULL;
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
    static Functions *get_function_at_index(string input, int &index);
    static void process_line(string input);
    static bool processor(string init);
    static bool search_inits(Functions *current_funct);
    string to_string();
    void deep_copy(Functions *fct);
    static bool get_responses(Functions *funct);
    static Functions *find_equalizer(Functions *funct);
};
//Functions *get_init(int i);