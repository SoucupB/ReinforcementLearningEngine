#pragma once
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include "utils.h"
using namespace std;
string evaluate_expression(const string &);
int first_degree_operation(const string &, int&);
int second_degree_operation(const string &, int&);
int get_number(const string &, int , int);
int evaluate(const string &);
void int_to_char(const string &, int , int );
bool is_predicate_or_number(const string &input, int &index);
bool is_expression(const string &input);



