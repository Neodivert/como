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

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <common/packables/array/packable_color.hpp>
#include <memory>
#include <stdexcept>
#include <common/packables/packable_integer.hpp>
#include <common/packables/ids/packable_resource_id.hpp>

#define DEFINE_SHARED_POINTERS( type, ptr, constPtr ) \
    typedef std::shared_ptr< type > ptr; \
    typedef std::shared_ptr< const type > constPtr; \


namespace como {

/*
 * Possible values for a command's target. A command's target indicates
 * the element / entity the command focuses on (ie. an user or a drawable).
 */
enum class CommandTarget : std::uint8_t
{
    USER = 0,
    SELECTION,
    PRIMITIVE,
    PRIMITIVE_CATEGORY,
    MATERIAL,
    LIGHT,
    RESOURCE,
    RESOURCES_SELECTION
};


/*
 * Convenient typedef for packing / unpacking CommandTarget values.
 * Used for network transfer transference.
*/
typedef PackableUint8<CommandTarget> PackableCommandTarget;


/*
 * Strings associated to the different values of CommandTarget enum.
 * Used for console output
*/
const char commandTargetStrings[][32]
{
    "USER"
    "SELECTION",
    "PRIMITIVE",
    "PRIMITIVE_CATEGORY",
    "MATERIAL",
    "LIGHT",
    "RESOURCE",
    "RESOURCES_SELECTION"
};


/*!
 * \class Command
 *
 * \brief Base class for all type of commands (orders sent through network)
 * supported by COMO.
 */
class Command : public CompositePackable
{
    private:
        /*! Target this command focuses on (an user, a drawable, etc) */
        const PackableCommandTarget commandTarget_;

        /*! ID of the user who performed this command */
        PackableUserID userID_;


    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        Command() = delete;

        /*!
         * \brief Constructs a command from the given arguments
         * \param commandTarget target affected by the command (an user, a
         * drawable, etc).
         * \param userID ID of the user who performed the command.
         */
        Command( CommandTarget commandTarget, UserID userID = 0 );

        /*! \brief Copy constructor */
        Command( const Command& b );

        /*! \brief Move constructor */
        Command( Command&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~Command() = default;


        /***
         * 3. Getters
         ***/

        /*!
         * \brief Returns the target this command focuses on (an user, a
         * drawable, etc)
         * \return The target this command focuses on (an user, a drawable,
         * etc).
         */
        CommandTarget getTarget() const ;

        /*! \brief Returns the ID of the user who performed this command */
        UserID getUserID() const ;


        /***
         * 4. Buffer pre reading
         ***/

        /*!
         * \brief Unpacks a command target from the first position of the
         * given buffer.
         * \param buffer buffer we are unpacking from.
         * \return a command target (see CommandTarget enum).
         */
        static CommandTarget getTarget( const void* buffer );


        /***
         * 5. Setters
         ***/

        /*!
         * \brief Change the ID for the user who performed this command.
         * \param userID new ID for the user who performed this command.
         */
        void setUserID( const UserID& userID );


        /***
         * 6. Operators
         ***/

        /*! \brief Copy assignment operator */
        CommandTarget& operator=( const CommandTarget& ) = delete;

        /*! \brief Move assignment operator */
        CommandTarget& operator=( CommandTarget&& ) = delete;
};

/*! Convenient typedefs */
typedef std::shared_ptr< Command > CommandPtr;
typedef std::shared_ptr< const Command > CommandConstPtr;

} // namespace como

#endif // COMMAND_HPP
