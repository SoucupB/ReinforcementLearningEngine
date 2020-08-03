#include "runner.h"

void Runner::add_to_clasifier(vector<string> input) {
    for(int i = 0; i < input.size(); i++) {
        Functions fcnt = Functions::get_function(input[i]);
        if(permitted[fcnt.name])
            clasifiers[fcnt.name].push_back(Functions::get_function(fcnt.args[0]));
    }
}

void Runner::show_by(string input) {
    for(int i = 0; i < clasifiers[input].size(); i++) {
        clasifiers[input][i].show_message();
    }
}

void Runner::add_predefined_predicates() {
    for(int i = 0; i < permited_strings.size(); i++) {
        permitted[permited_strings[i]] = 1;
    }
}