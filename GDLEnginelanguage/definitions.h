#pragma once
#include "function.h"
#include <vector>
using namespace std;
enum {
    AND,
    OR,
    NO
};
class Definition {
    public:
        vector<Functions*> childs;
        vector<int> signs;
};