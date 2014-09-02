/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef SERVER_INTERFACE_HPP
#define SERVER_INTERFACE_HPP

#include <common/packets/packets.hpp>
#include <common/commands/commands.hpp>
#include <common/utilities/log.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <functional>
#include <map>
#include <queue>
#include <QObject>
#include <common/ids/resource_id.hpp>

Q_DECLARE_METATYPE( std::shared_ptr< const como::Command > )

namespace como {

const unsigned int MAX_COMMANDS_PER_SCENE_UPDATE = 4;
const unsigned int SHIPMENTS_PER_SECOND = 33;

class ServerInterface : public QObject
{
    Q_OBJECT

    private:
        boost::asio::io_service io_service_;
        std::shared_ptr< boost::asio::io_service::work > work_;

        Socket socket_;

        SceneUpdatePacket sceneUpdatePacketFromServer_;
        SceneUpdatePacket sceneUpdatePacketToServer_;

        // Queue with scene commands to be sended to the server.
        std::queue< CommandConstPtr > sceneCommandsToServer_;

        boost::asio::deadline_timer timer_;

        ResourceID nextResourceID_;
        PackableColor localUserColor_;

        // Log
        LogPtr log_;

        boost::thread_group workerThreads_;

    public:
        /***
         * 1. Construction
         ***/
        ServerInterface() = delete;
        ServerInterface( const ServerInterface& ) = delete;
        ServerInterface( ServerInterface&& ) = delete;
        ServerInterface( const char* host, const char* port, const char* userName, const std::string& unpackingDirPath, LogPtr log, UserAcceptancePacket& userAcceptancePacket );


        /***
         * 2. Destruction
         ***/
        ~ServerInterface();


        /***
         * 2. Connection and disconnection
         ***/
        void connect( const char* host, const char* port, const char* userName, UserAcceptancePacket& userAcceptancePacket );
        void disconnect();


        /***
         * 3. Handlers
         ***/
    private:
        void onSceneUpdatePacketReceived( const boost::system::error_code& errorCode, PacketPtr packet );
        void onSceneUpdatePacketSended( const boost::system::error_code& errorCode, PacketPtr packet );


        /***
         * 4. Server communication
         ***/
    public:
        void sendCommand( CommandConstPtr sceneCommand );
        void sendCommand( const Command* sceneCommand );
        void run();

    private:
        void sendPendingCommands();
        void setTimer();

    private:
        void listen();
        void work();


        /***
         * 5. Getters
         ***/
    public:
        ResourceID getNewResourceID();
        UserID getLocalUserID() const;
        PackableColor getLocalUserColor() const;


        /***
         * 6. Operators
         ***/
    public:
        ServerInterface& operator=( const ServerInterface& ) = delete ;
        ServerInterface& operator=( ServerInterface&& ) = delete;


        /***
         * 7. Signals
         ***/
    signals:
        void commandReceived( std::shared_ptr< const Command > command );
};

typedef std::shared_ptr<ServerInterface> ServerInterfacePtr;

} // namespace como

#endif // SERVER_INTERFACE_HPP
