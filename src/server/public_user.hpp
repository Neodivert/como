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

#ifndef PUBLIC_USER_HPP
#define PUBLIC_USER_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>
#include "../common/packets/packets.hpp"
#include "../common/utilities/log.hpp"
#include <list>
#include "commands_historic.hpp"
#include "../common/users/basic_user.hpp"

namespace como {

const unsigned int MAX_COMMANDS_PER_PACKET = 4;

const unsigned int BUFFER_SIZE = 1024;

typedef boost::asio::ip::tcp::socket Socket;
typedef std::shared_ptr< Socket > SocketPtr;

typedef std::function< void (const boost::system::error_code& errorCode,
                             UserID userID,
                             SceneUpdateConstPtr sceneUpdate) > ProcessSceneUpdateCallback;

class PublicUser : public BasicUser
{
    private:
        // I/O service.
        std::shared_ptr< boost::asio::io_service > io_service_;

        SocketPtr socket_;

        ProcessSceneUpdateCallback processSceneUpdateCallback_;
        std::function<void (UserID)> removeUserCallback_;

        //std::function<void ()> broadcastCallback_;

        std::uint8_t nextCommand_;
        std::uint8_t nCommandsInLastPacket_;
        std::uint32_t lastCommandSent_;

        SceneUpdate sceneUpdatePacketFromUser_;
        SceneUpdate outSceneUpdatePacket_;

        mutable std::recursive_mutex mutex_;
        bool synchronizing_;


        CommandsHistoricPtr commandsHistoric_;

        LogPtr log_;

        bool updateRequested_;


    public:
        /***
         * 1. Initialization and destruction
         ***/
        PublicUser( UserID id, const char* name,
                    std::shared_ptr< boost::asio::io_service > io_service,
                    Socket socket,
                    ProcessSceneUpdateCallback processSceneUpdateCallback,
                    std::function<void (UserID)> removeUserCallback,
                    CommandsHistoricPtr commandsHistoric,
                    LogPtr log );
        ~PublicUser();


        /***
         * 2. User updating
         ***/
        void requestUpdate();
    //private:
        //void sync();
    public:

        /***
         * 3. Socket reading
         ***/
        void readSceneUpdate();
        void onReadSceneUpdate( const boost::system::error_code& errorCode, PacketPtr packet );


        /***
         * 4. Socket writing
         ***/
        bool needsSceneUpdate() const ;
        void sendNextSceneUpdate();
        void onWriteSceneUpdate( const boost::system::error_code& errorCode, PacketPtr packet );
};


typedef std::shared_ptr< PublicUser > PublicUserPtr;

} // namespace como

#endif // PUBLIC_USER_HPP
