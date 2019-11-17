import os
import sys
from subprocess import Popen
import time

args_dict = {}
for arg in sys.argv:
    args_dict[arg] = 1

programs_to_run = {"5x5": "tictac5x5//main_five.cpp", "connect4": "connect4//main_connect_four.cpp", "3x3": "tictac3x3//main_three.cpp", "7x7": "tictac7x7//main_seven.cpp"}

default_values = {"game": "5x5", "command": "play", "bot_name": "aqpyiu", "mode": "hybrid"}

permitted_commands = ["game", "command", "bot_name", "mode"]

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

def check_param_for_frontend():
    if "3x3" in args_dict:
        return "3x3"
    if "5x5" in args_dict:
        return "5x5"
    if "7x7" in args_dict:
        return "7x7"
    if "connect4" in args_dict:
        return "connect4"
    return None
os.system("g++ Components//NeuralNetwork.cpp Components//MathMatrix.cpp Components//Utils.cpp "  + programs_to_run[get_args(sys.argv, "game", permitted_commands)] + " -o game -O9 -lwsock32")
print("Source compiled!")
if get_args(sys.argv, "command", permitted_commands) == "play":
    p = Popen(['game.exe', get_args(sys.argv, "bot_name", permitted_commands), get_args(sys.argv, "mode", permitted_commands)])
    print("Source started!")
    time.sleep(4)
    os.system("python frontend.py " + get_args(sys.argv, "game", permitted_commands) + " " + get_args(sys.argv, "mode", permitted_commands))
elif get_args(sys.argv, "command", permitted_commands) == "train":
    os.system("game.exe " + get_args(sys.argv, "command", permitted_commands))
