//
// Created by kd2ma on 12/28/2021.
//

#include "Battle_Ships.h"
#include <sstream>

void Battle_Ships::init_board() {
    for (int i = 0; i < BOARD_DIMENSION; i++) {
        vector < string > temp;
        temp.reserve(BOARD_DIMENSION);
        for (int j = 0; j < BOARD_DIMENSION; j++) {
            temp.emplace_back("-");
        }
        board.emplace_back(temp);
        missed_board.emplace_back(temp);
    }
}

std::stringstream Battle_Ships::display_board(bool withSockets) {
    std::stringstream ss;
    cout << "  ";
    if (withSockets) ss << "  ";
    for (int i = 0; i < BOARD_DIMENSION; i++) {
        cout << i << "  ";
        if (withSockets) ss << i << "  ";
    }
    cout << "\n";
    if (withSockets) ss << "\n";
    for (int i = 0; i < BOARD_DIMENSION; i++) {
        cout << i << " ";
        if (withSockets) ss << i << " ";
        for (int j = 0; j < BOARD_DIMENSION; j++) {
            cout << board[i][j] << "  ";
            if (withSockets) ss << board[i][j] << "  ";
        }
        cout << endl;
        if (withSockets) ss << endl;
    }
    return ss;
}

std::stringstream Battle_Ships::display_missed_board(bool withSockets) {
    std::stringstream ss;
    cout << "  ";
    if (withSockets) ss << "  ";
    for (int i = 0; i < BOARD_DIMENSION; i++) {
        cout << i << "  ";
        if (withSockets) ss << i << "  ";
    }
    cout << "\n";
    if (withSockets) ss << "\n";
    for (int i = 0; i < BOARD_DIMENSION; i++) {
        cout << i << " ";
        if (withSockets) ss << i << " ";
        for (int j = 0; j < BOARD_DIMENSION; j++) {
            cout << missed_board[i][j] << "  ";
            if (withSockets) ss << missed_board[i][j] << "  ";
        }
        cout << endl;
        if (withSockets) ss << endl;
    }
    return ss;
}

void Battle_Ships::reset_board() {
    for (auto & i: board) {
        for (int j = 0; j < board.size(); j++) {
            i[j] = "-";
        }
    }
}

void Battle_Ships::init_fleet() {
    string input;
    cout << "Deploy Player Ships: \nSelect input configuration (file/manual/random): ";
    cin >> input;
    if (input == "manual") {
        init_fleet_manual();
    } else if (input == "file") {
        init_fleet_file();
    } else if (input == "random") {
        init_fleet_randomize();
    }
}

bool Battle_Ships::init_fleet_manual() {
    int inputCounter = 0;
    while (inputCounter < NUM_SHIPS) {
        printf("Enter Direction: 0 for Horizontal, 1 for Vertical for %s:", myarr[inputCounter].c_str());
        cin >> ic.direction;
        printf("\nEnter x coordinates for %s:", myarr[inputCounter].c_str());
        cin >> ic.x;
        printf("\nEnter y coordinates for %s:", myarr[inputCounter].c_str());
        cin >> ic.y;
        ic.whichShip = inputCounter;
        int range_x = 0;
        int range_y = 0;
        ic.isMoveValid(range_x, range_y);

        if ((ic.x <= range_x && ic.y <= range_y && ic.direction == 0) || (ic.x <= range_y && ic.y <= range_x && ic.direction == 1)) {
            if (board_set_values()) {
                inputCounter++;
            }
            else {
                which_ship_present();
            }
        } else {
            cout << "Check input coordinates for " << myarr[ic.whichShip] << "\n";
            ic.get_range(range_x, range_y);
        }
    }
    return true;
}

bool Battle_Ships::init_fleet_file() {
    ifstream fin;
    string fName;
    int inputCounter = 0;
    cout << "Enter file name to initialize the fleet:\n";
    cin >> fName;
    cout << fName << endl;
    fin.open(fName);
    if (fin.fail()) {
        cout << "Couldn't read the file!\n";
    }
    while (fin >> fName) {
        fin >> ic.x;
        fin >> ic.y;
        fin >> ic.direction;
        ic.whichShip = inputCounter;
        int range_x = 0;
        int range_y = 0;
        ic.isMoveValid(range_x, range_y);
        if ((ic.x <= range_x && ic.y <= range_y && ic.direction == 0) || (ic.x <= range_y && ic.y <= range_x && ic.direction == 1)) {
            if (board_set_values()) {
                inputCounter++;
            } else {
                cout << "Check input coordinates for " << fName << "\n";
                which_ship_present();
                exit(1);
            }
        } else {
            cout << "Check input coordinates for " << fName << "\n";
            ic.get_range(range_x, range_y);
            exit(1);
        }
    }
    return true;
}

