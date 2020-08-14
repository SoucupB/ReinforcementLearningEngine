#include "function.h"
unordered_map<string, int> inits;
vector<Functions*> predicate_definitions;
unordered_map<string, string> marker;
unordered_map<string, vector< vector<string> > > var_params;
unordered_map<string, vector<string> > string_definitions;
unordered_map<string, bool> does;

Functions *last_param = NULL;

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
    else this->function_type = PREDICATE;
}

Functions *Functions::get_function(string funct) {
    int index = 0;
    Functions *response = get_function_object(funct, index);
    if(index < (int)funct.size() - 1 && !funct.size()) {
        free(response);
        return NULL;
    }
    return response;
}

Functions *Functions::get_function_object(string input, int &index) {
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

string Functions::get_function_argument(string input, int &index) {
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

string Functions::get_simple_arguments(string input, int &index) {
    remove_spaces(input, index);
    string argument = "";
    int c_index = index;
    while(index < input.size() && (isdigit(input[index]) || isalpha(input[index]))) {
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

bool Functions::is_function(string input) {
    int index = 0;
    bool is_funct = is_function_recursion(input, index, 0);
    if(index < input.size() - 1)
        return false;
    return is_funct;
}

int Functions::argument_types_function(string argument) {
    if(is_integer(argument))
        return NUMBER;
    if(is_variable(argument))
        return VARIABLE;
    if(is_function_var(argument))
        return PREDICATE;
    return CONSTANT;
}

bool Functions::is_function_recursion(string input, int &index, int depth) {
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
    while(index < input.size() && input[index] != ')') {
        index++;
        bool is_arg = is_argument(input, index, depth + 1);
        if(!is_arg)
            return false;
        if(input[index] != ',' && input[index] != ')')
            return false;
    }
    if(index == input.size())
        return false;
    if(input[index] != ')') {
        return false;
    }
    return true;
}

bool Functions::is_argument(string input, int &index, int depth) {
    remove_spaces(input, index);
    string argument = "";
    int c_index = index;
    while(index < input.size() && (isdigit(input[index]) || isalpha(input[index]))) {
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

void Functions::remove_spaces(string element, int &index) {
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

void Functions::process_line(string input) {
    int index = 0, c_index = index;
    if(!is_function_recursion(input, index, 0)) {
        error("Syntax error");
    }
    Functions *first_operator = get_function(input.substr(c_index, index - c_index + 1));
    index++;
    if(first_operator->name == "init")
    {
        inits[get_function(first_operator->args[0])->to_string()] = 1;
        first_operator->function_type = INIT;
        return ;
    }
    remove_spaces(input, index);
    if(index + 1 < input.size() && input[index] == ':' && input[index + 1] == '-') {
        index += 2;
    }
    else
        return ;
    var_params[first_operator->name].push_back(first_operator->args);
    remove_spaces(input, index);
    int cutted_index = input.size() - index;
    string definition_string = input.substr(index, cutted_index);
    first_operator->function_type = PREDICATE;
    definition_string.erase(remove(definition_string.begin(), definition_string.end(), ' '), definition_string.end());
    string_definitions[first_operator->name].push_back(definition_string);
    return ;
}

string modifier(unordered_map<string, string> elems, string function_name, string to_modify, int index) {
    char final_str[512] = {0};
    if(!elems.size())
        return to_modify;
    int char_index = 0, indexes[512], strings[512], str_indexes = 0;
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
        if(i != indexes[k]) {
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

vector<string> *search_params(Functions *fct) {
    bool traversed = false;
    for(int i = 0; i < var_params[fct->name].size(); i++) {
        bool checker = true;
        traversed = true;
        for(int j = 0; j < fct->args.size(); j++)
        {
            if(fct->args.size() != var_params[fct->name][i].size()) {
                checker = false;
                break;
            }
            else {
                if(Functions::argument_types_function(var_params[fct->name][i][j]) != VARIABLE && fct->args[j] != var_params[fct->name][i][j])
                {
                    checker = false;
                    break;
                }
            }
        }
        if(checker) {
            return &var_params[fct->name][i];
        }
    }
    return NULL;
}

unordered_map<string, string> create_map(vector<string> *alpha, vector<string> *beta) {
    unordered_map<string, string> response;
    if(alpha->size() != beta->size())
        return response;
    for(int i = 0; i < alpha->size(); i++) {
        response[(*alpha)[i]] = (*beta)[i];
    }
    return response;
}
bool Functions::func_eval(string element) {
    return evaluate(element);
}

bool Functions::evaluate(string param) {
    int index = 0;
    Functions *initial_function = Functions::get_function(param);
    if(!initial_function)
        return false;
    if(search_inits(initial_function))
        return true;
    vector<string> *appropiate_definition = search_params(initial_function);
    if(!appropiate_definition)
        return false;
    unordered_map<string, string> respa = create_map(appropiate_definition, &initial_function->args);
    string response = modifier(respa, initial_function->name, string_definitions[initial_function->name][0], 0);
    return get_responses(response, index);
}

int get_string_response(string input, int &index) {
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

bool get_negation(string param, int &index) {
    bool response = false;
    while(index < param.size() && param[index] == '~') {
        index++;
        response = !response;
    }
    return response;
}

bool Functions::get_responses(string param, int &index) {
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
            error("Incorrect function syntax!");
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
    if(inits[current_funct->to_string()])
        return true;
    return false;
}

