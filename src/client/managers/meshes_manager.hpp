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

#include <client/managers/drawables_manager/drawables_manager.hpp>
#include <client/managers/drawables_selection/meshes_selection.hpp>

namespace como {

class MeshesManager;
typedef std::shared_ptr< MeshesManager > MeshesManagerPtr;

typedef std::map< UserID, MeshesSelection > MeshesSelectionMap;

class MeshesManager : public ResourcesManager< Mesh >
{
    private:
        DrawablesManagerPtr drawablesManager_;

        MeshesSelection* nonSelectedMeshes_;
        MeshesSelectionMap meshesSelections_;

        bool newMeshesDisplayVertexNormals_;


    public:
        /***
         * 1. Construction
         ***/
        MeshesManager( ServerInterfacePtr server, DrawablesManagerPtr drawablesManager, LogPtr log );
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
        MeshesSelection* getLocalUserSelection();


        /***
         * 5. Setters
         ***/
        void displayVertexNormals( bool display );


        /***
         * 6. Meshes management
         ***/
        ResourceID createMesh( MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials );
        void createMesh( ResourceID meshID, MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials );


        /***
         * 4. Resources selections management // TODO: Make this protected and make AbstractOwnershipManager observe a UsersList interface.
         ***/
        virtual void registerUser( UserID userID );
        virtual void removeUser( UserID userID );

    protected:
        /***
         * 5. Resources ownership management
         ***/
        virtual void lockResource( const ResourceID& resourceID, UserID userID );
        virtual void unlockResourcesSelection( UserID userID );
        virtual void deleteResourcesSelection( UserID userID );
};

} // namespace como

#endif // MESHES_MANAGER_HPP
