
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include <iostream>
#include "..//Components//BattleModel.h"
#include <ctime>
#include <vector>
#include <string>
#include "connect4.h"
#include "..//Components//Utils.h"
#include <fstream>
#include <map>


using namespace std;

void server(string file_name, string path, string mode) {
    WSADATA WSAData;

    SOCKET server, client;

    SOCKADDR_IN serverAddr, clientAddr;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    cout << "Listening for incoming connections..." << endl;

    char buffer_recv[1024] = {0}, buffer_send[1024] = {0};
    FirstPlayer ionica;
    SecondPlayer vasile;
    State state;
    BattleModel<SecondPlayer, FirstPlayer, State, FirstAction, FirstAction> bot_o;
    if(mode == "hybrid")
      bot_o.hybrid_mode();
    else if(mode == "neural")
      bot_o.activate_nn();
    bot_o.assign_params(9, path + file_name, {46, 52, 46}, 0.2);
    bot_o.get_bot();
    //bot_o.activate_prototype_search("connect4//SecondBots//");
    int clientAddrSize = sizeof(clientAddr);
    if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
    {
      cout << "Client connected!" << endl;
      while(true) {
        int flag = recv(client, buffer_recv, sizeof(buffer_recv), 0);
        if(flag != -1) {
          FirstAction serv_action;
          serv_action.x = buffer_recv[0] - 48;
          serv_action.y = buffer_recv[1] - 48;
          serv_action.id = 1;
          ionica.execute_action(serv_action, state);
          if(ionica.get_final_evaluation_board(state))
          {
            send(client, buffer_send, sizeof(buffer_send), 0);
            state.show();
            return ;
          }
          FirstAction action = bot_o.get_gradual_best_action(state);
          char xx = (char)action.x;
          char yy = (char)action.y;
          buffer_send[0] = xx;
          buffer_send[1] = yy;
          vasile.execute_action(action, state);
          if(vasile.get_final_evaluation_board(state))
          {
            send(client, buffer_send, sizeof(buffer_send), 0);
            state.show();
            return ;
          }
          state.show();
          cout << action.stre() << " Score " << bot_o.get_best_score() << "\n";
          send(client, buffer_send, sizeof(buffer_send), 0);
        }
        if(flag == 0)
          break;
      }

      closesocket(client);
      cout << "Client disconnected." << endl;
    }
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  cout << "start\n";
  string argument = argv[1];
  if(argument == "train") {
    string first_folder = Utils::get_path_from_cfg("connect4_train_first_bot_train");
    string second_folfer = Utils::get_path_from_cfg("connect4_train_second_bot_train");
    BattleModel<FirstPlayer, SecondPlayer, State, FirstAction, FirstAction>::tournament_learning(600, 6000, 1000, first_folder,
                                                                                                second_folfer, 3, 6, 42000000, true, {46, 52, 46}, 0.01, true, 0.11, false);
  }
  else {
    string mode = argv[2];
    server(argument, Utils::get_path_from_cfg("connect4_path_play"), mode);
  }
}