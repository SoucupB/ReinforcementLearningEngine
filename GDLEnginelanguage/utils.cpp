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

__int64 get_memory_of_process() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx (&statex);
    return (statex.ullTotalVirtual - statex.ullAvailVirtual) / DIV;
}

void free_pointer(void **pointer) {
    if(*pointer) {
        free(*pointer);
        *pointer = NULL;
    }
}

void free_array(vector<vector<string>* > &input) {
    for(int i = 0; i < input.size(); i++) {
        delete input[i];
        input[i] = NULL;
    }
}

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
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