#include "function.h"
unordered_map<string, int> inits;
vector<Functions*> predicate_definitions;

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
    bool is_funct = is_function_recursion(input, index);
    if(index < input.size() - 1)
        return false;
    return is_funct;
}

int Functions::argument_types_function(string argument) {
    if(is_integer(argument))
        return NUMBER;
    if(is_variable(argument))
        return VARIABLE;
    if(is_function(argument))
        return PREDICATE;
    return CONSTANT;
}

bool Functions::is_function_recursion(string input, int &index) {
    string function_name = "";
    remove_spaces(input, index);
   // cout << input << "-\n";
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
        bool is_arg = is_argument(input, index);
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

bool Functions::is_argument(string input, int &index) {
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
        bool inner_function = is_function_recursion(input, index);
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

bool Functions::regex_function(string mess) {
    int index = 0;
    while(index < mess.size()) {
        bool checker = false;
        remove_spaces(mess, index);
        while(isalpha(mess[index]) || isdigit(mess[index])) {
            index++;
            checker = true;
        }
        if(!checker)
            return false;
        remove_spaces(mess, index);
        if(index >= mess.size())
            return true;
        if(mess[index] != ',' && mess[index] != '(') {
            return false;
        }
        if(mess[index] == '(') {
            index++;
            string next_arg = get_arguments(mess, index);
            bool inner_function = regex_function(next_arg);
            if(!inner_function || next_arg == "-1")
                return false;
        }
        index++;
    }
    return true;
}

Functions *Functions::get_function_at_index(string input, int &index) {
    int c_index = index;
    string current_str = input.substr(c_index, index);
    while(index < input.size() && !is_function(current_str) || index == c_index) {
       // cout << input.substr(c_index, index - c_index) << " " << c_index << " " << index << " " << is_function(current_str) << "\n";
        index++;
        current_str = input.substr(c_index, index - c_index);
    }
   // cout << input.substr(c_index, index - c_index) << " " << c_index << " " << index << " " << is_function(current_str) << "\n";
    return get_function(current_str);
}

string Functions::to_string() {
    string full_argument = "";
    for(int i = 0; i < this->args.size() - 1; i++)
        full_argument += this->args[i] + ",";
    full_argument += this->args[this->args.size() - 1];
    return this->name + "(" + full_argument + ")";
}

void Functions::process_line(string input) {
    int index = 0;
    Functions *first_operator = get_function_at_index(input, index);
    if(first_operator->name == "init")
    {
        inits[get_function(first_operator->args[0])->to_string()] = 1;
        return ;
    }
    remove_spaces(input, index);
    if(index + 1 < input.size() && input[index] == ':' && input[index + 1] == '-') {
        index += 2;
    }
    else
        return ;
    remove_spaces(input, index);
    predicate_definitions.push_back(first_operator);
    Definitions *def = new Definitions;
    Functions *second_operator = get_function_at_index(input, index);
    remove_spaces(input, index);
    def->definition_vector.push_back(second_operator);
    while(index < input.size() && (input[index] == '|' || input[index] == '&')) {
        remove_spaces(input, index);
        if(input[index] == '|') {
            index++;
            Functions *third_operator = get_function_at_index(input, index);
            def->definition_vector_signs.push_back('|');
            def->definition_vector.push_back(third_operator);
        }
        if(input[index] == '&') {
            index++;
            Functions *third_operator = get_function_at_index(input, index);
            def->definition_vector_signs.push_back('&');
            def->definition_vector.push_back(third_operator);
        }
        remove_spaces(input, index);
    }
  //  cout << def->definition_vector.size() << "\n";
    first_operator->def = def;
}

bool Functions::processor(string init) {
    Functions *current_funct = get_function(init);
    if(current_funct->name == "init") {
        Functions *query_function = get_function(current_funct->args[1]);
        return get_responses(query_function);
    }
    return true;
}

bool Functions::get_responses(Functions *funct) {
  //  funct->show_message();
    Functions *recharger = find_equalizer(funct);
    // recharger->show_message();
    // exit(0);
    if(recharger) {
        funct = recharger;
    }
    if(!funct->def)
        return search_inits(funct);
    Definitions *current_def = funct->def;
   // for(int i = 0; i < current_def->definition_vector.size(); i++)
  //      current_def->definition_vector[i]->show_message();
  //  exit(0);

    bool result = get_responses(current_def->definition_vector[0]);
    //current_def->definition_vector[0]->show_message();
    for(int i = 0; i < current_def->definition_vector_signs.size(); i++) {
        char sign = current_def->definition_vector_signs[i];
        if(sign == '|') {
            bool response = get_responses(current_def->definition_vector[i + 1]);
            result |= response;
        }
        if(sign == '&') {
            bool response = get_responses(current_def->definition_vector[i + 1]);
            result &= response;
        }
    }
    return result;
}

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
  //  cout << funct->to_string() << "\n";
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

int Functions::get_function_index(string mess, int index) {
    while(index < mess.size() && mess[index] != '(') {
        if(!isalpha(mess[index]))
            return -1;
        index++;
    }
    if(index == mess.size())
        return -1;
    index++;
    int c_index = index;
    string response = get_arguments(mess, index);
    if(response == "-1")
        return -1;
    if(!regex_function(response))
        return -1;
    return c_index + response.size();
}