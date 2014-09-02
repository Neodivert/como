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

#ifndef MATERIALS_MANAGER_HPP
#define MATERIALS_MANAGER_HPP

#include <map>
#include <client/managers/managers/materials/material_handler.hpp>
#include <common/ids/resource_id.hpp>
#include <string>
#include <QObject>
#include <memory>
#include <common/commands/material_commands/material_commands.hpp>
#include <functional>
#include <common/utilities/observable_container/observable_container.hpp>
#include <common/mesh_info/material_info.hpp>
#include <client/managers/managers/resources/resources_manager.hpp>
#include <client/managers/selections/materials/local_materials_selection.hpp>

namespace como {

typedef std::map< ResourceID, UserID > MaterialsOwnershipMap;

struct MaterialHeader {
    ResourceID id;
    std::string name;
};

typedef std::list< MaterialHeader > MaterialsHeadersList;


class MaterialsManager : public QObject, public ServerWriter, public Observer, public ObservableContainer<ResourceID>
{
    Q_OBJECT

    // TODO: Use a better alternative for only allowing MeshesManager to
    // lock and remove materials and anyone to retrieve current material
    // handler.
    // - Maybe make both methods public, and create a base base class X
    // with the method X::selectMaterial(). MaterialsManager will be
    // private to MeshesManager and X will be public?
    friend class MeshesManager;

    private:
        std::map< ResourceID, MaterialPtr > materials_;
        MaterialsOwnershipMap materialsOwners_;
        std::map< ResourceID, std::vector< ResourceID > > meshMaterials_;

        MaterialHandlerPtr materialHandler_;

    public:
        /***
         * 1. Creation
         ***/
        MaterialsManager() = delete;
        MaterialsManager( ServerInterfacePtr server, LogPtr log );
        MaterialsManager( const MaterialsManager& ) = delete;
        MaterialsManager( MaterialsManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MaterialsManager() = default;


        /***
         * 3. Material creation
         ***/
    public:
        ResourceID createMaterial( const MaterialInfo& materialInfo );
        void createMaterials( const ResourceID& meshID, const std::vector<MaterialInfo>& materialsInfo, ResourceID& firstMaterialID );
        void createRemoteMaterials( const ResourceID& meshID, const std::vector< MaterialInfo >& materialsInfo, const ResourceID& firstMaterialID );
    //private:
        void createMaterial( ResourceID id, const MaterialInfo &materialInfo );



        /***
         * 4. Material selection
         ***/
    public:
        MaterialHandlerPtr selectMaterial( const ResourceID& id );


        /***
         * 5. Getters
         ***/
    public:
        MaterialsHeadersList getLocalMaterialsHeaders() const;
        bool materialOwnedByLocalUser( const ResourceID& resourceID ) const;
        virtual string getResourceName( const ResourceID& resourceID ) const;
        MaterialConstPtr getMaterial( const ResourceID& id ) const;
        std::vector< MaterialConstPtr > getMaterials( const ResourceID& firstMaterialID, unsigned int nMaterials ) const;


        /***
         * 6. Remote command execution
         ***/
        void executeRemoteCommand( const MaterialCommand& command );


        /***
         * 7. Updating
         ***/
        virtual void update();


        /***
         * 8. Materials locking
         ***/
    private:
        void lockMaterial( const ResourceID &materialID, UserID newOwner );
        void lockMeshMaterials( const ResourceID& meshID, UserID newOwner );

        void unlockMaterial( const ResourceID &materialID );
        void unlockMeshMaterials( const ResourceID &meshID );
        void unlockUserMaterials( UserID userID );


        /***
         * 9. Materials destruction
         ***/
        void removeMaterial( const ResourceID& materialID );
        void removeMeshMaterials( const ResourceID& meshID );
        void removeUserMaterials( UserID userID );


    public:
        /***
         * 10. Operators
         ***/
        MaterialsManager& operator = ( const MaterialsManager& ) = delete;
        MaterialsManager& operator = ( MaterialsManager&& ) = delete;


        /***
         * 11. Signals
         ***/
    signals:
        void materialSelectionConfirmed( MaterialHandlerPtr material );
        void materialSelectionDenied( ResourceID material );
};

typedef std::shared_ptr< MaterialsManager > MaterialsManagerPtr;

} // namespace como

#endif // MATERIALS_MANAGER_HPP
