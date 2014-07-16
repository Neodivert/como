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

#ifndef LIGHT_AMBIENT_COEFFICIENT_CHANGE_COMMAND_HPP
#define LIGHT_AMBIENT_COEFFICIENT_CHANGE_COMMAND_HPP

#include "light_command.hpp"

namespace como {

class LightAmbientCoefficientChangeCommand : public LightCommand
{
    private:
        PackableFloat ambientCoefficient_;

    public:
        /***
         * 1. Construction
         ***/
        LightAmbientCoefficientChangeCommand();
        LightAmbientCoefficientChangeCommand( UserID userID, LightID lightID, float ambientCoefficient );
        LightAmbientCoefficientChangeCommand( const LightAmbientCoefficientChangeCommand& ) = delete;
        LightAmbientCoefficientChangeCommand( LightAmbientCoefficientChangeCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightAmbientCoefficientChangeCommand() = default;


        /***
         * 3. Getters
         ***/
        float getAmbientCoefficient() const;


        /***
         * 4. Operators
         ***/
        LightAmbientCoefficientChangeCommand& operator = ( const LightAmbientCoefficientChangeCommand& ) = delete;
        LightAmbientCoefficientChangeCommand& operator = ( LightAmbientCoefficientChangeCommand&& ) = delete;
};

} // namespace como

#endif // LIGHT_AMBIENT_COEFFICIENT_CHANGE_COMMAND_HPP
