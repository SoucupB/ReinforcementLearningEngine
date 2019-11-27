import os
import sys
from subprocess import Popen
import time

args_dict = {}
for arg in sys.argv:
    args_dict[arg] = 1

programs_to_run = {"5x5": "tictac5x5//main_five.cpp", "connect4": "connect4//main_connect_four.cpp", "3x3": "tictac3x3//main_three.cpp", "7x7": "tictac7x7//main_seven.cpp", "10x10": "tictac10x10//main_ten.cpp"}

programs_to_compile = {"5x5": "tictac5x5//Tictactoe5x5.cpp", "connect4": "connect4//Connect4.cpp", "3x3": "tictac3x3//Tictactoe3x3.cpp", "7x7": "tictac7x7//Tictactoe7x7.cpp", "10x10": "tictac10x10//Tictactoe10x10.cpp"}

default_values = {"game": "5x5", "command": "play", "bot_name": "aqpyiu", "mode": "hybrid", "program": "game", "player": "first"}

permitted_commands = ["game", "command", "bot_name", "mode", "program", "player"]

def get_args(args, command, permitted_commands):
    param = ""
    if command not in permitted_commands:
        print("This argument is invalid for command " + command)
        exit()
    for arg in args:
        if command in arg:
            find_equal = "="
            index = arg.find('=')
            if index == -1:
                return None
            index += 1
            while index < len(arg):
                param += arg[index]
                index += 1
            break
    if param == "":
        return default_values[command]
    return param
os.system("g++ " + programs_to_compile[get_args(sys.argv, "game", permitted_commands)] + " Components//NeuralNetwork.cpp Components//MathMatrix.cpp Components//Utils.cpp "  + programs_to_run[get_args(sys.argv, "game", permitted_commands)] + " -o " + get_args(sys.argv, "program", permitted_commands) + " -O9 -lwsock32")
print("Source compiled!")
if get_args(sys.argv, "command", permitted_commands) == "play":
    p = Popen([get_args(sys.argv, "program", permitted_commands) + '.exe', get_args(sys.argv, "bot_name", permitted_commands), get_args(sys.argv, "mode", permitted_commands), get_args(sys.argv, "player", permitted_commands)])
    print("Source started!")
    print(get_args(sys.argv, "program", permitted_commands) + ".exe " + get_args(sys.argv, "command", permitted_commands) + " " + get_args(sys.argv, "mode", permitted_commands) + " " + get_args(sys.argv, "player", permitted_commands))
    time.sleep(4)
    os.system("python frontend.py " + get_args(sys.argv, "game", permitted_commands) + " " + get_args(sys.argv, "player", permitted_commands))
elif get_args(sys.argv, "command", permitted_commands) == "train":
    os.system(get_args(sys.argv, "program", permitted_commands) + ".exe " + get_args(sys.argv, "command", permitted_commands) + " " + get_args(sys.argv, "mode", permitted_commands) + " " + get_args(sys.argv, "player", permitted_commands))