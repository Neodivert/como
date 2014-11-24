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
    color_( materialInfo.color ),
    ambientReflectivity_( materialInfo.ambientReflectivity ),
    diffuseReflectivity_( materialInfo.diffuseReflectivity ),
    specularReflectivity_( materialInfo.specularReflectivity ),
    specularExponent_( materialInfo.specularExponent )
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
    return Color( color_ );
}

Color Material::getAmbientReflectivity() const
{
    return Color( ambientReflectivity_ );
}

Color Material::getDiffuseReflectivity() const
{
    return Color( diffuseReflectivity_ );
}

Color Material::getSpecularReflectivity() const
{
    return Color( specularReflectivity_ );
}

float Material::getSpecularExponent() const
{
    return specularExponent_;
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
    color_ = color.toVec4();
}

void Material::setAmbientReflectivity( const Color& ambientReflectivity )
{
    ambientReflectivity_ = ambientReflectivity.toVec3();
}

void Material::setDiffuseReflectivity( const Color& diffuseReflectivity )
{
    diffuseReflectivity_ = diffuseReflectivity.toVec3();
}

void Material::setSpecularReflectivity( const Color& specularReflectivity )
{
    specularReflectivity_ = specularReflectivity.toVec3();
}

void Material::setSpecularExponent( float specularExponent )
{
    specularExponent_ = specularExponent;
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
    glUniform4fv( uniformLocation, 1, glm::value_ptr( color_ ) );

    // Send ambient reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.ambientReflectivity" );
    glUniform3fv( uniformLocation, 1, glm::value_ptr( ambientReflectivity_ ) );

    // Send diffuse reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.diffuseReflectivity" );
    glUniform3fv( uniformLocation, 1, glm::value_ptr( diffuseReflectivity_ ) );

    // Send diffuse reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.specularReflectivity" );
    glUniform3fv( uniformLocation, 1, glm::value_ptr( specularReflectivity_ ) );

    // Send specular exponent to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.specularExponent" );
    glUniform1f( uniformLocation, specularExponent_ );

    // Send texture, if any, to shader.
    if( texture_ != nullptr ){
        texture_->sendToShader();
    }
}

} // namespace como
