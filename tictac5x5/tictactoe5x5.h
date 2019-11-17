#include <vector>
#include <string>
#include <iostream>
using namespace std;
class State {
public:
  char game[10][10] = { {0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0}, };
  void show() {
    for(int i = 0; i < 5; i++) {
      for(int j = 0; j < 5; j++)
        cout << (int)this->game[i][j] << " ";
      cout << "\n";
    }
  }
  vector<char> state_to_vec_hash() {
    vector<char> vecs;
    for(int i = 0; i < 25; i++) {
      if(game[i / 5][i % 5] == -1)
        vecs.push_back(2);
      else
        vecs.push_back(game[i / 5][i % 5]);
    }
    return vecs;
  }
  vector<char> state_to_vec_neural() {
    vector<char> vecs;
    for(int i = 0; i < 25; i++) {
      vecs.push_back(game[i / 5][i % 5]);
    }
    return vecs;
  }
};

class FirstAction {
public:
  int x, y, id;
  void execute(State &state) {
    if(state.game[this->y][this->x] == 0) {
      state.game[this->y][this->x] = this->id;
    }
    else {
      std::cout << "Invalid move";
      exit(0);
    }
  }
  string stre() {
    return "(" + to_string(this->y) + ", " + to_string(this->x) + ") ID: " + to_string(this->id);
  }
};

class FirstPlayer {
   int id = 1, MaxPoints = 50, MinPoints = -50, draw = 20;
   public:
      void execute_action(FirstAction action, State &state) {
        action.execute(state);
      }
      vector<FirstAction> get_all_actions(State &state) {
        vector<FirstAction> all_actions;
        for(int i = 0; i < 5; i++) {
          for(int j = 0; j < 5; j++) {
            if(state.game[i][j] == 0) {
              FirstAction action;
              action.y = i;
              action.x = j;
              action.id = this->id;
              all_actions.push_back(action);
              // int plm[] = {0, 1, 0, -1, 1, 1, -1, -1};
              // int plt[] = {1, 0, -1, 0, -1, 1, -1, 1};
              // for(int p = 0; p < 8; p++) {
              //   if(i + plm[p] >= 0 && i + plm[p] < 5 && j + plt[p] >= 0 && j + plt[p] < 5 && state.game[i + plm[p]][j + plt[p]]) {
              //     FirstAction action;
              //     action.y = i;
              //     action.x = j;
              //     action.id = this->id;
              //     all_actions.push_back(action);
              //     break;
              //   }
              // }
            }
          }
        }
        return all_actions;
      }
      int get_max_score() {
        return this->MaxPoints;
      }
      int get_min_score() {
        return this->MinPoints;
      }
      int get_draw_score() {
        return this->draw;
      }
      int get_final_evaluation_board(State &state) {
        int k = 0;
        char gridM[10][10];
        for(int i = 0; i < 5; i++)
          for(int j = 0; j < 5; j++)
            gridM[i][j] = state.game[i][j];
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 2; j++) {
                int ok = 1;
                for(int p = j; p < j + 3; p++)
                    if(gridM[i][p] != gridM[i][p + 1] || gridM[i][p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        }
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 5; j++) {
                int ok = 1;
                for(int p = i; p < i + 3; p++)
                    if(gridM[p][j] != gridM[p + 1][j] || gridM[p][j] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++) {
                int ok = 1;
                for(int p = 0; p < 3; p++)
                    if(gridM[i + p][j + p] != gridM[i + p + 1][j + p + 1] || gridM[i + p][j + p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        for(int i = 3; i < 5; i++)
            for(int j = 0; j < 2; j++) {
                int ok = 1;
                for(int p = 0; p < 3; p++)
                    if(gridM[i - p][j + p] != gridM[i - p - 1][j + p + 1] || gridM[i - p][j + p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        for(int i = 0; i < 5; i++)
          for(int j = 0; j < 5; j++)
            if(!gridM[i][j])
              return 0;
        return 20;
      }
};

class SecondPlayer {
   int id = -1, MaxPoints = 50, MinPoints = -50, draw = 20;
   public:
      void execute_action(FirstAction action, State &state) {
        action.execute(state);
      }
      vector<FirstAction> get_all_actions(State &state) {
        vector<FirstAction> all_actions;
        for(int i = 0; i < 5; i++) {
          for(int j = 0; j < 5; j++) {
            if(state.game[i][j] == 0) {
              FirstAction action;
              action.y = i;
              action.x = j;
              action.id = this->id;
              all_actions.push_back(action);
              // int plm[] = {0, 1, 0, -1, 1, 1, -1, -1};
              // int plt[] = {1, 0, -1, 0, -1, 1, -1, 1};
              // for(int p = 0; p < 8; p++) {
              //   if(i + plm[p] >= 0 && i + plm[p] < 5 && j + plt[p] >= 0 && j + plt[p] < 5 && state.game[i + plm[p]][j + plt[p]]) {
              //     FirstAction action;
              //     action.y = i;
              //     action.x = j;
              //     action.id = this->id;
              //     all_actions.push_back(action);
              //     break;
              //   }
              // }
            }
          }
        }
        return all_actions;
      }
      int get_max_score() {
        return this->MaxPoints;
      }
      int get_min_score() {
        return this->MinPoints;
      }
      int get_draw_score() {
        return this->draw;
      }
      int get_final_evaluation_board(State &state) {
        int k = 0;
        char gridM[10][10];
        for(int i = 0; i < 5; i++)
          for(int j = 0; j < 5; j++)
            gridM[i][j] = state.game[i][j];
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 2; j++) {
                int ok = 1;
                for(int p = j; p < j + 3; p++)
                    if(gridM[i][p] != gridM[i][p + 1] || gridM[i][p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        }
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 5; j++) {
                int ok = 1;
                for(int p = i; p < i + 3; p++)
                    if(gridM[p][j] != gridM[p + 1][j] || gridM[p][j] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++) {
                int ok = 1;
                for(int p = 0; p < 3; p++)
                    if(gridM[i + p][j + p] != gridM[i + p + 1][j + p + 1] || gridM[i + p][j + p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        for(int i = 3; i < 5; i++)
            for(int j = 0; j < 2; j++) {
                int ok = 1;
                for(int p = 0; p < 3; p++)
                    if(gridM[i - p][j + p] != gridM[i - p - 1][j + p + 1] || gridM[i - p][j + p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        for(int i = 0; i < 5; i++)
          for(int j = 0; j < 5; j++)
            if(!gridM[i][j])
              return 0;
        return 20;
      }
};