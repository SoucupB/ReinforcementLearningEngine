#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include "NeuralNetwork.h"
#include <map>
#include <windows.h>
#include "Utils.h"

template<class Player_one, class Player_two, class State, class Action_one, class Action_two> class BattleModel {
    private:
        Player_one self;
        Player_two enemy;
        Action_one best_action_one;
        State change_state;
        int maxDepth;
        int best_score;
        std::string name, name_without_prefix;
        float lr = 0.2;
		bool activete_zovrist = false;
        bool is_training = false;
        int random_percent_move = 2;
        bool user_neural_nets = false;
        NeuralNetwork net;
        bool hybrid_learn = false;
        bool normal_learning = true;
        int max_quantity = 10;
        double constant = 0.2;
        int wins = 0, loses = 0;
        int state_size = change_state.state_to_vec_hash().size();
        bool lock_random_chance = false;
        double elo = 1000;
        double discont = 0.98;
        bool prototype_search = false;
        //To add efficient full save states method

        std::vector<int> neural_structure, hidden_structure;

        std::unordered_map<long long, int> zovrist_keys;

        std::unordered_map<long long, int> counter_keys;

        std::unordered_map<long long, float> exp_keys, exp_keys_mine, exp_keys_enemy;

        std::unordered_map<long long, float> copys_exp_keys;

        std::unordered_map<long long, int> counter_keys_copy;

        std::vector<long long> zovrist_keys_vector;

        std::vector< std::vector<double> > states_game;

        std::unordered_map< long long, int > win_loose_states;

        std::vector< long long > states;

        std::vector<BattleModel> council_bots;

        int maxim(int a, int b) {
            return a > b ? a : b;
        }

        int mininm(int a, int b) {
            return a < b ? a : b;
        }

        void sort_by_exp(State state, std::vector<Action_one> &all_actions) {
            std::vector< std::pair<float, int> > actions;
            for(int i = 0; i < all_actions.size(); i++) {
                State next_state = state;
                if(normal_learning) {
                    this->self.execute_action(all_actions[i], next_state);
                    long long hash = get_hash(next_state);
                    if(this->exp_keys.find(hash) != this->exp_keys.end()) {
                        actions.push_back(std::make_pair(this->exp_keys[hash], i));
                    }
                    else {
                        actions.push_back(std::make_pair(0, i));
                    }
                }
                if(user_neural_nets){
                    if(!prototype_search) {
                        this->self.execute_action(all_actions[i], next_state);
                        actions.push_back(std::make_pair(get_inverse_normalisation(get_neural_response(next_state.state_to_vec_neural())), i));
                    }
                    else {
                        this->self.execute_action(all_actions[i], next_state);
                        actions.push_back(std::make_pair(get_inverse_normalisation(get_median_council_response(next_state.state_to_vec_neural())), i));
                    }
                }
                if(hybrid_learn) {
                    this->self.execute_action(all_actions[i], next_state);
                    long long hash = get_hash(next_state);
                    if(this->exp_keys.find(hash) != this->exp_keys.end()) {
                        actions.push_back(std::make_pair(this->exp_keys[hash], i));
                    }
                    else {
                        if(!prototype_search) {
                            double response = get_inverse_normalisation(get_neural_response(next_state.state_to_vec_neural()));
                            actions.push_back(std::make_pair(response, i));
                            this->exp_keys[hash] = response;
                        }
                        else {
                            double response = get_inverse_normalisation(get_median_council_response(next_state.state_to_vec_neural()));
                            actions.push_back(std::make_pair(response, i));
                            this->exp_keys[hash] = response;
                        }
                    }
                }
            }
            std::sort(actions.begin(), actions.end());
            std::reverse(actions.begin(), actions.end());
            std::vector<Action_one> all_actions_std;
            for(int i = 0; i < actions.size(); i++) {
                all_actions_std.push_back(all_actions[actions[i].second]);
               // std::cout << std::fixed << std::setprecision(10) << actions[i].first << " " << all_actions[actions[i].second].stre() << "\n";
            }
           // exit(0);
            all_actions = all_actions_std;
        }

        bool get_chance() {
            if(rand() % 101 <= random_percent_move) {
                return true;
            }
            return false;
        }

        Action_one random_action(State state) {
            std::vector<Action_one> all_actions = this->self.get_all_actions(state);
            return all_actions[(rand()) % all_actions.size()];
        }

        double get_neural_response(std::vector<char> char_state) {
            std::vector<double> doubleVec(char_state.begin(), char_state.end());
            return net.feed(doubleVec)[0];
        }

        double get_median_council_response(std::vector<char> char_state) {
            std::vector<double> doubleVec(char_state.begin(), char_state.end());
            double median_value = 0.0;
            for(int i = 0; i < council_bots.size(); i++) {
                double current_bot_estimation = council_bots[i].get_neural_response_for_training(doubleVec);
                median_value += current_bot_estimation;
            }
            return median_value / (double)council_bots.size();
        }

        double get_neural_response_for_training(std::vector<double> double_state) {
            return net.feed(double_state)[0];
        }

        int bot_action_first(State state, int depth, int alpha, int beta) {
            int gema_rez = this->self.get_final_evaluation_board(state);
            bool are_states_visited = false;
            if(gema_rez)
                return gema_rez;
            long long hash = get_hash(state);
            if(activete_zovrist && depth && this->zovrist_keys.find(hash) != this->zovrist_keys.end()) {
                return this->zovrist_keys[hash];
            }
            int max_score = -1<<30;
            if(depth == this->maxDepth)
                return gema_rez;
            Action_one current_action;
            std::vector<Action_one> all_actions = this->self.get_all_actions(state);

            if(is_training) {
                std::random_shuffle(all_actions.begin(), all_actions.end());
                sort_by_exp(state, all_actions);
            }
            else
                std::random_shuffle(all_actions.begin(), all_actions.end());

            for(int i = 0; i < all_actions.size(); i++) {
                State next_state = state;
                this->self.execute_action(all_actions[i], next_state);
                int score = -bot_action_second(next_state, depth + 1, -beta, -alpha);
               // state = previous_state;
                if(abs(score) == self.get_draw_score()) {
                    score = abs(score);
                }
                if(score > max_score) {
                    max_score = score;
                    current_action = all_actions[i];
                }
                alpha = maxim(alpha, score);
                if(alpha >= beta && depth) {
                    return alpha;
                }
            }
            if(depth == 0) {
                this->best_action_one = current_action;
                this->best_score = max_score;
            }
            if(activete_zovrist && max_score)
                zovrist_keys[hash] = max_score;
            return max_score;
        }
        int bot_action_second(State state, int depth, int alpha, int beta) {
            int gema_rez = this->enemy.get_final_evaluation_board(state);
            if(gema_rez)
                return gema_rez;
            long long hash = get_hash(state);
            if(activete_zovrist && this->zovrist_keys.find(hash) != this->zovrist_keys.end())
                return this->zovrist_keys[hash];
            if(depth == this->maxDepth)
                return gema_rez;
            int max_score = -1<<30;
            std::vector<Action_two> all_actions = this->enemy.get_all_actions(state);

            std::random_shuffle(all_actions.begin(), all_actions.end());

            for(int i = 0; i < all_actions.size(); i++) {
                State next_state = state;
                this->enemy.execute_action(all_actions[i], next_state);
                int score = -bot_action_first(next_state, depth + 1, -beta, -alpha);
                //state = previous_state;
                if(abs(score) == enemy.get_draw_score()) {
                    score = abs(score);
                }
                if(score > max_score) {
                    max_score = score;
                }
                alpha = maxim(alpha, score);
                if(alpha >= beta) {
                    return alpha;
                }
            }
            if(activete_zovrist && max_score)
                zovrist_keys[hash] = max_score;
            return max_score;
        }
    public:
        void assign_params(int maxDepth, std::string name, std::vector<int> neural_config, double lr) {
            this->generate_zovrist();
            this->activate_training();
            this->maxDepth = maxDepth;
            this->name = name;
            if(user_neural_nets || hybrid_learn) {
                int size_state = change_state.state_to_vec_neural().size();
                this->neural_structure = neural_config;
                this->hidden_structure = neural_config;
                this->neural_structure.insert(this->neural_structure.begin(), size_state);
                this->neural_structure.push_back(1);
                this->lr = lr;
                net.assign_params(this->neural_structure, "sigmoid", 4, "usebias", "meanSquaredError", lr);
            }
        }

        void set_timer(int timer) {

        }

        std::vector<int> get_hidden_structure() {
            return hidden_structure;
        }

        void get_nn() {

        }

        double get_elo() {
            return this->elo;
        }

        void modify_elo(double enemy_elo, double stats) {
            double r1 = pow(10, this->elo / 400), r2 = pow(10, enemy_elo / 400);
            double e1 = r1 / (r1 + r2), e2 = r2 / (r1 + r2);
            this->elo += 32.0 * (stats - e1);
        }

        void activate_nn() {
            user_neural_nets = true;
            normal_learning = false;
        }

        double normalize_reward(double reward) {
            return ((double)reward + fabs((double)self.get_min_score())) / ((double)self.get_max_score() + fabs((double)self.get_min_score()));
        }

        void generate_zovrist() {
            if(!zovrist_keys_vector.size()) {
                int state_size = change_state.state_to_vec_hash().size();
                for(int i = 0; i < state_size; i++) {
                    long long random_number = 1;
                    while(random_number < (1LL<<57)) {
                        random_number *= (long long)rand();
                    }
                    if(!(random_number % 2)) {
                        random_number++;
                    }
                    zovrist_keys_vector.push_back(random_number);
                }
            }
        }

        long long get_hash(State &state) {
            std::vector<char> char_state = state.state_to_vec_hash();
            long long atr = 0;
            for(int i = 0; i < zovrist_keys_vector.size(); i++) {
                atr ^= ((long long)char_state[i] * zovrist_keys_vector[i]);
            }
            return atr;
        }

        int get_best_score() {
            return best_score;
        }

        void hybrid_mode() {
            this->hybrid_learn = true;
            this->normal_learning = false;
            this->user_neural_nets = false;
        }

        void activate_hybrid_player(int max_quantity) {
            this->hybrid_learn = true;
            this->normal_learning = false;
            this->user_neural_nets = false;
            this->max_quantity = max_quantity;
        }

        void save_bot() {
            if(normal_learning || hybrid_learn) {
                std::ofstream writer;
                writer.open(name + ".mm", std::ios::out | std::ios::binary);
                delete_unecessary_keys();
                double current_elo = this->elo;
                writer.write ((char*)&current_elo, sizeof (current_elo));
                int zovrist_size = zovrist_keys_vector.size();
                writer.write ((char*)&zovrist_size, sizeof (zovrist_size));
                for(int i = 0; i < zovrist_size; i++) {
                    long long key = zovrist_keys_vector[i];
                    writer.write ((char*)&key, sizeof (key));
                }
                long long exp_size = exp_keys.size();
                writer.write ((char*)&exp_size, sizeof (exp_size));
                for(auto i = exp_keys.begin(); i != exp_keys.end(); i++) {
                    long long key = i->first;
                    float value = i->second;
                    writer.write ((char*)&key, sizeof (key));
                    writer.write ((char*)&value, sizeof (value));
                }
                writer.close();
                std::cout << "\nFile writted!";
            }
            if(user_neural_nets || hybrid_learn){
                net.saveModel((name + ".nn").c_str());
                std::cout << "\nNeural file writted!";
            }
        }

        void get_bot() {
            if(normal_learning || hybrid_learn) {
                std::ifstream reader;
                reader.open(name + ".mm", std::ios::in | std::ios::binary);
                if(!reader.is_open()) {
                    std::cout << "The file with name " + name + " does not exist!\n";
                    return ;
                }
                int zovrist_size;
                zovrist_keys_vector.clear();
                double current_elo;
                reader.read ((char*)&current_elo, sizeof (current_elo));
                this->elo = current_elo;
                std::cout << "\nThis bot got elo " << this->elo << "\n";
                reader.read ((char*)&zovrist_size, sizeof (zovrist_size));
                for(int i = 0; i < zovrist_size; i++) {
                    long long key;
                    reader.read ((char*)&key, sizeof (key));
                    zovrist_keys_vector.push_back(key);
                }
                long long exp_size;
                reader.read ((char*)&exp_size, sizeof (exp_size));
                for(auto i = 0; i < exp_size; i++) {
                    long long key;
                    float value;
                    reader.read ((char*)&key, sizeof (key));
                    reader.read ((char*)&value, sizeof (value));
                    exp_keys[key] = value;
                }
                reader.close();
                std::cout << "\nFile readed!\n";
            }
            if(user_neural_nets || hybrid_learn) {
                this->net.loadModel((name + ".nn").c_str());
                std::cout << "\nNeural file readed!\n";
            }
        }

        void save_state(State state, bool save_complete_info, bool player) {
            if(normal_learning)
                states.push_back(get_hash(state));
            if(user_neural_nets && save_complete_info){
                std::vector<char> state_c = state.state_to_vec_neural();
                states_game.push_back(std::vector<double>(state_c.begin(), state_c.end()) );
            }
            if(hybrid_learn && save_complete_info){
                std::vector<char> state_c = state.state_to_vec_neural();
                states_game.push_back(std::vector<double>(state_c.begin(), state_c.end()) );
                states.push_back(get_hash(state) );
            }
        }

        double adaptive_lr(double x) {
            return 1.0 / log(fmax(2, fabs(x))) + constant;
        }

        void adapt_random_quest() {
            if(lock_random_chance) {
                long long diff = maxim(5, this->loses - this->wins);
                this->random_percent_move = int(log2(diff));
            }
        }

        static int bot_match(BattleModel<Player_one, Player_two, State, Action_one, Action_two> &first,
                             BattleModel<Player_two, Player_one, State, Action_two, Action_one> &second, bool full_info) {
            State state;
            Player_one first_player;
            Player_two second_player;
            while(true) {
                Action_one action_one = first.get_best_action(state);
                first_player.execute_action(action_one, state);
                first.save_state(state, 1, true);
                second.save_state(state, full_info, false);
                if(first_player.get_final_evaluation_board(state) == first_player.get_max_score()) {
                    return 1;
                }
                if(abs(first_player.get_final_evaluation_board(state)) == first_player.get_draw_score()) {
                    return 0;
                }
                Action_two action_two = second.get_best_action(state);
                second_player.execute_action(action_two, state);
                second.save_state(state, 1, true);
                first.save_state(state, full_info, false);
                if(second_player.get_final_evaluation_board(state) == second_player.get_max_score())
                    return 2;
                if(abs(second_player.get_final_evaluation_board(state)) == second_player.get_draw_score()) {
                    return 0;
                }
            }
            return -1;
        }

        void assign_neural_net(std::vector<int> config) {
            config.insert(config.begin(), state_size);
            config.push_back(1);
            this->net.assign_params(config, "sigmoid", 4, "usebias", "meanSquaredError", 0.09);
        }

        void train(int result) {
            if(normal_learning) {
                float last_state = (float)result, index = 0;
                exp_keys[states.back()] = last_state;
                for(int i = states.size() - 2; i >= 0; i--) {
                    exp_keys[states[i]] += pow(discont, index++)
                                            * lr * (exp_keys[states[i + 1]] - exp_keys[states[i]]);
                    counter_keys[states[i]]++;
                }
                states.clear();
            }
            if(user_neural_nets){
                float last_state = (float)result;
                int repeats = 3;
                double normalized_rez = normalize_reward(last_state);
                for(int j = 0; j < repeats; j++) {
                    net.sgd(states_game.back(), {normalize_reward(last_state)});
                    for(int i = states_game.size() - 2; i >= 0; i--) {
                        double nn_result = normalized_rez;
                        net.sgd(states_game[i], {nn_result});
                    }
                }
                states_game.clear();
            }
            if(hybrid_learn) {
                float last_state = (float)result, index = 0;
                exp_keys[states.back()] = last_state;
                double last_value = normalize_reward(last_state);
                int repeats = 1;
                for(int i = states.size() - 2; i >= 0; i--) {
                    exp_keys[states[i]] += pow(discont, index++) * lr * (exp_keys[states[i + 1]] - exp_keys[states[i]]);
                    counter_keys[states[i]]++;
                }
                for(int j = 0; j < repeats; j++) {
                    for(int i = states.size() - 1; i >= 0; i--) {
                        double state_reward = normalize_reward(exp_keys[states[i]]);
                        net.sgd(states_game[i], {state_reward});
                    }
                }
                states_game.clear();
                states.clear();
            }
        }

        void set_constant(double constant) {
            this->constant = constant;
        }

        float get_inverse_normalisation(double value) {
            return value * (float)(self.get_max_score() + fabs((float)self.get_min_score())) + (float)self.get_min_score();
        }

        int get_score_from_index_one(int id)
        {
            if(id == 1)
                return self.get_max_score();
            if(id == 2)
                return self.get_min_score();
            if(id == 0)
                return abs(self.get_draw_score());
            return -1;
        }

        int get_score_from_index_two(int id)
        {
            if(id == 2)
                return self.get_max_score();
            if(id == 1)
                return self.get_min_score();
            if(id == 0)
                return abs(self.get_draw_score());
            return -1;
        }

        void activate_training() {
            is_training = true;
        }

        void reset_bot() {
            this->exp_keys.clear();
            this->zovrist_keys.clear();
            this->counter_keys.clear();
        }
        void set_lr(float lr) {
            this->lr = lr;
        }

        void delete_unecessary_keys() {
            std::cout << "Before deletion there are " << exp_keys.size() << "\n";
            for(auto i = exp_keys.begin(); i != exp_keys.end(); i++) {
                if(counter_keys[i->first] > 2) {
                    copys_exp_keys[i->first] = i->second;
                    counter_keys_copy[i->first] = counter_keys[i->first];
                }
            }
            exp_keys.clear();
            exp_keys = copys_exp_keys;
            copys_exp_keys.clear();
            counter_keys = counter_keys_copy;
            counter_keys_copy.clear();
            std::cout << "After deletion there are " << exp_keys.size() << "\n";
        }

        void make_exp_keys_undeletable() {
             for(auto i = exp_keys.begin(); i != exp_keys.end(); i++) {
                 counter_keys[i->first] = 3;
             }
        }

        bool is_exp_overloaded() {
            return exp_keys.size() >= max_quantity;
        }

        void reset_exp() {
            delete_unecessary_keys();
        }

        void set_random_chance(int number) {
            this->random_percent_move = number;
        }

        void add_win() {
            this->wins++;
        }

        void add_lose() {
            this->loses++;
        }

        void reset_stats() {
            this->wins = 0;
            this->loses = 0;
        }

        double get_net_raw_response(std::vector<double> raw_input) {
            return net.feed(raw_input)[0];
        }

        void generate_name(int length) {
            this->name = "";
            for(int i = 0; i < length; i++) {
                this->name += (rand() * rand()) % 26 + 97;
            }
        }

        void add_prefix(std::string prefix) {
            this->name_without_prefix = name;
            this->name = prefix + this->name;
        }

        std::string get_name() {
            return this->name_without_prefix;
        }

        void assign_name(std::string prefix, std::string name) {
            this->name = prefix + name;
            this->name_without_prefix = name;
        }

        std::string clean_entry(std::string file) {
            std::string new_str = "";
            int index = 0;
            while(index < file.size() && (file[index] >= 'a' && file[index] <= 'z')) {
                new_str += file[index];
                index++;
            }
            return new_str;
        }

        void activate_prototype_search(std::string folder_path) {
            std::vector<std::string> vector_content_for_player = Utils::get_folder_content(folder_path);
            for(int i = 0; i < vector_content_for_player.size(); i++) {
                BattleModel bot;
                bot.activate_nn();
                bot.assign_params(this->maxDepth, "", this->get_hidden_structure(), 0.11);
                bot.assign_name(folder_path, vector_content_for_player[i]);
                std::cout << bot.get_name() << "\n";
                bot.get_bot();
                council_bots.push_back(bot);
            }
            prototype_search = true;
        }

        std::string get_full_name() {
            return this->name;
        }

        static void tournament_learning(int matches, int batches, int delete_after, std::string folder_first, std::string folder_second, int number_of_bots,
                                        int name_length, int max_states, bool reload, std::vector<int> neural_config, double lr, bool load_from_file, double rl_lr, bool full_info) {
            BattleModel<Player_one, Player_two, State, Action_one, Action_two> first_bot[number_of_bots];
            BattleModel<Player_two, Player_one, State, Action_two, Action_one> second_bot[number_of_bots];
            std::vector<std::string> vector_content_first_player, vector_content_second_player;
            if(load_from_file) {
                vector_content_first_player = Utils::get_folder_content(folder_first);
                vector_content_second_player = Utils::get_folder_content(folder_second);
            }
            for(int i = 0; i < number_of_bots; i++) {
                first_bot[i].activate_hybrid_player(max_states);
                second_bot[i].activate_hybrid_player(max_states);

                second_bot[i].set_lr(rl_lr);
                second_bot[i].set_constant(0.09);
                first_bot[i].set_lr(rl_lr);
                first_bot[i].set_constant(0.09);

                first_bot[i].assign_params(1, "af", neural_config, lr);
                second_bot[i].assign_params(1, "f", neural_config, lr);

                first_bot[i].set_random_chance(95);
                second_bot[i].set_random_chance(95);

                first_bot[i].generate_name(name_length);
                first_bot[i].add_prefix(folder_first);
                second_bot[i].generate_name(name_length);
                second_bot[i].add_prefix(folder_second);

                first_bot[i].activate_training();
                second_bot[i].activate_training();
            }
            if(load_from_file) {
                for(int i = 0; i < number_of_bots; i++) {
                    first_bot[i].set_random_chance(1);
                    second_bot[i].set_random_chance(1);
                    first_bot[i].assign_name(folder_first, vector_content_first_player[i]);
                    second_bot[i].assign_name(folder_second, vector_content_second_player[i]);
                    first_bot[i].get_bot();
                    second_bot[i].get_bot();
                    first_bot[i].make_exp_keys_undeletable();
                    second_bot[i].make_exp_keys_undeletable();
                    std::cout << "Loaded bot first " + first_bot[i].get_name() + " and bot second " + second_bot[i].get_name() << '\n';
                }
            }
            int O = 0, X = 0, draw = 0, index = 0, chapper_index = 0, first_index, second_index;
            std::vector<int> first_vector_index, second_vector_index;
            first_vector_index = first_bot[chapper_index].get_random_array(number_of_bots);
            second_vector_index = second_bot[chapper_index].get_random_array(number_of_bots);
            while(batches--) {
                if(++index % delete_after == 0) {
                    for(int i = 0; i < number_of_bots; i++) {
                        first_bot[i].save_bot();
                        second_bot[i].save_bot();
                    }
                }
                int cpy_matches = matches;
                first_index = first_vector_index[chapper_index];
                second_index = second_vector_index[chapper_index];
                if((chapper_index + 1) % number_of_bots == 0) {
                    first_vector_index = first_bot[chapper_index].get_random_array(number_of_bots);
                    second_vector_index = second_bot[chapper_index].get_random_array(number_of_bots);
                    chapper_index = 0;
                }
                else
                    chapper_index++;
                while(cpy_matches--) {
                    int result_id = bot_match(first_bot[first_index], second_bot[second_index], full_info);
                    int result_for_one = first_bot[first_index].get_score_from_index_one(result_id);
                    int result_for_two = second_bot[second_index].get_score_from_index_two(result_id);
                    if(!result_id)
                       result_for_one = -result_for_one;
                    first_bot[first_index].train(result_for_one);
                    second_bot[second_index].train(result_for_two);
                    first_bot[first_index].decrease_hazard(2, 1);
                    second_bot[second_index].decrease_hazard(2, 1);
                    if(result_id == 0) {
                        draw++;
                        first_bot[first_index].modify_elo(second_bot[second_index].get_elo(), 0.5);
                        second_bot[second_index].modify_elo(first_bot[first_index].get_elo(), 0.5);
                    }
                    if(result_id == 1) {
                        X++;
                        first_bot[first_index].add_win();
                        second_bot[second_index].add_lose();
                        first_bot[first_index].modify_elo(second_bot[second_index].get_elo(), 1.0);
                        second_bot[second_index].modify_elo(first_bot[first_index].get_elo(), 0.0);
                    }
                    if(result_id == 2) {
                        O++;
                        first_bot[first_index].add_lose();
                        second_bot[second_index].add_win();
                        first_bot[first_index].modify_elo(second_bot[second_index].get_elo(), 0.0);
                        second_bot[second_index].modify_elo(first_bot[first_index].get_elo(), 1.0);
                    }
                }
                if(X)
                    std::cout << "Rating for batch: " << batches << " bot: " + first_bot[first_index].get_name() + " vs bot: " +
                              second_bot[second_index].get_name() << " " << float(O + draw) / (float)X << " " << X << " " << O << " " << draw << "\n";
                X = 0;
                O = 0;
                draw = 0;
            }
            for(int i = 0; i < number_of_bots; i++) {
                first_bot[i].save_bot();
                second_bot[i].save_bot();
            }
        }

        std::vector<int> get_random_array(int total_size) {
            std::vector<int> vect;
            for(int i = 0; i < total_size; i++)
                vect.push_back(i);
            std::random_shuffle(vect.begin(), vect.end());
            return vect;
        }

        void lock_chance() {
            this->lock_random_chance = true;
        }

        void unlock_chance() {
            this->lock_random_chance = false;
        }

        void decrease_hazard(int by, int target) {
            if(this->random_percent_move - by <= target) {
                this->random_percent_move = target;
                this->lock_chance();
            }
            else {
                this->random_percent_move -= by;
            }
        }

        void activate_zovrist() {
            this->zovrist_keys = true;
        }

        void reset_all() {
            this->exp_keys.clear();
            this->counter_keys.clear();
            this->counter_keys_copy.clear();
            if(hybrid_learn || user_neural_nets) {
                net.assign_params(neural_structure, "sigmoid", 4, "usebias", "meanSquaredError", 0.009);
            }
        }

        Action_one get_best_action(State state) {
            if(get_chance())
                return this->random_action(state);
            else
                this->bot_action_first(state, 0, -1<<30, 1<<30);
            return this->best_action_one;
        }

        Action_one get_gradual_best_action(State state) {
            this->bot_action_first(state, 0, -1<<30, 1<<30);
            return this->best_action_one;
        }
};