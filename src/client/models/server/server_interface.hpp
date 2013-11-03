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

#ifndef SERVER_INTERFACE_HPP
#define SERVER_INTERFACE_HPP

#include "../../../common/packets/packets.hpp"
#include <boost/asio.hpp>
#include <thread>

namespace como {

class ServerInterface
{
    private:
        boost::asio::io_service io_service_;

        SocketPtr socket_;

        std::thread* listenerThread;

        std::mutex closeConnectionMutex_;
        bool closeConnection_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        ServerInterface();
        ~ServerInterface();


        /***
         * 2. Connection and disconnection
         ***/
        void connect( const char* host, const char* port, const char* userName );
        void disconnect();

        /***
         * 3. Handlers
         ***/
    private:
        void onNewUserPacketSent( PacketPtr );


    private:
        void listen();
};

typedef std::shared_ptr<ServerInterface> ServerInterfacePtr;

} // namespace como

#endif // SERVER_INTERFACE_HPP
