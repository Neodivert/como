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

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <client/models/dependencies.hpp>
#include <common/packables/array/packable_color.hpp>
#include <common/primitives/primitive_data/material_info.hpp>
#include <client/models/3d/textures/texture.hpp>
#include <list>
#include <common/resources/resource.hpp>


namespace como {

class Material : public Resource
{
    private:
        glm::vec4 color_;

        glm::vec3 ambientReflectivity_;  // ("Ka" in a *.mtl file).
        glm::vec3 diffuseReflectivity_;  // ("Kd" in a *.mtl file).
        glm::vec3 specularReflectivity_; // ("Ks" in a *.mtl file).
        float specularExponent_;         // ("Ns" in a *.mtl file).

        std::unique_ptr< Texture > texture_;


        /***
         * 1. Construction
         ***/
    public:
        //Material();
        //TODO: Delete? Material( const std::string& name );
        Material( const ResourceID& materialID, const MaterialInfo& materialInfo );
        //TODO: Delete? Material( PackableColor color );
        Material( const Material& ) = delete;
        Material( Material&& ) = delete;


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
         * 6. Operators
         ***/
        Material& operator = ( const Material& ) = delete;
        Material& operator = ( Material&& ) = delete;
};

typedef std::shared_ptr< Material > MaterialPtr;
typedef std::shared_ptr< const Material > MaterialConstPtr;
typedef std::list< MaterialPtr > MaterialsList;
typedef std::vector< MaterialPtr > MaterialsVector;
typedef std::vector< MaterialConstPtr > ConstMaterialsVector;

} // namespace como

#endif // MATERIAL_HPP
