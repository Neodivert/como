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
#include <common/utilities/observer_pattern/observable.hpp>
#include <client/models/3d/entities_set.hpp>

namespace como {

typedef std::map< ResourceID, MeshPtr > MeshesMap;

class MeshesSelection;
typedef std::shared_ptr< MeshesSelection > MeshesSelectionPtr;

enum class ElementsMeetingCondition {
    ALL,
    NONE,
    SOME
};

class MeshesSelection : public virtual EntitiesSet< Mesh >, public AbstractMesh
{
    public:
        /***
         * 1. Construction
         ***/
        MeshesSelection( glm::vec4 borderColor = DEFAULT_BORDER_COLOR );
        MeshesSelection( const MeshesSelection& ) = default;
        MeshesSelection( MeshesSelection&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~MeshesSelection() = default;


        /***
         * 3. Getters
         ***/
        bool containsResource( const ResourceID& resourceID ) const;
        std::string getResourceName( const ResourceID& resourceID ) const;
        virtual void intersects( glm::vec3 r0, glm::vec3 r1, float& t, unsigned int* triangle ) const;
        virtual bool containsProperty( const void *property ) const;
        virtual ElementsMeetingCondition displaysVertexNormals() const;


        /***
         * 4. Setters
         ***/
        virtual void displayVertexNormals( bool display );


        /***
         * 5. Operators
         ***/
        MeshesSelection& operator = ( const MeshesSelection& ) = default;
        MeshesSelection& operator = ( MeshesSelection&& ) = default;
};

} // namespace como

#endif // MESHES_SELECTION_HPP
