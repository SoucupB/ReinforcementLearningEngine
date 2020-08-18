#include "function.h"

vector<long long> zovrist_keys_vector;
unordered_map<unsigned long long, bool> inits;
unordered_map<unsigned short, vector< vector<unsigned short> > > definitions;
unordered_map<unsigned short, vector< vector<unsigned short> > > params;
int max_hash_size = 128;

unsigned long long get_hash_predicate(vector<unsigned short> &hash_vector, int l, int r) {
    int k = 0;
    unsigned long long hasher = 0;
    for(int i = l; i <= r; i++) {
        hasher ^= (hash_vector[i] * zovrist_keys_vector[k++]);
    }
    return hasher;
}

bool add_special_binary_function(Functions *&input) {
    if(input->name == "init" && input->args.size() == 1)
    {
        Functions *response = Functions::get_function(input->args[0]);
        vector<unsigned short> hash_vector;
        Functions::transform_into_hash(response, hash_vector, 0);
        Functions::free_mem(response);
        inits[get_hash_predicate(hash_vector, 0, hash_vector.size() - 1)] = 1;
        return true;
    }
    // if(input->name == "role" && input->args.size() == 1)
    // {
    //     if(player_names.size() >= 2)
    //     {
    //         error("Too many roles, only 2 are supported!");
    //     }
    //     player_names.push_back(input->args[0]);
    //     Functions::free_mem(input);
    //     return true;
    // }
    // if(input->name == "next") {
    //     if(player_names.size() != 2)
    //     {
    //         error("Wrong number of parameters for next!");
    //     }
    //     Functions *first = Functions::get_function(input->args[0]), *second = Functions::get_function(input->args[1]);
    //     if(!Functions::search_inits(first)) {
    //         Functions::free_mem(second);
    //         Functions::free_mem(first);
    //         Functions::free_mem(input);
    //         return false;
    //     }
    //     transform(first, second);
    //     Functions::free_mem(second);
    //     Functions::free_mem(first);
    //     Functions::free_mem(input);
    //     return true;
    // }
    // Functions::free_mem(input);
    return false;
}

void generate_zovrist() {
    if(!zovrist_keys_vector.size()) {
        for(int i = 0; i < max_hash_size; i++) {
            unsigned long long random_number = 1;
            while(random_number < (1LL<<35)) {
                random_number *= (unsigned long long)rand();
            }
            if(!(random_number % 2)) {
                random_number++;
            }
            zovrist_keys_vector.push_back(random_number);
        }
    }
}


void Functions::process_line_binary(string input) {
    int index = 0, c_index = index;
    if(!is_function_recursion(input, index, 0)) {
        error("Syntax error");
    }
    Functions *first_operator = get_function(input.substr(c_index, index - c_index + 1)), *c_first_operator = NULL;
    vector<unsigned short> init_function, responser;
    Functions::transform_into_hash(first_operator, init_function, 0);
    index++;
    if(is_special_function(first_operator)) {
        add_special_binary_function(first_operator);
        return ;
    }
    add_legality_or_actions(first_operator);
    remove_spaces(input, index);
    if(index + 1 < input.size() && input[index] == ':' && input[index + 1] == '-') {
        index += 2;
    }
    else {
        Functions::free_mem(first_operator);
        return ;
    }
    if(is_different(first_operator->args, first_operator->name)) {
        error("Multiple definition for function: '" + first_operator->name + "' multiple definitions are only allowed if the parameters are the same type!");
    }
    remove_spaces(input, index);
    int cutted_index = input.size() - index;
    string definition_string = input.substr(index, cutted_index);
    first_operator->function_type = PREDICATE;
    definition_string.erase(remove(definition_string.begin(), definition_string.end(), ' '), definition_string.end());
    for(int i = 0; i < init_function[1]; i++) {
        responser.push_back(init_function[i]);
    }
    params[init_function[0]].push_back(responser);
    get_message(init_function[0], definition_string);
    // if(is_legal_or_actions(first_operator) != WRONG)
    //     c_first_operator = get_function(first_operator->args[1]);
    // else
    //     c_first_operator = first_operator;
    // switch(is_legal_or_actions(first_operator)) {
    //     case LEGAL:
    //        // legal[c_first_operator->name].push_back(definition_string);
    //     //    legal_params[c_first_operator->name].push_back(c_first_operator->args);
    //         break;
    //     case ACTION_PREDICATE:
    //       //  actions[c_first_operator->name].push_back(definition_string);
    //      //   action_params[c_first_operator->name].push_back(c_first_operator->args);
    //         break;
    //     default:
    //       //  string_definitions[c_first_operator->name].push_back(definition_string);
    //       //  var_params[c_first_operator->name].push_back(c_first_operator->args);
    //         break;
    // }
    return ;
}

