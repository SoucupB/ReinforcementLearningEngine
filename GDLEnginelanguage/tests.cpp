#include "tests.h"

vector<string> input_file = {"role(xplayer)",
                                 "role(oplayer)",
                                 "init(cell(1, 1, b))",
                                 "init(cell(1, 2, b))",
                                 "init(cell(1, 3, b))",
                                 "init(cell(2, 1, b))",
                                 "init(cell(2, 2, b))",
                                 "init(cell(2, 3, b))",
                                 "init(cell(3, 1, b))",
                                 "init(cell(3, 2, b))",
                                 "init(cell(3, 3, b))",
                                 "row(Player, X) :- cell(X, 1, Player) & cell(X, 2, Player) & cell(X, 3, Player)",
                                 "row(Player, X) :- cell(1, X, Player) & cell(2, X, Player) & cell(3, X, Player)",
                                 "rowscols(Player) :- row(Player, 1) | row(Player, 2) | row(Player, 3)",
                                 "diagonal(Player) :- cell(1, 1, Player) & cell(2, 2, Player) & cell(3, 3, Player)",
                                 "diagonal(Player) :- cell(1, 3, Player) & cell(2, 2, Player) & cell(3, 1, Player)",
                                 "is_full() :- ~cell(1, 1, b) & ~cell(1, 2, b) & ~cell(1, 3, b) & ~cell(2, 1, b) & ~cell(2, 2, b) & ~cell(2, 3, b) & ~cell(3, 1, b) & ~cell(3, 2, b) & ~cell(3, 3, b)",
                                 "draw() :- ~diagonal(x) & ~diagonal(o) & ~rowscols(x) & ~rowscols(o) & is_full()",
                                 "goal_first() :- rowscols(x) | diagonal(x)",
                                 "goal_last() :- rowscols(o) | diagonal(o)",
                                 "terminal() :- goal_first() | goal_last() | draw()",
                                 "legal(xplayer, markx(A, B)) :- cell(A, B, b)",
                                 "does(xplayer, markx(A, B)) :- next(cell(A, B, b), cell(A, B, x))",
                                 "legal(oplayer, marko(A, B)) :- cell(A, B, b)",
                                 "does(oplayer, marko(A, B)) :- next(cell(A, B, b), cell(A, B, o))"
                                };

void test_manager(string test) {
    uint64_t start;
    uint64_t finish;
    cout << "Starting memory is: " << get_memory_of_process() << " megabytes!\n";
    if(test == "tictac") {
        reserve_arrays();
        for(int i = 0; i < input_file.size(); i++) {
            Functions::process_line(input_file[i]);
        }
        cout << "Compiled!\n";
        start = timeSinceEpochMillisec();
        test_random_tic_tac();
        finish = timeSinceEpochMillisec();
    }
    if(test == "function") {
        start = timeSinceEpochMillisec();
        test_function();
        finish = timeSinceEpochMillisec();
    }
    cout << "\nEnding memory is: " << get_memory_of_process() << " megabytes!\n";
    cout << "\nTotal time for " + test + " is: " << finish - start << " miliseconds!";
}

void test_function() {
    int max_test = 10000000;
    string arama = "a(1, 2, dafd, fds, bbgf, das, ewe, ds)";
    for(int i = 0; i < max_test; i++) {
        Functions *aram = Functions::get_function(arama);
        Functions::free_mem(aram);
    }
}

void test_random_tic_tac() {
    int index = 0, max_ind = 5000, f = 0, s = 0, d = 0;
    while(index < max_ind) {
        save_state();
        while(!Functions::evaluate("terminal()")) {
            Functions::evaluate(get_random_action_first_player());
            if(!Functions::evaluate("terminal()"))
                Functions::evaluate(get_random_action_second_player());
        }
        if(Functions::evaluate("goal_first()"))
            f++;
        if(Functions::evaluate("goal_last()"))
            s++;
        if(Functions::evaluate("draw()"))
            d++;
       // show_all_inits();
       // exit(0);
        load_state();
        index++;
    }
    cout << f << " " << s << " " << d;
}