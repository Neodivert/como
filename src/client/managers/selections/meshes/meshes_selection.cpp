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

#include "meshes_selection.hpp"

namespace como {

/***
 * 1. Construction
 ***/

MeshesSelection::MeshesSelection( glm::vec4 borderColor ) :
    EntitiesSet( borderColor ),
    displayEdges_( true )
{}


/***
 * 3. Resources management
 ***/

void MeshesSelection::addResource( ResourceID id, std::unique_ptr<Mesh> resource, bool notifyObservers )
{
    resource->displayEdges( displayEdges_ );

    EntitiesSet<Mesh>::addResource( id, std::move( resource ), notifyObservers );
}


/***
 * 4. Getters
 ***/

bool MeshesSelection::containsResource( const ResourceID& resourceID ) const
{
    return resources_.count( resourceID );
}


std::string MeshesSelection::getResourceName( const ResourceID& resourceID ) const
{
    return resources_.at( resourceID )->name();
}


void MeshesSelection::intersects( glm::vec3 r0, glm::vec3 r1, float& t, unsigned int* triangle ) const
{
    for( const auto& mesh : resources_ ){
        mesh.second->intersects( r0, r1, t, triangle );
    }
}

bool MeshesSelection::containsProperty( const void* property ) const
{
    for( const auto& mesh : resources_ ){
        if( mesh.second->containsProperty( property ) ){
            return true;
        }
    }
    return false;
}

ElementsMeetingCondition MeshesSelection::displaysVertexNormals() const
{
    // Check whether first mesh in the selection is displaying normals or not.
    bool firstMeshDisplaysVertexNormals = false;
    if( resources_.size() ){
        firstMeshDisplaysVertexNormals = resources_.begin()->second->displaysVertexNormals();
    }

    // Check whether any other mesh in the selection isn't displaying vertex
    // normals when the first mesh does so or viceversa.
    for( const auto& meshPair : resources_ ){
        if( meshPair.second->displaysVertexNormals() != firstMeshDisplaysVertexNormals ){
            return ElementsMeetingCondition::SOME;
        }
    }

    // All the meshes are whether displaying normals or not, return the
    // appropiate value.
    if( firstMeshDisplaysVertexNormals ){
        return ElementsMeetingCondition::ALL;
    }else{
        return ElementsMeetingCondition::NONE;
    }
}


/***
 * 5. Setters
 ***/

void MeshesSelection::displayVertexNormals( bool display )
{
    for( auto& mesh : resources_ ){
        mesh.second->displayVertexNormals( display );
    }
    notifyObservers();
}


void MeshesSelection::displayEdges( bool display )
{
    for( auto& mesh : resources_ ){
        mesh.second->displayEdges( display );
    }
    displayEdges_ = display;

    notifyObservers();
}


} // namespace como
