#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <cstdint>
#include <Windows.h>
#include <tchar.h>
#include <vector>
#define DIV 1048576
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
void free_pointer(void **pointer);
void free_array(vector<vector<string>* > &input);
int min(int a, int b);
__int64 get_memory_of_process();
uint64_t timeSinceEpochMillisec();