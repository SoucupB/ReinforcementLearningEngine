#include "expressions.h"

string evaluate_expression(const string &input) {
    int evaluation = evaluate(input);
    if(evaluation == -(1<<30))
        return "";
    return to_string(evaluation);
}

bool is_expression(const string &input) {
    int index = 0;
    bool response = 0;
    while(index < input.size()) {
        remove_spaces(input, index);
        response = is_predicate_or_number(input, index);
        if(!response)
            return false;
        remove_spaces(input, index);
        if(index >= input.size() || (input[index] == '+' | input[index] == '-' | input[index] == '*' | input[index] == '/'))
            index++;
        else
            return false;
    }
    return true;
}

int get_number(const string &input, int &index) {
    int sum = 0;
    while(index < input.size() && input[index] >= '0' && input[index] <= '9') {
        sum = sum * 10;
        sum += input[index] - 48;
        index++;
    }
    return sum;
}

bool is_predicate_or_number(const string &input, int &index) {
    string final_answer = "";
    while(index < input.size() && (isdigit(input[index]) | isalpha(input[index]))) {
        final_answer += input[index];
        index++;
    }
    char* p;
    strtol(final_answer.c_str(), &p, 10);
    return (*p == 0) | (final_answer.size() && isupper(final_answer[0]));
}

int evaluate(const string &input) {
    int index = 0;
    int number = first_degree_operation(input, index);
    if(index < input.size()) {
        return -(1<<30);
    }
    return number;
}

int second_degree_operation(const string &input, int &index) {
    int element = get_number(input, index);
    while(index < input.size() && (input[index] == '*' || input[index] == '/')) {
        if(input[index] == '*') {
            index++;
            element *= get_number(input, index);
        }
        if(input[index] == '/') {
            index++;
            element /= get_number(input, index);
        }
    }
    return element;
}

int first_degree_operation(const string &input, int &index) {
    int element = second_degree_operation(input, index);
    while(index < input.size() && (input[index] == '+' || input[index] == '-')) {
        if(input[index] == '+') {
            index++;
            element += second_degree_operation(input, index);
        }
        if(input[index] == '-') {
            index++;
            element -= second_degree_operation(input, index);
        }
    }
    return element;
}