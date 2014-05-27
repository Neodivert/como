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

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <client/models/dependencies.hpp>
#include <common/packables/array/packable_color.hpp>

namespace como {

class Material
{
    private:
        glm::vec4 color_;

        glm::vec3 ambientReflectivity_;  // ("Ka" in a *.mtl file).
        glm::vec3 diffuseReflectivity_;  // ("Kd" in a *.mtl file).
        glm::vec3 specularReflectivity_; // ("Ks" in a *.mtl file).
        float specularExponent_;         // ("Ns" in a *.mtl file).


        /***
         * 1. Construction
         ***/
    public:
        Material();
        Material( PackableColor color );
        Material( const Material& ) = default;
        Material( Material&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~Material() = default;


        /***
         * 3. Getters
         ***/
        PackableColor getColor() const;
        PackableColor getAmbientReflectivity() const;
        PackableColor getDiffuseReflectivity() const;
        PackableColor getSpecularReflectivity() const;
        float getSpecularExponent() const;


        /***
         * 4. Setters
         ***/
        void setColor( const PackableColor& color );
        void setAmbientReflectivity( const PackableColor& ambientReflectivity );
        void setDiffuseReflectivity( const PackableColor& diffuseReflectivity );
        void setSpecularReflectivity( const PackableColor& specularReflectivity );
        void setSpecularExponent( float specularExponent );


        /***
         * 5. Shader comunication
         ***/
        void sendToShader() const;


        /***
         * 6. Auxiliar methods
         ***/
        void print() const;


        /***
         * 7. Operators
         ***/
        Material& operator = ( const Material& ) = default;
        Material& operator = ( Material&& ) = default;
};

} // namespace como

#endif // MATERIAL_HPP
