#include "function.h"
unordered_map<string, int> inits, c_inits;
vector<Functions*> predicate_definitions;
unordered_map<string, vector<string> > marker;
unordered_map<string, vector< vector<string> > > var_params, legal_params, action_params;
unordered_map<string, vector<string> > string_definitions, actions, legal;
unordered_map<string, bool> does;
vector<string> player_names;
unordered_map<string*, bool> first_player_legality, second_player_legality, first_player_action, second_player_action;
vector<string*> first_action_names, second_action_names;
unordered_map<string, bool> *inits_vars = new unordered_map<string, bool>;
unordered_map<int, string> hash_mapper;

int hasher(string elem) {
    int element = std::hash<std::string>()(elem) % 1024;
    while(hash_mapper[element].size()) {
        if(hash_mapper[element] == elem)
            return element;
        element = (element + 3) % 1024;
    }
    hash_mapper[element] = elem;
    return element;
}

string get_hash(int number) {
    if(number < 1024)
        return hash_mapper[number];
    if(number < 1024 * 2)
        return hash_mapper[number - 1024];
    if(number < 1024 * 3)
        return hash_mapper[number - 1024 * 2];
    return hash_mapper[number - 1024 * 3];
}

void Functions::transform_into_hash(Functions *funct, vector<unsigned short> &buffer, int offset) {
    int initial_offset = offset;
    buffer.push_back(hasher(funct->name));
    buffer.push_back(0);
    int start_offset = offset + 1;
    buffer.push_back(funct->args.size());
    offset += 3;
    for(int i = 0; i < funct->args.size(); i++) {
        if(funct->argument_types[i] == PREDICATE) {
            Functions *child = get_function(funct->args[i]);
            transform_into_hash(child, buffer, offset);
            free_mem(child);
            offset += buffer[offset + 1];
        } else
        if(funct->argument_types[i] == VARIABLE) {
            buffer.push_back(hasher(funct->args[i]) + 1024);
            offset++;
        } else
        if(funct->argument_types[i] == NUMBER) {
            buffer.push_back(hasher(funct->args[i]) + 1024 * 2);
            offset++;
        } else
        if(funct->argument_types[i] == CONSTANT) {
            buffer.push_back(hasher(funct->args[i]) + 1024 * 3);
            offset++;
        }
    }
    buffer[start_offset] = offset - initial_offset;
}

void function_describer(vector<unsigned short> &buffer, int offset, int depth) {
    cout << spaces(depth) << "The function has " << buffer[offset + 1] << " parameters!\n";
    offset += 2;
    int c_offset = offset;
    for(int i = 0; i < buffer[c_offset]; i++) {
        if(get_type(buffer[i + offset + 1]) == VARIABLE) {
            cout << spaces(depth) << i << "'th param is a VARIABLE\n";
        } else
        if(get_type(buffer[i + offset + 1]) == NUMBER) {
            cout << spaces(depth) << i << "'th param is a NUMBER\n";
        } else
        if(get_type(buffer[i + offset + 1]) == PREDICATE) {
            cout << spaces(depth) << i << "'th param is a PREDICATE\n";
            function_describer(buffer, i + offset + 1, depth + 1);
            offset += buffer[i + offset + 2];
        } else
        if(get_type(buffer[i + offset + 1]) == CONSTANT) {
            cout << spaces(depth) << i << "'th param is a CONSTANT\n";
        }
    }
}

int get_type(short number) {
    if(number < 1024)
        return PREDICATE;
    if(number < 1024 * 2)
        return VARIABLE;
    if(number < 1024 * 3)
        return NUMBER;
    return CONSTANT;
}

void reserve_arrays() {
    predicate_definitions.reserve(256);
    player_names.reserve(16);
    second_action_names.reserve(128);
    second_action_names.reserve(128);
}

vector<string*> *get_first_player_actions() {
    return &first_action_names;
}

vector<string*> *get_second_player_actions() {
    return &second_action_names;
}

unordered_map<string, bool> *get_map() {
    return inits_vars;
}

void save_state() {
    c_inits = inits;
}

void load_state() {
    inits = c_inits;
}

