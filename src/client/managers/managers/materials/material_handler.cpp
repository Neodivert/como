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

#include "material_handler.hpp"

namespace como {

/***
 * 1. Construction
 ***/

MaterialHandler::MaterialHandler( ResourceID materialID, MaterialPtr material, ServerInterfacePtr server ) :
    ServerWriter( server ),
    materialID_( materialID ),
    material_( material )
{}


MaterialHandler::MaterialHandler( MaterialHandler&& b ) :
    ServerWriter( b.server() ),
    materialID_( b.materialID_ ),
    material_( b.material_ )
{}


/***
 * 3. Getters
 ***/

ResourceID MaterialHandler::getID() const
{
    return materialID_;
}


std::string MaterialHandler::getName() const
{
    return material_->name();
}


Color MaterialHandler::getColor() const
{
    return material_->getColor();
}

Color MaterialHandler::getAmbientReflectivity() const
{
    return material_->getAmbientReflectivity();
}

Color MaterialHandler::getDiffuseReflectivity() const
{
    return material_->getDiffuseReflectivity();
}

Color MaterialHandler::getSpecularReflectivity() const
{
    return material_->getSpecularReflectivity();
}

float MaterialHandler::getSpecularExponent() const
{
    return material_->getSpecularExponent();
}


/***
 * 4. Setters
 ***/

void MaterialHandler::setColor( const Color& color )
{
    material_->setColor( color );

    sendCommandToServer( CommandConstPtr( new MaterialColorChangeCommand( localUserID(), materialID_, color ) ) );

    notifyObservers();
}

void MaterialHandler::setAmbientReflectivity( const Color& ambientReflectivity )
{
    material_->setAmbientReflectivity( ambientReflectivity );

    sendCommandToServer( CommandConstPtr( new MaterialAmbientReflectivityChangeCommand( localUserID(), materialID_, ambientReflectivity ) ) );

    notifyObservers();
}

void MaterialHandler::setDiffuseReflectivity( const Color& diffuseReflectivity )
{
    material_->setDiffuseReflectivity( diffuseReflectivity );

    sendCommandToServer( CommandConstPtr( new MaterialDiffuseReflectivityChangeCommand( localUserID(), materialID_, diffuseReflectivity ) ) );

    notifyObservers();
}

void MaterialHandler::setSpecularReflectivity( const Color& specularReflectivity )
{
    material_->setSpecularReflectivity( specularReflectivity );

    sendCommandToServer( CommandConstPtr( new MaterialSpecularReflectivityChangeCommand( localUserID(), materialID_, specularReflectivity ) ) );

    notifyObservers();
}

void MaterialHandler::setSpecularExponent( float specularExponent )
{
    material_->setSpecularExponent( specularExponent );

    sendCommandToServer( CommandConstPtr( new MaterialSpecularExponentChangeCommand( localUserID(), materialID_, specularExponent ) ) );

    notifyObservers();
}

} // namespace como
