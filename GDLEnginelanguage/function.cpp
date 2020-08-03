#include "function.h"
Functions Functions::get_function(string funct) {
    Functions response;
    string function_name = "";
    int index = 0;
    while(index < funct.size() && funct[index] != '(') {
        function_name += funct[index];
        index++;
    }
    response.name = function_name;
    index++;
    vector<string> arg_response;
    while(index < funct.size() && funct[index] != ')') {
        string argv = Functions::get_argument(funct, index);
        arg_response.push_back(argv);
        if(funct[index] != ',' && funct[index] != ')')
        {
            cout << "Error: Function format is invalid!";
            exit(0);
        }
        index++;
    }
    response.args = arg_response;
    if(funct[index - 1] != ')') {
        cout << "Error: Function format is invalid!";
        exit(0);
    }
    return response;
}

string Functions::get_argument(string arge, int &index) {
    string argument = "";
    while(index < arge.size() && arge[index] != ',' && arge[index] != ')' && arge[index] != '(')
    {
        if(arge[index] != ' ')
            argument += arge[index];
        index++;
    }
    if(arge[index] == ',' || arge[index] == ')')
        return argument;
    if(arge[index] == '(') {
        index++;
        return argument + "(" + get_arguments(arge, index) + ")";
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

void Functions::show_message() {
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