void Functions::add_function_type() {
    if(this->name == "init")
        this->function_type = INIT;
    else if(this->name == "true")
        this->function_type = TRUE;
    else if(this->name == "legal")
        this->function_type = LEGAL;
    else if(this->name == "does")
        this->function_type = DOES;
    else if(this->name == "next")
        this->function_type = NEXT;
    else if(this->name == "role")
        this->function_type = ROLE;
    else this->function_type = PREDICATE;
}

Functions *Functions::get_function(const string &funct) {
    int index = 0;
    Functions *response = get_function_object(funct, index);
    if(index < (int)funct.size() - 1 && !funct.size()) {
        free(response);
        return NULL;
    }
    response->add_function_type();
    return response;
}

void show_all_inits() {
    for(auto& it: inits) {
        cout << it.first << "\n";
    }
}

void Functions::free_mem(Functions *&func) {
    if(func) {
        delete func;
        func = NULL;
    }
}

Functions *Functions::get_function_object(const string &input, int &index) {
    string function_name = "";
    remove_spaces(input, index);
    while(index < input.size() && (isdigit(input[index]) || isalpha(input[index]) || input[index] == '_')) {
        function_name += input[index];
        index++;
    }
    if(function_name == "")
        return NULL;
    if(!isalpha(function_name[0]))
        return NULL;
    if(index == input.size() || input[index] != '(')
        return NULL;
    Functions *function = new Functions;
    function->name = function_name;
    while(index < input.size() && input[index] != ')') {
        index++;
        string current_arg = get_simple_arguments(input, index);
        if(current_arg == "")
            return function;
        if(input[index] != ',' && input[index] != ')')
            return NULL;
        function->args.push_back(current_arg);
        function->argument_types.push_back(argument_types_function(current_arg));
    }
    if(index == input.size())
        return NULL;
    if(input[index] != ')') {
        return NULL;
    }
    return function;
}

string Functions::get_function_argument(const string &input, int &index) {
    string function_name = "";
    remove_spaces(input, index);
    while(index < input.size() && (isdigit(input[index]) || isalpha(input[index]) || input[index] == '_')) {
        function_name += input[index];
        index++;
    }
    if(function_name == "")
        return "";
    if(!isalpha(function_name[0]))
        return "";
    if(index == input.size() || input[index] != '(')
        return "";
    string response = function_name + "(";
    while(index < input.size() && input[index] != ')') {
        index++;
        string current_argument = get_simple_arguments(input, index);
        if(current_argument == "")
            return "";
        response += current_argument;
        if(input[index] != ',' && input[index] != ')')
            return "";
        if(input[index] == ',' )
            response += ", ";
        if(input[index] == ')' )
            response += ")";
    }
    if(index == input.size())
        return "";
    if(input[index] != ')') {
        return "";
    }
    return response;
}

string Functions::get_simple_arguments(const string &input, int &index) {
    remove_spaces(input, index);
    string argument = "";
    int c_index = index;
    while(index < input.size() && (isdigit(input[index]) || is_arithmetic_sign(input[index]) || isalpha(input[index]))) {
        argument += input[index];
        index++;
    }
    if(argument == "")
        return "";
    if(input[index] == '(') {
        index = c_index;
        string inner_function = get_function_argument(input, index);
        index++;
        return inner_function;
    }
    remove_spaces(input, index);
    return argument;
}

bool Functions::is_function(const string &input) {
    int index = 0;
    bool is_funct = is_function_recursion(input, index, 0);
    if(index < input.size() - 1)
        return false;
    return is_funct;
}

int Functions::argument_types_function(string &argument) {
    if(is_integer(argument))
        return NUMBER;
    if(is_variable(argument))
        return VARIABLE;
    if(is_function_var(argument))
        return PREDICATE;
    return CONSTANT;
}

bool Functions::is_function_recursion(const string &input, int &index, int depth) {
    string function_name = "";
    remove_spaces(input, index);
    while(index < input.size() && (isdigit(input[index]) || isalpha(input[index]) || input[index] == '_')) {
        function_name += input[index];
        index++;
    }
    if(function_name == "")
        return false;
    if(!isalpha(function_name[0]))
        return false;
    if(index == input.size() || input[index] != '(')
        return false;
    index++;
    while(index < input.size() && input[index] != ')') {
        bool is_arg = is_argument(input, index, depth + 1);
        if(!is_arg)
            return false;
        if(input[index] != ',' && input[index] != ')')
            return false;
        if(input[index] != ')')
            index++;
    }
    if(index == input.size())
        return false;
    if(input[index] != ')') {
        return false;
    }
    return true;
}

