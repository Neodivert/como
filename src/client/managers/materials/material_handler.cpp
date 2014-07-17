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

MaterialHandler::MaterialHandler( MaterialID materialID, MaterialPtr material, ServerInterfacePtr server ) :
    materialID_( materialID ),
    material_( material ),
    server_( server )
{}


MaterialHandler::MaterialHandler( MaterialHandler&& b ) :
    materialID_( b.materialID_ ),
    material_( b.material_ ),
    server_( b.server_ )
{}


/***
 * 3. Getters
 ***/

MaterialID MaterialHandler::getID() const
{
    return materialID_;
}


std::string MaterialHandler::getName() const
{
    return material_->getName();
}

PackableColor MaterialHandler::getColor() const
{
    return material_->getColor();
}

PackableColor MaterialHandler::getAmbientReflectivity() const
{
    return material_->getAmbientReflectivity();
}

PackableColor MaterialHandler::getDiffuseReflectivity() const
{
    return material_->getDiffuseReflectivity();
}

PackableColor MaterialHandler::getSpecularReflectivity() const
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

void MaterialHandler::setColor( const PackableColor& color )
{
    material_->setColor( color );

    server_->sendCommand( CommandConstPtr( new MaterialColorChangeCommand( server_->getLocalUserID(), materialID_, color ) ) );

    notifyObservers();
}

void MaterialHandler::setAmbientReflectivity( const PackableColor& ambientReflectivity )
{
    material_->setAmbientReflectivity( ambientReflectivity );

    server_->sendCommand( CommandConstPtr( new MaterialAmbientReflectivityChangeCommand( server_->getLocalUserID(), materialID_, ambientReflectivity ) ) );

    notifyObservers();
}

void MaterialHandler::setDiffuseReflectivity( const PackableColor& diffuseReflectivity )
{
    material_->setDiffuseReflectivity( diffuseReflectivity );

    server_->sendCommand( CommandConstPtr( new MaterialDiffuseReflectivityChangeCommand( server_->getLocalUserID(), materialID_, diffuseReflectivity ) ) );

    notifyObservers();
}

void MaterialHandler::setSpecularReflectivity( const PackableColor& specularReflectivity )
{
    material_->setSpecularReflectivity( specularReflectivity );

    server_->sendCommand( CommandConstPtr( new MaterialSpecularReflectivityChangeCommand( server_->getLocalUserID(), materialID_, specularReflectivity ) ) );

    notifyObservers();
}

void MaterialHandler::setSpecularExponent( float specularExponent )
{
    material_->setSpecularExponent( specularExponent );

    server_->sendCommand( CommandConstPtr( new MaterialSpecularExponentChangeCommand( server_->getLocalUserID(), materialID_, specularExponent ) ) );

    notifyObservers();
}


/***
 * 3. Operators
 ***/

MaterialHandler& MaterialHandler::operator = ( MaterialHandler&& b )
{
    materialID_ = b.materialID_;
    material_ = b.material_;
    server_ = b.server_;

    return *this;
}

} // namespace como
