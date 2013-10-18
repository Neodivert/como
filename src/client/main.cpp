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

#include <QGuiApplication>
#include "gui/main_window.hpp"
#include <glm/gtx/vector_angle.hpp>
#include "managers/tester.hpp"

//#define TESTING_MODE 1

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

void connect( const char* server );

int main( int argc, char *argv[] )
{
    //connect( "localhost" );
    //exit( 0 );

    // Create a Qt application.
    QApplication app( argc, argv );

#ifdef TESTING_MODE
    // Run tests.
    como::Tester* tester = como::Tester::getInstance();
    tester->testMeshes();
    tester->destroy();
#else
    // Create a Qt window and show it.
    como::MainWindow window;
    window.show();

    // Run Qt application.
    return app.exec();
#endif
}





using boost::asio::ip::tcp;

void connect( const char* server )
{
    /*
     * This code was created from the following example:
     *
     * Daytime.1 - A synchronous TCP daytime client
     * http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/tutorial/tutdaytime1.html
     */

    try
    {
        // All programs that use asio need to have at least one io_service object.
        boost::asio::io_service io_service;

        // Convert the server name into a TCP endpoint.
        tcp::resolver resolver(io_service);

        // A resolver takes a query object and turns it into a list of endpoints.
        // We construct a query using the name of the server, specified in argv[1],
        // and the name of the service, in this case "daytime".
        tcp::resolver::query query( server, "daytime");

        // The list of endpoints is returned using an iterator of type ip::tcp::resolver::iterator.
        // (Note that a default constructed ip::tcp::resolver::iterator object can be used as an end iterator.)
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        // Now we create and connect the socket. The list of endpoints obtained above may contain both IPv4
        // and IPv6 endpoints, so we need to try each of them until we find one that works. This keeps the
        // client program independent of a specific IP version. The boost::asio::connect() function does
        // this for us automatically.
        std::cout << "Connecting ..." << std::endl;
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        // We use a boost::array to hold the received data. The boost::asio::buffer() function automatically
        // determines the size of the array to help prevent buffer overruns. Instead of a boost::array, we
        // could have used a char [] or std::vector.
        for (;;){
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            // When the server closes the connection, the ip::tcp::socket::read_some() function will exit
            // with the boost::asio::error::eof error, which is how we know to exit the loop.
            if (error == boost::asio::error::eof){
                std::cout << "<EOF>" << std::endl;
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
