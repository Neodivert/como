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

#ifndef MESHES_SELECTION_HPP
#define MESHES_SELECTION_HPP

#include <client/models/3d/mesh.hpp>
#include <map>
#include <common/ids/resource_id.hpp>

namespace como {

typedef std::map< ResourceID, MeshPtr > MeshesMap;

class MeshesSelection;
typedef std::shared_ptr< MeshesSelection > MeshesSelectionPtr;


class MeshesSelection : public AbstractMesh
{
    private:
        MeshesMap meshes_;


    public:
        /***
         * 1. Construction
         ***/
        MeshesSelection() = default;
        MeshesSelection( const MeshesSelection& ) = default;
        MeshesSelection( MeshesSelection&& ) = default;
        virtual DrawablePtr clone();


        /***
         * 2. Destruction
         ***/
        ~MeshesSelection() = default;


        /***
         * 3. Getters
         ***/
        virtual glm::vec4 getCentroid() const;
        virtual void intersects( glm::vec3 r0, glm::vec3 r1, float& t, unsigned int* triangle ) const;
        virtual bool containsProperty( const void *property ) const;


        /***
         * 4. Setters
         ***/
        virtual void displayVertexNormals( bool display );


        /***
         * 5. Meshes management
         ***/
        void addMesh( const ResourceID& id, MeshPtr mesh );
        void removeMesh( const ResourceID& id );
        void clear();


        /***
         * 6. Drawing
         ***/
        virtual void draw( OpenGLPtr openGL, const glm::mat4& viewProjMatrix, const GLfloat* contourColor ) const;


        /***
         * 7. Operators
         ***/
        MeshesSelection& operator = ( const MeshesSelection& ) = default;
        MeshesSelection& operator = ( MeshesSelection&& ) = default;
};

} // namespace como

#endif // MESHES_SELECTION_HPP