bool Functions::is_argument(const string &input, int &index, int depth) {
    remove_spaces(input, index);
    string argument = "";
    int c_index = index;
    while(index < input.size() && (isdigit(input[index]) || is_arithmetic_sign(input[index]) || isalpha(input[index]))) {
        argument += input[index];
        index++;
    }
    if(argument == "")
        return false;
    if(input[index] == '(') {
        index = c_index;
        bool inner_function = is_function_recursion(input, index, depth + 1);
        index++;
        return inner_function;
    }
    remove_spaces(input, index);
    return true;
}

void Functions::show_message() {
    if(!this)
        error("Function is null!!");
    cout << "Name is: " << this->name << "\nArguments: ";
    for(int i = 0; i < this->args.size(); i++) {
        cout << this->args[i] << " ";
    }
    cout << "\n";
}

void Functions::remove_spaces(const string &element, int &index) {
    while(index < element.size() && element[index] == ' ')
        index++;
}

string Functions::to_string() {
    string full_argument = "";
    for(int i = 0; i < (int)this->args.size() - 1; i++)
        full_argument += this->args[i] + ",";
    if(this->args.size())
        full_argument += this->args[this->args.size() - 1];
    return this->name + "(" + full_argument + ")";
}

bool is_different(vector<string> arguments, string name) {
    if(!marker[name].size()) {
        marker[name] = arguments;
        return false;
    }
    if(marker[name].size() != arguments.size())
        return true;
    for(int i = 0; i < arguments.size(); i++) {
        if(Functions::argument_types_function(arguments[i]) != Functions::argument_types_function(marker[name][i])) {
            return true;
        }
    }
    marker[name] = arguments;
    return false;
}

void transform(Functions *first, Functions *second) {
    inits.erase(first->to_string());
    inits[second->to_string()] = true;
}

void add_params(vector<string> args) {
    for(int i = 0; i < args.size(); i++) {
        (*inits_vars)[args[i]] = true;
    }
}

bool add_special_function(Functions *&input) {
    if(input->name == "init" && input->args.size() == 1)
    {
        Functions *response = Functions::get_function(input->args[0]);
        inits[response->to_string()] = 1;
        input->function_type = INIT;
        add_params(response->args);
        Functions::free_mem(response);
        Functions::free_mem(input);
        return true;
    }
    if(input->name == "role" && input->args.size() == 1)
    {
        if(player_names.size() >= 2)
        {
            error("Too many roles, only 2 are supported!");
        }
        player_names.push_back(input->args[0]);
        Functions::free_mem(input);
        return true;
    }
    if(input->name == "next") {
        if(player_names.size() != 2)
        {
            error("Wrong number of parameters for next!");
        }
        Functions *first = Functions::get_function(input->args[0]), *second = Functions::get_function(input->args[1]);
        if(!Functions::search_inits(first)) {
            Functions::free_mem(second);
            Functions::free_mem(first);
            Functions::free_mem(input);
            return false;
        }
        transform(first, second);
        Functions::free_mem(second);
        Functions::free_mem(first);
        Functions::free_mem(input);
        return true;
    }
    Functions::free_mem(input);
    return false;
}

bool is_special_function(Functions *function) {
    if(function->function_type == INIT) {
        return true;
    }
    if(function->function_type == ROLE) {
        return true;
    }
    if(function->function_type == NEXT) {
        return true;
    }
    return false;
}

int is_legal_or_actions(Functions *funct) {
    if(funct->name == "legal")
        return LEGAL;
    if(funct->name == "does")
        return ACTION_PREDICATE;
    return WRONG;
}

void add_legality_or_actions(Functions *funct) {
    if(funct->name == "legal" && funct->args.size() != 2) {
        error("Wrong number of arguments for legality");
    }
    if(funct->name == "does") {
        if(funct->args.size() != 2) {
            error("Wrong number of arguments for actions");
        }
        if(funct->args[0] == player_names[0])
            first_action_names.push_back(&funct->args[1]);
        else
            second_action_names.push_back(&funct->args[1]);
    }
}

