/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
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
#include <client/models/3d/materials/material.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <common/ids/material_id.hpp>
#include <string>
#include <QObject>
#include <memory>

namespace como {

typedef std::map< MaterialID, UserID > MaterialsOwnershipMap;

class MaterialsManager : public QObject
{
    Q_OBJECT

    private:
        std::map< MaterialID, MaterialPtr > materials_;
        MaterialsOwnershipMap materialsOwners_;

        MaterialID nextLocalMaterialID_;
        const UserID& localUserID_;
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
        void createMaterial( const MaterialID& id, const std::string& namePrefix );


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
         * 6. Operators
         ***/
        MaterialsManager& operator = ( const MaterialsManager& ) = delete;
        MaterialsManager& operator = ( MaterialsManager&& ) = delete;


        /***
         * 7. Signals
         ***/
    signals:
        void materialCreated( MaterialID id, const std::string& name );
        void materialSelectionConfirmed( MaterialPtr material );
        void materialSelectionDenied( MaterialID material );
};

typedef std::shared_ptr< MaterialsManager > MaterialsManagerPtr;

} // namespace como

#endif // MATERIALS_MANAGER_HPP
