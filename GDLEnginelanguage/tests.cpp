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
    cout << "\nStarting memory is: " << get_memory_of_process() << " megabytes!\n";
    if(test == "tictac") {
        start = timeSinceEpochMillisec();
        test_random_tic_tac();
        finish = timeSinceEpochMillisec();
    }
    if(test == "function") {
        start = timeSinceEpochMillisec();
        test_function();
        finish = timeSinceEpochMillisec();
    }
    if(test == "hash") {
        start = timeSinceEpochMillisec();
        test_function_transformation();
        finish = timeSinceEpochMillisec();
    }
    if(test == "binary_function") {
        start = timeSinceEpochMillisec();
        test_binary_function();
        finish = timeSinceEpochMillisec();
    }
    if(test == "test_speed_old") {
        start = timeSinceEpochMillisec();
        test_speed_comparison(1);
        finish = timeSinceEpochMillisec();
    }
    if(test == "test_speed_new") {
        start = timeSinceEpochMillisec();
        test_speed_comparison(0);
        finish = timeSinceEpochMillisec();
    }
    if(test == "test_special") {
        start = timeSinceEpochMillisec();
        test_special_functions_function();
        finish = timeSinceEpochMillisec();
    }
    if(test == "test_tic_tac") {
        start = timeSinceEpochMillisec();
        test_tic_tac_toe();
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
    reserve_arrays();
    for(int i = 0; i < input_file.size(); i++) {
        Functions::process_line(input_file[i]);
    }
    cout << "Compiled!\n";
    int index = 0, max_ind = 10000, f = 0, s = 0, d = 0;
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
        load_state();
        index++;
    }
    cout << f << " " << s << " " << d;
}

void test_function_transformation() {
    Functions *tester = Functions::get_function("vasila(bads(4, 3, bads(4, 3, bads(4, bads(4, 3, a, b), a, b), b), b), AnaMere, 43, bads(4, 3, a, b), 5, adfds)");
    string arb = "bads(4, 3, bads(4, bads(4, 3, a, b), a, b), b)";
    vector<unsigned short> maxer;
    maxer.reserve(256);
    Functions::transform_into_hash(tester, maxer, 0);
    for(int i = 0; i < maxer.size(); i++) {
        cout << get_hash(maxer[i]) << " ";
    }
    cout << "\n";
    function_describer(maxer, 0, 0);
}

void test_process_function() {
    Functions *tester = Functions::get_function("vasila(bads(4, 3, bads(4, 3, bads(4, bads(4, 3, a, b), a, b), b), b), AnaMere, 43, bads(4, 3, a, b), 5, adfds)");
    string arb = "bads(4, 3, bads(4, bads(4, 3, a, b), a, b), b)";
    vector<unsigned short> maxer;
    maxer.reserve(256);
    Functions::transform_into_hash(tester, maxer, 0);
    for(int i = 0; i < maxer.size(); i++) {
        cout << maxer[i] << " ";
    }
    cout << "\n";
    function_describer(maxer, 0, 0);
}

