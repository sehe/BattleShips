//
// Created by kd2ma on 12/30/2021.
//

#ifndef INPUT_COORDINATES_H
#define INPUT_COORDINATES_H

#include <iostream>
using std::cout;
struct Input_Coordinates {
    int x, y;
    int direction;
    int whichShip;
    Input_Coordinates();
    void isMoveValid(int& range_x, int& range_y) const;
    void get_range(int &range_x, int &range_y) const;
    void get_run_upto(int& runUpto, std::string& value, std::string* myarr) const;
};
#endif //INPUT_COORDINATES_H
