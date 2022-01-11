//
// Created by kd2ma on 1/4/2022.
//

#ifndef GAME_H
#define GAME_H
#include "Battle_Ships.h"
#include <map>
#include <boost/asio/ip/tcp.hpp>
#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

class Game : public Battle_Ships {
private:
    int standing_ships;
    std::multimap <int,int> mymap;
    Game* player1;
    Game* player2;
public:
    void start_game();
    bool attack(Input_Coordinates& ic);
    bool is_winner() const;
    bool is_not_tried(Input_Coordinates coordinates);
    void play_vs_human();
    void play_vs_computer();
    void play_computer_vs_computer();
    void playLocal_vs_human();
//    void playSocket_vs_human(boost::asio::ip::tcp::socket &sock);
    void play_sockets();
    void play_game_s(boost::asio::ip::tcp::socket && sock);
};
#endif //GAME_H
