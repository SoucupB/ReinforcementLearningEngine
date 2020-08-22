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
    if(test == "test_number_params") {
        start = timeSinceEpochMillisec();
        test_numbers_params_function();
        finish = timeSinceEpochMillisec();
    }
    if(test == "test_boat_game") {
        start = timeSinceEpochMillisec();
        test_boat_game();
        finish = timeSinceEpochMillisec();
    }
    if(test == "test_4_in_line") {
        start = timeSinceEpochMillisec();
        test_4_in_line();
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
                                    "terminal() :- goal_mark(a) | goal_circle(a) | is_full(a)",
                                    "goal_mark(a) :- row_mark_final(a) | diagonal_mark(a)",
                                    "goal_circle(a) :- row_circle_final(a) | diagonal_circle(a)",
                                    "goal_first() :- goal_mark(a)",
                                    "goal_last() :- goal_circle(a)",
                                    "freeing(A) :- next(mark(A, 1), free(A, 1)) | next(mark(A, 2), free(A, 2)) | next(mark(A, 3), free(A, 3))",
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
    simulate_player(another_version, 1);
}

void show_game_three() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            int x = Functions::evaluate_binary("mark(" + to_string(i * 3 + j + 1) + ")");
            int o = Functions::evaluate_binary("circle(" + to_string(i * 3 + j + 1) + ")");
            int b = Functions::evaluate_binary("free(" + to_string(i * 3 + j + 1) + ")");
            int z = Functions::evaluate_binary("wall(" + to_string(i * 3 + j + 1) + ")");
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

