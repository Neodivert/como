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

#include <common/packables/packable_color.hpp>
#include <common/primitives/primitive_data/material_info.hpp>
#include <client/models/3d/textures/texture.hpp>
#include <list>
#include <common/resources/resource.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include "abstract_material.hpp"

namespace como {

class Material : public Resource, public AbstractMaterial
{
    private:
        PlainMaterialData materialData_;

        std::unique_ptr< Texture > texture_;


        /***
         * 1. Construction
         ***/
    public:
        Material( const ResourceID& materialID, const MaterialInfo& materialInfo );
        Material() = delete;
        Material( const Material& ) = delete;
        Material( Material&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~Material() = default;


        /***
         * 3. Getters
         ***/
        virtual Color getColor() const;
        virtual Color getAmbientReflectivity() const;
        virtual Color getDiffuseReflectivity() const;
        virtual Color getSpecularReflectivity() const;
        virtual float getSpecularExponent() const;
        virtual std::string typeName() const;
        bool includesTexture() const;


        /***
         * 4. Setters
         ***/
        virtual void setColor( const Color& color );
        virtual void setAmbientReflectivity( const Color& ambientReflectivity );
        virtual void setDiffuseReflectivity( const Color& diffuseReflectivity );
        virtual void setSpecularReflectivity( const Color& specularReflectivity );
        virtual void setSpecularExponent( float specularExponent );


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
