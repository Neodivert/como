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

#ifndef LIGHT_CREATION_COMMAND_HPP
#define LIGHT_CREATION_COMMAND_HPP

#include "light_command.hpp"

namespace como {

enum class LightType : std::uint8_t
{
    DIRECTIONAL_LIGHT = 0
};
typedef PackableUint8< LightType > PackableLightType;

class LightCreationCommand : public LightCommand
{
    private:
        const PackableLightType lightType_;
        PackableColor lightColor_;

    protected:
        /***
         * 1. Construction
         ***/
        LightCreationCommand( LightType lightType, UserID userID, ResourceID lightID, const PackableColor& lightColor );
        LightCreationCommand( const LightCreationCommand& b );
        LightCreationCommand( LightCreationCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightCreationCommand() = default;

    public:
        /***
         * 3. Getters
         ***/
        LightType getLightType() const;
        const PackableColor& getLightColor() const;


        /***
         * 4. Buffer pre-reading
         ***/
        static LightType getLightType( const void* buffer );


        /***
         * 5. Operators
         ***/
        LightCreationCommand& operator=( const LightCreationCommand& ) = delete;
        LightCreationCommand& operator=( LightCreationCommand&& ) = delete;
};

typedef std::shared_ptr< LightCreationCommand > LightCreationCommandPtr;
typedef std::shared_ptr< const LightCreationCommand > LightCreationCommandConstPtr;

} // namespace como


#endif // LIGHT_CREATION_COMMAND_HPP
