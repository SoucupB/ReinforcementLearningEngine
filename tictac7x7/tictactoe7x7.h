#include <vector>
#include <string>
#include <iostream>
using namespace std;
class State {
public:
vector<char> vecs_hash{vector<char>(49,0)}, vecs_neural_net{vector<char>(49,0)};
  char game[10][10] = { {0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0}, };
  void show() {
    for(int i = 0; i < 7; i++) {
      for(int j = 0; j < 7; j++)
        cout << (int)this->game[i][j] << " ";
        cout << "\n";
      }
  }
  int turn = 0;
  vector<char> state_to_vec_hash() {
    return vecs_hash;
  }
  vector<char> state_to_vec_neural() {
    return vecs_neural_net;
  }
};

class FirstAction {
public:
  int x, y, id;
  void execute(State &state) {
    if(state.game[this->y][this->x] == 0) {
      state.turn++;
      state.game[this->y][this->x] = this->id;
      state.vecs_hash[this->y * 7 + this->x] = (this->id == -1 ? 2 : 1);
      state.vecs_neural_net[this->y * 7 + this->x] = this->id;
    }
    else {
      std::cout << "Invalid move ";
      state.show();
      exit(0);
    }
  }
  string stre() {
    return "(" + to_string(this->y) + ", " + to_string(this->x) + ") ID: " + to_string(this->id);
  }
};

class FirstPlayer {

   public:
      int id = 1, MaxPoints = 50, MinPoints = -50, draw = 20;
      void execute_action(FirstAction action, State &state) {
        action.execute(state);
      }
      vector<FirstAction> get_all_actions(State &state) {
        vector<FirstAction> all_actions;
        for(int i = 0; i < 7; i++) {
          for(int j = 0; j < 7; j++) {
            if(state.game[i][j] == 0) {
              int plm[] = {0, 1, 0, -1, 1, 1, -1, -1};
              int plt[] = {1, 0, -1, 0, -1, 1, -1, 1};
              for(int p = 0; p < 8; p++) {
                if(state.turn == 0 || (i + plm[p] >= 0 && i + plm[p] < 7 && j + plt[p] >= 0 && j + plt[p] < 7 && state.game[i + plm[p]][j + plt[p]])) {
                  FirstAction action;
                  action.y = i;
                  action.x = j;
                  action.id = this->id;
                  all_actions.push_back(action);
                  break;
                }
              }
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
        for(int i = 0; i < 7; i++)
          for(int j = 0; j < 7; j++)
            gridM[i][j] = state.game[i][j];
        for(int i = 0; i < 7; i++) {
            for(int j = 0; j < 3; j++) {
                int ok = 1;
                for(int p = j; p < j + 4; p++)
                    if(gridM[i][p] != gridM[i][p + 1] || gridM[i][p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        }
        for(int i = 0; i < 7; i++) {
            for(int j = 0; j < 3; j++) {
                int ok = 1;
                for(int p = j; p < j + 4; p++)
                    if(gridM[p][i] != gridM[p + 1][i] || gridM[p][i] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[j][i] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        }
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++) {
                int ok = 1;
                for(int p = 0; p < 4; p++)
                    if(gridM[i + p][j + p] != gridM[i + p + 1][j + p + 1] || gridM[i + p][j + p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        for(int i = 4; i < 7; i++)
            for(int j = 0; j < 3; j++) {
                int ok = 1;
                for(int p = 0; p < 4; p++)
                    if(gridM[i - p][j + p] != gridM[i - p - 1][j + p + 1] || gridM[i - p][j + p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        for(int i = 0; i < 7; i++)
          for(int j = 0; j < 7; j++)
            if(!gridM[i][j])
              return 0;
        return 20;
      }
};

class SecondPlayer {

   public:
      int id = -1, MaxPoints = 50, MinPoints = -50, draw = 20;
      void execute_action(FirstAction action, State &state) {
        action.execute(state);
      }
      vector<FirstAction> get_all_actions(State &state) {
        vector<FirstAction> all_actions;
        for(int i = 0; i < 7; i++) {
          for(int j = 0; j < 7; j++) {
            if(state.game[i][j] == 0) {
              int plm[] = {0, 1, 0, -1, 1, 1, -1, -1};
              int plt[] = {1, 0, -1, 0, -1, 1, -1, 1};
              for(int p = 0; p < 8; p++) {
                if(state.turn == 0 || (i + plm[p] >= 0 && i + plm[p] < 7 && j + plt[p] >= 0 && j + plt[p] < 7 && state.game[i + plm[p]][j + plt[p]])) {
                  FirstAction action;
                  action.y = i;
                  action.x = j;
                  action.id = this->id;
                  all_actions.push_back(action);
                  break;
                }
              }
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
        for(int i = 0; i < 7; i++)
          for(int j = 0; j < 7; j++)
            gridM[i][j] = state.game[i][j];
        for(int i = 0; i < 7; i++) {
            for(int j = 0; j < 3; j++) {
                int ok = 1;
                for(int p = j; p < j + 4; p++)
                    if(gridM[i][p] != gridM[i][p + 1] || gridM[i][p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        }
        for(int i = 0; i < 7; i++) {
            for(int j = 0; j < 3; j++) {
                int ok = 1;
                for(int p = j; p < j + 4; p++)
                    if(gridM[p][i] != gridM[p + 1][i] || gridM[p][i] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[j][i] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        }
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++) {
                int ok = 1;
                for(int p = 0; p < 4; p++)
                    if(gridM[i + p][j + p] != gridM[i + p + 1][j + p + 1] || gridM[i + p][j + p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        for(int i = 4; i < 7; i++)
            for(int j = 0; j < 3; j++) {
                int ok = 1;
                for(int p = 0; p < 4; p++)
                    if(gridM[i - p][j + p] != gridM[i - p - 1][j + p + 1] || gridM[i - p][j + p] == 0)
                        ok = 0;
                if(ok) {
                    if(gridM[i][j] == id)
                        return MaxPoints;
                    return MinPoints;
                }
            }
        for(int i = 0; i < 7; i++)
          for(int j = 0; j < 7; j++)
            if(!gridM[i][j])
              return 0;
        return 20;
      }
};