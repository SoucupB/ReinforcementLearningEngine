#include <iostream>
#include <vector>
#include <Windows.h>
#include <map>
#include "..//Components//BattleModel.h"
#include "..//Components//Utils.h"
#include <string>
using namespace std;
class Bombs {
public:
	int x, y, timer, bomb_id, player_id, bomb_size;
};
class Crosses {
public:
	int x, y, cross_size, duration;
};
class State {
public:
	int is_first_player_dead = 0, is_second_player_dead = 0, max_turns = 240, current_turn = 0, timer = 7, z_id = 9, cross_duration = 2;
	int x = 1, y = 1, xx = 8, yy = 8;
	vector< Bombs > bombs;
	map< pair<int, int>, int> affected_zones;
	map<int, int> number_of_bombs;
	vector < Crosses > zones;
	vector< vector<char> > game = { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
									{ 1, 2, 0, 8, 8, 8, 8, 0, 8, 1 },
									{ 1, 0, 1, 8, 1, 8, 1, 8, 0, 1 },
									{ 1, 8, 8, 8, 0, 8, 8, 8, 8, 1 },
									{ 1, 8, 1, 8, 1, 0, 1, 8, 8, 1 },
									{ 1, 8, 8, 8, 0, 0, 0, 8, 8, 1 },
									{ 1, 8, 1, 0, 1, 0, 1, 8, 8, 1 },
									{ 1, 0, 0, 8, 0, 0, 0, 1, 0, 1 },
									{ 1, 8, 0, 8, 8, 0, 0, 0, 3, 1 },
									{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, };
	vector< char > state_to_vec_hash() {
		vector<char> state;
		for (int i = 0; i < game.size(); i++) {
			for (int j = 0; j < game[i].size(); j++) {
				state.push_back(game[i][j]);
			}
		}
		state.push_back(current_turn);
		return state;
	}
	vector< char > state_to_vec_neural() {
		vector<char> state;
		for (int i = 0; i < game.size(); i++) {
			for (int j = 0; j < game[i].size(); j++) {
				state.push_back(game[i][j]);
			}
		}
		state.push_back(current_turn);
		return state;
	}
	void return_final_state() {
		if (affected_zones[make_pair(y, x)])
			is_first_player_dead++;
		if (affected_zones[make_pair(yy, xx)])
			is_second_player_dead++;
	}
	bool is_time_up() {
		return current_turn >= max_turns;
	}
	void add_bomb_to_queue(int y, int x, int bomb_id, int player_id, int bomb_size) {
		Bombs bomb;
		bomb.y = y;
		bomb.x = x;
		bomb.timer = timer;
		bomb.bomb_id = bomb_id;
		bomb.player_id = player_id;
		bomb.bomb_size = bomb_size;
		bombs.push_back(bomb);
		game[y][x] = bomb_id;
	}
	void evaluate_bombs() {
		for (int i = 0; i < bombs.size(); i++) {
			if (bombs[i].timer <= 0) {
				game[bombs[i].y][bombs[i].x] = 0;
				create_z(bombs[i].y, bombs[i].x, bombs[i].bomb_size);
				bombs.erase(bombs.begin() + i);
				i--;
			}
		}
	}
	void evaluate_zones() {
		int cross_x[] = { 1, 0, -1, 0 };
		int cross_y[] = { 0, -1, 0, 1 };
		for (int i = 0; i < zones.size(); i++) {
			if (zones[i].duration <= 0) {
				affected_zones[make_pair(zones[i].y, zones[i].x)]--;
				if (!affected_zones[make_pair(zones[i].y, zones[i].x)]) {
					game[zones[i].y][zones[i].x] = 0;
				}
				for (int j = 0; j < 4; j++) {
					int cx = zones[i].x, cy = zones[i].y;
					for (int k = 0; k < zones[i].cross_size; k++) {
						cx += cross_x[j];
						cy += cross_y[j];
						affected_zones[make_pair(cy, cx)]--;
						if (!affected_zones[make_pair(cy, cx)]) {
							game[cy][cx] = 0;
						}
					}
				}
				zones.erase(zones.begin() + i);
				i--;
			}
		}
	}
	void decrease_zones() {
		for (int i = 0; i < zones.size(); i++) {
			zones[i].duration--;
		}
	}
	void decrease_bombs() {
		for (int i = 0; i < bombs.size(); i++) {
			bombs[i].timer--;
		}
	}
	void show() {
		for (int i = 0; i < game.size(); i++) {
			for (int j = 0; j < game[i].size(); j++)
			{
				if (game[i][j] == 0)
					cout << " ";
				if (game[i][j] == 1)
					cout << "#";
				if (game[i][j] == 2)
					cout << "@";
				if (game[i][j] == 3)
					cout << "&";
				if (game[i][j] == 4)
					cout << "G";
				if (game[i][j] == 9)
					cout << "Z";
				if (game[i][j] == 8)
					cout << "*";
			}
			cout << "\n";
		}
	}
	void create_z(int y, int x, int bmb_size) {
		int cross_x[] = { 1, 0, -1, 0 };
		int cross_y[] = { 0, -1, 0, 1 };
		Crosses current_cross;
		current_cross.y = y;
		current_cross.x = x;
		current_cross.cross_size = bmb_size;
		current_cross.duration = cross_duration;
		game[y][x] = z_id;
		affected_zones[make_pair(y, x)]++;
		zones.push_back(current_cross);
		for (int i = 0; i < 4; i++) {
			int cx = x, cy = y;
			for (int j = 0; j < bmb_size; j++) {
				cx += cross_x[i];
				cy += cross_y[i];
				if (cx >= 0 && cy >= 0 && cx < game[0].size() && cy < game.size() && game[cy][cx] != 1) {
					affected_zones[make_pair(cy, cx)]++;
					if(game[cy][cx] != 4)
						game[cy][cx] = z_id;
				}
				else
					break;
			}
		}
	}
	int state_size() {
		return game.size() * game[0].size();
	}
};
class FirstAction {
public:
	int x, y, xx, yy, bomb_id, player_id, bmb_size;
	string type;
	void execute_move(State &state) {
		if (state.game[y][x] == player_id)
			state.game[y][x] = 0;
		state.game[yy][xx] = player_id;
	}
	void execute_bomb(State &state) {
		state.add_bomb_to_queue(y, x, bomb_id, player_id, bmb_size);
	}
	string stre() {
		return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(xx) + " " + std::to_string(yy) + " " + type + "\n";
	}
};
class FirstPlayer {
public:
	int id = 2, MaxPoints = 50, MinPoints = -50, draw = 20, x = 1, y = 1, max_bombs = 1, bomb_id = 4, bomb_size = 2;
	vector< pair<int, int> > bombs;

