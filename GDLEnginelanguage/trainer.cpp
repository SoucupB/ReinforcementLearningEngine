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

void show_actions(vector< vector<unsigned short> > current_actions) {
    for(int i = 0; i < current_actions.size(); i++) {
        for(int j = 0; j < current_actions[i].size(); j++)
            cout << get_hash(current_actions[i][j]) << " ";
        cout << "\n";
    }
}

void legal_to_actions(vector< vector<unsigned short> > &current_actions, unordered_map<unsigned short, unsigned short> &all_maps) {
    for(int i = 0; i < current_actions.size(); i++) {
        current_actions[i][0] = all_maps[current_actions[i][0]];
    }
}

vector< vector<unsigned short> > get_total_binary_actions(vector<unsigned short> &constants, unsigned short player_name, unordered_map<unsigned short, unsigned short> &all_maps) {
    vector< vector<unsigned short> > total_actions;
    vector< vector<unsigned short> > current_actions = get_legals(player_name);
    for(int i = 0; i < current_actions.size(); i++)
        get_binary_strings(3, constants, current_actions[i], total_actions);
    legal_to_actions(total_actions, all_maps);
    return total_actions;
}

vector<unsigned short> get_random_action(vector<unsigned short> &constants, unsigned short player_name, unordered_map<unsigned short, unsigned short> &all_maps) {
    vector< vector<unsigned short> > all_acts = get_total_binary_actions(constants, player_name, all_maps);
    vector<unsigned short> null;
    if(all_acts.size())
        return all_acts[rand() * rand() % all_acts.size()];
    return null;
}

void play() {
    unordered_map<unsigned short, unsigned short> all_maps = get_legal_maps();
}

void show_game() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            int x = Functions::evaluate_binary("cell(" + to_string(i + 1) + ", " + to_string(j + 1) + ", x)");
            int o = Functions::evaluate_binary("cell(" + to_string(i + 1) + ", " + to_string(j + 1) + ", o)");
            int b = Functions::evaluate_binary("cell(" + to_string(i + 1) + ", " + to_string(j + 1) + ", b)");
            if(x)
                cout << 1 << " ";
            if(o)
                cout << 2 << " ";
            if(b)
                cout << 0 << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void simulate_player(vector<string> current_file, int total_matches) {
    generate_zovrist();
    predefined_hashed();
    for(int i = 0; i < current_file.size(); i++) {
        Functions::process_line_binary(current_file[i]);
    }
    unordered_map<unsigned short, unsigned short> all_maps = get_legal_maps();
    vector<unsigned short> names = get_names();
    vector<unsigned short> constants = get_total_params(), terminal, goal_first, goal_last, draw;
    Functions *first = Functions::get_function("terminal()"),
              *second = Functions::get_function("goal_first()"),
              *third = Functions::get_function("goal_last()"),
              *fourth = Functions::get_function("draw()");
    Functions::transform_into_hash(first, terminal, 0);
    Functions::transform_into_hash(second, goal_first, 0);
    Functions::transform_into_hash(third, goal_last, 0);
    Functions::transform_into_hash(fourth, draw, 0);
    Functions::free_mem(first);
    Functions::free_mem(second);
    Functions::free_mem(third);
    Functions::free_mem(fourth);
    int mathces = 0;
    int f = 0, s = 0, d = 0;
    save_states();
    while(mathces < total_matches) {
        load_states();
        while(!Functions::evaluate_binary(terminal)) {
            Functions::evaluate_binary(get_random_action(constants, names[0], all_maps));
            show_game();
            if(!Functions::evaluate_binary(terminal)) {
                Functions::evaluate_binary(get_random_action(constants, names[1], all_maps));
                show_game();
            }
        }
        if(Functions::evaluate_binary(goal_first))
            f++;
        if(Functions::evaluate_binary(goal_last))
            s++;
        if(Functions::evaluate_binary(draw))
            d++;
        mathces++;
    }
    cout << f << " " << s << " " << d;
}

void get_binary_strings(int k, vector<unsigned short> &constants, vector<unsigned short> to_modify, vector< vector<unsigned short> > &total_actions) {
    if(k == to_modify.size()) {
        if(Functions::evaluate_binary(to_modify)) {
            total_actions.push_back(to_modify);
        }
        return ;
    }
    for(int i = 0; i < constants.size(); i++) {
        if(get_type(to_modify[k]) == VARIABLE) {
            unsigned short cpy = to_modify[k];
            to_modify[k] = constants[i];
            get_binary_strings(k + 1, constants, to_modify, total_actions);
            to_modify[k] = cpy;
        }
    }
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