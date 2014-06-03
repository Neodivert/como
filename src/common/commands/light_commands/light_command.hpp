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

#ifndef LIGHT_COMMAND_HPP
#define LIGHT_COMMAND_HPP

#include <common/packables/packable_uint8.hpp>
#include <common/utilities/ids.hpp>
#include <common/commands/type_command.hpp>

namespace como {

enum class LightCommandType : std::uint8_t
{
    LIGHT_CREATION = 0
};

typedef PackableDrawableID LightID; // TODO: Change this.
typedef PackableDrawableID PackableLightID;

class LightCommand : public TypeCommand< LightCommandType >
{
    private:
        PackableLightID lightID_;

        /***
         * 1. Construction
         ***/
    public: // FIXME: This should be "protected" but LightCreationCommand::getLightType static method needs it.
        LightCommand( LightCommandType lightType, const PackableLightID& lightID );
        LightCommand( const LightCommand& b );
        LightCommand( LightCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightCommand() = default;


        /***
         * 3. Getters
         ***/
        LightID getLightID() const;


        /***
         * 4. Operators
         ***/
        LightCommand& operator = ( const LightCommand& ) = delete;
        LightCommand& operator = ( LightCommand&& ) = delete;
};

typedef std::shared_ptr< LightCommand > LightCommandPtr;
typedef std::shared_ptr< const LightCommand > LightCommandConstPtr;

} // namespace como

#endif // LIGHT_COMMAND_HPP
