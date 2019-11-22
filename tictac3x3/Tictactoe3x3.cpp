#include <vector>
#include <string>
#include <iostream>
#include "Tictactoe3x3.h"
using namespace std;
void State::show() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++)
        cout << this->game[i][j] << " ";
        cout << "\n";
    }
}
vector<char> State::state_to_vec_hash() {
    vector<char> vecs;
    for(int i = 0; i < 9; i++) {
        if(game[i / 3][i % 3] == -1)
        vecs.push_back(2);
        else
        vecs.push_back(game[i / 3][i % 3]);
    }
    return vecs;
}
vector<char> State::state_to_vec_neural() {
    vector<char> vecs;
    for(int i = 0; i < 9; i++) {
        vecs.push_back(game[i / 3][i % 3]);
    }
    return vecs;
}

void FirstAction::execute(State &state) {
    if(state.game[this->y][this->x] == 0)
        state.game[this->y][this->x] = this->id;
}
string FirstAction::stre() {
    return "(" + to_string(this->y) + ", " + to_string(this->x) + ") ID: " + to_string(this->id);
}

void FirstPlayer::execute_action(FirstAction action, State &state) {
    action.execute(state);
}
vector<FirstAction> FirstPlayer::get_all_actions(State &state) {
    vector<FirstAction> all_actions;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(state.game[i][j] == 0) {
                FirstAction action;
                action.y = i;
                action.x = j;
                action.id = this->id;
                all_actions.push_back(action);
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
    for(int i = 0; i < 3; i++)
    {
        if(state.game[i][0] == state.game[i][1] && state.game[i][0] == state.game[i][2] && state.game[i][0] == this->id)
            return 50;
        if(state.game[i][0] == state.game[i][1] && state.game[i][0] == state.game[i][2] && state.game[i][0] != 0)
            return -50;
    }
    for(int i = 0; i < 3; i++)
    {
        if(state.game[0][i] == state.game[1][i] && state.game[0][i] == state.game[2][i] && state.game[0][i] == this->id)
            return 50;
        if(state.game[0][i] == state.game[1][i] && state.game[0][i] == state.game[2][i] && state.game[0][i] != 0)
            return -50;
    }
    if(state.game[0][0] == state.game[1][1] && state.game[0][0] == state.game[2][2] && state.game[0][0] == this->id)
        return 50;
    if(state.game[0][0] == state.game[1][1] && state.game[0][0] == state.game[2][2] && state.game[0][0] != 0)
        return -50;
    if(state.game[0][2] == state.game[1][1] && state.game[0][2] == state.game[2][0] && state.game[0][2] == this->id)
        return 50;
    if(state.game[0][2] == state.game[1][1] && state.game[0][2] == state.game[2][0] && state.game[0][2] != 0)
        return -50;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++)
        if(state.game[i][j] == 0)
            return 0;
    }
    return 20;
}

void SecondPlayer::execute_action(FirstAction action, State &state) {
    action.execute(state);
}
vector<FirstAction> SecondPlayer::get_all_actions(State &state) {
    vector<FirstAction> all_actions;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(state.game[i][j] == 0) {
                FirstAction action;
                action.y = i;
                action.x = j;
                action.id = this->id;
                all_actions.push_back(action);
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
    for(int i = 0; i < 3; i++)
    {
        if(state.game[i][0] == state.game[i][1] && state.game[i][0] == state.game[i][2] && state.game[i][0] == this->id)
        return 50;
        if(state.game[i][0] == state.game[i][1] && state.game[i][0] == state.game[i][2] && state.game[i][0] != 0)
        return -50;
    }
    for(int i = 0; i < 3; i++)
    {
        if(state.game[0][i] == state.game[1][i] && state.game[0][i] == state.game[2][i] && state.game[0][i] == this->id)
        return 50;
        if(state.game[0][i] == state.game[1][i] && state.game[0][i] == state.game[2][i] && state.game[0][i] != 0)
        return -50;
    }
    if(state.game[0][0] == state.game[1][1] && state.game[0][0] == state.game[2][2] && state.game[0][0] == this->id)
        return 50;
    if(state.game[0][0] == state.game[1][1] && state.game[0][0] == state.game[2][2] && state.game[0][0] != 0)
        return -50;
    if(state.game[0][2] == state.game[1][1] && state.game[0][2] == state.game[2][0] && state.game[0][2] == this->id)
        return 50;
    if(state.game[0][2] == state.game[1][1] && state.game[0][2] == state.game[2][0] && state.game[0][2] != 0)
        return -50;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++)
        if(state.game[i][j] == 0)
            return 0;
    }
    return 20;
}