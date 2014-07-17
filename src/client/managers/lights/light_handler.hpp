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

#ifndef LIGHT_HANDLER_HPP
#define LIGHT_HANDLER_HPP

#include <client/managers/server_interface/server_interface.hpp>
#include <client/models/3d/lights/light_properties.hpp>
#include <functional>

namespace como {

class LightHandler
{
    private:
        LightPropertiesSharedPtr light_;
        ResourceID lightID_;
        ServerInterfacePtr server_;

        // Call this method when a notification has changed.
        std::function< void(void) > notifyChange_;

    public:
        /***
         * 1. Construction
         ***/
        LightHandler( LightPropertiesSharedPtr light, ResourceID lightID, ServerInterfacePtr server, std::function< void(void) > notifyChange );
        LightHandler( const LightHandler& ) = delete;
        LightHandler( LightHandler&& ) = delete;

        /***
         * 2. Destruction
         ***/
        ~LightHandler() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getResourceID() const;
        PackableColor getLightColor() const;
        float getAmbientCoefficient() const;


        /***
         * 4. Seters
         ***/
        void setLightColor( const PackableColor& lightColor );
        void setAmbientCoefficient( float coefficient );


        /***
         * 5. Operators
         ***/
        LightHandler& operator = ( const LightHandler& ) = delete;
        LightHandler& operator = ( LightHandler&& ) = delete;
};

typedef std::shared_ptr< LightHandler > LightHandlerPtr;
typedef std::shared_ptr< const LightHandler > LightHandlerConstPtr;

} // namespace como

#endif // LIGHT_HANDLER_HPP
