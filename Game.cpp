//
// Created by kd2ma on 1/4/2022.
//
#include <map>
#include <iterator>
#include <utility>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <thread>

#include "Game.h"

using std::map;
using namespace boost::asio;

void Game::start_game() {
    player1 = new Game;
    player2 = new Game;
    player1->standing_ships = 17;
    player2->standing_ships = 17;

    string input;

    cout << setw(25) << "====================\n";
    cout << setw(25) << "Game of Battle Ships\n";
    cout << setw(25) << "====================\n";

    player1->init_board();
    player2->init_board();
    cout << setw(25) << "Player 1 Board: \n" , player1->display_board(false);
    cout << setw(25) << "Player 2 Board: \n" , player2->display_board(false);
    player1->init_fleet();
    player2->init_fleet();
    cout << setw(25) << "Player 1 Board: \n" , player1->display_board(false);
    cout << setw(25) << "Player 2 Board: \n" , player2->display_missed_board(false);

    cout << "How would whom would like to play against? Select from human/computer/simulate/sockets: ";
    cin >> input;
    if (input == "human") {
        play_vs_human();
    } else if (input == "computer") {
        play_vs_computer();
    } else if (input == "simulate") {
        play_computer_vs_computer();
    } else if (input == "sockets") {
        play_sockets();
    }
}

bool Game::attack(Input_Coordinates& ic) {
    if(update_ship(ic)) {
        cout << "Hit!\n";
        standing_ships--;
    } else {
        cout << "Miss!\n";
    }
    return false;
}

bool Game::is_winner() const {
    if (standing_ships < 1) {
        return true;
    }
    return false;
}

bool Game::is_not_tried(Input_Coordinates ic) {
    std::ofstream fout;
    fout.open("mylog.txt", std::ios::out | std::ios::app);
    std::multimap<int, int>::iterator itr;
    bool present = false;
    for (itr = mymap.begin(); itr != mymap.end(); itr++) {
        if (itr->first == ic.x && itr->second == ic.y) {
            present = true;
        }
    }
    if (!present) {
        mymap.insert(std::pair<int,int>(ic.x, ic.y));
    } else {
        cout << "Coordinates X = " << ic.x << " Y =  " << ic.y << "are already attacked, try new coordinate:\n";
        fout << "Coordinates X = " << ic.x << " Y =  " << ic.y << "are already attacked, try new coordinate:\n";

    }
    for (itr = mymap.begin(); itr != mymap.end(); itr++) {
        std::cout << "Pair: " << itr->first << "," << itr->second << endl;
        fout << "Pair: " << itr->first << "," << itr->second << endl;

    }
    fout << "\n\n\n";
    return present;
}

void Game::play_vs_human() {
    Input_Coordinates ic;
    while (is_winner()) {
        cout << "(Player 1 turn) ";
        do {
            cin >> ic.x;
            cin >> ic.y;
        } while (player1->is_not_tried(ic));

        player2->attack(ic);
        cout << setw(25) << "Player 2 Board: \n" , player2->display_board(false);
        cout << setw(25) << "Player 2 Board: \n" , player2->display_missed_board(false);

        if (player2->is_winner()) {
            cout << "Player 1 is winner!\n";
            cout << "Player 2 : standing ships : " << player2->standing_ships;
            cout << setw(25) << "Player 1 Board: \n" , player1->display_board(false);
            cout << setw(25) << "Player 2 Board: \n" , player2->display_board(false);
            cout << setw(25) << "Player 2 MISSED Board: \n" , player2->display_missed_board(false);
            // system("PAUSE");
            exit(1);
        }
        cout << "(Player 2 turn) ";
        do {
            cin >> ic.x;
            cin >> ic.y;
        } while (player2->is_not_tried(ic));
        player1->attack(ic);
        cout << setw(25) << "Player 1 Board: \n" , player1->display_board(false);

        if (player1->is_winner()) {
            cout << "Player 2 is winner!\n";
            cout << "Player 1 : standing ships : " << player1->standing_ships;
            cout << setw(25) << "Player 1 Board: \n" , player1->display_board(false);
            cout << setw(25) << "Player 2 Board: \n" , player2->display_board(false);
            cout << setw(25) << "Player 2 MISSED Board: \n" , player2->display_missed_board(false);
            // system("PAUSE");
            exit(1);
        }
    }
}



