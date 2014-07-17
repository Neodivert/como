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

LightHandler::LightHandler( LightPropertiesSharedPtr light, ResourceID lightID, ServerInterfacePtr server, std::function< void(void) > notifyChange ) :
    light_( light ),
    lightID_( lightID ),
    server_( server ),
    notifyChange_( notifyChange )
{}


/***
 * 3. Getters
 ***/

ResourceID LightHandler::getResourceID() const
{
    return lightID_;
}


PackableColor LightHandler::getLightColor() const
{
    return light_->getLightColor();
}


float LightHandler::getAmbientCoefficient() const
{
    return light_->getAmbientCoefficient();
}


/***
 * 4. Seters
 ***/

void LightHandler::setLightColor( const PackableColor& lightColor)
{
    light_->setLightColor( lightColor );

    server_->sendCommand( CommandConstPtr( new LightColorChangeCommand( server_->getLocalUserID(), lightID_, lightColor ) ) );

    notifyChange_();
}


void LightHandler::setAmbientCoefficient( float coefficient )
{
    light_->setAmbientCoefficient( coefficient );

    server_->sendCommand( CommandConstPtr( new LightAmbientCoefficientChangeCommand( server_->getLocalUserID(), lightID_, coefficient ) ) );

    notifyChange_();
}

} // namespace como