void test_speed_comparison(int index) {
    vector<string> current_file = { "init(cell(1, 1, b))",
                                    "init(cell(1, 2, b))",
                                    "init(cell(1, 3, b))",
                                    "init(cell(2, 1, b))",
                                    "init(cell(2, 2, b))",
                                    "init(cell(2, 3, b))",
                                    "init(cell(3, 1, b))",
                                    "init(cell(3, 2, b))",
                                    "init(cell(3, 3, b))",
                                    "row(Player, X) :- cell(X, 1, Player) & cell(X, 2, Player) & cell(X, 3, Player)",
                                    "row(Player, A) :- cell(1, A, Player) & cell(2, A, Player) & cell(3, A, Player)",
                                    "is_full() :- ~cell(1, 1, b) & ~cell(1, 2, b) & ~cell(1, 3, b) & ~cell(2, 1, b) & ~cell(2, 2, b) & ~cell(2, 3, b) & ~cell(3, 1, b) & ~cell(3, 2, b) & ~cell(3, 3, b)",
                                    "diagonal(Player) :- cell(1, 1, Player) & cell(2, 2, Player) & cell(3, 3, Player)",
                                    "diagonal(Player) :- cell(1, 3, Player) & cell(2, 2, Player) & cell(3, 1, Player)",
                                    "draw() :- ~goal(o) & ~goal(x) & is_full()",
                                    "goal(Player) :- row(Player, 1) | row(Player, 2) | row(Player, 3) | diagonal(Player)",
                                    "terminal() :- goal(x) | goal(o) | is_full()"
                                    };
    int max_tests_size = 100000;
    if(index == 0) {
        predefined_hashed();
        generate_zovrist();
        for(int i = 0; i < current_file.size(); i++) {
            Functions::process_line_binary(current_file[i]);
        }
        int max_tests = max_tests_size;
        for(int i = 0; i < max_tests; i++)
            Functions::evaluate_binary("terminal()");
    }
    else {
        reserve_arrays();
        for(int i = 0; i < current_file.size(); i++) {
            Functions::process_line(current_file[i]);
        }
        int max_tests = max_tests_size;
        for(int i = 0; i < max_tests; i++)
            Functions::evaluate("terminal()");
    }
}

void test_binary_function() {
    vector<string> current_file = { "role(xplayer)",
                                    "role(oplayer)",
                                    "init(cell(1, 1, x))",
                                    "init(cell(1, 2, b))",
                                    "init(cell(1, 3, x))",
                                    "init(cell(2, 1, b))",
                                    "init(cell(2, 2, b))",
                                    "init(cell(2, 3, b))",
                                    "init(cell(3, 1, b))",
                                    "init(cell(3, 2, b))",
                                    "init(cell(3, 3, b))",
                                    "row(Player, X) :- cell(X, 1, Player) & cell(X, 2, Player) & cell(X, 3, Player)",
                                    "row(Player, A) :- cell(1, A, Player) & cell(2, A, Player) & cell(3, A, Player)",
                                    "is_full() :- ~cell(1, 1, b) & ~cell(1, 2, b) & ~cell(1, 3, b) & ~cell(2, 1, b) & ~cell(2, 2, b) & ~cell(2, 3, b) & ~cell(3, 1, b) & ~cell(3, 2, b) & ~cell(3, 3, b)",
                                    "diagonal(Player) :- cell(1, 1, Player) & cell(2, 2, Player) & cell(3, 3, Player)",
                                    "diagonal(Player) :- cell(1, 3, Player) & cell(2, 2, Player) & cell(3, 1, Player)",
                                    "draw() :- ~goal(o) & ~goal(x) & is_full()",
                                    "goal(Player) :- row(Player, 1) | row(Player, 2) | row(Player, 3) | diagonal(Player)",
                                    "terminal() :- goal(x) | goal(o) | draw()"
                                    };
    generate_zovrist();
    predefined_hashed();
    for(int i = 0; i < current_file.size(); i++) {
        Functions::process_line_binary(current_file[i]);
    }
    cout << Functions::evaluate_binary("terminal()") << " ";
}

