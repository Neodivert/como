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

#ifndef PUBLIC_USER_HPP
#define PUBLIC_USER_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>
#include <common/packets/packets.hpp>
#include <common/utilities/log.hpp>
#include <list>
#include "commands_historic.hpp"
#include <common/users/basic_user.hpp>

namespace como {

const unsigned int MAX_COMMANDS_PER_PACKET = 4;

const unsigned int BUFFER_SIZE = 1024;

typedef boost::asio::ip::tcp::socket Socket;
typedef std::shared_ptr< Socket > SocketPtr;

typedef std::function< void (const boost::system::error_code& errorCode,
                             UserID userID,
                             SceneUpdatePacketConstPtr sceneUpdate) > ProcessSceneUpdatePacketCallback;

class PublicUser : public BasicUser
{
    private:
        // I/O service.
        std::shared_ptr< boost::asio::io_service > io_service_;

        SocketPtr socket_;

        ProcessSceneUpdatePacketCallback processSceneUpdatePacketCallback_;
        std::function<void (UserID)> removeUserCallback_;

        //std::function<void ()> broadcastCallback_;

        std::uint32_t nextCommand_;
        std::uint8_t nCommandsInLastPacket_;
        std::uint32_t lastCommandSent_;

        SceneUpdatePacket sceneUpdatePacketFromUser_;
        SceneUpdatePacket outSceneUpdatePacketPacket_;

        mutable std::recursive_mutex mutex_;
        bool synchronizing_;


        CommandsHistoricPtr commandsHistoric_;

        LogPtr log_;

        bool updateRequested_;

        SelectionResponseCommandPtr selectionResponse_;

        std::uint32_t color_;

    public:
        /***
         * 1. Construction
         ***/
        PublicUser() = delete;
        PublicUser( const PublicUser& ) = delete;
        PublicUser( PublicUser&& ) = delete;
        PublicUser( UserID id, const char* name,
                    std::shared_ptr< boost::asio::io_service > io_service,
                    Socket socket,
                    ProcessSceneUpdatePacketCallback processSceneUpdatePacketCallback,
                    std::function<void (UserID)> removeUserCallback,
                    CommandsHistoricPtr commandsHistoric,
                    LogPtr log,
                    std::uint32_t color );


        /***
         * 2. Destruction
         ***/
        ~PublicUser() = default;


        /***
         * 3. User updating
         ***/
        void requestUpdate();

    //private:
        //void sync();
    public:

        /***
         * 4. Socket reading
         ***/
        void readSceneUpdatePacket();
        void onReadSceneUpdatePacket( const boost::system::error_code& errorCode, PacketPtr packet );


        /***
         * 5. Socket writing
         ***/
        bool needsSceneUpdatePacket() const ;
        void sendNextSceneUpdatePacket();
        void onWriteSceneUpdatePacket( const boost::system::error_code& errorCode, PacketPtr packet );


        /***
         * 6. Selection responses
         ***/
        void addSelectionResponse( bool selectionResponse );


        /***
         * 7. Getters
         ***/
        std::uint32_t getColor();


        /***
         * 8. Operators
         ***/
        PublicUser& operator = (const PublicUser& ) = delete;
        PublicUser& operator = ( PublicUser&& ) = delete;
};


typedef std::shared_ptr< PublicUser > PublicUserPtr;

} // namespace como

#endif // PUBLIC_USER_HPP
