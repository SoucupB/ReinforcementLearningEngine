#include <iostream>
#include "function.h"
using namespace std;

int main() {
    Functions frt = Functions::get_function("legal(xplayer, mark(X,Y))");
    string query = "arb(b, inerr(5, 4, 2), 1) :- reach(aabd, fds, d)";
    frt.show_message();
    Functions frz = Functions::get_function(frt.args[1]);
    frz.show_message();
    cout << Functions::get_function_index(query, 0);
}