void test_special_functions_function() {
    vector<string> current_file = { "role(xplayer)",
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
                                    "row(Player, A) :- cell(1, A, Player) & cell(2, A, Player) & cell(3, A, Player)",
                                    "is_full() :- ~cell(1, 1, b) & ~cell(1, 2, b) & ~cell(1, 3, b) & ~cell(2, 1, b) & ~cell(2, 2, b) & ~cell(2, 3, b) & ~cell(3, 1, b) & ~cell(3, 2, b) & ~cell(3, 3, b)",
                                    "diagonal(Player) :- cell(1, 1, Player) & cell(2, 2, Player) & cell(3, 3, Player)",
                                    "diagonal(Player) :- cell(1, 3, Player) & cell(2, 2, Player) & cell(3, 1, Player)",
                                    "draw() :- ~goal(o) & ~goal(x) & is_full()",
                                    "goal(Player) :- row(Player, 1) | row(Player, 2) | row(Player, 3) | diagonal(Player)",
                                    "terminal() :- goal(x) | goal(o) | draw()",
                                    "bark(A, B) :- next(cell(A, B, b), cell(A, B, x))",
                                    "does(xplayer, marker(A, B)) :- next(cell(A, B, b), cell(A, B, x))",
                                    "legal(xplayer, marker(A, B)) :- cell(A, B, b)"
                                    };
    generate_zovrist();
    predefined_hashed();
    for(int i = 0; i < current_file.size(); i++) {
        Functions::process_line_binary(current_file[i]);
    }
    cout << Functions::evaluate_binary("cell(3, 6, x)") << "\n";
    cout << Functions::evaluate_binary("mark(3, 6)") << "\n";
    cout << Functions::evaluate_binary("cell(3, 6, x)") << "\n";
    cout << Functions::evaluate_binary("mark(3, 6)") << "\n";
    cout << Functions::evaluate_binary("cell(1, 1, b)") << "\n";
    cout << Functions::evaluate_binary("cell(1, 1, x)") << "\n";
    cout << Functions::evaluate_binary("mark_two(5, 6)") << "\n";
    cout << Functions::evaluate_binary("cell(1, 1, b)") << "\n";
    cout << Functions::evaluate_binary("cell(1, 1, x)") << "\n";
    cout << Functions::evaluate_binary("cell(1, 1, x)") << "\n";
    cout << Functions::evaluate_legality("marker(1, 1)") << "\n";
    cout << Functions::evaluate_binary("cell(1, 1, x)") << "\n";
}