bool search_inits_binary(vector<unsigned short> &hash_vector, int l, int r) {
    unsigned long long hasher = get_hash_predicate(hash_vector, 0, hash_vector.size() - 1);
    return inits.find(hasher) != inits.end();
}

unsigned short get_sign(unsigned short sign) {
    return sign + 1024 * 4;
}

vector< vector<unsigned short> > get_clossest_function(vector<unsigned short> &param) {
    int offset = param[2];
    vector< vector<unsigned short> > input = params[param[0]], response;
    for(int i = 0; i < input.size(); i++) {
        bool check = 1;
        for(int j = 3; j < input[i][2]; j++) {
            if(get_type(input[i][j]) != VARIABLE && input[i][j] != param[j]) {
                check = 0;
            }
        }
        if(check)
            response.push_back(input[i]);
    }
    return response;
}

unordered_map<unsigned short, unsigned short> get_equalizer(vector<unsigned short> &param, vector<unsigned short> &to_solve) {
    unordered_map<unsigned short, unsigned short> response;
    response.reserve(64);
    for(int i = 0; i < param.size(); i++) {
        response[to_solve[i]] = param[i];
    }
    return response;
}

vector<unsigned short> get_expression(vector<unsigned short> &def, int &index, unordered_map<unsigned short, unsigned short> &mapper) {
    vector<unsigned short> response;
    response.reserve(128);
    int total_size = def[index + 1];
    for(int i = 0; i < total_size; i++) {
        if(mapper[def[i + index]])
            response.push_back(mapper[def[i + index]]);
        else
            response.push_back(def[i + index]);
    }
    index += total_size;
    return response;
}

bool Functions::evaluate_binary_expression(vector<unsigned short> def, unordered_map<unsigned short, unsigned short> &mapper) {
    int index = 0;
    bool element = evaluate_binary_vector(get_expression(def, index, mapper));
    while(index < def.size() && (def[index] == get_sign(AND) || def[index] == get_sign(OR))) {
        if(def[index] == get_sign(AND)) {
            index++;
            element &= evaluate_binary_vector(get_expression(def, index, mapper));
        }
        else
        if(def[index] == get_sign(OR)) {
            index++;
            element |= evaluate_binary_vector(get_expression(def, index, mapper));
        }
    }
    return element;
}

bool Functions::evaluate_binary_vector(vector<unsigned short> param) {
    bool response_query = false;
    if(search_inits_binary(param, 0, param.size() - 1)) {
        return true;
    }
    vector< vector<unsigned short> > response = get_clossest_function(param);
    for(int i = 0; i < response.size(); i++) {
        unordered_map<unsigned short, unsigned short> mapper = get_equalizer(param, response[i]);
        response_query = evaluate_binary_expression(definitions[response[i][0]][i], mapper);
        if(response_query)
            return response_query;
    }
    return response_query;
}


