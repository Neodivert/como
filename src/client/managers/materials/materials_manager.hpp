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

#include <list>
#include <map>
#include <client/managers/materials/material_handler.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <common/ids/material_id.hpp>
#include <string>
#include <QObject>
#include <memory>
#include <common/commands/material_commands/material_commands.hpp>
#include <client/models/utilities/changeable/changeable.hpp>
#include <functional>
#include <common/utilities/observer_pattern/observable_container.hpp>
#include <common/mesh_info/material_info.hpp>

namespace como {

typedef std::map< MaterialID, UserID > MaterialsOwnershipMap;

class MaterialsManager : public QObject, public Changeable, public ObservableContainer< MaterialID >, public Observer
{
    Q_OBJECT

    private:
        std::map< MaterialID, MaterialPtr > materials_;
        MaterialsOwnershipMap materialsOwners_;

        MaterialID nextLocalMaterialID_;
        UserID localUserID_; // TODO: Use a reference or pointer to nextLocalMaterialID_.getCreatorID();
        ServerInterfacePtr server_;
        LogPtr log_;

        MaterialHandlerPtr materialHandler_;

    public:
        /***
         * 1. Creation
         ***/
        MaterialsManager() = delete;
        MaterialsManager( UserID localUserID, ServerInterfacePtr server, LogPtr log );
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
        MaterialID createMaterial( const MaterialInfo& materialInfo );
        void createMaterials( const std::vector<MaterialInfo>& materialsInfo, MaterialID& firstMaterialID );
        void createRemoteMaterials( const std::vector< MaterialInfo >& materialsInfo, const MaterialID& firstMaterialID );
    //private:
        void createMaterial( MaterialID id, const MaterialInfo &materialInfo );



        /***
         * 4. Material selection
         ***/
    public:
        void selectMaterial( const MaterialID& id );
    private:
        void selectMaterial( UserID userID, const MaterialID& id );


        /***
         * 5. Getters
         ***/
    public:
        MaterialConstPtr getMaterial( const MaterialID& id ) const;
        std::vector< MaterialConstPtr > getMaterials( const MaterialID& firstMaterialID, unsigned int nMaterials ) const;


        /***
         * 6. Remote command execution
         ***/
        void executeRemoteCommand( MaterialCommandConstPtr command );


        /***
         * 7. Updating
         ***/
        virtual void onChange(){}
        virtual void update();


        /***
         * 7. Operators
         ***/
        MaterialsManager& operator = ( const MaterialsManager& ) = delete;
        MaterialsManager& operator = ( MaterialsManager&& ) = delete;


        /***
         * 8. Signals
         ***/
    signals:
        void materialSelectionConfirmed( MaterialHandlerPtr material );
        void materialSelectionDenied( MaterialID material );
};

typedef std::shared_ptr< MaterialsManager > MaterialsManagerPtr;

} // namespace como

#endif // MATERIALS_MANAGER_HPP
