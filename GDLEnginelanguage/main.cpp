#include <iostream>
#include "utils.h"
#include "function.h"
#include <vector>
#include "expressions.h"
using namespace std;

int main() {
   // Functions *frt = Functions::get_function("legal(xplayer, mark(X,Y))");
    string query = "arb(a, c, f(5), b) :- reach(aabd, fds, d)";
    vector<string> input_file = {"role(xplayer)",
                                 "role(oplayer)",
                                 "init(cell(1, 1, x))",
                                 "init(cell(1, 2, o))",
                                 "init(cell(1, 3, x))",
                                 "init(cell(2, 1, o))",
                                 "init(cell(2, 2, o))",
                                 "init(cell(2, 3, x))",
                                 "init(cell(3, 1, x))",
                                 "init(cell(3, 2, x))",
                                 "init(cell(3, 3, o))",
                                 "row(Player, X) :- cell(X, 1, Player) & cell(X, 2, Player) & cell(X, 3, Player)",
                                 "row(Player, X) :- cell(1, X, Player) & cell(2, X, Player) & cell(3, X, Player)",
                                 "rowscols(Player) :- row(Player, 1) | row(Player, 2) | row(Player, 3)",
                                 "diagonal(Player) :- cell(1, 1, Player) & cell(2, 2, Player) & cell(3, 3, Player)",
                                 "diagonal(Player) :- cell(1, 3, Player) & cell(2, 2, Player) & cell(3, 1, Player)",
                                 "is_full(b) :- ~cell(1, 1, b) & ~cell(1, 2, b) & ~cell(1, 3, b) & ~cell(2, 1, b) & ~cell(2, 2, b) & ~cell(2, 3, b)& ~cell(3, 1, b) & ~cell(3, 2, b) & ~cell(3, 3, b)",
                                 "draw(p) :- ~diagonal(x) & ~diagonal(o) & ~rowscols(x) & ~rowscols(o) & is_full(b)",
                                 "goal_first(xplayer) :- rowscols(x) | diagonal(x)",
                                 "goal_last(oplayer) :- rowscols(o) | diagonal(o)",
                                 "terminal(f) :- goal(x) | goal(o)",
                                 "something(X) :- cell(X, 1, x) & init(cell(4, 4, a))",
                                 "legal(xplayer, markx(A, B)) :- cell(A, B, b)",
                                 "does(xplayer, markx(A, B)) :- next(cell(A, B, b), cell(A, B, x))",
                                 "legal(oplayer, marko(A, B)) :- cell(A, B, b)",
                                 "does(oplayer, marko(A, B)) :- next(cell(A, B, b), cell(A, B, o))"
                                };

    for(int i = 0; i < input_file.size(); i++) {
        Functions::process_line(input_file[i]);
    }
  //  cout << Functions::evaluate("try(4, 4)") << "\n";
  //  cout << Functions::evaluate("something(1)") << "\n";
  //  cout << Functions::evaluate("try(4, 4)") << "\n";
   // cout << Functions::evaluate("goal(xplayer)") << "\n";
    // cout << Functions::evaluate("markx(1, 1)") << "\n";
    // cout << Functions::evaluate("goal(oplayer)") << "\n";
    // cout << Functions::evaluate("markx(1, 2)") << "\n";
    // cout << Functions::evaluate("goal(oplayer)") << "\n";
    // cout << Functions::evaluate("markx(1, 3)") << "\n";
    // cout << Functions::evaluate("goal_first(xplayer)") << "\n";
    cout << Functions::evaluate("draw(p)") << "\n";
}