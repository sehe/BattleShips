//
// Created by kd2ma on 12/30/2021.
//

#include "Input_Coordinates.h"

Input_Coordinates::Input_Coordinates() {
x = 0;
y = 0;
direction = 0;
whichShip = 0;
}

void Input_Coordinates::isMoveValid(int& range_x, int& range_y) const {
    for (int i = whichShip; i < whichShip + 1; i++) {
        switch (i) {
            case 0:
                range_x = 9;
                range_y = 5;
                break;
            case 1:
                range_x = 9;
                range_y = 6;
                break;
            case 2:
            case 3:
                range_x = 9;
                range_y = 7;
                break;
            case 4:
                range_x = 9;
                range_y = 8;
                break;
            default:
                range_x = 0;
                range_y = 0;
                break;
        }
    }
}

void Input_Coordinates::get_range(int &range_x, int &range_y) const {
    cout << "Error at Input Coordinates: x = " << x << " y = " << y << " direction = " << direction << "\n";
    if (x > range_x && direction == 0) {
        cout << "Max X coordinate: " << range_x << "\n";
    }
    if (y > range_y && direction == 0) {
        cout << "Max Y coordinate: " << range_y << "\n";
    }
    if (x > range_y && direction == 1) {
        cout << "Max X coordinate: " << range_y << "\n";
    }
    if (y > range_x && direction == 1) {
        cout << "Max Y coordinate: " << range_x << "\n";
    }
}

void Input_Coordinates::get_run_upto(int& runUpto, std::string& value, std::string* myarr) const {
    if (myarr[whichShip] == "Carrier") {
        runUpto = 5;
        value = "c";
    } else if (myarr[whichShip] == "BattleShip") {
        runUpto = 4;
        value = "b";
    } else if (myarr[whichShip] == "Cruiser") {
        runUpto = 3;
        value = "r";
    } else if (myarr[whichShip] == "Submarine") {
        runUpto = 3;
        value = "s";
    } else if (myarr[whichShip] == "Destroyer") {
        runUpto = 2;
        value = "d";
    }
}





