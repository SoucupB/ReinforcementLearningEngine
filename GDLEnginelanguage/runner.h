#pragma once
#include "function.h"
#include <vector>
#include <map>
#include <valarray>
#include "utils.h"
using namespace std;
class Runner{
public:
    vector<string> permited_strings = {"init", "next", "does", "legal", "true"};
    map<string, int> permitted;
    map<string, vector<Functions*> > clasifiers;
    void add_to_clasifier(vector<string> input);
    void show_by(string input);
    void add_predefined_predicates();
    void process_line(string input);
    void process_line_definition(string input, Functions *&father, int index);
};