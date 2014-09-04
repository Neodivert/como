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

#ifndef LIGHT_CREATION_RESPONSE_COMMAND_HPP
#define LIGHT_CREATION_RESPONSE_COMMAND_HPP

#include <common/commands/light_commands/light_command.hpp>

namespace como {

class LightCreationResponseCommand : public LightCommand
{
    public:
        /***
         * 1. Construction
         ***/
        LightCreationResponseCommand( const ResourceID& lightID, bool response, UserID userID = 0 );
        LightCreationResponseCommand();
        LightCreationResponseCommand( const LightCreationResponseCommand& );
        LightCreationResponseCommand( LightCreationResponseCommand&& ) = delete;
        COMMAND_CLONE_METHOD( LightCreationResponseCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~LightCreationResponseCommand() = default;


        /***
         * 3. Getters
         ***/
        bool getResponse() const;


        /***
         * 4. Operators
         ***/
        LightCreationResponseCommand& operator = ( const LightCreationResponseCommand& ) = delete;
        LightCreationResponseCommand& operator = ( LightCreationResponseCommand&& ) = delete;


    private:
        PackableUint8<bool> response_;
};

} // namespace como

#endif // LIGHT_CREATION_RESPONSE_COMMAND_HPP
