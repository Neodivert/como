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

#ifndef MATERIAL_HANDLER_HPP
#define MATERIAL_HANDLER_HPP

#include <client/managers/server_interface/server_interface.hpp>
#include <client/models/3d/materials/material.hpp>
#include <common/commands/material_commands/material_commands.hpp>
#include <functional>
#include <common/utilities/observer_pattern/observable.hpp>
#include <client/managers/utilities/server_writer.hpp>

namespace como {

// TODO: Inherit this class and Material from a common MaterialInterface.
class MaterialHandler : public ServerWriter, public Observable
{
    private:
        ResourceID materialID_; // TODO: Remove this attribute and use material_->id() instead.
        MaterialPtr material_;

    public:
        /***
         * 1. Construction
         ***/
        MaterialHandler() = default;
        MaterialHandler( ResourceID materialID, MaterialPtr material, ServerInterfacePtr server );
        MaterialHandler( const MaterialHandler& ) = delete;
        MaterialHandler( MaterialHandler&& );


        /***
         * 2. Destruction
         ***/
        ~MaterialHandler() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getID() const;
        std::string getName() const;
        Color getColor() const;
        Color getAmbientReflectivity() const;
        Color getDiffuseReflectivity() const;
        Color getSpecularReflectivity() const;
        float getSpecularExponent() const;


        /***
         * 4. Setters
         ***/
        void setColor( const Color& color );
        void setAmbientReflectivity( const Color& ambientReflectivity );
        void setDiffuseReflectivity( const Color& diffuseReflectivity );
        void setSpecularReflectivity( const Color& specularReflectivity );
        void setSpecularExponent( float specularExponent );


        /***
         * 3. Operators
         ***/
        MaterialHandler& operator = ( const MaterialHandler& ) = delete;
        MaterialHandler& operator = ( MaterialHandler&& ) = default;
};

typedef std::shared_ptr< MaterialHandler > MaterialHandlerPtr;

} // namespace como

#endif // MATERIAL_HANDLER_HPP
