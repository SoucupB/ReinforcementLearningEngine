#include <iostream>
#include "utils.h"
#include "function.h"
#include "runner.h"
#include <vector>
using namespace std;

int main() {
   // Functions *frt = Functions::get_function("legal(xplayer, mark(X,Y))");
    string query = "arb(a, c, f(5), b) :- reach(aabd, fds, d)";
    vector<string> input_file = {"init(cell(1, 1, b))",
                                 "init(cell(1, 2, b))",
                                 "init(cell(1, 3, o))",
                                 "init(cell(2, 1, b))",
                                 "init(cell(2, 2, o))",
                                 "init(cell(2, 3, b))",
                                 "init(cell(3, 1, o))",
                                 "init(cell(3, 2, b))",
                                 "init(cell(3, 3, b))",
                                 "row(Player, X) :- cell(X, 1, Player) & cell(X, 2, Player) & cell(X, 3, Player)",
                                 "column(Player, X) :- cell(1, X, Player) & cell(2, X, Player) & cell(3, X, Player)",
                                 "diagonalfirst(Player) :- cell(1, 1, Player) & cell(2, 2, Player) & cell(3, 3, Player)",
                                 "diagonalsecond(Player) :- cell(1, 3, Player) & cell(2, 2, Player) & cell(3, 1, Player)",
                                 "goal(xplayer, 50) :- row(x, 1) | row(x, 2) | row(x, 3) | column(x, 1) | column(x, 2) | column(x, 3) | diagonalfirst(x) | diagonalsecond(x)",
                                 "goal(oplayer, 50) :- row(o, 1) | row(o, 2) | row(o, 3) | column(o, 1) | column(o, 2) | column(o, 3) | diagonalfirst(o) | diagonalsecond(o)"
                                };
   // cout << Functions::is_function("arb(a, 4353, fgf(x, u, z), az(43, z(6, 7), 1), 88, b, f(x, v(43)), 43)") << "\n";
   // Functions *pnte = Functions::get_function("arb(aZZ, 4353, fgf(x, u, z, g(4, 5),5), az(43, z(6, 7), 1), 88, arb(a, c, f(3, ss(7, 6, nn), A), b), f(x, v(43)), 43)");
   // pnte->show_message();
   // cout << Functions::is_function("cell(X, 1, Player)");
  //  exit(0);
    for(int i = 0; i < input_file.size(); i++) {
        Functions::process_line(input_file[i]);
    }
   // cout << Functions::processor("init(xplayer, cell(1, 1, b))") << "\n";
  //  cout << Functions::processor("init(xplayer, line(x, 1))") << "\n";
   // cout << Functions::processor("init(xplayer, diagonal(x))") << "\n";
    cout << Functions::processor("init(xplayer, goal(oplayer, 50))") << "\n";
  //  cout << Functions::processor("init(xplayer, goal(oplayer, 50))") << "\n";
   // cout << Functions::processor("init(xplayer, goal(oplayer, 50))") << "\n";
    // for(int i = 0; i < 9; i++) {
    //     get_init(i)->show_message();
    // }
    exit(0);
}