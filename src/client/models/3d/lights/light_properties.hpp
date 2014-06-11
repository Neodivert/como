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

#ifndef LIGHT_PROPERTIES_HPP
#define LIGHT_PROPERTIES_HPP


#include "client/models/3d/drawable.hpp" // OpenGL::checkStatus() funciton and GL types.

#include <common/commands/light_commands/light_creation_command.hpp> // "LightType" type.
#include "abstract_light.hpp"

namespace como {

class LightProperties : public AbstractLight
{
    private:
        /*! MeshLight type */
        const LightType type_;

        /*! Light index in the GLSL shader */
        GLuint index_;

        /*!
         * Location of the GLSL shader uniform variable keeping this light's
         * color.
         */
        GLint colorLocation_;

    public:
        /***
         * 1. Construction
         ***/
        LightProperties() = delete;
    protected:
        LightProperties( LightType type, GLuint index, const PackableColor& color );
    public:
        LightProperties( const LightProperties& ) = delete; // TODO: Implement using shader limit (MAX_LIGHTS).
        LightProperties( LightProperties&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightProperties() = default;


        /***
         * 3. Getters
         ***/
        virtual LightType getLightType() const;
        virtual PackableColor getLightColor() const;
        virtual GLint getLightIndex() const = 0;
        virtual GLuint getBaseLightIndex() const;


        /***
         * 4. Setters
         ***/
        virtual void setLightColor(const PackableColor &color);


        /***
         * 5. Operators
         ***/
        LightProperties& operator = ( const LightProperties& ) = delete;
        LightProperties& operator = ( LightProperties&& ) = delete;
};

typedef std::shared_ptr< LightProperties > LightPropertiesSharedPtr;
typedef std::shared_ptr< const LightProperties > LightPropertiesConstSharedPtr;

} // namespace como

#endif // LIGHT_PROPERTIES_HPP
