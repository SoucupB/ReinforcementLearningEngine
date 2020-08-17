#include <iostream>
#include "utils.h"
#include "function.h"
#include <vector>
#include "expressions.h"
#include <ctime>
#include "tests.h"
using namespace std;

int main() {
   // Functions *frt = Functions::get_function("legal(xplayer, mark(X,Y))");
    srand(time(NULL));
    test_manager("tictac");
    //test_manager("function");
}