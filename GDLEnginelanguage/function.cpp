#include "function.h"
unordered_map<string, int> inits;
vector<Functions*> predicate_definitions;
unordered_map<string, string> marker;
unordered_map<string, vector< vector<string> > > var_params;
unordered_map<string, vector<string> > string_definitions;

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

string Functions::get_argument(string arge, int &index) {
    string argument = "";
    while(index < arge.size() && arge[index] != ',' && arge[index] != ')' && arge[index] != '(')
    {
        if(arge[index] != ' ')
            argument += arge[index];
        index++;
    }
    if(index < arge.size() && (arge[index] == ',' || arge[index] == ')'))
        return argument;
    if(index < arge.size() && arge[index] == '(') {
        index++;
        string args = get_arguments(arge, index);
        return argument + "(" + args + ")";
    }
    return argument;
}

string Functions::get_arguments(string arge, int &index) {
    string argument = "";
    while(index < arge.size() && arge[index] != ')')
    {
        if(arge[index] == '(')
        {
            index++;
            argument += "(" + get_arguments(arge, index) + ")";
        }
        else {
            argument += arge[index];
            index++;
        }
    }
    if(index < arge.size() && arge[index] != ')') {
        return "-1";
    }
    index++;
    return argument;
}

Functions *Functions::get_function(string funct) {
    int index = 0;
    Functions *response = get_function_object(funct, index);
    if(index < funct.size() - 1) {
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
            return NULL;
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

Functions *Functions::get_function_at_index(string input, int &index) {
    int c_index = index;
    string current_str = input.substr(c_index, index);
    while(index < input.size() && !is_function(current_str) || index == c_index) {
        index++;
        current_str = input.substr(c_index, index - c_index);
    }
    return get_function(current_str);
}

string Functions::to_string() {
    string full_argument = "";
    for(int i = 0; i < this->args.size() - 1; i++)
        full_argument += this->args[i] + ",";
    full_argument += this->args[this->args.size() - 1];
    return this->name + "(" + full_argument + ")";
}

// void Functions::process_line(string input) {
//     int index = 0;
//     Functions *first_operator = get_function_at_index(input, index);
//     if(first_operator->name == "init")
//     {
//         inits[get_function(first_operator->args[0])->to_string()] = 1;
//         return ;
//     }
//     remove_spaces(input, index);
//     if(index + 1 < input.size() && input[index] == ':' && input[index + 1] == '-') {
//         index += 2;
//     }
//     else
//         return ;
//     remove_spaces(input, index);
//     predicate_definitions.push_back(first_operator);
//     Definitions *def = new Definitions;
//     Functions *second_operator = get_function_at_index(input, index);
//     remove_spaces(input, index);
//     def->definition_vector.push_back(second_operator);
//     while(index < input.size() && (input[index] == '|' || input[index] == '&')) {
//         remove_spaces(input, index);
//         if(input[index] == '|') {
//             index++;
//             Functions *third_operator = get_function_at_index(input, index);
//             def->definition_vector_signs.push_back('|');
//             def->definition_vector.push_back(third_operator);
//         }
//         if(input[index] == '&') {
//             index++;
//             Functions *third_operator = get_function_at_index(input, index);
//             def->definition_vector_signs.push_back('&');
//             def->definition_vector.push_back(third_operator);
//         }
//         remove_spaces(input, index);
//     }
//     first_operator->def = def;
// }

void Functions::process_function(Functions *fct) {
   // var_params
}

void Functions::process_line(string input) {
    int index = 0;
    Functions *first_operator = get_function_at_index(input, index);
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
            i += var_params[function_name][index][strings[k]].size() - 1;
            k++;
        }
    }
    return string(final_str);
}