void show_games() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            int x = Functions::evaluate_binary("mark(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            int o = Functions::evaluate_binary("circle(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            int b = Functions::evaluate_binary("free(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            int z = Functions::evaluate_binary("wall(" + to_string(i + 1) + ", " + to_string(j + 1) + ")");
            if(x)
                cout << 1 << " ";
            if(o)
                cout << 2 << " ";
            if(b)
                cout << 0 << " ";
            if(z)
                cout << 3 << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void test_tic_tac_toe() {
    vector<string> current_file = { "role(xplayer)",
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
                                    "row(Player, A) :- cell(1, A, Player) & cell(2, A, Player) & cell(3, A, Player)",
                                    "is_full() :- ~cell(1, 1, b) & ~cell(1, 2, b) & ~cell(1, 3, b) & ~cell(2, 1, b) & ~cell(2, 2, b) & ~cell(2, 3, b) & ~cell(3, 1, b) & ~cell(3, 2, b) & ~cell(3, 3, b)",
                                    "diagonal(Player) :- cell(1, 1, Player) & cell(2, 2, Player) & cell(3, 3, Player)",
                                    "diagonal(Player) :- cell(1, 3, Player) & cell(2, 2, Player) & cell(3, 1, Player)",
                                    "draw() :- ~goal(o) & ~goal(x) & is_full()",
                                    "goal(Player) :- row(Player, 1) | row(Player, 2) | row(Player, 3) | diagonal(Player)",
                                    "terminal() :- goal(x) | goal(o) | is_full()",
                                    "goal_first() :- goal(x)",
                                    "goal_last() :- goal(o)",
                                    "does(xplayer, marker(A, B)) :- next(cell(A, B, b), cell(A, B, x))",
                                    "legal(xplayer, marker(A, B)) :- cell(A, B, b)",
                                    "does(oplayer, markers(A, B)) :- next(cell(A, B, b), cell(A, B, o))",
                                    "legal(oplayer, markers(A, B)) :- cell(A, B, b)"
                                    };
    vector<string> another_version = { "role(xplayer)",
                                    "role(oplayer)",
                                    "init(free(1, 1))",
                                    "init(free(1, 2))",
                                    "init(free(1, 3))",
                                    "init(free(2, 1))",
                                    "init(free(2, 2))",
                                    "init(free(2, 3))",
                                    "init(free(3, 1))",
                                    "init(free(3, 2))",
                                    "init(free(3, 3))",
                                    "init(true(a))",
                                    "row_mark(X) :- mark(X, 1) & mark(X, 2) & mark(X, 3)",
                                    "row_mark(X) :- mark(1, X) & mark(2, X) & mark(3, X)",
                                    "row_circle(X) :- circle(X, 1) & circle(X, 2) & circle(X, 3)",
                                    "row_circle(X) :- circle(1, X) & circle(2, X) & circle(3, X)",
                                    "row_circle_final(a) :- row_circle(1) | row_circle(2) | row_circle(3)",
                                    "row_mark_final(a) :- row_mark(1) | row_mark(2) | row_mark(3)",
                                    "is_f(a) :- free(1, 1) | free(1, 2) | free(1, 3) | free(2, 1) | free(2, 2) | free(2, 3) | free(3, 1) | free(3, 2) | free(3, 3)",
                                    "is_full(a) :- ~is_f(a)",
                                    "diagonal_mark(a) :- mark(1, 1) & mark(2, 2) & mark(3, 3)",
                                    "diagonal_mark(a) :- mark(1, 3) & mark(2, 2) & mark(3, 1)",
                                    "diagonal_circle(a) :- circle(1, 1) & circle(2, 2) & circle(3, 3)",
                                    "diagonal_circle(a) :- circle(1, 3) & circle(2, 2) & circle(3, 1)",
                                    "draw() :- ~goal_mark(a) & ~goal_circle(a) & is_full(a)",
                                    "terminal(a) :- goal_mark(a) | goal_circle(a) | is_full(a)",
                                    "goal_mark(a) :- row_mark_final(a) | diagonal_mark(a)",
                                    "goal_circle(a) :- row_circle_final(a) | diagonal_circle(a)",
                                    "goal_first(a) :- goal_mark(a)",
                                    "goal_last(a) :- goal_circle(a)",
                                    "freeing(A) :- next(mark(A, 1), free(A, 1)) | next(mark(A, 2), free(A, 2)) | next(mark(A, 3), free(A, 3))",
                                   // "does(xplayer, makefree(A)) :- next(free(A, 1), wall(A, 1)) & next(free(A, 2), wall(A, 2)) & init(first(a))",
                                   // "legal(xplayer, makefree(A)) :- free(A, 1) & free(A, 2)",
                                    "does(xplayer, domark(A, B)) :- next(free(A, B), mark(A, B)) & init(first(a))",
                                    "legal(xplayer, domark(A, B)) :- free(A, B)",
                                    "does(oplayer, docircle(A, B)) :- next(free(A, B), circle(A, B)) & init(first(a))",
                                    "legal(oplayer, docircle(A, B)) :- free(A, B)"
                                    };
    // generate_zovrist();
    // predefined_hashed();
    // for(int i = 0; i < another_version.size(); i++) {
    //     Functions::process_line_binary(another_version[i]);
    // }
    // show_games();
    // cout << Functions::evaluate_binary("mover(3, 3)") << "\n";
    // cout << Functions::evaluate_binary("circle(3, 3)") << "\n";
    // cout << Functions::evaluate_binary("free(3, 3)") << "\n";
    // cout << Functions::evaluate_binary("circle(1, 1)") << "\n";
    // cout << Functions::evaluate_binary("free(1, 1)") << "\n";
    // show_games();
   // cout << Functions::evaluate_action("domark(1, 1)") << "\n";
   // cout << Functions::evaluate_binary("mark(1, 1)") << "\n";
    simulate_player(another_version, 1);
}