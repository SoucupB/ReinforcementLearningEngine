#pragma once
#include <iostream>
#include <vector>
#include <ctype.h>
#include "utils.h"
#include <unordered_map>
#include <algorithm>
#include "expressions.h"
#include <algorithm>
using namespace std;
class Functions {
public:
    string name;
    vector<string> args;
    vector<int> argument_types;
    int function_type;
    string player_name;
    int function_index = 0;
    static void transform_into_hash(Functions *funct, vector<unsigned short> &buffer, int offset);
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
    static void process_line_binary(string input);
    static bool processor(string init);
    static bool search_inits(Functions *current_funct);
    static bool evaluate(const string &param);
    string to_string();
    static bool get_responses(const string &param, int &index);
    static void free_mem(Functions *&func);
    static bool evaluate_binary(const string &param);
    static bool get_responses_binary(const string &param, int &index, vector<unsigned short> &buffer, int l);
    static bool evaluate_binary_expression(vector<unsigned short> def, unordered_map<unsigned short, unsigned short> &mapper);
    static bool evaluate_binary_vector(vector<unsigned short> param);
    static bool evaluate_binary(vector<unsigned short> param);
    static bool evaluate_binary_action(const string &param, char sign);
    static bool evaluate_legality(const string &param);
    static bool evaluate_action(const string &param);
};

//function.cpp
vector<unsigned short> get_message(unsigned short name, string param);
int get_string_response(const string &input, int &index);
string modifier(unordered_map<string, string> &elems, string &function_name, string &to_modify, int index);
bool is_action(Functions *input);
bool is_action_legal(Functions *input);
bool process_actions(Functions *&input);
bool get_negation(const string &param, int &index);
void add_legality_or_actions(Functions *funct);
int is_legal_or_actions(Functions *funct);
bool is_special_function(Functions *function);
bool add_special_function(Functions *&input);
bool is_different(vector<string> arguments, string name);
vector<string*> *get_first_player_actions();
vector<string*> *get_second_player_actions();
unordered_map<string, bool> *get_map();
void save_state();
void load_state();
unordered_map<string, string> create_map(vector<string> &alpha, vector<string> &beta);
void reserve_arrays();
int hasher(string elem);
int get_type(short number);
void function_describer(vector<unsigned short> &buffer, int offset, int depth);
string get_hash(int number);



//trainer.cpp
void show_all_inits();
vector<string> get_total_actions();
string get_random_action_first_player();
string get_random_action_second_player();
void get_strings(Functions *current_function, vector<string> &constants, int k, vector<string> &response);
void get_binary_strings(int k, vector<unsigned short> &constants, vector<unsigned short> to_modify, vector< vector<unsigned short> > &total_actions);
void play();
void simulate_player(vector<string> current_file, int total_matches);
int bot_action_first(int depth, int max_depth, int alpha, int beta, vector<unsigned short> &constants, vector<unsigned short> &player_names,
                     unordered_map<unsigned short, unsigned short> &all_maps, vector<unsigned short> &action);
int bot_action_second(int depth, int max_depth, int alpha, int beta, vector<unsigned short> &constants, vector<unsigned short> &player_names,
                      unordered_map<unsigned short, unsigned short> &all_maps, vector<unsigned short> &action);
vector<unsigned short> get_best_action(int max_depth, vector<unsigned short> &constants, vector<unsigned short> &player_name,
                                        unordered_map<unsigned short, unsigned short> &all_maps, bool player);

//binary_function.cpp
void predefined_hashed();
void generate_zovrist();
vector<unsigned short> get_total_params();
void show_set();
vector<unsigned short> get_names();
vector< vector<unsigned short> > get_legals(unsigned short name);
unordered_map<unsigned short, unsigned short> get_legal_maps();
void save_states();
void load_states();
unordered_map<unsigned long long, bool> get_states_inits();
void copy_current_state(unordered_map<unsigned long long, bool> &t_inits);