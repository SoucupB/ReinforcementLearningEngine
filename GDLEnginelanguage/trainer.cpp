#include "function.h"

int mad(int a, int b) {
    return a + b;
}
vector<string> stated_variables;

vector<unsigned short> final_result;

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
            int x = Functions::evaluate_binary("mark(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            int o = Functions::evaluate_binary("circle(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            int b = Functions::evaluate_binary("free(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            int z = Functions::evaluate_binary("wall(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            if(x)
                cout << 1 << " ";
            if(o)
                cout << 2 << " ";
            if(b)
                cout << 0 << " ";
            if(z)
                cout << 3 << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void show_game_two() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            int x = Functions::evaluate_binary("mark(" + to_string(i * 3 + j + 1) + ")");
            int o = Functions::evaluate_binary("circle(" + to_string(i * 3 + j + 1) + ")");
            int b = Functions::evaluate_binary("free(" + to_string(i * 3 + j + 1) + ")");
            int z = Functions::evaluate_binary("wall(" + to_string(i * 3 + j + 1) + ")");
            if(x)
                cout << 1 << " ";
            if(o)
                cout << 2 << " ";
            if(b)
                cout << 0 << " ";
            if(z)
                cout << 3 << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void show_connect_four() {
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 7; j++) {
            int x = Functions::evaluate_binary("mark(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            int o = Functions::evaluate_binary("circle(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            int b = Functions::evaluate_binary("free(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            int z = Functions::evaluate_binary("wall(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            if(x)
                cout << 1 << " ";
            if(o)
                cout << 2 << " ";
            if(b)
                cout << 0 << " ";
            if(z)
                cout << 3 << " ";
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
    cout << "Compiled!\n";
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
          //  cout << get_best_action(9, constants, names, all_maps, 0) << "\n";
           // Functions::evaluate_binary(get_random_action(constants, names[0], all_maps));
            Functions::evaluate_binary(get_best_action(9, constants, names, all_maps, 0));
           // exit(0);
            show_connect_four();
            if(!Functions::evaluate_binary(terminal)) {
               // Functions::evaluate_binary(get_random_action(constants, names[1], all_maps));
                Functions::evaluate_binary(get_best_action(9, constants, names, all_maps, 1));
               // exit(0);
                show_connect_four();
            }
        }
        if(Functions::evaluate_binary(goal_first))
            f++;
        else
        if(Functions::evaluate_binary(goal_last))
            s++;
        else
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
int max_score = 50, draw_score = 25, min_score = -50;

int get_result_first_bot() {
    bool result = Functions::evaluate_binary("goal_last(a)");
    if(result) return min_score;
    result = Functions::evaluate_binary("draw()");
    if(result) return draw_score;
    return 0;
}

int get_result_second_bot() {
    bool result = Functions::evaluate_binary("goal_first(a)");
    if(result) return min_score;
    result = Functions::evaluate_binary("draw()");
    if(result) return draw_score;
    return 0;
}

int bot_action_first(int depth, int max_depth, int alpha, int beta, vector<unsigned short> &constants, vector<unsigned short> &player_names,
                     unordered_map<unsigned short, unsigned short> &all_maps, vector<unsigned short> &action) {
    int game_result = get_result_first_bot();
    if(game_result)
        return game_result;
    if(depth >= max_depth)
        return 0;
    int max_score = -1<<30;
    vector<unsigned short> best_action;
    vector< vector<unsigned short> > all_actions = get_total_binary_actions(constants, player_names[0], all_maps);
    random_shuffle ( all_actions.begin(), all_actions.end() );
    unordered_map<unsigned long long, bool> current_state = get_states_inits();
    for(int i = 0; i < all_actions.size(); i++) {
        Functions::evaluate_binary(all_actions[i]);
        int score = -bot_action_second(depth + 1, max_depth, -beta, -alpha, constants, player_names, all_maps, action);
        copy_current_state(current_state);
        if(abs(score) == draw_score)
            score = draw_score;
        if(score > max_score) {
            if(!depth)
                best_action = all_actions[i];
            max_score = score;
        }
        alpha = max(alpha, score);
        if(alpha >= beta && depth) {
            return alpha;
        }
    }
    if(depth == 0) {
        final_result = best_action;
    }
    return max_score;
}

int bot_action_second(int depth, int max_depth, int alpha, int beta, vector<unsigned short> &constants, vector<unsigned short> &player_names,
                      unordered_map<unsigned short, unsigned short> &all_maps, vector<unsigned short> &action) {
    int game_result = get_result_second_bot();
    if(game_result)
        return game_result;
    if(depth >= max_depth)
        return 0;
    int max_score = -1<<30;
    vector<unsigned short> best_action;
    vector< vector<unsigned short> > all_actions = get_total_binary_actions(constants, player_names[1], all_maps);
    random_shuffle ( all_actions.begin(), all_actions.end() );
    unordered_map<unsigned long long, bool> current_state = get_states_inits();
    for(int i = 0; i < all_actions.size(); i++) {
        Functions::evaluate_binary(all_actions[i]);
        int score = -bot_action_first(depth + 1, max_depth, -beta, -alpha, constants, player_names, all_maps, action);
        copy_current_state(current_state);
        if(abs(score) == draw_score)
            score = draw_score;
        if(score > max_score) {
            if(!depth)
                best_action = all_actions[i];
            max_score = score;
        }
        alpha = max(alpha, score);
        if(alpha >= beta && depth) {
            return alpha;
        }
    }
    if(depth == 0) {
        final_result = best_action;
    }
    return max_score;
}

vector<unsigned short> get_best_action(int max_depth, vector<unsigned short> &constants, vector<unsigned short> &player_name,
                     unordered_map<unsigned short, unsigned short> &all_maps, bool player) {
    vector<unsigned short> action;
    int score = 0;
    if(!player)
        score = bot_action_first(0, max_depth, -1<<30, 1<<30, constants, player_name, all_maps, action);
    else
        score = bot_action_second(0, max_depth, -1<<30, 1<<30, constants, player_name, all_maps, action);
    return final_result;
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