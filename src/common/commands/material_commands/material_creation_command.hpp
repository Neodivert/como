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

#ifndef MATERIAL_CREATION_COMMAND_HPP
#define MATERIAL_CREATION_COMMAND_HPP

#include "material_command.hpp"
#include <common/mesh_info/material_info.hpp>

namespace como {

class MaterialCreationCommand : public MaterialCommand
{
    private:
        PackableString<64> materialName_;
        PackableColor materialAmbientReflectivity_;
        PackableColor materialDiffuseReflectivity_;
        PackableColor materialSpecularReflectivity_;
        PackableFloat materialSpecularExponent_;


    /***
     * 1. Construction
     ***/
    public:
        MaterialCreationCommand();
        MaterialCreationCommand( const MaterialID& materialID, const std::string& materialName  );
        MaterialCreationCommand( MaterialCreationCommand& b );
        MaterialCreationCommand( MaterialCreationCommand&& ) = delete;

    /***
     * 2. Destruction
     ***/
        ~MaterialCreationCommand() = default;


    /***
     * 3. Getters
     ***/
        MaterialInfo getMaterialInfo() const;


    /***
     * 4. Operators
     ***/
        MaterialCreationCommand& operator = ( const MaterialCreationCommand& ) = delete;
        MaterialCreationCommand& operator = ( MaterialCreationCommand&& ) = delete;
};


typedef std::shared_ptr< MaterialCreationCommand > MaterialCreationCommandPtr;
typedef std::shared_ptr< const MaterialCreationCommand > MaterialCreationCommandConstPtr;

} // namespace como

#endif // MATERIAL_CREATION_COMMAND_HPP
