# Reinforcement Learning Engine
Reinforcement Learning Engine for different state-action board games.
This is an engine which is made with the purpose of creating and testing bots that can take human-like decisions in order to acomplish their goal.
Wether its winning a game or solving a problem, this bots can create strong and sneaky strategies if enough training is provided, however to achieve perfection in case of some games is computationally impossible with my actual tools.
# Getting Started
In order to make this project run, there are 3 dependencies that need to be installed first.
    -g++ > 8.1.0 (Can be aquired from this link https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/seh/)
    -python > 3.5 (Can be aquired from this link https://www.python.org/downloads/release/python-354/ This python version was used to build the project's front-end part)
    -PIL (This is a python graphic library's. Which can be aquired by adding command "pip install Pillow" in any console after the python installation)
gcc and python need to be added to enviroment variables.
# Testing
There are currently 3 games implemented in this repository which can be tested.

# TicTacToe 3x3
The hello world for everyone who wants to start developing game AI's, this is a classic game for testing and developing different algorithms such as searching or learning game-states.
The command for testing the game is

python start.py game=3x3 command=play bot_name=akmqag mode=hybrid

(The commands will be explained later in this post)

# TicTacToe 5x5
A slightly harder version of its classic 3x3 counterpart, this game is played on a 5 by 5 map where each player have the goal to make 4 pieces in line or diagonal

The command for testing the game is
python start.py game=5x5 command=play bot_name=aqpyiu mode=hybrid

# Connect 4
This is a game where developing an AI is significantly harder, since an early wrong move will cost you the game if the enemy can take advantege of it.
A simple alpha-beta minmax search is not gonna be very good in this situation here.
More details here!
https://mindyourdecisions.com/blog/2013/08/20/how-to-win-at-connect-four-every-time-a-game-solver-you-can-use-from-your-smartphone-or-computer/

python start.py game=connect4 command=play bot_name=jdafmq mode=hybrid

# TicTacToe 7x7
Has not been trained yet, however it will update soon. For now its still available to play it (make five in a row column or diagonal).
python start.py game=7x7 command=play

# Options
    -command: "play, train" (this command starts the learning in 2 specific folders where the engine is putting its agents when they will be created, 1 folder is for the first player and the second for the second player, so far the learning has hardcoded parameters, however when the tutorial is released it will be editable from params or in a cfg file) The folders where the engine its putting its engine are in the "cfg" file
    -game: "3x3, 5x5, connect4, 7x7" this are the games available right now from this command
    -bot_name: This command is the name of the bot that will be loaded when the game starts, the bot name is also the file name, they can be found on ExampleBots
    -mode: "hybrid, neural" this are 2 mods for the bot to play, neural means that the bot loads only the neural network file (the file which finish with .nn) in order to generate its heuristics and hybrid means that it loads the neural_network file and the hash_table which is a transposition table for the same reason (more details in the upcoming tutorial).

# Tutorial
There will be an available tutorial in the near future that will explain the current learning methods used and the overall engine and how it works.

# Compatibilty Systems
Windows 7,8,8.1,10

# Personal Note
This engine is used for educational purposes and to test and train different agents with different methods. Right now all agents from every game are trained with a variation of deep-q-nets and negamax-algorithm.
Feedback is greatly appreciated.
My goal with this engine is to extend it and apply it on a variaty of games to train on and create bots that will be a worthy opponent for everyone who wants a challange.
(Right now its supports only perfect-information no random chance board games with 2 opponents)

# Contributions
If you encoutered some bugs and want to fix them feel free to make a pull request with a specification of the fix or mention it in the comments.
Every new ideea or game that you want to add to use with this engine are welcome to be added to the repository!

# Notes
Right now on all the games the human player starts.
More stronger trained bots for all the games are coming soon.
More responsive UI and win/lose message will be implemented.