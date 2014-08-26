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

#ifndef MESHES_MANAGER_HPP
#define MESHES_MANAGER_HPP

#include <client/managers/specialized_entities_manager.hpp>
#include <client/managers/drawables_selection/meshes_selection.hpp>
#include <client/managers/selections/meshes/local_meshes_selection.hpp>

namespace como {

enum class MeshEdgesDisplayFrequency {
    ALWAYS,
    ONLY_WHEN_SELECTED
};

class MeshesManager;
typedef std::shared_ptr< MeshesManager > MeshesManagerPtr;

class MeshesManager : public SpecializedEntitiesManager< Mesh, MeshesSelection, LocalMeshesSelection >
{
    public:
        /***
         * 1. Construction
         ***/
        MeshesManager( ServerInterfacePtr server, LogPtr log );
        MeshesManager() = delete;
        MeshesManager( const MeshesManager& ) = delete;
        MeshesManager( MeshesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MeshesManager() = default;


        /***
         * 3. Operators
         ***/
        MeshesManager& operator = ( const MeshesManager& ) = delete;
        MeshesManager& operator = ( MeshesManager&& ) = delete;


        /***
         * 4. Getters
         ***/
        virtual string getResourceName( const ResourceID& resourceID ) const;
        ElementsMeetingCondition displaysVertexNormals() const;
        unsigned int getTotalMeshes() const;


        /***
         * 5. Setters
         ***/
        void displayVertexNormals( bool display );
        void displayEdges( MeshEdgesDisplayFrequency frequency );


        /***
         * 6. Meshes management
         ***/
        ResourceID createMesh( MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials );
        void createMesh( ResourceID meshID, MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials );


        /***
         * 4. Resources selections management // TODO: Make this protected and make AbstractOwnershipManager observe a UsersList interface.
         ***/
        // TODO: Remove.
        virtual void registerUser( UserID userID );
        virtual void removeUser( UserID userID );


    private:
        bool newMeshesDisplayVertexNormals_;
};

} // namespace como

#endif // MESHES_MANAGER_HPP
