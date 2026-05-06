//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/asio.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace asio = boost::asio;
using asio::ip::tcp;

void write_data(tcp::socket& sock) {
    std::cout << "Your turn: ";

    // Message to sent for server
    std::string result;
    for (std::string msg; std::getline(std::cin, msg);) {
        if (msg == "\0") {
            break;
        }
        result += msg + "\n";
    }
    // std::cout << result << std::endl;
    boost::system::error_code error;
    // Write message to socket
    write(sock, asio::buffer(result + "\a"), error);
    if (!error) {
        std::cout << "Client sent hello message!" << std::endl;
    } else {
        std::cout << "send failed: " << error.message() << std::endl;
    }
}

void read_data(tcp::socket& sock) {
    std::streamsize size;
    do {
        boost::system::error_code error;
        // 	getting response from server
        asio::streambuf receive_buffer;
        //  read(socket, receive_buffer, asio::transfer_all(), error);
        read_until(sock, receive_buffer, "\a");

        size = receive_buffer.in_avail();

        if (size <= 1) {
            break;
        }
        if (error && error != asio::error::eof) {
            std::cout << "receive failed: " << error.message() << std::endl;
        } else {
            auto data = reinterpret_cast<char const*>(receive_buffer.data().data());
            std::cout << data << std::endl;
        }
    } while (true);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
        return 1;
    }
    asio::io_context io_service;
    tcp::resolver    resolver(io_service);
    auto             eps = resolver.resolve(tcp::v4(), argv[1], argv[2]);

    tcp::socket s(io_service);
    asio::connect(s, eps);

    while (1) {
        read_data(s);
        write_data(s);
    }
}
