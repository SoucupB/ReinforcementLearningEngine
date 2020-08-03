#include <iostream>
#include "function.h"
#include "runner.h"
#include <vector>
using namespace std;

int main() {
    Functions frt = Functions::get_function("legal(xplayer, mark(X,Y))");
    vector<Functions> resolvers;
    string query = "arb(a, c, f(5), b) :- reach(aabd, fds, d)";
    vector<string> input_file = {"init(cell(1, 1, b))",
                                 "init(cell(1, 2, b))",
                                 "init(cell(1, 3, b))",
                                 "init(cell(2, 1, b))",
                                 "init(cell(2, 2, b))",
                                 "init(cell(2, 3, b))",
                                 "init(cell(3, 1, b))",
                                 "init(cell(3, 2, b))",
                                 "init(cell(3, 3, b))",
                                };
    Runner runer;
    runer.add_predefined_predicates();
    runer.add_to_clasifier(input_file);
    runer.show_by("init");
    frt.show_message();
    Functions frz = Functions::get_function(frt.args[1]);
    frz.show_message();
    cout << Functions::get_function_index(query, 0);
}