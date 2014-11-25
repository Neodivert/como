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
#include <common/primitives/primitive_data/material_info.hpp>
#include <common/packables/packable_string.hpp>
#include <common/packables/packable_float.hpp>

namespace como {

// NOTE: MaterialCreationCommand isn't used / supported on this version of
// COMO.
class MaterialCreationCommand : public MaterialCommand
{
    private:
        PackableString materialName_;
        PackableColor materialAmbientReflectivity_;
        PackableColor materialDiffuseReflectivity_;
        PackableColor materialSpecularReflectivity_;
        PackableFloat materialSpecularExponent_;


    /***
     * 1. Construction
     ***/
    public:
        MaterialCreationCommand();
        MaterialCreationCommand( const ResourceID& materialID, const std::string& materialName  );
        MaterialCreationCommand( const MaterialCreationCommand& b );
        MaterialCreationCommand( MaterialCreationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( MaterialCreationCommand )

    /***
     * 2. Destruction
     ***/
        virtual ~MaterialCreationCommand() = default;


    /***
     * 3. Getters
     ***/
        MaterialInfo getMaterialData() const;


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
