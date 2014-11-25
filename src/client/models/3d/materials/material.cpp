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

#include "material.hpp"
#include <client/models/3d/entity.hpp>
#include <boost/tokenizer.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

namespace como {

Material::Material( const ResourceID& materialID, const MaterialInfo& materialInfo ) :
    Resource( materialID, materialInfo.name ),
    materialData_( materialInfo )
{
    if( materialInfo.textureInfo ){
        texture_ = std::unique_ptr< Texture >( new Texture( NO_RESOURCE, "<texture>", *( materialInfo.textureInfo ) ) );
    }else{
        texture_ = nullptr;
    }
}

/***
 * 3. Getters
 ***/

Color Material::getColor() const
{
    return Color( materialData_.color );
}

Color Material::getAmbientReflectivity() const
{
    return Color( materialData_.ambientReflectivity );
}

Color Material::getDiffuseReflectivity() const
{
    return Color( materialData_.diffuseReflectivity );
}

Color Material::getSpecularReflectivity() const
{
    return Color( materialData_.specularReflectivity );
}

float Material::getSpecularExponent() const
{
    return materialData_.specularExponent;
}

std::string Material::typeName() const
{
    return "Material";
}

bool Material::includesTexture() const
{
    return ( texture_ != nullptr );
}


/***
 * 4. Setters
 ***/

void Material::setColor( const Color& color )
{
    materialData_.color = color.toVec4();
}

void Material::setAmbientReflectivity( const Color& ambientReflectivity )
{
    materialData_.ambientReflectivity = ambientReflectivity.toVec3();
}

void Material::setDiffuseReflectivity( const Color& diffuseReflectivity )
{
    materialData_.diffuseReflectivity = diffuseReflectivity.toVec3();
}

void Material::setSpecularReflectivity( const Color& specularReflectivity )
{
    materialData_.specularReflectivity = specularReflectivity.toVec3();
}

void Material::setSpecularExponent( float specularExponent )
{
    materialData_.specularExponent = specularExponent;
}


/***
 * 5. Shader comunication
 ***/

void Material::sendToShader() const
{
    GLint currentShaderProgram = -1;
    GLint uniformLocation = -1;

    // Get current shader program ID.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Send material color to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.color" );
    glUniform4fv( uniformLocation, 1, glm::value_ptr( materialData_.color ) );

    // Send ambient reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.ambientReflectivity" );
    glUniform3fv( uniformLocation, 1, glm::value_ptr( materialData_.ambientReflectivity ) );

    // Send diffuse reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.diffuseReflectivity" );
    glUniform3fv( uniformLocation, 1, glm::value_ptr( materialData_.diffuseReflectivity ) );

    // Send diffuse reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.specularReflectivity" );
    glUniform3fv( uniformLocation, 1, glm::value_ptr( materialData_.specularReflectivity ) );

    // Send specular exponent to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.specularExponent" );
    glUniform1f( uniformLocation, materialData_.specularExponent );

    // Send texture, if any, to shader.
    if( texture_ != nullptr ){
        texture_->sendToShader();
    }
}

} // namespace como