bool Functions::evaluate_binary(const string &param) {
    Functions *initial_function = Functions::get_function(param);
    vector<unsigned short> hash_vector;
    Functions::transform_into_hash(initial_function, hash_vector, 0);
    if(!initial_function) {
        return false;
    }
    Functions::free_mem(initial_function);
    return evaluate_binary_vector(hash_vector);
    // for(int i = 0; i < initial_function->args.size(); i++) {
    //     string evaluation_function = evaluate_expression(initial_function->args[i]);
    //     if(evaluation_function.size())
    //         initial_function->args[i] = evaluation_function;
    // }
    // if(!initial_function) {
    //     Functions::free_mem(initial_function);
    //     return false;
    // }
    // if(is_special_function(initial_function)) {
    //     return add_special_function(initial_function);
    // }
    // if(is_action(initial_function)) {
    //     return process_actions(initial_function);
    // }
    // if(search_inits(initial_function)) {
    //     Functions::free_mem(initial_function);
    //     return true;
    // }
    // bool final_response = false;
    // vector<vector<string>* > appropiate_definition = search_params(initial_function, var_params);
    // for(int i = 0; i < appropiate_definition.size(); i++) {
    //     int index = 0;
    //     if(!appropiate_definition[i]) {
    //         Functions::free_mem(initial_function);
    //         return false;
    //     }
    //     unordered_map<string, string> respa = create_map(*appropiate_definition[i], initial_function->args);
    //     string response = modifier(respa, initial_function->name, string_definitions[initial_function->name][i], i, var_params);
    //     final_response = get_responses(response, index);
    //     if(final_response) {
    //         Functions::free_mem(initial_function);
    //         return final_response;
    //     }
    // }
    // Functions::free_mem(initial_function);
    // return final_response;
}

bool Functions::get_responses_binary(const string &param, int &index, vector<unsigned short> &buffer, int l) {
    bool negative_expression = get_negation(param, index);
    int c_index = index;
    bool first_param;
    if(param[index] == '(') {
        int relative_index = 0;
        get_string_response(param, index);
        if(c_index == -1) {
            error("Incorrect function syntax!");
        }
        get_responses_binary(param.substr(c_index + 1, index - c_index - 1), relative_index, buffer, l);
    }
    else {
        if(!is_function_recursion(param, index, 0)) {
            error("Incorrect function syntax! aa");
        }
        Functions *current_param = get_function(param.substr(c_index, index - c_index + 1));
        Functions::transform_into_hash(current_param, buffer, l);
        Functions::free_mem(current_param);
        l += buffer[l + 1];
    }
    index++;
    negative_expression = get_negation(param, index);
    while(index < param.size() && (param[index] == '|' || param[index] == '&')) {
        if(param[index] == '&') {
            index++;
            negative_expression = get_negation(param, index);
            c_index = index;
            if(param[index] == '(') {
                int relative_index = 0;
                get_string_response(param, index);
                if(c_index == -1) {
                    error("Incorrect function syntax!");
                }
                get_responses_binary(param.substr(c_index + 1, index - c_index - 1), relative_index, buffer, l);
            }
            else {
                if(!is_function_recursion(param, index, 0)) {
                    error("Incorrect function syntax!");
                }
                buffer.push_back(get_sign(AND));
                l++;
                Functions *current_param = get_function(param.substr(c_index, index - c_index + 1));
                Functions::transform_into_hash(current_param, buffer, l);
                Functions::free_mem(current_param);
                l += buffer[l + 1];
            }
        }
        else if(param[index] == '|') {
            index++;
            negative_expression = get_negation(param, index);
            c_index = index;
            if(param[index] == '(') {
                int relative_index = 0;
                get_string_response(param, index);
                if(c_index == -1) {
                    error("Incorrect function syntax!");
                }
                get_responses_binary(param.substr(c_index + 1, index - c_index - 1), relative_index, buffer, l);
            }
            else {
                if(!is_function_recursion(param, index, 0)) {
                    error("Incorrect function syntax!");
                }
                buffer.push_back(get_sign(OR));
                l++;
                Functions *current_param = get_function(param.substr(c_index, index - c_index + 1));
                Functions::transform_into_hash(current_param, buffer, l);
                Functions::free_mem(current_param);
                l += buffer[l + 1];
            }
        }
        index++;
    }
    return first_param;
}

vector<unsigned short> get_message(unsigned short name, string param) {
    int index = 0, left = 0;
    vector<unsigned short> response;
    response.reserve(256);
    Functions::get_responses_binary(param, index, response, left);
    definitions[name].push_back(response);
    return response;
}