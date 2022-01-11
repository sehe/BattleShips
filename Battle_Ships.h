//
// Created by kd2ma on 12/28/2021.
//

#ifndef BATTLESHIP_H
#define BATTLESHIP_H
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <random>
#include "Input_Coordinates.h"
const int BOARD_DIMENSION = 10;
const int NUM_SHIPS = 5;

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::setw;
using std::endl;
using std::ifstream;

class Battle_Ships {
private:
    vector<vector<string>> board;
    vector<vector<string>> missed_board;
    Input_Coordinates ic;


public:
    const std::string myarr[NUM_SHIPS] = {"Carrier", "BattleShip", "Cruiser", "Submarine", "Destroyer"};
    void init_board();
    std::stringstream display_board(bool withSockets);
    std::stringstream display_missed_board(bool withSockets);
    void reset_board();

    void init_fleet();
    bool init_fleet_randomize();
    bool init_fleet_manual();
    bool init_fleet_file();

    bool board_set_values();
    bool check_if_occupied();

    void which_ship_present();

    bool update_ship(struct Input_Coordinates& ic);
    static int getRandom(int min, int max);
};
#endif // BATTLESHIP_H
