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
                                 "init(cell(1, 2, x))",
                                 "init(cell(1, 3, b))",
                                 "init(cell(2, 1, b))",
                                 "init(cell(2, 2, b))",
                                 "init(cell(2, 3, b))",
                                 "init(cell(3, 1, b))",
                                 "init(cell(3, 2, b))",
                                 "init(cell(3, 3, b))",
                                // "legal(xplayer, mark(X, Y)) :- cell(X, Y, b) & cell(Y, X, x)",
                                 "line(Player, X) :- cell(X, 1, Player) & cell(X, 2, Player) & cell(X, 3, Player)",
                                 "diagonal(Player) :- cell(1, 1, Player) & cell(2, 2, Player) & cell(3, 3, Player)"
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
    cout << Functions::processor("init(xplayer, line(x, 1))") << "\n";
    cout << Functions::processor("init(xplayer, diagonal(x))") << "\n";
    // for(int i = 0; i < 9; i++) {
    //     get_init(i)->show_message();
    // }
    exit(0);
    // string vas = "arb(a, c, f(3, ss(7, 6, nn), A), b)";
    // Functions::get_function(vas)->show_message();
    // exit(0);
    // Runner runer;
    // runer.add_predefined_predicates();
    // runer.add_to_clasifier(input_file);
    // runer.show_by("legal");
   // frt->show_message();
   // Functions *frz = Functions::get_function(frt->args[1]);
   // frz->show_message();
    //cout << Functions::get_function_index(query, 0);
}