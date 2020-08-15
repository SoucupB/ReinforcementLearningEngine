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
    CONSTANT,
    INIT,
    PREDICATE,
    LEGAL,
    DOES,
    NEXT,
    ACTION_PREDICATE,
    TRUE,
    AND,
    OR,
    NO,
    WRONG
};


void error(string error);
bool is_integer(string input);
bool is_variable(string input);
bool is_function_var(string input);
bool is_arithmetic_sign(char element);
int min(int a, int b);