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
#include <QObject>
#include <common/commands/command.hpp>
#include <common/utilities/log.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <functional>
#include <map>
#include <queue>
#include <common/ids/resource_id.hpp>
#include <common/ids/resource_ids_generator.hpp>

Q_DECLARE_METATYPE( std::shared_ptr< const como::Command > )

namespace como {

const unsigned int MAX_COMMANDS_PER_SCENE_UPDATE = 4;
const unsigned int SHIPMENTS_PER_SECOND = 33;

class ServerInterface : public QObject, public Lockable
{
    Q_OBJECT

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
         * 3. Connection and disconnection
         ***/
        void connect( const char* host, const char* port, const char* userName, UserAcceptancePacket& userAcceptancePacket );
        void disconnect();


        /***
         * 4. Getters
         ***/
        ResourceID reserveResourceIDs( unsigned int nIDs );
        UserID getLocalUserID() const;
        Color getLocalUserColor() const;


        /***
         * 5. Server communication
         ***/
        void sendCommand( CommandConstPtr sceneCommand );
        void run();


        /***
         * 6. Operators
         ***/
        ServerInterface& operator=( const ServerInterface& ) = delete ;
        ServerInterface& operator=( ServerInterface&& ) = delete;


    private:
        /***
         * 7. Commands shipments
         ***/
        void setTimer();
        void sendPendingCommands();


        /***
         * 8. Commands reception.
         ***/
        void listen();


        /***
         * 9. Signals
         ***/
    signals:
        void commandReceived( std::shared_ptr< const Command > command );


        /***
         * 10. Handlers
         ***/
    private:
        void onSceneUpdatePacketReceived( const boost::system::error_code& errorCode, PacketPtr packet );
        void onSceneUpdatePacketSended( const boost::system::error_code& errorCode, PacketPtr packet );


        /***
         * 11. Worker threads
         ***/
        void work();


    private:
        // Boost ASIO's objects for asynchronous I/O.
        boost::asio::io_service io_service_;
        std::shared_ptr< boost::asio::io_service::work > work_;
        Socket socket_;

        // I/O packets.
        SceneUpdatePacket sceneUpdatePacketFromServer_;
        SceneUpdatePacket sceneUpdatePacketToServer_;

        // Queue with scene commands to be sended to the server.
        std::queue< CommandConstPtr > sceneCommandsToServer_;

        // Timer used for shipping packets to the server on a regular basis.
        boost::asio::deadline_timer timer_;

        // Generator of ResourceIDs for local user's created resources.
        ResourceIDsGeneratorPtr resourceIDsGenerator_;

        // Local user's color.
        Color localUserColor_;

        // Log.
        LogPtr log_;

        // Worker threads.
        boost::thread_group workerThreads_;

        // This is used for catching, in the main thread, the exception thrown
        // by a worker thread.
        std::exception_ptr lastException_;
};

typedef std::shared_ptr<ServerInterface> ServerInterfacePtr;

} // namespace como

#endif // SERVER_INTERFACE_HPP
