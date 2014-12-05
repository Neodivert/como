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

#ifndef PACKABLE_COMMANDS_LIST_HPP
#define PACKABLE_COMMANDS_LIST_HPP

#include "commands.hpp"
#include <common/packables/packable.hpp>
#include <list>

namespace como {

typedef std::list< CommandConstPtr > CommandsList;

/*!
 * \class PackableCommandsList
 *
 * \brief Commands list that can be packed to / unpacked from a buffer.
 */
class PackableCommandsList : public Packable
{
    private:
        /*! Path of the unpacking directory for commands including files. */
        std::string unpackingDirPath_;

        /*! List of commands */
        CommandsList commands_;

    public:
        /***
         * 1. Construction.
         ***/

        /*! \brief Default constructor */
        PackableCommandsList() = delete;

        /*! \brief Constructor */
        PackableCommandsList( const std::string& unpackingDirPath );

        /*! \brief Copy constructor */
        PackableCommandsList( const PackableCommandsList& b );

        /*! \brief Move constructor */
        PackableCommandsList( PackableCommandsList&& b ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        virtual ~PackableCommandsList() = default;


        /***
         * 3. Packing and unpacking
         ***/

        /*!
         * \brief Packs all the commands held by this list into the given
         * buffer.
         * \param buffer buffer for paking the commands into.
         * \return a pointer to the next available position in buffer.
         */
        virtual void* pack( void* buffer ) const;

        /*!
         * \brief Unpacks a list of commands from the given buffer.
         * \param buffer buffer for unpaking the commands from.
         * \return a pointer to the next available position in buffer.
         */
        virtual const void* unpack( const void* buffer );

        /*!
         * \brief Unpacks PackableCommandsList data from the given buffer and
         * throw an exception if it doesn't match this CommandList's contents.
         * \param buffer buffer for unpaking the commands from.
         * \return a pointer to the next available position in buffer.
         */
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 4. Getters
         ***/

        /*!
         * \brief Returns the size (in bytes) that this CommandList would
         * occupy once packed into a buffer.
         */
        virtual PacketSize getPacketSize() const;

        /*!
         * \brief Returns a pointer to the plain list of commands held by this
         * instance of PackableCommandsList.
         * \return a pointer to the plain list of commands held by this
         * instance of PackableCommandsList
         */
        const CommandsList* getCommands() const;


        /***
         * 5. Commands management
         ***/

        /*! \brief Add a command to the list */
        void addCommand( CommandConstPtr command );

        /*! \brief Clear the commands list by removing all its elements */
        void clear();


        /***
         * 6. Auxiliar methods
         ***/
        static CommandPtr createEmtpyCommandFromBuffer( const void* buffer, const std::string& unpackingDirPath );


        /***
         * 7. Operators
         ***/
        /*! \brief Copy assignment operator */
        PackableCommandsList& operator = (const PackableCommandsList& ) = delete;

        /*! \brief Move assignment operator */
        PackableCommandsList& operator = ( PackableCommandsList&& ) = delete;
};

}// namespace como

#endif // PACKABLE_COMMANDS_LIST_HPP
