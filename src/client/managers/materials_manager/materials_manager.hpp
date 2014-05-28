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

namespace como {

typedef std::map< MaterialID, UserID > MaterialsOwnershipMap;

class MaterialsManager
{
    private:
        std::map< MaterialID, MaterialPtr > materials_;
        MaterialsOwnershipMap materialsOwners_;

        MaterialID nextLocalMaterialID_;
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
        ~MaterialsManager() = delete;


        /***
         * 3. Materials creation
         ***/
    public:
        MaterialID createMaterial( const std::string& namePrefix );
    private:
        void createMaterial( const MaterialID& id, const std::string& namePrefix );


        /***
         * 4. Operators
         ***/
        MaterialsManager& operator = ( const MaterialsManager& ) = delete;
        MaterialsManager& operator = ( MaterialsManager&& ) = delete;
};

} // namespace como

#endif // MATERIALS_MANAGER_HPP
