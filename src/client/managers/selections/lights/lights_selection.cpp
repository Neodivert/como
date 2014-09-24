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

#include "lights_selection.hpp"

namespace como {


/***
 * 1. Construction
 ***/

LightsSelection::LightsSelection( glm::vec4 borderColor ) :
    EntitiesSet( borderColor )
{}


/***
 * 3. Getters
 ***/

PackableColor LightsSelection::getLightColor() const
{
    if( resources_.size() == 0 ){
        throw std::runtime_error( "LightsSelection::getLightColor() called on empty LightsSelection" );
    }
    return resources_.begin()->second->getLightColor();
}


float LightsSelection::getAmbientCoefficient() const
{
    if( resources_.size() == 0 ){
        throw std::runtime_error( "LightsSelection::getAmbientCoefficient() called on empty LightsSelection" );
    }
    return resources_.begin()->second->getAmbientCoefficient();
}


/***
 * 4. Setters
 ***/

void LightsSelection::setLightColor( const PackableColor& color )
{
    for( auto& lightPair : resources_ ){
        lightPair.second->setLightColor( color );
    }
}


void LightsSelection::setAmbientCoefficient( float coefficient )
{
    for( auto& lightPair : resources_ ){
        lightPair.second->setAmbientCoefficient( coefficient );
    }
}


/***
 * 5. Shader communication
 ***/

void LightsSelection::sendToShader( OpenGL &openGL, const glm::mat4& viewMatrix ) const
{
    for( const auto& light : resources_ ){
        light.second->sendToShader( openGL, viewMatrix );
    }
}

} // namespace como
