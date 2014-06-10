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

#ifndef LIGHT_COLOR_CHANGE_COMMAND_HPP
#define LIGHT_COLOR_CHANGE_COMMAND_HPP

#include "light_command.hpp"

namespace como {

class LightColorChangeCommand : public LightCommand
{
    private:
        PackableColor lightColor_;

    public:
        /***
         * 1. Construction
         ***/
        LightColorChangeCommand();
        LightColorChangeCommand( PackableLightID lightID, PackableColor lightColor );
        LightColorChangeCommand( const LightColorChangeCommand& ) = delete;
        LightColorChangeCommand( LightColorChangeCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightColorChangeCommand() = default;


        /***
         * 3. Getters
         ***/
        PackableColor getLightColor() const;


        /***
         * 4. Operators
         ***/
        LightColorChangeCommand& operator = ( const LightColorChangeCommand& ) = delete;
        LightColorChangeCommand& operator = ( LightColorChangeCommand&& ) = delete;
};

} // namespace como

#endif // LIGHT_COLOR_CHANGE_COMMAND_HPP