void Functions::process_line(string input) {
    int index = 0, c_index = index;
    if(!is_function_recursion(input, index, 0)) {
        error("Syntax error");
    }
    Functions *first_operator = get_function(input.substr(c_index, index - c_index + 1)), *c_first_operator = NULL;
    index++;
    if(is_special_function(first_operator)) {
        add_special_function(first_operator);
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
    if(is_legal_or_actions(first_operator) != WRONG)
        c_first_operator = get_function(first_operator->args[1]);
    else
        c_first_operator = first_operator;
    switch(is_legal_or_actions(first_operator)) {
        case LEGAL:
            legal[c_first_operator->name].push_back(definition_string);
            legal_params[c_first_operator->name].push_back(c_first_operator->args);
            break;
        case ACTION_PREDICATE:
            actions[c_first_operator->name].push_back(definition_string);
            action_params[c_first_operator->name].push_back(c_first_operator->args);
            break;
        default:
            string_definitions[c_first_operator->name].push_back(definition_string);
            var_params[c_first_operator->name].push_back(c_first_operator->args);
            break;
    }
    return ;
}

string modifier(unordered_map<string, string> &elems, string &function_name, string &to_modify, int index, unordered_map<string, vector< vector<string> > > &var_params) {
    char final_str[512] = {0};
    if(!elems.size())
        return to_modify;
    int char_index = 0, indexes[512] = {0}, strings[512] = {0}, str_indexes = 0;
    for(int i = 0; i < to_modify.size(); i++) {
        for(int j = 0; j < var_params[function_name][index].size(); j++) {
            bool checker = 1;
            for(int k = 0; checker == 1, k < var_params[function_name][index][j].size(); k++) {
                if(i + k >= to_modify.size())
                    checker = 0;
                if(i + k < to_modify.size() && var_params[function_name][index][j][k] != to_modify[i + k])
                    checker = 0;
            }
            if(checker) {
                indexes[str_indexes] = i;
                strings[str_indexes] = j;
                str_indexes++;
            }
        }
    }
    int k = 0;
    for(int i = 0; i < to_modify.size(); i++) {
        if(k >= i || i != indexes[k]) {
            final_str[char_index++] = to_modify[i];
        }
        else {
            string equalizer = elems[var_params[function_name][index][strings[k]]];
            for(int j = 0; j < equalizer.size(); j++) {
                final_str[char_index++] = equalizer[j];
            }
            i += (int)var_params[function_name][index][strings[k]].size() - 1;
            k++;
        }
    }
    return string(final_str);
}

vector< vector<string>* > search_params(Functions *fct, unordered_map<string, vector< vector<string> > > &var_params) {
    bool traversed = false;
    string key_map = fct->name;
    vector< vector<string>* > response;
    response.reserve(32);
    for(int i = 0; i < var_params[key_map].size(); i++) {
        bool checker = true;
        traversed = true;
        for(int j = 0; j < fct->args.size(); j++)
        {
            if(fct->args.size() != var_params[key_map][i].size()) {
                checker = false;
                break;
            }
            else {
                if(Functions::argument_types_function(var_params[key_map][i][j]) != VARIABLE && fct->args[j] != var_params[key_map][i][j])
                {
                    checker = false;
                    break;
                }
            }
        }
        if(checker) {
            response.push_back(&var_params[key_map][i]);
        }
    }
    return response;
}

unordered_map<string, string> create_map(vector<string> &alpha, vector<string> &beta) {
    unordered_map<string, string> response;
    if(alpha.size() != beta.size())
        return response;
    for(int i = 0; i < alpha.size(); i++) {
        response[alpha[i]] = beta[i];
    }
    return response;
}

bool Functions::func_eval(string element) {
    return evaluate(element);
}

bool is_action(Functions *input) {
    if(actions[input->name].size()) {
        return true;
    }
    return false;
}

bool is_action_legal(Functions *input) {
    if(legal[input->name].size()) {
        int index = 0;
        vector<vector<string>* > appropiate_definition = search_params(input, legal_params);
        unordered_map<string, string> respa = create_map(*appropiate_definition[0], input->args);
        string response = modifier(respa, input->name, legal[input->name][0], 0, legal_params);
        return Functions::get_responses(response, index);
    }
    return true;
}

bool process_actions(Functions *&input) {
    if(!is_action_legal(input)) {
        Functions::free_mem(input);
        return false;
    }
    int index = 0;
    vector<vector<string>* > appropiate_definition = search_params(input, action_params);
    unordered_map<string, string> respa = create_map(*appropiate_definition[0], input->args);
    string response = modifier(respa, input->name, actions[input->name][0], 0, action_params);
    Functions::free_mem(input);
    return Functions::get_responses(response, index);
}

bool Functions::evaluate(const string &param) {
    Functions *initial_function = Functions::get_function(param);
    for(int i = 0; i < initial_function->args.size(); i++) {
        string evaluation_function = evaluate_expression(initial_function->args[i]);
        if(evaluation_function.size())
            initial_function->args[i] = evaluation_function;
    }
    if(!initial_function) {
        Functions::free_mem(initial_function);
        return false;
    }
    if(is_special_function(initial_function)) {
        return add_special_function(initial_function);
    }
    if(is_action(initial_function)) {
        return process_actions(initial_function);
    }
    if(search_inits(initial_function)) {
        Functions::free_mem(initial_function);
        return true;
    }
    bool final_response = false;
    vector<vector<string>* > appropiate_definition = search_params(initial_function, var_params);
    for(int i = 0; i < appropiate_definition.size(); i++) {
        int index = 0;
        if(!appropiate_definition[i]) {
            Functions::free_mem(initial_function);
            return false;
        }
        unordered_map<string, string> respa = create_map(*appropiate_definition[i], initial_function->args);
        string response = modifier(respa, initial_function->name, string_definitions[initial_function->name][i], i, var_params);
        final_response = get_responses(response, index);
        if(final_response) {
            Functions::free_mem(initial_function);
            return final_response;
        }
    }
    Functions::free_mem(initial_function);
    return final_response;
}

int get_string_response(const string &input, int &index) {
    int counter = 0;
    if(input[index] != '(')
        return -1;
    while(index < input.size()) {
        if(input[index] == '(')
            counter++;
        if(input[index] == ')')
            counter--;
        if(!counter)
            return index;
        if(counter < 0)
            return -1;
        index++;
    }
    return -1;
}

bool get_negation(const string &param, int &index) {
    bool response = false;
    while(index < param.size() && param[index] == '~') {
        index++;
        response = !response;
    }
    return response;
}

bool Functions::get_responses(const string &param, int &index) {
    bool negative_expression = get_negation(param, index);
    int c_index = index;
    bool first_param;
    if(param[index] == '(') {
        int relative_index = 0;
        get_string_response(param, index);
        if(c_index == -1) {
            error("Incorrect function syntax!");
        }
        first_param = (negative_expression ^ get_responses(param.substr(c_index + 1, index - c_index - 1), relative_index));
    }
    else {
        if(!is_function_recursion(param, index, 0)) {
            error("Incorrect function syntax! aa");
        }
        first_param = (negative_expression ^ evaluate(param.substr(c_index, index - c_index + 1)));
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
                first_param &= negative_expression ^ get_responses(param.substr(c_index + 1, index - c_index - 1), relative_index);
            }
            else {
                if(!is_function_recursion(param, index, 0)) {
                    error("Incorrect function syntax!");
                }
                first_param &= negative_expression ^ evaluate(param.substr(c_index, index - c_index + 1));
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
                first_param |= negative_expression ^ get_responses(param.substr(c_index + 1, index - c_index - 1), relative_index);
            }
            else {
                if(!is_function_recursion(param, index, 0)) {
                    error("Incorrect function syntax!");
                }
                first_param |= negative_expression ^ evaluate(param.substr(c_index, index - c_index + 1));
            }
        }
        index++;
    }
    return first_param;
}

bool Functions::search_inits(Functions *current_funct) {
    string fct_str = current_funct->to_string();
    if(inits.find(fct_str) != inits.end())
        return true;
    return false;
}