vector<string> *search_params(Functions *fct) {
    for(int i = 0; i < var_params[fct->name].size(); i++) {
        bool checker = true;
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

bool Functions::get_responses(string param, int &index) {
    int c_index = index;
    bool first_param;
    if(param[index] == '(') {
        int relative_index = 0;
        get_string_response(param, index);
        if(c_index == -1) {
            error("Incorrect function syntax!");
        }
        first_param = get_responses(param.substr(c_index + 1, index - c_index - 1), relative_index);
    }
    else {
        if(!is_function_recursion(param, index, 0)) {
            error("Incorrect function syntax!");
        }
        first_param = evaluate(param.substr(c_index, index - c_index + 1));
    }
    index++;
    while(index < param.size() && (param[index] == '|' || param[index] == '&')) {
        if(param[index] == '&') {
            index++;
            c_index = index;
            if(param[index] == '(') {
                int relative_index = 0;
                get_string_response(param, index);
                if(c_index == -1) {
                    error("Incorrect function syntax!");
                }
                first_param &= get_responses(param.substr(c_index + 1, index - c_index - 1), relative_index);
            }
            else {
                if(!is_function_recursion(param, index, 0)) {
                    error("Incorrect function syntax!");
                }
                first_param &= evaluate(param.substr(c_index, index - c_index + 1));
            }
        }
        else if(param[index] == '|') {
            index++;
            c_index = index;
            if(param[index] == '(') {
                int relative_index = 0;
                get_string_response(param, index);
                if(c_index == -1) {
                    error("Incorrect function syntax!");
                }
                first_param |= get_responses(param.substr(c_index + 1, index - c_index - 1), relative_index);
            }
            else {
                if(!is_function_recursion(param, index, 0)) {
                    error("Incorrect function syntax!");
                }
                first_param |= evaluate(param.substr(c_index, index - c_index + 1));
            }
        }
        index++;
    }
    return first_param;
}

// bool Functions::processor(string init) {
//     Functions *current_funct = get_function(init);
//     if(current_funct->name == "init") {
//         Functions *query_function = get_function(current_funct->args[1]);
//         return get_responses(query_function);
//     }
//     return true;
// }

// bool Functions::get_responses(Functions *funct) {
//     Functions *recharger = find_equalizer(funct);
//     if(recharger) {
//         funct = recharger;
//     }
//     if(!funct->def)
//         return search_inits(funct);
//     Definitions *current_def = funct->def;
//     bool result = get_responses(current_def->definition_vector[0]);
//     for(int i = 0; i < current_def->definition_vector_signs.size(); i++) {
//         char sign = current_def->definition_vector_signs[i];
//         if(sign == '|') {
//             bool response = get_responses(current_def->definition_vector[i + 1]);
//             result |= response;
//         }
//         if(sign == '&') {
//             bool response = get_responses(current_def->definition_vector[i + 1]);
//             result &= response;
//         }
//     }
//     return result;
// }

void Functions::deep_copy(Functions *fct) {
    this->name = fct->name;
    this->args = fct->args;
    this->argument_types = fct->argument_types;
    Definitions *new_definition = new Definitions;
    if(fct && fct->def) {
        for(int i = 0; i < fct->def->definition_vector.size(); i++)
        {
            Functions *new_function = new Functions;
            new_function->name = fct->def->definition_vector[i]->name;
            new_function->args = fct->def->definition_vector[i]->args;
            new_function->argument_types = fct->def->definition_vector[i]->argument_types;
            new_function->def = fct->def->definition_vector[i]->def;
            new_definition->definition_vector.push_back(new_function);
        }
    }
    new_definition->definition_vector_signs = fct->def->definition_vector_signs;
    free(this->def);
    this->def = new_definition;
}

Functions *Functions::find_equalizer(Functions *funct) {
    Functions *appropiat_function = new Functions;
    bool function_checker = 0;
    for(int i = 0; i < predicate_definitions.size(); i++) {
        if(predicate_definitions[i]->name == funct->name) {
            bool checker = 1;
            for(int j = 0; checker && j < predicate_definitions[i]->args.size(); j++) {
                if(predicate_definitions[i]->args[j] != funct->args[j] && predicate_definitions[i]->argument_types[j] != VARIABLE) {
                    checker = 0;
                }
            }
            if(checker) {
                appropiat_function->deep_copy(predicate_definitions[i]);
                function_checker = 1;
                break;
            }
        }
    }
    if(!function_checker) {
        free(appropiat_function);
        return NULL;
    }
    unordered_map<string, string> parameters;
    unordered_map<string, bool> used;
    for(int i = 0; i < appropiat_function->args.size(); i++) {
        if(appropiat_function->argument_types[i] == VARIABLE) {
            parameters[appropiat_function->args[i]] = funct->args[i];
            used[appropiat_function->args[i]] = 1;
            appropiat_function->args[i] = funct->args[i];
        }
    }
    Definitions *def = appropiat_function->def;
    for(int i = 0; i < def->definition_vector.size(); i++) {
        for(int j = 0; j < def->definition_vector[i]->args.size(); j++) {
            if(used[def->definition_vector[i]->args[j]])
                def->definition_vector[i]->args[j] = parameters[def->definition_vector[i]->args[j]];
        }
    }
    appropiat_function->def = def;
    return appropiat_function;
}

bool Functions::search_inits(Functions *current_funct) {
    if(inits[current_funct->to_string()])
        return true;
    return false;
}

