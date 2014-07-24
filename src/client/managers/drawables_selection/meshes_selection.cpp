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

DrawablePtr MeshesSelection::clone()
{
    return DrawablePtr( new MeshesSelection( *this ) );
}


/***
 * 2. Getters
 ***/

glm::vec4 MeshesSelection::getCentroid() const
{
    glm::vec4 centroid( 0.0f );

    for( auto mesh : meshes_ ){
        centroid += mesh.second->getCentroid();
    }

    if( meshes_.size() ){
        centroid /= meshes_.size();
    }

    return centroid;
}


void MeshesSelection::intersects( glm::vec3 r0, glm::vec3 r1, float& t, unsigned int* triangle ) const
{
    for( auto mesh : meshes_ ){
        mesh.second->intersects( r0, r1, t, triangle );
    }
}

bool MeshesSelection::containsProperty( const void* property ) const
{
    for( auto mesh : meshes_ ){
        if( mesh.second->containsProperty( property ) ){
            return true;
        }
    }
    return false;
}


/***
 * 4. Setters
 ***/

void MeshesSelection::displayVertexNormals( bool display )
{
    for( auto mesh : meshes_ ){
        mesh.second->displayVertexNormals( display );
    }
}


/***
 * 5. Meshes management
 ***/

void MeshesSelection::addMesh( const ResourceID& id, MeshPtr mesh )
{
    meshes_[id] = mesh;
}


void MeshesSelection::removeMesh( const ResourceID& id )
{
    meshes_.erase( id );
}


void MeshesSelection::clear()
{
    meshes_.clear();
}


/***
 * 6. Drawing
 ***/

void MeshesSelection::draw( OpenGLPtr openGL, const glm::mat4& viewProjMatrix, const GLfloat* contourColor ) const
{
    for( auto mesh : meshes_ ){
        mesh.second->draw( openGL, viewProjMatrix, contourColor );
    }
}

} // namespace como