bool Battle_Ships::init_fleet_randomize() {
    int inputCounter = 0;
    while (inputCounter < NUM_SHIPS) {
        ic.whichShip = inputCounter;
        int range_x = 0;
        int range_y = 0;
        ic.direction = getRandom(0, 1);
        ic.isMoveValid(range_x, range_y);
        if (ic.direction == 0) {
            int random_number = getRandom(0, range_x);
            ic.x = random_number;
            random_number = getRandom(0, range_y);
            ic.y = random_number;

        } else if (ic.direction == 1) {
            int random_number = getRandom(0, range_y);
            ic.x = random_number;
            random_number = getRandom(0, range_x);
            ic.y = random_number;
        }

        if (board_set_values()) {
            inputCounter++;
        }
    }
    return true;
}

bool Battle_Ships::board_set_values() {
    bool validFile = true;
    int runUpto = 0;
    string valueToPut;

    ic.get_run_upto(runUpto, valueToPut,(string *)myarr);

    if (ic.direction == 0) {
        if (!check_if_occupied()) {
            int i = 0;
            while (i < runUpto) {
            board[ic.x][ic.y + i] = valueToPut;
                i++;
            }
        } else {
            validFile = false;
        }
    } else if (ic.direction == 1) {
        if (!check_if_occupied()) {
            int i = 0;
                while (i < runUpto) {
                board[ic.x + i][ic.y] = valueToPut;
                i++;
            }
        } else {
            validFile = false;
        }
    }
    return validFile;
}

bool Battle_Ships::check_if_occupied() {
    bool isPresent = false;
    int runUpto = 0;
    string valueToPut;
    ic.get_run_upto(runUpto, valueToPut,(string *)myarr);

    if (ic.direction == 0) {
        int i = 0;
        while (i < runUpto) {
            if (board[ic.x][ic.y + i] != "-") {
                isPresent = true;
            }
            i++;
        }
    } else if (ic.direction == 1) {
        int i = 0;
        while (i < runUpto) {
            if (board[ic.x + i][ic.y] != "-") {
                isPresent = true;
            }
            i++;
        }
    }
    return isPresent;
}

int Battle_Ships::getRandom(int min, int max) {
     static std::random_device random_device; // create object for seeding
     static std::mt19937 engine {
            random_device()
    }; // create engine and seed it
     std::uniform_int_distribution < > dist(min, max); // create distribution for integers with [1; 9] range
    auto random_number = dist(engine);
    return random_number;
}

void Battle_Ships::which_ship_present() {
    int runUpto = 0;
    string ship_name;
    ic.get_run_upto(runUpto, ship_name, (string *) myarr);
    int i = 0;
    while (i < runUpto) {
        if (ic.direction == 0) {
            if (board[ic.x][ic.y + i] != "-") {
                ship_name = board[ic.x][ic.y + i];
            }
        } else if (ic.direction == 1) {
            if (board[ic.x + i][ic.y] != "-") {
                ship_name = board[ic.x + i][ic.y];
            }
        }
        if (ship_name == "c") {
            ship_name = myarr[0];
        } else if (ship_name == "b") {
            ship_name = myarr[1];
        } else if (ship_name == "r") {
            ship_name = myarr[2];
        } else if (ship_name == "s") {
            ship_name = myarr[3];
        } else if (ship_name == "d") {
            ship_name = myarr[4];
        }
        i++;
    }
    cout << "Ship already present: " << ship_name << " @ x = " << ic.x << " y = " << ic.y << "\n";
}

bool Battle_Ships::update_ship(struct Input_Coordinates& i_c) {
    bool sunk = false;
    if (board[i_c.x][i_c.y] == "-") {
    board[i_c.x][i_c.y] = "m";
    missed_board[i_c.x][i_c.y] = "m";
    }
    else if (board[i_c.x][i_c.y] != "-" && board[i_c.x][i_c.y] != "m" && board[i_c.x][i_c.y] != "*") {
        board[i_c.x][i_c.y] = "*";
        missed_board[i_c.x][i_c.y] = "*";
        sunk = true;
    }
    return sunk;
}





