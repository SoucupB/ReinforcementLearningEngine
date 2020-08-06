#include "runner.h"

void Runner::add_to_clasifier(vector<string> input) {
    for(int i = 0; i < input.size(); i++) {
        process_line(input[i]);
    }
}

void Runner::process_line(string input) {
    int first = 0, last = Functions::get_function_index(input, first);
    if(last == -1) {
        error("Incorect definition at the current line!");
    }
    last++;
    Functions *predicate_function = Functions::get_function(input.substr(first, last));
    if(permitted[predicate_function->name] && predicate_function->name == "init") {
        Functions *argument_predicate = Functions::get_function(predicate_function->args[0]);
        clasifiers[predicate_function->name].push_back(argument_predicate);
    }
    if(permitted[predicate_function->name] && predicate_function->name == "legal") {
        Functions *argument_predicate = Functions::get_function(predicate_function->args[1]);
        clasifiers[predicate_function->name].push_back(argument_predicate);
        predicate_function->player_name = predicate_function->args[0];
        process_line_definition(input, argument_predicate, last);
    }
}

void Runner::process_line_definition(string input, Functions *&father, int index) {
    Functions::remove_spaces(input, index);
    if(index + 1 < input.size() && input[index] == ':' && input[index + 1] == '-') {
        index += 2;
    }
    else {
        error("Incorect definition at the current line!");
    }
    vector<Functions*> definitions;
    vector<int> signs;
    Functions::remove_spaces(input, index);
    while(true) {
        int c_index = index;
        index = Functions::get_function_index(input, index);
        if(index == -1)
            break;
        index++;
        Functions *predicate_function = Functions::get_function(input.substr(c_index, index - c_index));
        definitions.push_back(predicate_function);
        Functions::remove_spaces(input, index);
        if(index < input.size()) {
            if(input[index] == '&')
                signs.push_back(AND);
            if(input[index] == '|')
                signs.push_back(OR);
            index++;
        }
        Functions::remove_spaces(input, index);
    }
    father->childs.push_back(definitions);
    father->signs = signs;
}

void Runner::show_by(string input) {
    for(int i = 0; i < clasifiers[input].size(); i++) {
        clasifiers[input][i]->show_message();
        for(int j = 0; j < clasifiers[input][i]->childs.size(); j++)
        {
            cout << "Definition " << j << "\n";
            for(int k = 0; k < clasifiers[input][i]->childs[j].size(); k++) {
                clasifiers[input][i]->childs[j][k]->show_message();
            }
        }
    }
}

void Runner::add_predefined_predicates() {
    for(int i = 0; i < permited_strings.size(); i++) {
        permitted[permited_strings[i]] = 1;
    }
}