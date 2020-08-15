#include "utils.h"

void error(string error) {
    cout << "Error: " + error;
    exit(0);
}

bool is_integer(string input) {
    char* p;
    strtol(input.c_str(), &p, 10);
    return *p == 0;
}

bool is_variable(string input) {
    if(input != "" && isupper(input[0]))
        return true;
    return false;
}

bool is_function_var(string input) {
    return input.find('(') != string::npos;
}

int min(int a, int b) {
    return a < b ? a : b;
}

bool is_arithmetic_sign(char element) {
    if(element == '+' || element == '-' || element == '*' || element == '/')
        return true;
    return false;
}