#include <iostream>
#include "utils.h"
#include "function.h"
#include "runner.h"
#include <vector>
using namespace std;

int main() {
   // Functions *frt = Functions::get_function("legal(xplayer, mark(X,Y))");
    string query = "arb(a, c, f(5), b) :- reach(aabd, fds, d)";
    vector<string> input_file = {"init(cell(1, 1, x))",
                                 "init(cell(1, 2, o))",
                                 "init(cell(1, 3, x))",
                                 "init(cell(2, 1, b))",
                                 "init(cell(2, 2, o))",
                                 "init(cell(2, 3, b))",
                                 "init(cell(3, 1, b))",
                                 "init(cell(3, 2, o))",
                                 "init(cell(3, 3, b))",
                                 "row(Player, X) :- (cell(X, 1, Player) & cell(X, 2, Player) & cell(X, 3, Player)) | (cell(1, X, Player) & cell(2, X, Player) & cell(3, X, Player))",
                                 "rowscols(Player) :- row(Player, 1) | row(Player, 2) | row(Player, 3)",
                                 "diagonal(Player) :- (cell(1, 1, Player) & cell(2, 2, Player) & cell(3, 3, Player)) | (cell(1, 3, Player) & cell(2, 2, Player) & cell(3, 1, Player))",
                                 "goal(X) :- rowscols(X) | diagonal(X)",
                                 "terminal(f) :- goal(x) | goal(o)"
                                };

    for(int i = 0; i < input_file.size(); i++) {
        Functions::process_line(input_file[i]);
    }
    //cout << Functions::evaluate("terminal()") << "\n";
    cout << Functions::evaluate("terminal(f)");
}