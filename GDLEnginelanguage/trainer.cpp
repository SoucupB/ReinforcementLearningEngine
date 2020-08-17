#include "function.h"

int mad(int a, int b) {
    return a + b;
}
vector<string> stated_variables;

vector<string> get_total_actions(vector<string*> *actions) {
    vector<string> total_triess;
    unordered_map<string, bool> *mapper = get_map();
    if(!stated_variables.size()) {
        for(auto& it: *mapper) {
            stated_variables.push_back(it.first);
        }
    }
    for(int i = 0; i < actions->size(); i++) {
        Functions *current_func = Functions::get_function(*(*actions)[i]);
        get_strings(current_func, stated_variables, 0, total_triess);
        Functions::free_mem(current_func);
    }
    return total_triess;
}

void get_strings(Functions *current_function, vector<string> &constants, int k, vector<string> &response) {
    if(k == current_function->args.size()) {
        if(is_action_legal(current_function)) {
            response.push_back(current_function->to_string());
        }
        return ;
    }
    for(int i = 0; i < constants.size(); i++) {
        if(current_function->argument_types[k] == VARIABLE) {
            string *current_var = &current_function->args[k];
            current_function->args[k] = constants[i];
            get_strings(current_function, constants, k + 1, response);
            current_function->args[k] = *current_var;
        }
    }
}

string get_random_action_first_player() {
    vector<string> response = get_total_actions(get_first_player_actions());
    if(response.size())
        return response[rand() * rand() % response.size()];
    return "";
}

string get_random_action_second_player() {
    vector<string> response = get_total_actions(get_second_player_actions());
    if(response.size())
        return response[rand() * rand() % response.size()];
    return "";
}