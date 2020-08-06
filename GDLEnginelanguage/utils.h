#pragma once
#include <iostream>
#include <string>
using namespace std;

enum {
    NUMBER,
    STRING,
    VARIABLE,
    PREDICATE_TYPE,
    FUNCTION,
    CONSTANT
};
enum {
    INIT,
    PREDICATE,
    LEGAL,
    DOES,
    NEXT,
    ACTION_PREDICATE,
    TRUE
};
enum {
    AND,
    OR,
    NO
};

void error(string error);
bool is_integer(string input);
bool is_variable(string input);
bool is_function(string input);
int min(int a, int b);