#include "function.h"

vector<long long> zovrist_keys_vector;
unordered_map<unsigned long long, bool> inits, c_inits;
unordered_map<unsigned short, vector< vector<unsigned short> > > definitions, does_param, legal_param;
unordered_map<unsigned short, vector< vector<unsigned short> > > params;
vector<unsigned short> names;
unordered_map<unsigned short, unsigned short> special_functions, consts_and_numbers, legal_and_actions;
int max_hash_size = 128;

vector<unsigned short> get_total_params() {
    vector<unsigned short> total_params;
    total_params.reserve(128);
    for(auto &l : consts_and_numbers) {
        total_params.push_back(l.first);
    }
    return total_params;
}

unordered_map<unsigned long long, bool> get_states_inits() {
    return inits;
}

void copy_current_state(unordered_map<unsigned long long, bool> &t_inits) {
    inits = t_inits;
}

void save_states() {
    c_inits = inits;
}

void load_states() {
    inits = c_inits;
}

vector<unsigned short> get_names() {
    return names;
}

vector< vector<unsigned short> > get_legals(unsigned short name) {
    return legal_param[name];
}

void predefined_hashed() {
    special_functions[hasher("init")] = INIT;
    special_functions[hasher("next")] = NEXT;
}

unsigned long long get_hash_predicate(vector<unsigned short> &hash_vector, int l, int r) {
    int k = 0;
    unsigned long long hasher = 0;
    for(int i = l; i <= r; i++) {
        hasher ^= (hash_vector[i] * zovrist_keys_vector[k++]);
    }
    return hasher;
}

void show_set() {
    cout << "\n";
    for(auto &l : consts_and_numbers) {
        cout << get_hash(l.first) << "\n";
    }
    cout << "\n";
}

