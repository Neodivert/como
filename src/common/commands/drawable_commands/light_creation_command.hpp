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

#ifndef LIGHT_CREATION_COMMAND_HPP
#define LIGHT_CREATION_COMMAND_HPP

#include <common/commands/drawable_commands/mesh_creation_command.hpp>
#include <common/packables/packables.hpp>

namespace como {

enum class LightType : std::uint8_t
{
    DIRECTIONAL_LIGHT = 0
};


class LightCreationCommand : public MeshCreationCommand
{
    private:
        const PackableUint8< LightType > lightType_;
        PackableColor lightColor_;

    protected:
        /***
         * 1. Construction
         ***/
        LightCreationCommand( LightType lightType, PackableDrawableID drawableID, const MaterialID& materialID, const PackableColor& lightColor );
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

} // namespace como


#endif // LIGHT_CREATION_COMMAND_HPP
