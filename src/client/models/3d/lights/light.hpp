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

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "client/models/3d/entity.hpp" // GL types.
#include <client/models/3d/meshes/imported_mesh.hpp>
#include <common/commands/light_commands/light_creation_command.hpp> // "LightType" type.
#include "abstract_light.hpp"

namespace como {

class Light : public AbstractLight, public ImportedMesh
{
    private:
        /*! MeshLight type */
        const LightType type_;

        /*! Light index in the GLSL shader */
        GLuint index_;

        GLint isValidLocation_;

        GLint ambientCoefficientLocation_;

        /*!
         * Location of the GLSL shader uniform variable keeping this light's
         * color.
         */
        GLint colorLocation_;

    public:
        /***
         * 1. Construction
         ***/
        Light() = delete;
    protected:
        Light( LightType type, const PackableColor& color, std::string path, OpenGL& openGL );
    public:
        Light( const Light& ) = default; // TODO: Implement using shader limit (MAX_LIGHTS) (or remove clone()).
        Light( Light&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~Light();


        /***
         * 3. Getters
         ***/
        virtual LightType getLightType() const;
        virtual PackableColor getLightColor() const;
        virtual float getAmbientCoefficient() const;
        virtual GLint getLightIndex() const = 0;
        virtual GLuint getBaseLightIndex() const;


        /***
         * 4. Setters
         ***/
        virtual void setLightColor(const PackableColor &color);
        virtual void setAmbientCoefficient( float coefficient );


        /***
         * 5. Operators
         ***/
        Light& operator = ( const Light& ) = delete;
        Light& operator = ( Light&& ) = delete;


    private:
        /***
         * 6. Auxiliar methods
         ***/
        static GLuint lockShaderLight( OpenGL& openGL );
};

typedef std::shared_ptr< Light > LightSharedPtr;
typedef std::shared_ptr< const Light > LightConstSharedPtr;

} // namespace como

#endif // LIGHT