	void execute_action(FirstAction action, State &state) {
		if (action.type == "move") {
			if (action.xx != x || action.yy != y) {
				action.execute_move(state);
				x = action.xx;
				y = action.yy;
			}
			state.x = x;
			state.y = y;
			state.decrease_zones();
			state.decrease_bombs();
			state.evaluate_bombs();
			state.evaluate_zones();
			state.current_turn++;
		}
		else if (action.type == "bomb") {
			action.execute_bomb(state);
			state.x = x;
			state.y = y;
			state.decrease_zones();
			state.decrease_bombs();
			state.evaluate_bombs();
			state.evaluate_zones();
			state.current_turn++;
		}
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
	vector<FirstAction> get_all_actions(State &state) {
		x = state.x;
		y = state.y;
		int vec[] = { 0, 1, 0, -1 };
		int vel[] = { 1, 0, -1, 0 };
		vector<FirstAction> all_actions;
		FirstAction stay;
		stay.xx = x;
		stay.yy = y;
		stay.x = x;
		stay.y = y;
		stay.player_id = id;
		stay.type = "move";
		all_actions.push_back(stay);
		int ok = true;
		for (int i = 0; i < state.bombs.size(); i++) {
			if (state.bombs[i].x == x && state.bombs[i].y == y)
				ok = false;
		}
		if (ok) {
			FirstAction bomb;
			bomb.xx = x;
			bomb.yy = y;
			bomb.x = x;
			bomb.y = y;
			bomb.bomb_id = bomb_id;
			bomb.player_id = id;
			bomb.type = "bomb";
			bomb.bmb_size = bomb_size;
			all_actions.push_back(bomb);
		}
		for (int i = 0; i < 4; i++) {
			int cx = vec[i] + x;
			int cy = vel[i] + y;
			if (cx >= 0 && cx < state.game[0].size() && cy >= 0 && cy < state.game.size() && state.game[cy][cx] == 0) {
				FirstAction action;
				action.xx = cx;
				action.yy = cy;
				action.x = x;
				action.y = y;
				action.player_id = id;
				action.type = "move";
				all_actions.push_back(action);
			}
		}
		return all_actions;
	}
	void move_left(State &state) {
		FirstAction action;
		if (x - 1 >= 0 && state.game[y][x - 1] == 0)
		{
			action.xx = x - 1;
			action.yy = y;
			action.x = x;
			action.y = y;
			action.player_id = id;
			action.type = "move";
			this->execute_action(action, state);
		}
	}
	void move_right(State &state) {
		FirstAction action;
		if (x + 1 >= 0 && state.game[y][x + 1] == 0)
		{
			action.xx = x + 1;
			action.yy = y;
			action.x = x;
			action.y = y;
			action.player_id = id;
			action.type = "move";
			this->execute_action(action, state);
		}
	}
	void move_up(State &state) {
		FirstAction action;
		if (y - 1 >= 0 && state.game[y - 1][x] == 0)
		{
			action.xx = x;
			action.yy = y - 1;
			action.x = x;
			action.y = y;
			action.player_id = id;
			action.type = "move";
			this->execute_action(action, state);
		}
	}
	void move_down(State &state) {
		FirstAction action;
		if (x >= 0 && state.game[y + 1][x] == 0)
		{
			action.xx = x;
			action.yy = y + 1;
			action.x = x;
			action.y = y;
			action.player_id = id;
			action.type = "move";
			this->execute_action(action, state);
		}
	}
	void move_bomb(State &state) {
		FirstAction action;
		action.x = x;
		action.y = y;
		action.bomb_id = bomb_id;
		action.player_id = id;
		action.type = "bomb";
		action.bmb_size = bomb_size;
		this->execute_action(action, state);
	}
	int get_final_evaluation_board(State &state) {
		state.return_final_state();
		if (state.is_time_up())
			return draw;
		if (state.is_first_player_dead && state.is_second_player_dead)
			return draw;
		if (state.is_first_player_dead)
			return MinPoints;
		if (state.is_second_player_dead)
			return MaxPoints;
		return 0;
	}
};
class SecondPlayer {
public:
	int id = 3, MaxPoints = 50, MinPoints = -50, draw = 20, x = 8, y = 8, max_bombs = 1, bomb_id = 4, bomb_size = 2;
	vector< pair<int, int> > bombs;

