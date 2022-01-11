//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
const int max_length = 1024;

void write_data(boost::asio::ip::tcp::socket &sock)
{
    std::cout << "Your turn: ";
    //Message to sent for server
    std::string msg;
    std::string result;
    while ( std::getline(std::cin, msg)) {
        if (msg == "\0") {
            break;
        }
        result += msg + "\n";
    }
 //   std::cout << result << std::endl;
    boost::system::error_code error;
    //Write message to socket
    boost::asio::write( sock, boost::asio::buffer(result+ "\a"), error );
    if( !error ) {
        std::cout << "Client sent hello message!" << std::endl;
    }
    else {
        std::cout << "send failed: " << error.message() << std::endl;
    }
}

void read_data(boost::asio::ip::tcp::socket &sock)
{
    std::streamsize size;
        do {
        boost::system::error_code error;
        // 	getting response from server
        boost::asio::streambuf receive_buffer;
        //  boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
        boost::asio::read_until(sock, receive_buffer, "\a");

        size = receive_buffer.in_avail();

        if (size <= 1) {
            break;
        }
            if( error && error != boost::asio::error::eof ) {
            std::cout << "receive failed: " << error.message() << std::endl;
        }
        else {
            const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
            std::cout << data << std::endl;
        }
    } while (true);

}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
        return 1;
    }
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    boost::asio::connect(s, iterator);

    while (1) {
        read_data(s);
        write_data(s);
    }
}
