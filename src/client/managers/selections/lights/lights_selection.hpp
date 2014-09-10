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

#ifndef LIGHT_SSELECTION_HPP
#define LIGHT_SSELECTION_HPP

#include <client/models/3d/lights/lights.hpp>
#include <client/models/3d/entities_set.hpp>

namespace como {

class LightsSelection : public virtual EntitiesSet<DirectionalLight>, public AbstractLight
{
    public:
        /***
         * 1. Construction
         ***/
        LightsSelection( glm::vec4 borderColor = DEFAULT_BORDER_COLOR );
        LightsSelection( const LightsSelection& ) = delete;
        LightsSelection( LightsSelection&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~LightsSelection() = default;


        /***
         * 3. Getters
         ***/
        virtual PackableColor getLightColor() const;
        virtual float getAmbientCoefficient() const;


        /***
         * 4. Setters
         ***/
        virtual void setLightColor( const PackableColor& color );
        virtual void setAmbientCoefficient( float coefficient );


        /***
         * 5. Shader communication
         ***/
        void sendToShader( OpenGL& openGL ) const;


        /***
         * 6. Operators
         ***/
        LightsSelection& operator = ( const LightsSelection& ) = delete;
        LightsSelection& operator = ( LightsSelection&& ) = delete;
};

} // namespace como

#endif // LIGHT_SSELECTION_HPP