	void execute_action(FirstAction action, State &state) {
		if (action.type == "move") {
			if (x != action.xx || y != action.yy) {
				action.execute_move(state);
				x = action.xx;
				y = action.yy;
			}
			state.xx = x;
			state.yy = y;
			state.decrease_zones();
			state.decrease_bombs();
			state.evaluate_bombs();
			state.evaluate_zones();
			state.current_turn++;
		}
		else if (action.type == "bomb") {
			action.execute_bomb(state);
			state.xx = x;
			state.yy = y;
			state.decrease_zones();
			state.decrease_bombs();
			state.evaluate_bombs();
			state.evaluate_zones();
			state.current_turn++;
		}
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
	vector<FirstAction> get_all_actions(State &state) {
		x = state.xx;
		y = state.yy;
		int vec[] = { 0, 1, 0, -1 };
		int vel[] = { 1, 0, -1, 0 };
		vector<FirstAction> all_actions;
		FirstAction stay;
		stay.xx = x;
		stay.yy = y;
		stay.x = x;
		stay.y = y;
		stay.player_id = id;
		stay.type = "move";
		all_actions.push_back(stay);
		int ok = true;
		for (int i = 0; i < state.bombs.size(); i++) {
			if (state.bombs[i].x == x && state.bombs[i].y == y)
				ok = false;
		}
		if (ok) {
			FirstAction bomb;
			bomb.xx = x;
			bomb.yy = y;
			bomb.x = x;
			bomb.y = y;
			bomb.bomb_id = bomb_id;
			bomb.player_id = id;
			bomb.type = "bomb";
			bomb.bmb_size = bomb_size;
			all_actions.push_back(bomb);
		}
		for (int i = 0; i < 4; i++) {
			int cx = vec[i] + x;
			int cy = vel[i] + y;
			if (cx >= 0 && cx < state.game[0].size() && cy >= 0 && cy < state.game.size() && state.game[cy][cx] == 0) {
				FirstAction action;
				action.xx = cx;
				action.yy = cy;
				action.x = x;
				action.y = y;
				action.player_id = id;
				action.type = "move";
				all_actions.push_back(action);
			}
		}
		//if (state.current_turn >= 3) {
		//	for (int i = 0; i < all_actions.size(); i++) {
		//		cout << all_actions[i].stre();
		//	}
		//	system("pause");
		//	exit(0);
		//}
		//system("pause");
		//exit(0);
		return all_actions;
	}
	void move_left(State &state) {
		FirstAction action;
		if (x - 1 >= 0 && state.game[y][x - 1] == 0)
		{
			action.xx = x - 1;
			action.yy = y;
			action.x = x;
			action.y = y;
			action.player_id = id;
			action.type = "move";
			this->execute_action(action, state);
		}
	}
	void move_right(State &state) {
		FirstAction action;
		if (x + 1 >= 0 && state.game[y][x + 1] == 0)
		{
			action.xx = x + 1;
			action.yy = y;
			action.x = x;
			action.y = y;
			action.player_id = id;
			action.type = "move";
			this->execute_action(action, state);
		}
	}
	void move_up(State &state) {
		FirstAction action;
		if (y - 1 >= 0 && state.game[y - 1][x] == 0)
		{
			action.xx = x;
			action.yy = y - 1;
			action.x = x;
			action.y = y;
			action.player_id = id;
			action.type = "move";
			this->execute_action(action, state);
		}
	}
	void move_down(State &state) {
		FirstAction action;
		if (x >= 0 && state.game[y + 1][x] == 0)
		{
			action.xx = x;
			action.yy = y + 1;
			action.x = x;
			action.y = y;
			action.player_id = id;
			action.type = "move";
			this->execute_action(action, state);
		}
	}
	void move_bomb(State &state) {
		int ok = true;
		for (int i = 0; i < state.bombs.size(); i++) {
			if (state.bombs[i].x == x && state.bombs[i].y == y)
				ok = false;
		}
		if (ok) {
			FirstAction bomb;
			bomb.xx = x;
			bomb.yy = y;
			bomb.x = x;
			bomb.y = y;
			bomb.bomb_id = bomb_id;
			bomb.player_id = id;
			bomb.type = "bomb";
			bomb.bmb_size = bomb_size;
			this->execute_action(bomb, state);
		}
	}
	int get_final_evaluation_board(State &state) {
		state.return_final_state();
		if (state.is_time_up())
			return draw;
		if (state.is_first_player_dead && state.is_second_player_dead)
			return draw;
		if (state.is_second_player_dead)
			return MinPoints;
		if (state.is_first_player_dead)
			return MaxPoints;
		return 0;
	}
};
void actions_player(FirstPlayer &human, State &state) {
	if (GetAsyncKeyState('A')) {
		human.move_left(state);
	}
	else
		if (GetAsyncKeyState('D')) {
			human.move_right(state);
		}
		else
			if (GetAsyncKeyState('W')) {
				human.move_up(state);
			}
			else
				if (GetAsyncKeyState('S')) {
					human.move_down(state);
				}
				else
					if (GetAsyncKeyState('B')) {
						human.move_bomb(state);
					}
					else {
						state.decrease_zones();
						state.decrease_bombs();
						state.evaluate_bombs();
						state.evaluate_zones();
						state.current_turn++;
					}
}
void actions_bot(SecondPlayer &bot, State &state) {
	if (GetAsyncKeyState(VK_LEFT)) {
		bot.move_left(state);
	}
	else
		if (GetAsyncKeyState(VK_RIGHT)) {
			bot.move_right(state);
		}
		else
			if (GetAsyncKeyState(VK_UP)) {
				bot.move_up(state);
			}
			else
				if (GetAsyncKeyState(VK_DOWN)) {
					bot.move_down(state);
				}
				else
					if (GetAsyncKeyState('M')) {
						bot.move_bomb(state);
					}
					else {
						state.current_turn++;
						state.evaluate_bombs();
						state.evaluate_zones();
						state.decrease_zones();
						state.decrease_bombs();
					}
}
void game() {
	srand(time(NULL));
	FirstPlayer human;
	SecondPlayer bot;
	State state;
	BattleModel<SecondPlayer, FirstPlayer, State, FirstAction, FirstAction> botbot;
	BattleModel<FirstPlayer, SecondPlayer, State, FirstAction, FirstAction> robot;
	robot.assign_params(8, "lamar");
	botbot.assign_params(11, "boraichu");
	//BattleModel<SecondPlayer, FirstPlayer, State, FirstAction, FirstAction> bot_o;
	while (true) {
		//actions_player(human, state);
		FirstAction actione = robot.get_gradual_best_action(state);
		human.execute_action(actione, state);
		state.return_final_state();
		if (state.is_first_player_dead && state.is_second_player_dead)
		{
			cout << "Both players died";
			Sleep(9000);
			exit(0);
		}
		if (state.is_first_player_dead)
		{
			cout << "Player loosed";
			Sleep(9000);
			exit(0);
		}
		if (state.is_second_player_dead)
		{
			cout << "Bot loosed";
			Sleep(9000);
			exit(0);
		}
		//actions_bot(bot, state);
		FirstAction action = botbot.get_gradual_best_action(state);
		//cout << "\n\n" << botbot.get_best_score() << "\n";
		bot.execute_action(action, state);
		if (state.is_first_player_dead && state.is_second_player_dead)
		{
			cout << "Both players died";
			Sleep(9000);
			exit(0);
		}
		if (state.is_first_player_dead)
		{
			cout << "Player loosed";
			Sleep(9000);
			exit(0);
		}
		if (state.is_second_player_dead)
		{
			cout << "Bot loosed";
			Sleep(9000);
			exit(0);
		}
		system("cls");
		state.show();
		//cout << action.stre() << " " << bot.y << " " << bot.x << "\n";
		//system("pause");
		//exit(0);
		Sleep(200);
	}
}

int main() {
	//BattleModel<FirstPlayer, SecondPlayer, State, FirstAction, FirstAction>::tournament_learning(1000, 100, 2000, "FirstBots//", "SecondBots//", 7, 6, 42000000, false);
	game();
}
