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
    EntitiesSet( borderColor )
{}


DrawablePtr MeshesSelection::clone()
{
    return DrawablePtr( new MeshesSelection( *this ) );
}


/***
 * 2. Getters
 ***/

bool MeshesSelection::containsResource( const ResourceID& resourceID ) const
{
    return resources_.count( resourceID );
}


string MeshesSelection::getResourceName( const ResourceID& resourceID ) const
{
    return resources_.at( resourceID )->getName();
}


glm::vec3 MeshesSelection::centroid() const
{
    glm::vec3 centroid( 0.0f );

    for( auto mesh : resources_ ){
        centroid += mesh.second->centroid();
    }

    if( resources_.size() ){
        centroid /= resources_.size();
    }

    return centroid;
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
    for( auto mesh : resources_ ){
        if( mesh.second->displaysVertexNormals() != firstMeshDisplaysVertexNormals ){
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


unsigned int MeshesSelection::size() const
{
    return resources_.size();
}


/***
 * 4. Setters
 ***/

void MeshesSelection::displayVertexNormals( bool display )
{
    for( auto& mesh : resources_ ){
        mesh.second->displayVertexNormals( display );
    }
    notifyObservers();
}


/***
 * 5. Meshes management
 ***/

void MeshesSelection::addMesh( const ResourceID& id, MeshPtr mesh )
{
    resources_[id] = mesh;
    notifyObservers();
}


void MeshesSelection::removeMesh( const ResourceID& id )
{
    resources_.erase( id );
    notifyObservers();
}


void MeshesSelection::clear()
{
    resources_.clear();
    notifyObservers();
}


bool MeshesSelection::moveMesh( const ResourceID& meshID, MeshesSelection& dstMeshesSelection )
{
    if( resources_.count( meshID ) ){
        dstMeshesSelection.addMesh( meshID, resources_.at( meshID ) );
        removeMesh( meshID );

        return true;
    }else{
        return false;
    }
}


void MeshesSelection::moveAll( MeshesSelection& dstMeshesSelection )
{
    for( auto mesh : resources_ ){
        dstMeshesSelection.addMesh( mesh.first, mesh.second );
    }

    resources_.clear();
}


/***
 * 6. Drawing
 ***/

void MeshesSelection::draw( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec4* contourColor ) const
{
    for( auto mesh : resources_ ){
        mesh.second->draw( openGL, viewMatrix, projectionMatrix, contourColor );
    }
}

} // namespace como
