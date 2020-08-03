#pragma once
#include "function.h"
#include <vector>
#include <map>
using namespace std;
class Runner{
public:
    vector<string> permited_strings = {"init", "next", "does", "legal"};
    map<string, int> permitted;
    map<string, vector<Functions> > clasifiers;
    void add_to_clasifier(vector<string> input);
    void show_by(string input);
    void add_predefined_predicates();
};