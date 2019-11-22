#include <vector>
#include <string>
#include <iostream>
using namespace std;
class State {
public:
  int game[10][10] = { {0, 0, 0},
                       {0, 0, 0},
                       {0, 0, 0} };
  void show();
  vector<char> state_to_vec_hash();
  vector<char> state_to_vec_neural();
};

class FirstAction {
public:
  int x, y, id;
  void execute(State &state);
  string stre();
};

class FirstPlayer {

   public:
      int id = 1, MaxPoints = 50, MinPoints = -50, draw = 20;
      void execute_action(FirstAction action, State &state);
      vector<FirstAction> get_all_actions(State &state);
      int get_max_score();
      int get_min_score();
      int get_draw_score();
      int get_final_evaluation_board(State &state);
};

class SecondPlayer {

   public:
      int id = -1, MaxPoints = 50, MinPoints = -50, draw = 20;
      void execute_action(FirstAction action, State &state);
      vector<FirstAction> get_all_actions(State &state);
      int get_max_score();
      int get_min_score();
      int get_draw_score();
      int get_final_evaluation_board(State &state);
};