void Game::play_vs_computer() {
    Input_Coordinates ic;
    while (is_winner()) {
        cout << "(Player 1 turn) ";
        do {
            cin >> ic.x;
            cin >> ic.y;
        } while (player1->is_not_tried(ic));
        player2->attack(ic);
        cout << setw(25) << "Computer Board: \n" , player2->display_board(false);
        cout << setw(25) << "Computer Missed Board: \n" , player2->display_missed_board(false);
        if (player2->is_winner()) {
            cout << "Player 1 is winner!\n";
            cout << "Computer : standing ships : " << player2->standing_ships;
            cout << setw(25) << "Player 1 Board: \n" , player1->display_board(false);
            cout << setw(25) << "Computer Board: \n" , player2->display_board(false);
            cout << setw(25) << "Computer Missed Board: \n" , player2->display_missed_board(false);
            // system("PAUSE");
            exit(1);
        }
        cout << "(Computer turn) ";
        do {
            ic.x = getRandom(0,9);
            ic.y = getRandom(0,9);
        } while (player2->is_not_tried(ic));
        player1->attack(ic);
        cout << setw(25) << "Player 1 Board: \n" , player1->display_board(false);

        if (player1->is_winner()) {
            cout << "Player 2/Computer is winner!\n";
            cout << "Player 1 : standing ships : " << player1->standing_ships;
            cout << setw(25) << "Player 1 Board: \n" , player1->display_board(false);
            cout << setw(25) << "Computer Board: \n" , player2->display_board(false);
            cout << setw(25) << "Computer Missed Board: \n" , player2->display_missed_board(false);
            // system("PAUSE");
            exit(1);
        }
    }
}

void Game::play_computer_vs_computer() {
    Input_Coordinates ic;
    while (is_winner()) {
        cout << "(Computer 1 turn) ";
        do {
            ic.x = getRandom(0,9);
            ic.y = getRandom(0,9);
        } while (player1->is_not_tried(ic));
        player2->attack(ic);
        cout << setw(25) << "Computer 2 Board: \n" , player2->display_board(false);
        cout << setw(25) << "Computer 2 Missed Board: \n" , player2->display_missed_board(false);
        if (player2->is_winner()) {
            cout << "Computer 1 is winner!\n";
            cout << "Computer 2 : standing ships : " << player2->standing_ships;
            cout << setw(25) << "Computer 1 Board: \n" , player1->display_board(false);
            cout << setw(25) << "Computer 2 Board: \n" , player2->display_board(false);
            cout << setw(25) << "Computer 2 Missed Board: \n" , player2->display_missed_board(false);
            // system("PAUSE");
            exit(1);
        }
        cout << "(Computer 2 turn) ";
        do {
            ic.x = getRandom(0,9);
            ic.y = getRandom(0,9);
        } while (player2->is_not_tried(ic));
        player1->attack(ic);
        cout << setw(25) << "Computer 1 Board: \n" , player1->display_board(false);

        if (player1->is_winner()) {
            cout << "Computer 2 is winner!\n";
            cout << "Computer 1 : standing ships : " << player1->standing_ships;
            cout << setw(25) << "Computer 1 Board: \n" , player1->display_board(false);
            cout << setw(25) << "Computer 2 Board: \n" , player2->display_board(false);
            cout << setw(25) << "Computer 2 Missed Board: \n" , player2->display_missed_board(false);
            // system("PAUSE");
            exit(1);
        }
    }
}

std::string read_data(boost::asio::ip::tcp::socket & sock)
{
    boost::asio::streambuf buf;
    boost::asio::read_until(sock, buf, "\a");
    std::string data = boost::asio::buffer_cast<const char*>(buf.data());
    return data;
}

void write_data(std::stringstream& ss, boost::asio::ip::tcp::socket & sock)
{
    boost::system::error_code error;
    std::string msg;
    std::string result;

    //  std::cout<<"Enter the message \n";
//  std::cout << ss.str().c_str() << "this is ss" << std::endl;

    while ( std::getline(ss, msg)) {
        if (msg == "send") {
            break;
        }
        result += msg + "\n";
    }

    std::cout << result << std::endl;
    boost::asio::write( sock, boost::asio::buffer(result+ "\a"), error );
    if( !error ) {
        //    std::cout << "Server sent hello message!" << std::endl;
    }
    else {
        std::cout << "send failed: " << error.message() << std::endl;
    }
}

void Game::play_sockets() {
    boost::asio::io_context m_io_context;

    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), 54000);
        // Step 3. Instantiating and opening an acceptor socket.
        boost::asio::ip::tcp::acceptor acceptor(m_io_context, ep.protocol());
        // Step 4. Binding the acceptor socket to the server endpint.
        acceptor.bind(ep);
        // Step 5. Starting to listen for incoming connection requests.
        acceptor.listen();
        // Step 6. Creating an active socket.
        boost::asio::ip::tcp::socket m_socket(m_io_context);

        // Step 7. Processing the next connection request and connecting the active socket to the client.
        acceptor.accept(m_socket);

    //all steps for creating socket using boost::asio are done.
    //Now perform read write operations in a function.
    //while(1)

    std::thread(&Game::play_game_s, this, std::move(m_socket));
}


