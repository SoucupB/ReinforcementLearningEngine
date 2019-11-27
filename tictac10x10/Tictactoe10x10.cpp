#include <vector>
#include <string>
#include <iostream>
#include "Tictactoe10x10.h"
using namespace std;
void State::show() {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++)
            cout << (int)this->game[i][j] << " ";
        cout << "\n";
    }
}
int turn = 0;
vector<char> State::state_to_vec_hash() {
    return vecs_hash;
}
vector<char> State::state_to_vec_neural() {
    return vecs_neural_net;
}

void FirstAction::execute(State &state) {
    if(state.game[this->y][this->x] == 0) {
        state.game[this->y][this->x] = this->id;
        state.vecs_hash[this->y * 10 + this->x] = (this->id == -1 ? 2 : 1);
        state.vecs_neural_net[this->y * 10 + this->x] = this->id;
        state.turn++;
    }
    else {
        std::cout << "Invalid move ";
        state.show();
        exit(0);
    }
}
string FirstAction::stre() {
    return "(" + to_string(this->y) + ", " + to_string(this->x) + ") ID: " + to_string(this->id);
}

void FirstPlayer::execute_action(FirstAction action, State &state) {
    action.execute(state);
}
vector<FirstAction> FirstPlayer::get_all_actions(State &state) {
    vector<FirstAction> all_actions;
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            if(state.game[i][j] == 0) {
              int plm[] = {0, 1, 0, -1, 1, 1, -1, -1};
              int plt[] = {1, 0, -1, 0, -1, 1, -1, 1};
              for(int p = 0; p < 8; p++) {
                if(state.turn == 0 || (i + plm[p] >= 0 && i + plm[p] < 10 && j + plt[p] >= 0 && j + plt[p] < 10 && state.game[i + plm[p]][j + plt[p]])) {
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
int FirstPlayer::get_max_score() {
    return this->MaxPoints;
}
int FirstPlayer::get_min_score() {
    return this->MinPoints;
}
int FirstPlayer::get_draw_score() {
    return this->draw;
}
int FirstPlayer::get_final_evaluation_board(State &state) {
    int k = 0;
    char gridM[12][12];
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
        gridM[i][j] = state.game[i][j];
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 6; j++) {
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
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 6; j++) {
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
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 6; j++) {
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
    for(int i = 4; i < 10; i++)
        for(int j = 0; j < 6; j++) {
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
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
        if(!gridM[i][j])
            return 0;
    return 20;
}

void SecondPlayer::execute_action(FirstAction action, State &state) {
    action.execute(state);
}
vector<FirstAction> SecondPlayer::get_all_actions(State &state) {
    vector<FirstAction> all_actions;
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            if(state.game[i][j] == 0) {
              int plm[] = {0, 1, 0, -1, 1, 1, -1, -1};
              int plt[] = {1, 0, -1, 0, -1, 1, -1, 1};
              for(int p = 0; p < 8; p++) {
                if(state.turn == 0 || (i + plm[p] >= 0 && i + plm[p] < 10 && j + plt[p] >= 0 && j + plt[p] < 10 && state.game[i + plm[p]][j + plt[p]])) {
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
int SecondPlayer::get_max_score() {
    return this->MaxPoints;
}
int SecondPlayer::get_min_score() {
    return this->MinPoints;
}
int SecondPlayer::get_draw_score() {
    return this->draw;
}
int SecondPlayer::get_final_evaluation_board(State &state) {
    int k = 0;
    char gridM[12][12];
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
        gridM[i][j] = state.game[i][j];
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 6; j++) {
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
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 6; j++) {
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
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 6; j++) {
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
    for(int i = 4; i < 10; i++)
        for(int j = 0; j < 6; j++) {
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
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
        if(!gridM[i][j])
            return 0;
    return 20;
}