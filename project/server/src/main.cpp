/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#define CLIENT

#ifdef CLIENT

/*
 * Daytime.1 - A synchronous TCP daytime client
 * http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/tutorial/tutdaytime1.html
 */

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
          std::cerr << "Usage: client <host>" << std::endl;
          return 1;
        }

        // All programs that use asio need to have at least one io_service object.
        boost::asio::io_service io_service;

        // Convert the server name into a TCP endpoint.
        tcp::resolver resolver(io_service);

        // A resolver takes a query object and turns it into a list of endpoints.
        // We construct a query using the name of the server, specified in argv[1],
        // and the name of the service, in this case "daytime".
        tcp::resolver::query query(argv[1], "daytime");

        // The list of endpoints is returned using an iterator of type ip::tcp::resolver::iterator.
        // (Note that a default constructed ip::tcp::resolver::iterator object can be used as an end iterator.)
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        // Now we create and connect the socket. The list of endpoints obtained above may contain both IPv4
        // and IPv6 endpoints, so we need to try each of them until we find one that works. This keeps the
        // client program independent of a specific IP version. The boost::asio::connect() function does
        // this for us automatically.
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        // We use a boost::array to hold the received data. The boost::asio::buffer() function automatically
        // determines the size of the array to help prevent buffer overruns. Instead of a boost::array, we
        // could have used a char [] or std::vector.
        for (;;){
            std::cout << "Connecting ..." << std::endl;

            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            // When the server closes the connection, the ip::tcp::socket::read_some() function will exit
            // with the boost::asio::error::eof error, which is how we know to exit the loop.
            if (error == boost::asio::error::eof){
                std::cout << "The server closed the connection! D:" << std::endl;
                break; // Connection closed cleanly by peer.
            }else if (error){
                std::cout << "There was an error! D:" << std::endl;
                throw boost::system::system_error(error); // Some other error.
            }

            std::cout.write(buf.data(), len);
        }

    }catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

#else

/*
 * Daytime.2 - A synchronous TCP daytime server
 * http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/tutorial/tutdaytime2.html
 */

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int main()
{
    try {
        boost::asio::io_service io_service;

        // A ip::tcp::acceptor object needs to be created to listen for new connections. It is initialised to listen on
        // TCP port 13, for IP version 4.
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));

        // This is an iterative server, which means that it will handle one connection at a time. Create a socket
        // that will represent the connection to the client, and then wait for a connection.
        for (;;){
            std::cout << "Waiting ..." << std::endl;

            tcp::socket socket(io_service);
            acceptor.accept(socket);

            // A client is accessing our service. Determine the current time and transfer this information to the client.
            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

#endif