bool add_special_binary_function(Functions *&input) {
    if(input->name == "init" && input->args.size() == 1)
    {
        Functions *response = Functions::get_function(input->args[0]);
        vector<unsigned short> hash_vector;
        Functions::transform_into_hash(response, hash_vector, 0);
        Functions::free_mem(response);
        inits[get_hash_predicate(hash_vector, 0, hash_vector.size() - 1)] = 1;
        for(int i = 3; i < hash_vector.size(); i++)
            consts_and_numbers[hash_vector[i]] = 1;
        return true;
    }
    if(input->name == "role")
    {
        if(input->args.size() != 1)
        {
            error("Role has only 1 param!");
        }
        if(names.size() >= 2)
        {
            error("Too many roles, only 2 are supported!");
        }
        names.push_back(hasher(input->args[0]));
        return true;
    }
    Functions::free_mem(input);
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

void add_legality_or_actions_binary(Functions *&funct) {
    if(funct->name == "legal" && funct->args.size() != 2) {
        error("Wrong number of arguments for legality");
    }
    if(funct->name == "does") {
        if(funct->args.size() != 2) {
            error("Wrong number of arguments for actions!");
        }
        vector<unsigned short> responser;
        responser.reserve(128);
        Functions *argv = Functions::get_function(funct->args[1]);
        argv->name = "+" + argv->name;
        Functions::transform_into_hash(argv, responser, 0);
        does_param[hasher(funct->args[0])].push_back(responser);
        Functions::free_mem(funct);
        funct = argv;
    }
    if(funct->name == "legal") {
        if(funct->args.size() != 2) {
            error("Wrong number of arguments for legality!");
        }
        vector<unsigned short> responser;
        responser.reserve(128);
        Functions *argv = Functions::get_function(funct->args[1]);
        string response = "+" + argv->name;
        argv->name = "-" + argv->name;
        legal_and_actions[hasher(argv->name)] = hasher(response);
        Functions::transform_into_hash(argv, responser, 0);
        legal_param[hasher(funct->args[0])].push_back(responser);
        Functions::free_mem(funct);
        funct = argv;
    }
}

unordered_map<unsigned short, unsigned short> get_legal_maps() {
    return legal_and_actions;
}


void Functions::process_line_binary(string input) {
    int index = 0, c_index = index;
    if(!is_function_recursion(input, index, 0)) {
        error("Syntax error");
    }
    Functions *first_operator = get_function(input.substr(c_index, index - c_index + 1)), *c_first_operator = NULL;
    add_legality_or_actions_binary(first_operator);
    vector<unsigned short> init_function, responser;
    Functions::transform_into_hash(first_operator, init_function, 0);
    index++;
    if(is_special_function(first_operator)) {
        add_special_binary_function(first_operator);
        return ;
    }
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
    Functions::free_mem(first_operator);
    definition_string.erase(remove(definition_string.begin(), definition_string.end(), ' '), definition_string.end());
    for(int i = 0; i < init_function[1]; i++) {
        responser.push_back(init_function[i]);
    }
    params[init_function[0]].push_back(responser);
    get_message(init_function[0], definition_string);
    return ;
}

bool search_inits_binary(vector<unsigned short> &hash_vector, int l, int r) {
    unsigned long long hasher = get_hash_predicate(hash_vector, l, r);
    return inits.find(hasher) != inits.end();
}

unsigned short get_sign(unsigned short sign) {
    return sign + params_offset * 4;
}

unsigned short get_sign_from_integer(unsigned short sign) {
    return sign - params_offset * 4;
}

vector< vector<unsigned short> > get_clossest_function(vector<unsigned short> &param, int start_offset) {
    int offset = param[2 + start_offset];
    vector< vector<unsigned short> > input = params[param[start_offset]], response;
    for(int i = 0; i < input.size(); i++) {
        bool check = 1;
        for(int j = 3; j < input[i][2]; j++) {
            if(get_type(input[i][j]) != VARIABLE && input[i][j] != param[j + start_offset]) {
                check = 0;
            }
        }
        if(check)
            response.push_back(input[i]);
    }
    return response;
}

unordered_map<unsigned short, unsigned short> get_equalizer(vector<unsigned short> &param, vector<unsigned short> &to_solve, int starter) {
    unordered_map<unsigned short, unsigned short> response;
    response.reserve(64);
    for(int i = starter; i < param.size(); i++) {
        response[to_solve[i - starter]] = param[i];
    }
    return response;
}

unsigned short get_current_element(unsigned short label, unordered_map<unsigned short, unsigned short> &mapper) {
    if(mapper[label])
        return get_hash_integers(mapper[label]);
    return get_hash_integers(label);
}

unsigned short get_expression_result(unsigned short label, unordered_map<unsigned short, unsigned short> &mapper) {
    vector<unsigned short> get_responser = get_expression_from_label(label);
    int element = get_current_element(get_responser[0], mapper);
    int index = 1;
    while(index < get_responser.size() && (get_sign_from_integer(get_responser[index]) == PLUS || get_sign_from_integer(get_responser[index]) == MINUS)) {
        if(get_sign_from_integer(get_responser[index]) == PLUS) {
            index++;
            element += get_current_element(get_responser[index], mapper);
        } else
        if(get_sign_from_integer(get_responser[index]) == MINUS) {
            index++;
            element -= get_current_element(get_responser[index], mapper);
        }
        index++;
    }
    return hasher_number(element) + 6 * params_offset;
}

vector<unsigned short> get_expression(vector<unsigned short> &def, int &index, unordered_map<unsigned short, unsigned short> &mapper) {
    vector<unsigned short> response;
    response.reserve(128);
    if(def[index] == get_sign(NOT)) {
        response.push_back(def[index]);
        index++;
    }
    int total_size = def[index + 1];
    for(int i = 0; i < total_size; i++) {

        if(mapper[def[i + index]])
            response.push_back(mapper[def[i + index]]);
        else {
            if(is_expression(def[i + index])) {
                unsigned short get_responser = get_expression_result(def[i + index], mapper);
                response.push_back(get_expression_result(def[i + index], mapper));
            }
            else
                response.push_back(def[i + index]);
        }
        // if(mapper[def[i + index]])
        //     response.push_back(mapper[def[i + index]]);
        // else
        //     response.push_back(def[i + index]);
    }
    index += total_size;
    // for(int i = 0; i < response.size(); i++)
    //     cout << get_hash(response[i]) << " ";
    // exit(0);
    return response;
}

bool Functions::evaluate_binary_expression(vector<unsigned short> def, unordered_map<unsigned short, unsigned short> &mapper) {
    int index = 0;
    bool element = evaluate_binary_vector(get_expression(def, index, mapper));
    while(index < def.size() && (def[index] == get_sign(AND) || def[index] == get_sign(OR))) {
        if(def[index] == get_sign(AND)) {
            index++;
            if(element)
                element &= evaluate_binary_vector(get_expression(def, index, mapper));
            else {
                if(def[index] == get_sign(NOT))
                    index += def[index + 2];
                else
                    index += def[index + 1];
            }
        }
        else
        if(def[index] == get_sign(OR)) {
            index++;
            if(!element)
                element |= evaluate_binary_vector(get_expression(def, index, mapper));
            else {
                if(def[index] == get_sign(NOT))
                    index += def[index + 2];
                else
                    index += def[index + 1];
            }
        }
    }
    return element;
}

bool is_special_func_runner(vector<unsigned short> &param) {
    int index = 0;
    if(param[index] == get_sign(NOT))
        index++;
    if(special_functions[param[index]] == INIT)
        return true;
    if(special_functions[param[index]] == NEXT)
        return true;
    return false;
}

bool process_special_function_run(vector<unsigned short> &param) {
    int index = 0;
    if(param[index] == get_sign(NOT))
        index++;
    if(special_functions[param[index]] == INIT) {
        int starting_index = 3;
        if(get_type(param[starting_index]) != PREDICATE) error("Init function allows only predicate arguments!");
        if(param[starting_index - 1] != 1) error("Init function has only one argument!");
        unsigned long long current_hash = get_hash_predicate(param, starting_index, starting_index + param[starting_index + 1] - 1);
        if(inits.find(current_hash) != inits.end())
            return false;
        inits[current_hash] = 1;
        return true;
    }
    if(special_functions[param[index]] == NEXT) {
        int starting_index = 3;
        if(get_type(param[starting_index]) != PREDICATE || get_type(param[starting_index + param[starting_index + 1]]) != PREDICATE) error("Next function allows only exclusively 2 predicates!");
        if(param[starting_index - 1] != 2) error("Next function has exclusively 2 arguments!");
        unsigned long long first_hash = get_hash_predicate(param, starting_index, starting_index + param[starting_index + 1] - 1);
        int ending_index = starting_index + param[starting_index + 1];
        unsigned long long second_hash = get_hash_predicate(param, ending_index, ending_index + param[ending_index + 1] - 1);
        if(inits.find(first_hash) == inits.end())
            return false;
        inits.erase(first_hash);
        if(inits.find(second_hash) != inits.end())
            return false;
        inits[second_hash] = 1;
        return true;
    }
    return false;
}

bool Functions::evaluate_binary_vector(vector<unsigned short> param) {
    bool response_query = false, negation = false;
    int starter = 0;
    if(param[0] == get_sign(NOT))
    {
        starter++;negation = !negation;
    }
    if(is_special_func_runner(param)) {
        return negation ^ process_special_function_run(param);
    }
   // for(int i = 0; i < param.size(); i++)
   //     cout << get_hash(param[i]) << " ";
   // cout << "\n";
  //  exit(0);
    // for(int i = 0; i < param.size(); i++) {
    //     if(param[i] == get_sign(NOT))
    //         cout << "~" << " ";
    //     else if(param[i] == get_sign(OR))
    //         cout << "|" << " ";
    //     else if(param[i] == get_sign(AND))
    //         cout << "&" << " ";
    //     else if(param[i] == get_sign(OPEN_PARANTH))
    //         cout << "(" << " ";
    //     else if(param[i] == get_sign(CLOSE_PARANTH))
    //         cout << ")" << " ";
    //     else
    //         cout << get_hash(param[i]) << " ";
    // }
    // cout << "\n";
    if(search_inits_binary(param, starter, param.size() - 1)) {
        return true ^ negation;
    }
    vector< vector<unsigned short> > response = get_clossest_function(param, starter);
    for(int i = 0; i < response.size(); i++) {
        unordered_map<unsigned short, unsigned short> mapper = get_equalizer(param, response[i], starter);
        response_query = evaluate_binary_expression(definitions[response[i][0]][i], mapper);
        if(response_query)
            return response_query ^ negation;
    }
    return response_query ^ negation;
}

bool Functions::evaluate_binary(vector<unsigned short> param) {
    return evaluate_binary_vector(param);
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
}

bool Functions::evaluate_legality(const string &param) {
    return evaluate_binary_action(param, '-');
}

bool Functions::evaluate_action(const string &param) {
    return evaluate_binary_action(param, '+');
}

bool Functions::evaluate_binary_action(const string &param, char sign) {
    Functions *initial_function = Functions::get_function(param);
    if(!initial_function) {
        return false;
    }
    initial_function->name = sign + initial_function->name;
    vector<unsigned short> hash_vector;
    Functions::transform_into_hash(initial_function, hash_vector, 0);
    Functions::free_mem(initial_function);
    return evaluate_binary_vector(hash_vector);
}

bool Functions::get_responses_binary(const string &param, int &index, vector<unsigned short> &buffer, int l) {
    bool negative_expression = get_negation(param, index);
    int c_index = index;
    bool first_param;
    if(param[index] == '(') {
        buffer.push_back(get_sign(OPEN_PARANTH));
        l++;
        int relative_index = 0;
        get_string_response(param, index);
        if(c_index == -1) {
            error("Incorrect function syntax!");
        }
        get_responses_binary(param.substr(c_index + 1, index - c_index - 1), relative_index, buffer, l);
        buffer.push_back(get_sign(CLOSE_PARANTH));
        l++;
    }
    else {
        if(!is_function_recursion(param, index, 0)) {
            error("Incorrect function syntax!");
        }
        Functions *current_param = get_function(param.substr(c_index, index - c_index + 1));
        if(negative_expression) {
            buffer.push_back(get_sign(NOT));
            l++;
        }
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
                buffer.push_back(get_sign(AND));
                l++;
                buffer.push_back(get_sign(OPEN_PARANTH));
                l++;
                int relative_index = 0;
                get_string_response(param, index);
                if(c_index == -1) {
                    error("Incorrect function syntax!");
                }
                get_responses_binary(param.substr(c_index + 1, index - c_index - 1), relative_index, buffer, l);
                buffer.push_back(get_sign(CLOSE_PARANTH));
                l++;
            }
            else {
                if(!is_function_recursion(param, index, 0)) {
                    error("Incorrect function syntax!");
                }
                buffer.push_back(get_sign(AND));
                l++;
                if(negative_expression) {
                    buffer.push_back(get_sign(NOT));
                    l++;
                }
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
                buffer.push_back(get_sign(OR));
                l++;
                buffer.push_back(get_sign(OPEN_PARANTH));
                l++;
                int relative_index = 0;
                get_string_response(param, index);
                if(c_index == -1) {
                    error("Incorrect function syntax!");
                }
                get_responses_binary(param.substr(c_index + 1, index - c_index - 1), relative_index, buffer, l);
                buffer.push_back(get_sign(CLOSE_PARANTH));
                l++;
            }
            else {
                if(!is_function_recursion(param, index, 0)) {
                    error("Incorrect function syntax!");
                }
                buffer.push_back(get_sign(OR));
                l++;
                if(negative_expression) {
                    buffer.push_back(get_sign(NOT));
                    l++;
                }
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
    // for(int i = 0; i < response.size(); i++) {
    //     cout << response[i] << " ";
    // }
    // cout << "\n";
    // for(int i = 0; i < response.size(); i++) {
    //     if(response[i] == get_sign(NOT))
    //         cout << "~" << " ";
    //     else if(response[i] == get_sign(OR))
    //         cout << "|" << " ";
    //     else if(response[i] == get_sign(AND))
    //         cout << "&" << " ";
    //     else if(response[i] == get_sign(OPEN_PARANTH))
    //         cout << "(" << " ";
    //     else if(response[i] == get_sign(CLOSE_PARANTH))
    //         cout << ")" << " ";
    //     else
    //         cout << get_hash(response[i]) << " ";
    // }
    // cout << "\n";
   // exit(0);
    definitions[name].push_back(response);
    return response;
}