void Game::play_game_s(ip::tcp::socket && sock) {
            Input_Coordinates ic;
            while (is_winner()) {
                std::stringstream ss;

                ss << setw(25) << "Player 1 Board:\n";
                write_data(ss,sock);
                std::stringstream().swap(ss);
                ss = player1->display_board(true);
                write_data(ss,sock);

                std::stringstream().swap(ss);
                ss << setw(25) << "Player 2 Board:\n";
                write_data(ss,sock);
                std::stringstream().swap(ss);
                ss = player2->display_board(true);
                write_data(ss,sock);

                cout << "(Player 1 turn) ";
                std::stringstream().swap(ss);
                ss << "(Player 1 turn) ";
                write_data(ss,sock);
                //sock.wait(boost::asio::ip::tcp::socket::wait_write);

                do {
                    cin >> ic.x;
                    cin >> ic.y;
                } while (player1 -> is_not_tried(ic));
                player2 -> attack(ic);

                cout << setw(25) << "Player 2 Board: \n";
                std::stringstream().swap(ss);
                ss << setw(25) << "Player 2 Board:\n";
                write_data(ss,sock);
                std::stringstream().swap(ss);
                ss = player2 -> display_board(true);
                write_data(ss,sock);

                cout << setw(25) << "Player 2 Missed Board: \n";
                std::stringstream().swap(ss);
                ss << setw(25) << "Player 2 Missed Board:\n";
                write_data(ss,sock);
                std::stringstream().swap(ss);
                ss = player2 -> display_missed_board(true);
                write_data(ss,sock);

                if (player2 -> is_winner()) {
                    cout << "Player 1 is winner!\n";
                    std::stringstream().swap(ss);
                    ss << "Player 1 is winner!\n";
                    write_data(ss,sock);

                    cout << "Player 2 : standing ships : " << player2 -> standing_ships;
                    std::stringstream().swap(ss);
                    ss << "Player 2 : standing ships : " << player2 -> standing_ships;
                    write_data(ss,sock);

                    cout << setw(25) << "Player 1 Board: \n", player1 -> display_board(true);
                    std::stringstream().swap(ss);
                    ss << setw(25) << "Player 1 Board: \n";
                    write_data(ss,sock);

                    std::stringstream().swap(ss);
                    ss = player1 -> display_board(true);
                    write_data(ss,sock);

                    cout << setw(25) << "Player 2 Board: \n";
                    std::stringstream().swap(ss);
                    ss << setw(25) << "Player 2 Board: \n";
                    write_data(ss,sock);
                    std::stringstream().swap(ss);
                    ss = player2 -> display_board(true);
                    write_data(ss,sock);

                    cout << setw(25) << "Player 2 Missed Board: \n";
                    std::stringstream().swap(ss);
                    ss << setw(25) << "Player 2 Missed Board: \n";
                    write_data(ss,sock);
                    std::stringstream().swap(ss);
                    ss = player2 -> display_missed_board(true);
                    write_data(ss,sock);

                    // system("PAUSE");
                    exit(1);
                }
                cout << "(Player 2 turn) ";
                std::stringstream().swap(ss);
                ss << "(Player 2 turn) ";
                write_data(ss,sock);
                std::stringstream().swap(ss);
                write_data(ss,sock);
                std::stringstream().swap(ss);
                do {
                    std::string receivedInput = read_data(sock);
                    ss << receivedInput;
                    ss >> ic.x;
                    ss >> ic.y;
                } while (player2 -> is_not_tried(ic));

                player1 -> attack(ic);
                cout << setw(25) << "Player 1 Board: \n";
                std::stringstream().swap(ss);
                ss << setw(25) << "Player 1 Board: \n";
                write_data(ss,sock);
                std::stringstream().swap(ss);
                ss = player1 -> display_board(true);
                write_data(ss,sock);

                if (player1 -> is_winner()) {
                    cout << "Player 2 is winner!\n";
                    std::stringstream().swap(ss);
                    ss << "Player 2 is winner!\n";
                    write_data(ss,sock);
                    cout << "Player 1 : standing ships : " << player1 -> standing_ships;
                    std::stringstream().swap(ss);
                    ss << "Player 1 : standing ships : " << player1 -> standing_ships;
                    write_data(ss,sock);


                    cout << setw(25) << "Player 1 Board: \n";
                    std::stringstream().swap(ss);
                    ss << setw(25) << "Player 1 Board: \n";
                    write_data(ss,sock);


                    std::stringstream().swap(ss);
                    ss = player1 -> display_board(true);
                    write_data(ss,sock);

                    cout << setw(25) << "Player 2 Board: \n";
                    std::stringstream().swap(ss);
                    ss << setw(25) << "Player 2 Board: \n";
                    write_data(ss,sock);

                    std::stringstream().swap(ss);
                    ss = player2 -> display_board(true);
                    write_data(ss,sock);

                    cout << setw(25) << "Player 2 Missed Board: \n";
                    std::stringstream().swap(ss);
                    ss << setw(25) << "Player 2 Missed Board: \n";
                    write_data(ss,sock);

                    std::stringstream().swap(ss);
                    ss = player2 -> display_missed_board(true);
                    write_data(ss,sock);

                    // system("PAUSE");
                    exit(1);
                }
            }
}








