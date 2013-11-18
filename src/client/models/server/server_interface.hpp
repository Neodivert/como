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
#include "../../../common/packets/scene_commands/scene_commands.hpp"
#include "../../../common/utilities/log.hpp"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <functional>
#include <map>

namespace como {

class ServerInterface
{
    private:
        boost::asio::io_service io_service_;
        std::shared_ptr< boost::asio::io_service::work > work_;

        SocketPtr socket_;

        boost::thread_group workerThreads_;

        SceneUpdate sceneUpdatePacketFromServer_;

        std::function< void (const SceneCommand*) > executeRemoteCommand_;

        // Log
        LogPtr log_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        ServerInterface( std::function< void (const SceneCommand*) > executeRemoteCommand, LogPtr log );
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
        void onSceneUpdateReceived( const boost::system::error_code& errorCode, PacketPtr packet );


    private:
        void listen();
        void work();
};

typedef std::shared_ptr<ServerInterface> ServerInterfacePtr;

} // namespace como

#endif // SERVER_INTERFACE_HPP
