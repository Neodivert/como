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
#include <glm/gtc/type_ptr.hpp>


namespace como {

const char DEFAULT_MATERIAL_NAME[] = "Unnamed material";
const glm::vec4 DEFAULT_MATERIAL_COLOR = glm::vec4( 1.0f );
const glm::vec3 DEFAULT_MATERIAL_AMBIENT_REFLECTIVITY = glm::vec3( 0.9f );
const glm::vec3 DEFAULT_MATERIAL_DIFFUSE_REFLECTIVITY = glm::vec3( 0.9f );
const glm::vec3 DEFAULT_MATERIAL_SPECULAR_REFLECTIVITY = glm::vec3( 0.9f );
const float DEFAULT_MATERIAL_SPECULAR_EXPONENT = 0.9f;


#define MATERIAL_DEFAULT_INITIALIZATION \
    name_( DEFAULT_MATERIAL_NAME ), \
    color_( DEFAULT_MATERIAL_COLOR ), \
    ambientReflectivity_( DEFAULT_MATERIAL_AMBIENT_REFLECTIVITY ), \
    diffuseReflectivity_( DEFAULT_MATERIAL_DIFFUSE_REFLECTIVITY ), \
    specularReflectivity_( DEFAULT_MATERIAL_SPECULAR_REFLECTIVITY ), \
    specularExponent_( DEFAULT_MATERIAL_SPECULAR_EXPONENT ), \
    texture_( nullptr )


/***
 * 1. Construction
 ***/

Material::Material() :
    MATERIAL_DEFAULT_INITIALIZATION
{}


Material::Material( const std::string& name ) :
    MATERIAL_DEFAULT_INITIALIZATION
{
    name_ = name;
}

Material::Material( const MaterialInfo& materialInfo ) :
    name_( materialInfo.name ),
    color_( DEFAULT_MATERIAL_COLOR ),
    ambientReflectivity_( materialInfo.ambientReflectivity ),
    diffuseReflectivity_( materialInfo.diffuseReflectivity ),
    specularReflectivity_( materialInfo.specularReflectivity ),
    specularExponent_( materialInfo.specularExponent )
{
    if( materialInfo.textureInfo ){
        texture_ = std::unique_ptr< Texture >( new Texture( *( materialInfo.textureInfo ) ) );
    }else{
        texture_ = nullptr;
    }
}


Material::Material( PackableColor color ) :
    MATERIAL_DEFAULT_INITIALIZATION
{
        color_ = color.toVec4();
}


Material::Material( const Material& b ) :
    name_( b.name_ ),
    color_( b.color_ ),
    ambientReflectivity_( b.ambientReflectivity_ ),
    diffuseReflectivity_( b.diffuseReflectivity_ ),
    specularReflectivity_( b.specularReflectivity_ ),
    specularExponent_( b.specularExponent_ )
{
    if( b.texture_ ){
        texture_ = std::unique_ptr< Texture >( new Texture( std::move( *( b.texture_ ) ) ) );
    }else{
        texture_ = nullptr;
    }
}


/***
 * 3. Getters
 ***/

std::string Material::getName() const
{
    return name_;
}

PackableColor Material::getColor() const
{
    return PackableColor( color_ );
}

PackableColor Material::getAmbientReflectivity() const
{
    return PackableColor( ambientReflectivity_ );
}

PackableColor Material::getDiffuseReflectivity() const
{
    return PackableColor( diffuseReflectivity_ );
}

PackableColor Material::getSpecularReflectivity() const
{
    return PackableColor( specularReflectivity_ );
}

float Material::getSpecularExponent() const
{
    return specularExponent_;
}


/***
 * 4. Setters
 ***/

void Material::setColor( const PackableColor& color )
{
    color_ = color.toVec4();
}

void Material::setAmbientReflectivity( const PackableColor& ambientReflectivity )
{
    ambientReflectivity_ = ambientReflectivity.toVec3();
}

void Material::setDiffuseReflectivity( const PackableColor& diffuseReflectivity )
{
    diffuseReflectivity_ = diffuseReflectivity.toVec3();
}

void Material::setSpecularReflectivity( const PackableColor& specularReflectivity )
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
