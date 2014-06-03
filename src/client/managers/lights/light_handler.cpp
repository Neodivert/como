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

#include "light_handler.hpp"

namespace como {

/***
 * 1. Construction
 ***/

LightHandler::LightHandler( LightPtr light, LightID lightID, ServerInterfacePtr server ) :
    light_( light ),
    lightID_( lightID ),
    server_( server )
{}


/***
 * 3. Getters
 ***/

PackableColor LightHandler::getLightColor() const
{
    return light_->getLightColor();
}


/***
 * 4. Seters
 ***/

void LightHandler::setLightColor( const PackableColor& lightColor)
{
    light_->setLightColor( lightColor );
}




} // namespace como
