#include <iostream>
#include "utils.h"
#include "function.h"
#include <vector>
#include "expressions.h"
#include <ctime>
#include "tests.h"
using namespace std;

int main() {
    srand(time(NULL));
    //test_manager("tictac");
    //test_manager("function");
   // test_manager("hash");
   // test_manager("binary_function");
   // test_manager("test_special");
    test_manager("test_tic_tac");
  //  test_manager("test_speed_old");
   // test_manager("test_speed_new");
}