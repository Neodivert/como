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
#include <client/managers/material_handler.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <common/ids/material_id.hpp>
#include <string>
#include <QObject>
#include <memory>
#include <common/commands/material_commands/material_commands.hpp>
#include <client/models/utilities/changeable/changeable.hpp>
#include <functional>

namespace como {

typedef std::map< MaterialID, UserID > MaterialsOwnershipMap;

class MaterialsManager : public QObject, public Changeable
{
    Q_OBJECT

    private:
        std::map< MaterialID, MaterialPtr > materials_;
        MaterialsOwnershipMap materialsOwners_;

        MaterialID nextLocalMaterialID_;
        UserID localUserID_; // TODO: Use a reference or pointer to nextLocalMaterialID_.getCreatorID();
        ServerInterfacePtr server_;
        LogPtr log_;


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
        MaterialID createMaterial( const std::string& namePrefix );
    private:
        void createMaterial( const MaterialID& id, const std::string& name );


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


        /***
         * 6. Remote command execution
         ***/
        void executeRemoteCommand( MaterialCommandConstPtr command );


        /***
         * 7. Updating
         ***/
    private:
        virtual void onChange(){}


        /***
         * 7. Operators
         ***/
        MaterialsManager& operator = ( const MaterialsManager& ) = delete;
        MaterialsManager& operator = ( MaterialsManager&& ) = delete;


        /***
         * 8. Signals
         ***/
    signals:
        void materialCreated( MaterialID id, std::string name );
        void materialSelectionConfirmed( MaterialHandlerPtr material );
        void materialSelectionDenied( MaterialID material );
};

typedef std::shared_ptr< MaterialsManager > MaterialsManagerPtr;

} // namespace como

#endif // MATERIALS_MANAGER_HPP
