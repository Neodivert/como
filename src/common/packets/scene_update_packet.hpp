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

#ifndef SCENE_UPDATE_PACKET_HPP
#define SCENE_UPDATE_PACKET_HPP

#include "packet.hpp"
#include <common/commands/commands.hpp>
#include <common/commands/packable_commands_list.hpp>
#include <vector>
#include <list>

namespace como {

/*!
 * \class SceneUpdatePacket
 *
 * \brief Unique type of packet sent between client and server once the
 * connection has been established. Every SceneUpdatePacket consists of
 * a sequence of commands affecting the current scene being synchronised
 * by the server.
 */
class SceneUpdatePacket : public Packet
{
    private:
        /*!
         * Number of commands to be received from server to be fully
         * synchronised with it
         */
        PackableUint32< std::uint32_t > nUnsyncCommands_;

        /*! List of commands attached to this SceneUpdatePacket */
        PackableCommandsList commands_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        SceneUpdatePacket() = delete;

        /*!
         * \brief Constructs an emtpy
         * SceneUpdatePacket (0 commands).
         */
        SceneUpdatePacket( const std::string& unpackingDirPath );

        /*! \brief Copy constructor */
        SceneUpdatePacket( const SceneUpdatePacket& b );

        /*! \brief Move constructor */
        SceneUpdatePacket( SceneUpdatePacket&& ) = delete;

        /*!
         * \brief Clone method
         * \return a pointer to a copy of this SceneUpdatePacket.
         */
        virtual Packet* clone() const ;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        virtual ~SceneUpdatePacket() = default;


        /***
         * 3. Getters
         ***/

        /*!
         * \brief Returns the number of commands to be synchronised from
         * server to be fully synchronised with it.
         */
        std::uint32_t getUnsyncCommands() const ;

        /*! \brief Get a pointer to the list of commands inside this packet */
        const CommandsList* getCommands() const ;

        /*! \brief see Packet::expectedType const */
        virtual bool expectedType() const ;


        /***
         * 4. Setters
         ***/

        /*!
         * \brief Adds the given command to the list of commands held by this
         * packet.
         */
        void addCommand( CommandConstPtr command );

        /*!
         * \brief Adds one command from a command' historic to the list of
         * commands held by this packet.
         * \param command command to be added.
         * \param commandIndex command's index in the historic.
         * \param historicSize size of the historic where the command was
         * retrieved from.
         */
        void addCommand( CommandConstPtr command,
                         const std::uint32_t& commandIndex,
                         const std::uint32_t& historicSize );


        /*!
         * \brief Empty the commands list of this SceneUpdatePacket and set
         * the number of commands to be synchronised from server to zero.
         */
        void clear();


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        SceneUpdatePacket& operator = (const SceneUpdatePacket& b) = delete;

        /*! \brief Move assignment operator */
        SceneUpdatePacket& operator = ( SceneUpdatePacket&& ) = delete;
};

} // namespace como

#endif // SCENE_UPDATE_PACKET_HPP
