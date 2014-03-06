/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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

#ifndef SERVER_HPP
#define SERVER_HPP

#include <ctime>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/lexical_cast.hpp>
#include "public_user.hpp"
#include "../common/packets/packets.hpp"
#include "../common/utilities/log.hpp"
#include "commands_historic.hpp"
#include <map>

using boost::asio::ip::tcp;

namespace como {

typedef std::map< PackableDrawableID, UserID > DrawableOwners;
typedef std::map< UserID, PublicUserPtr > UsersMap;

/*! Main server manager */
class Server
{
    private:
        // I/O service.
        std::shared_ptr< boost::asio::io_service > io_service_;

        // Acceptor for new TCP connections
        boost::asio::ip::tcp::acceptor acceptor_;

        // Work object.
        boost::asio::io_service::work work_;

        // Users map (ID, user).
        UsersMap users_;

        // Number of worker threads in the server.
        const unsigned int N_THREADS;

        // Maximum number of sessions (users) allowed in the server.
        const unsigned int MAX_SESSIONS;

        // Threads pool
        boost::thread_group threads_;

        // Mutex for console output (cout).
        boost::mutex coutMutex_;

        // New socket
        Socket newSocket_;

        // When a new user connect to the server, this will be the id givent to him/her.
        UserID newId_;

        // Server's port.
        unsigned int port_;

        // Historic of commands performed on the scene.
        CommandsHistoricPtr commandsHistoric_;

        // A map that relates each drawable in the scene with its owner.
        DrawableOwners drawableOwners_;

        // Log
        LogPtr log_;

        mutable std::recursive_mutex mutex_;

    public:
        /***
         * 1. Construction
         ***/
        Server() = delete;
        Server( const Server& ) = delete;
        Server( Server&& ) = delete;

        /*! \brief Initialize a server.
         * \param port_ port the server will be listening to.
         * \param maxSessions maximum number of allowed simultaneous clients.
         * \param nThreads number of threads used during server execution.
         */
        Server( unsigned int port_, unsigned int maxSessions, unsigned int nThreads = 3 );


        /***
         * 2. Destruction
         ***/
        ~Server() = default;


        /***
         * 3. Main loop
         ***/
        /*! \brief Start the server's main loop. */
        void run();

    private:
        /*! \brief Notify to all the users that there is new commands to synchronize. */
        void broadcast();

        /***
         * 4. Listeners
         ***/
        /*! \brief Listen for a new connection */
        void listen();


        /***
         * 5. Handlers
         ***/
        /*! \brief Handler for a new connection */
        void onAccept( const boost::system::error_code& errorCode );

        /*! \brief Process a SCENE_UPDATE packet received from client
         * \param errorCode Error code associated with the packet reception.
         * \param userID ID of the user who sent the packet.
         * \param sceneUpdate SCENE_UPDATE packet received from client.
        */
        void processSceneUpdatePacket( const boost::system::error_code& errorCode,
                                 UserID userID,
                                 SceneUpdatePacketConstPtr sceneUpdate );

        /*! \brief Process a scene command.
         * \param userID ID of the user who sent the command.
         * \param sceneCommand Scene command.
         */
        void processSceneCommand( CommandConstPtr sceneCommand );


        /***
         * 6. Commands historic management.
         ***/
        /*! \brief Add a command to the historic. */
        void addCommand( CommandConstPtr sceneCommand );


        /***
         * 7. Auxiliar methods
         ***/
        void deleteUser( UserID id );
        void unselectAll( UserID userID );
        void deleteSelection( UserID userID );
    private:
        std::string getCurrentDayTime() const ;
        void workerThread();
        void openAcceptor();
        bool nameInUse( const char* newName ) const ;

    public:
        /***
         * 8. Operators
         ***/
        Server& operator = (const Server& ) = delete;
        Server& operator = ( Server&& ) = delete;
};

} // namespace como

#endif // SERVER_HPP