void show_gamer() {
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

void show_connec_four() {
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 7; j++) {
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

void test_numbers_params_function() {
    vector<string> current_file = { "role(xplayer)",
                                    "role(oplayer)",
                                    "init(free(1))",
                                    "init(free(2))",
                                    "init(free(3))",
                                    "init(free(4))",
                                    "init(free(5))",
                                    "init(free(6))",
                                    "init(free(7))",
                                    "init(free(8))",
                                    "init(free(9))",
                                    "row_mark(X) :- mark(X) & mark(X+1) & mark(X+2)",
                                    "row_mark_two(X) :- mark(X) & mark(X+3) & mark(X+6)",
                                    "rows_mark() :- row_mark_two(1) | row_mark_two(2) | row_mark_two(3)",
                                    "diagonal_mark() :- mark(1) & mark(5) & mark(9)",
                                    "diagonal_mark() :- mark(3) & mark(5) & mark(7)",
                                    "columns_mark() :- row_mark(1) | row_mark(4) | row_mark(7)",
                                    "row_circle(X) :- circle(X) & circle(X+1) & circle(X+2)",
                                    "row_circle_two(X) :- circle(X) & circle(X+3) & circle(X+6)",
                                    "rows_circle() :- row_circle_two(1) | row_circle_two(2) | row_circle_two(3)",
                                    "columns_circle() :- row_circle(1) | row_circle(4) | row_circle(7)",
                                    "diagonal_circle() :- circle(1) & circle(5) & circle(9)",
                                    "diagonal_circle() :- circle(3) & circle(5) & circle(7)",
                                    "finish_mark() :- rows_mark() | columns_mark() | diagonal_mark()",
                                    "finish_circle() :- rows_circle() | columns_circle() | diagonal_circle()",
                                    "is_f() :- free(1) | free(2) | free(3) | free(4) | free(5) | free(6) | free(7) | free(8) | free(9)",
                                    "is_full() :- ~is_f()",
                                    "goal_first() :- finish_mark()",
                                    "goal_last() :- finish_circle()",
                                    "terminal() :- finish_circle() | finish_mark() | is_full()",
                                    "draw() :- ~finish_circle() & ~finish_mark() & is_full()",
                                    "mover(A) :- next(free(A+1), mark(A+1))",
                                    "log(A) :- free(A+1) & mark(A)",
                                    "does(xplayer, move(A)) :- next(free(A+1), circle(A+1)) & next(circle(A), free(A))",
                                    "legal(xplayer, move(A)) :- free(A+1) & circle(A)",
                                    "does(xplayer, domark(A)) :- next(free(A), mark(A))",
                                    "legal(xplayer, domark(A)) :- free(A)",
                                    "does(oplayer, docircle(A)) :- next(free(A), circle(A))",
                                    "legal(oplayer, docircle(A)) :- free(A)"
                                    };
    int max_tests = 100000;
    // generate_zovrist();
    // predefined_hashed();
    // for(int i = 0; i < current_file.size(); i++) {
    //     Functions::process_line_binary(current_file[i]);
    // }
    // show_game_three();
    // cout << Functions::evaluate_binary("mover(5)") << "\n";
    // show_game_three();
    // show_game_three();
    simulate_player(current_file, 1);
}

void test_boat_game() {
    vector<string> current_file = { "role(xplayer)",
                                    "role(oplayer)",
                                    "init(mark(1, 1))",
                                    "init(free(1, 2))",
                                    "init(free(1, 3))",
                                    "init(free(2, 1))",
                                    "init(free(2, 2))",
                                    "init(free(2, 3))",
                                    "init(free(3, 1))",
                                    "init(free(3, 2))",
                                    "init(true(a))",
                                    "init(circle(3, 3))",
                                    "draw() :- ~true(a)",
                                    "terminal() :- general_goal_first() | general_goal_last()",
                                    "gge(A) :- ~circle(1, A) & ~circle(2, A) & ~circle(3, A)",
                                    "general_goal_first() :- gge(1) & gge(2) & gge(3)",
                                    "gger(A) :- ~mark(1, A) & ~mark(2, A) & ~mark(3, A)",
                                    "general_goal_last() :- gger(1) & gger(2) & gger(3)",
                                    "goal_first() :- general_goal_first()",
                                    "goal_last() :- general_goal_last()",
                                    "correct(A, B) :- free(A+1, B) & mark(A, B)",
                                    "movedown(A, B) :- correct(A, B) & next(free(A+1, B), mark(A+1, B)) & next(mark(A, B), free(A, B))",
                                    "does(xplayer, movedown(A, B)) :- next(free(A+1, B), mark(A+1, B)) & next(mark(A, B), free(A, B))",
                                    "legal(xplayer, movedown(A, B)) :- free(A+1, B) & mark(A, B)",
                                    "does(xplayer, moveup(A, B)) :- next(free(A-1, B), mark(A-1, B)) & next(mark(A, B), free(A, B))",
                                    "legal(xplayer, moveup(A, B)) :- free(A-1, B) & mark(A, B)",
                                    "does(xplayer, moveright(A, B)) :- next(free(A, B+1), mark(A, B+1)) & next(mark(A, B), free(A, B))",
                                    "legal(xplayer, moveright(A, B)) :- free(A, B+1) & mark(A, B)",
                                    "does(xplayer, moveleft(A, B)) :- next(free(A, B-1), mark(A, B-1)) & next(mark(A, B), free(A, B))",
                                    "legal(xplayer, moveleft(A, B)) :- free(A, B-1) & mark(A, B)",
                                    "does(xplayer, attackdown(A, B)) :- next(circle(A+1, B), mark(A+1, B)) & next(mark(A, B), free(A, B))",
                                    "legal(xplayer, attackdown(A, B)) :- circle(A+1, B) & mark(A, B)",
                                    "does(xplayer, attackup(A, B)) :- next(circle(A-1, B), mark(A-1, B)) & next(mark(A, B), free(A, B))",
                                    "legal(xplayer, attackup(A, B)) :- circle(A-1, B) & mark(A, B)",
                                    "does(xplayer, attackright(A, B)) :- next(circle(A, B+1), mark(A, B+1)) & next(mark(A, B), free(A, B))",
                                    "legal(xplayer, attackright(A, B)) :- circle(A, B+1) & mark(A, B)",
                                    "does(xplayer, attackleft(A, B)) :- next(circle(A, B-1), mark(A, B-1)) & next(mark(A, B), free(A, B))",
                                    "legal(xplayer, attackleft(A, B)) :- circle(A, B-1) & mark(A, B)",
                                    "does(xplayer, staying2(A)) :- true(a)",
                                    "legal(xplayer, staying2(A)) :- true(a)",
                                  //  "does(oplayer, trans(A, B)) :- next(circle(A, B), wall(A, B))",
                                  //  "legal(oplayer, trans(A, B)) :- circle(A, B)",
                                    "does(oplayer, movedown1(A, B)) :- next(free(A+1, B), circle(A+1, B)) & next(circle(A, B), free(A, B))",
                                    "legal(oplayer, movedown1(A, B)) :- free(A+1, B) & circle(A, B)",
                                    "does(oplayer, moveup1(A, B)) :- next(free(A-1, B), circle(A-1, B)) & next(circle(A, B), free(A, B))",
                                    "legal(oplayer, moveup1(A, B)) :- free(A-1, B) & circle(A, B)",
                                    "does(oplayer, moveright1(A, B)) :- next(free(A, B+1), circle(A, B+1)) & next(circle(A, B), free(A, B))",
                                    "legal(oplayer, moveright1(A, B)) :- free(A, B+1) & circle(A, B)",
                                    "does(oplayer, moveleft1(A, B)) :- next(free(A, B-1), circle(A, B-1)) & next(circle(A, B), free(A, B))",
                                    "legal(oplayer, moveleft1(A, B)) :- free(A, B-1) & circle(A, B)",
                                    "does(oplayer, attackdown1(A, B)) :- next(mark(A+1, B), circle(A+1, B)) & next(circle(A, B), free(A, B))",
                                    "legal(oplayer, attackdown1(A, B)) :- mark(A+1, B) & circle(A, B)",
                                    "does(oplayer, attackup1(A, B)) :- next(mark(A-1, B), circle(A-1, B)) & next(circle(A, B), free(A, B))",
                                    "legal(oplayer, attackup1(A, B)) :- mark(A-1, B) & circle(A, B)",
                                    "does(oplayer, attackright1(A, B)) :- next(mark(A, B+1), circle(A, B+1)) & next(circle(A, B), free(A, B))",
                                    "legal(oplayer, attackright1(A, B)) :- mark(A, B+1) & circle(A, B)",
                                    "does(oplayer, attackleft1(A, B)) :- next(mark(A, B-1), circle(A, B-1)) & next(circle(A, B), free(A, B))",
                                    "legal(oplayer, attackleft1(A, B)) :- mark(A, B-1) & circle(A, B)",
                                    "does(oplayer, staying(A)) :- true(a)",
                                    "legal(oplayer, staying(A)) :- true(a)",
                                    };
    int max_tests = 100000;
    // generate_zovrist();
    // predefined_hashed();
    // for(int i = 0; i < current_file.size(); i++) {
    //     Functions::process_line_binary(current_file[i]);
    // }
    // show_gamer();
    // cout << Functions::evaluate_binary("movedown(1, 1)") << "\n";
    // cout << Functions::evaluate_binary("movedown(2, 1)") << "\n";
    // show_gamer();
    simulate_player(current_file, 1);
}

void test_4_in_line() {
    vector<string> current_file = { "role(xplayer)",
                                    "role(oplayer)",
                                    "init(free(1, 1))",
                                    "init(free(1, 2))",
                                    "init(free(1, 3))",
                                    "init(free(1, 4))",
                                    "init(free(1, 5))",
                                    "init(free(1, 6))",
                                    "init(free(1, 7))",

                                    "init(free(2, 1))",
                                    "init(free(2, 2))",
                                    "init(free(2, 3))",
                                    "init(free(2, 4))",
                                    "init(free(2, 5))",
                                    "init(free(2, 6))",
                                    "init(free(2, 7))",

                                    "init(free(3, 1))",
                                    "init(free(3, 2))",
                                    "init(free(3, 3))",
                                    "init(free(3, 4))",
                                    "init(free(3, 5))",
                                    "init(free(3, 6))",
                                    "init(free(3, 7))",

                                    "init(free(4, 1))",
                                    "init(free(4, 2))",
                                    "init(free(4, 3))",
                                    "init(free(4, 4))",
                                    "init(free(4, 5))",
                                    "init(free(4, 6))",
                                    "init(free(4, 7))",

                                    "init(free(5, 1))",
                                    "init(free(5, 2))",
                                    "init(free(5, 3))",
                                    "init(free(5, 4))",
                                    "init(free(5, 5))",
                                    "init(free(5, 6))",
                                    "init(free(5, 7))",

                                    "init(free(6, 1))",
                                    "init(free(6, 2))",
                                    "init(free(6, 3))",
                                    "init(free(6, 4))",
                                    "init(free(6, 5))",
                                    "init(free(6, 6))",
                                    "init(free(6, 7))",

                                    "init(block(7, 1))",
                                    "init(block(7, 2))",
                                    "init(block(7, 3))",
                                    "init(block(7, 4))",
                                    "init(block(7, 5))",
                                    "init(block(7, 6))",
                                    "init(block(7, 7))",

                                    "g_v_m(A, B) :- mark(A, B) & mark(A+1, B) & mark(A+2, B) & mark(A+3, B)",
                                    "g_v_z(A) :- g_v_m(A, 1) | g_v_m(A, 2) | g_v_m(A, 3) | g_v_m(A, 4) | g_v_m(A, 5) | g_v_m(A, 6) | g_v_m(A, 7)",
                                    "goal_vertical_mark() :- g_v_z(1) | g_v_z(2) | g_v_z(3)",
                                    "g_v_t(A, B) :- mark(A, B) & mark(A, B+1) & mark(A, B+2) & mark(A, B+3)",
                                    "g_v_r(A) :- g_v_t(1, A) | g_v_t(2, A) | g_v_t(3, A) | g_v_t(4, A) | g_v_t(5, A) | g_v_t(6, A)",
                                    "goal_horizontal_mark() :- g_v_r(1) | g_v_r(2) | g_v_r(3) | g_v_r(4)",
                                    "first_diagonal(A, B) :- mark(A, B) & mark(A+1, B+1) & mark(A+2, B+2) & mark(A+3, B+3)",
                                    "first_d(A) :- first_diagonal(A, 1) | first_diagonal(A, 2) | first_diagonal(A, 3) | first_diagonal(A, 4)",
                                    "diagonal_mark() :- first_d(1) | first_d(2) | first_d(3)",
                                    "second_diagonal(A, B) :- mark(A, B) & mark(A+1, B-1) & mark(A+2, B-2) & mark(A+3, B-3)",
                                    "second_d(A) :- second_diagonal(A, 4) | second_diagonal(A, 5) | second_diagonal(A, 6) | second_diagonal(A, 7)",
                                    "diagonal_mark() :- second_d(1) | second_d(2) | second_d(3)",
                                    "goal_first() :- goal_vertical_mark() | goal_horizontal_mark() | diagonal_mark()",

                                    "g_a_m(A, B) :- circle(A, B) & circle(A+1, B) & circle(A+2, B) & circle(A+3, B)",
                                    "g_a_z(A) :- g_a_m(A, 1) | g_a_m(A, 2) | g_a_m(A, 3) | g_a_m(A, 4) | g_a_m(A, 5) | g_a_m(A, 6) | g_a_m(A, 7)",
                                    "goal_vertical_circle() :- g_a_z(1) | g_a_z(2) | g_a_z(3)",
                                    "g_a_t(A, B) :- circle(A, B) & circle(A, B+1) & circle(A, B+2) & circle(A, B+3)",
                                    "g_a_r(A) :- g_a_t(1, A) | g_a_t(2, A) | g_a_t(3, A) | g_a_t(4, A) | g_a_t(5, A) | g_a_t(6, A)",
                                    "goal_horizontal_circle() :- g_a_r(1) | g_a_r(2) | g_a_r(3) | g_a_r(4)",
                                    "first_diagonal_circ(A, B) :- circle(A, B) & circle(A+1, B+1) & circle(A+2, B+2) & circle(A+3, B+3)",
                                    "first_d_c(A) :- first_diagonal_circ(A, 1) | first_diagonal_circ(A, 2) | first_diagonal_circ(A, 3) | first_diagonal_circ(A, 4)",
                                    "diagonal_circle() :- first_d_c(1) | first_d_c(2) | first_d_c(3)",
                                    "second_diagonal_circle(A, B) :- circle(A, B) & circle(A+1, B-1) & circle(A+2, B-2) & circle(A+3, B-3)",
                                    "second_d_r(A) :- second_diagonal_circle(A, 4) | second_diagonal_circle(A, 5) | second_diagonal_circle(A, 6) | second_diagonal_circle(A, 7)",
                                    "diagonal_circle() :- second_d_r(1) | second_d_r(2) | second_d_r(3)",
                                    "goal_last() :- goal_vertical_circle() | goal_horizontal_circle() | diagonal_circle()",

                                    "go_f(A) :- ~free(1, A) & ~free(2, A) & ~free(3, A) & ~free(4, A) & ~free(5, A) & ~free(6, A)",
                                    "go_full() :- go_f(1) & go_f(2) & go_f(3) & go_f(4) & go_f(5) & go_f(6) & go_f(7)",
                                    "draw() :- go_full()",
                                    "terminal() :- goal_first() | goal_last() | draw()",
                                    // "ball(A, B) :- block(A+1, B) | ~free(A+1, B)",
                                    // "marker(A, B) :- ball(A, B) & next(free(A, B), mark(A, B))",
                                    "lastblock(A, B) :- block(A+1, B) | ~free(A+1, B)",

                                    "does(xplayer, marker(A, B)) :- next(free(A, B), mark(A, B))",
                                    "legal(xplayer, marker(A, B)) :- free(A, B) & lastblock(A, B)",
                                    "does(oplayer, circler(A, B)) :- next(free(A, B), circle(A, B))",
                                    "legal(oplayer, circler(A, B)) :- free(A, B) & lastblock(A, B)",
                                    };
    int max_tests = 100000;
    // generate_zovrist();
    // predefined_hashed();
    // for(int i = 0; i < current_file.size(); i++) {
    //     Functions::process_line_binary(current_file[i]);
    // }
    // cout << "Compiled!\n";
    // show_connec_four();
    // // // show_gamer();
    // cout << Functions::evaluate_binary("draw()") << "\n";
    // cout << Functions::evaluate_binary("marker(4, 4)") << "\n";
    // show_connec_four();
    // cout << Functions::evaluate_binary("movedown(2, 1)") << "\n";
    // show_gamer();
    simulate_player(current_file, 1);
}