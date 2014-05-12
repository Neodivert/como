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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef TYPE_COMMAND_HPP
#define TYPE_COMMAND_HPP

#include <common/commands/command.hpp>

namespace como {

// FIXME: This value could / should be computed at compilation time instead of
// being a constant. The problem is: how I compute it at compilation time and
// use it in a static method (CommandType TypeCommand<CommandType>::getType())?
const unsigned COMMAND_TYPE_OFFSET = 3;


// TODO: Maybe a better name?
template <class CommandType>
class TypeCommand : public Command {
    private:
        const PackableUint8< CommandType > type_;

    public:
        /***
         * 1. Construction
         ***/

        TypeCommand( CommandTarget commandTarget, CommandType commandType, UserID userID );

        /*! \brief Copy constructor */
        TypeCommand( const TypeCommand& b );

        /*! \brief Move constructor */
        TypeCommand( TypeCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~TypeCommand() = default;


        /***
         * 3. Getters
         ***/

        CommandType getType() const;


        /***
         * 4. Buffer pre reading
         ***/

        static CommandType getType( const void* buffer );


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        TypeCommand& operator=( const TypeCommand& ) = delete;

        /*! \brief Move assignment operator */
        TypeCommand& operator=( TypeCommand&& ) = delete;
};


/***
 * 1. Construction
 ***/

template <class CommandType>
TypeCommand<CommandType>::TypeCommand( CommandTarget commandTarget, CommandType commandType, UserID userID ) :
    Command( commandTarget, userID ),
    type_( commandType )
{
    addPackable( &type_ );
}


template <class CommandType>
TypeCommand<CommandType>::TypeCommand( const TypeCommand& b ) :
    Command( b ),
    type_( b.type_ )
{
    addPackable( &type_ );
}


/***
 * 3. Getters
 ***/

template <class CommandType>
CommandType TypeCommand<CommandType>::getType() const
{
    return type_.getValue();
}


/***
 * 4. Buffer pre reading
 ***/

template <class CommandType>
CommandType TypeCommand<CommandType>::getType( const void* buffer )
{
    return static_cast< CommandType >( ( static_cast< const std::uint8_t* >( buffer ) )[COMMAND_TYPE_OFFSET] );
}

} // namespace como

#endif // TYPE_COMMAND_HPP
