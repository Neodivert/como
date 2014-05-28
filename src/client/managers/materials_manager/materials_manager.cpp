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

#include "materials_manager.hpp"
#include <boost/lexical_cast.hpp>

namespace como {

/***
 * 1. Creation
 ***/

MaterialsManager::MaterialsManager( UserID localUserID, ServerInterfacePtr server, LogPtr log ) :
    nextLocalMaterialID_( localUserID, 0 ),
    server_( server ),
    log_( log )
{}


/***
 * 3. Materials creation
 ***/

MaterialID MaterialsManager::createMaterial( const std::string& namePrefix )
{
    createMaterial( nextLocalMaterialID_, namePrefix );

    return nextLocalMaterialID_++;
}

void MaterialsManager::createMaterial( const MaterialID& id, const std::string& namePrefix )
{
    // Generate a name for the new material from the given ID and name prefix.
    std::string materialName =
            namePrefix +
            std::string( " # " ) +
            boost::lexical_cast< std::string >( static_cast< int >( id.getCreatorID() ) ) +
            std::string( "," ) +
            boost::lexical_cast< std::string >( static_cast< int >( id.getMaterialIndex() ) );

    // Create the new material and insert it in the materials container.
    materials_[id] = MaterialPtr( new Material( materialName ) );

    // Set the creator of the material as its current owner.
    materialsOwners_[id] = id.getCreatorID();

    // Emit a signal indicating that a new material has been created.
    emit materialCreated( id, materials_.at( id )->getName() );
}


/***
 * 4. Getters
 ***/

MaterialConstPtr MaterialsManager::getMaterial( const MaterialID& id ) const
{
    return materials_.at( id );
}


} // namespace como
