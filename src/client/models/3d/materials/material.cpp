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
#include <client/models/3d/drawable.hpp>
#include <boost/tokenizer.hpp>


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
 * 3. Initialization
 ***/

void Material::loadFromFile( const string &filePath, const string &materialName )
{
    std::ifstream materialFile;
    std::string line;

    std::function< bool( std::string ) > materialFoundPredicate;

    if( materialName == "*" ){
        materialFoundPredicate = []( std::string line ){
            return ( line.substr( 0, strlen( "newmtl" ) ) == "newmtl" );
        };
    }else{
        materialFoundPredicate = [=]( std::string line ){
            return ( line != "newmtl " + materialName );
        };
    }

    // Copy the material name.
    // TODO: Add the material ID to the name.
    name_ = materialName;

    // Open the material file.
    materialFile.open( filePath );
    if( !materialFile.is_open() ){
        throw std::runtime_error( std::string( "Couldn't open file [" ) + filePath + "]" );
    }

    // Keep reading until finding the requested material.
    do{
        std::getline( materialFile, line );
    }while( !materialFile.eof() && !materialFoundPredicate( line ) );

    if( materialFile.eof() ){
        throw std::runtime_error( std::string( "Material [" ) +
                                  materialName +
                                  "] not found in file [" +
                                  filePath +
                                  "]" );
    }

    // If no material name was specified, retrieve it from the file.
    if( materialName == "*" ){
        name_ = line.substr( line.find( ' ' ) + 1 );
    }

    // Keep reading the material info until reaching the next material or the
    // file end.
    do{
        std::getline( materialFile, line );

        if( line.size() ){
            // Lines are in the form "<variable> <value>". We retrieve the
            // <variable> part in lineHeader and the <value> part in lineBody.
            std::string lineHeader = line.substr( 0, line.find( ' ' ) );
            std::string lineBody = line.substr( line.find( ' ' ) + 1 );

            // Retrieve material parameters from file.
            if( lineHeader == "Ka" ){
                ambientReflectivity_ = readVec3( lineBody );
            }else if( lineHeader == "Kd" ){
                diffuseReflectivity_ = readVec3( lineBody );
            }else if( lineHeader == "Ks" ){
                specularReflectivity_ = readVec3( lineBody );
            }else if( lineHeader == "Ns" ){
                specularExponent_ = std::atof( lineBody.c_str() );
            }

        }
    }while( !materialFile.eof() && line.substr( 0, strlen( "newmtl" ) ) != "newmtl" );
}


/***
 * 4. Getters
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
 * 5. Setters
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
 * 6. Shader comunication
 ***/

void Material::sendToShader() const
{
    GLint currentShaderProgram = -1;
    GLint uniformLocation = -1;

    // Get current shader program ID.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Send material color to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.color" );
    OpenGL::checkStatus( "Getting location of material.color in shader" );
    glUniform4fv( uniformLocation, 1, &color_[0] );
    OpenGL::checkStatus( "Sending material.color to shader" );

    // Send ambient reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.ambientReflectivity" );
    OpenGL::checkStatus( "Getting location of material.ambientReflectivity in shader" );
    glUniform3fv( uniformLocation, 1, &ambientReflectivity_[0] );
    OpenGL::checkStatus( "Sending material.ambientReflectivity to shader" );

    // Send diffuse reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.diffuseReflectivity" );
    OpenGL::checkStatus( "Getting location of material.diffuseReflectivity in shader" );
    glUniform3fv( uniformLocation, 1, &diffuseReflectivity_[0] );
    OpenGL::checkStatus( "Sending material.diffuseReflectivity to shader" );

    // Send diffuse reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.specularReflectivity" );
    OpenGL::checkStatus( "Getting location of material.specularReflectivity in shader" );
    glUniform3fv( uniformLocation, 1, &specularReflectivity_[0] );
    OpenGL::checkStatus( "Sending material.specularReflectivity to shader" );

    // Send specular exponent to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.specularExponent" );
    OpenGL::checkStatus( "Getting location of material.specularExponent in shader" );
    glUniform1f( uniformLocation, specularExponent_ );
    OpenGL::checkStatus( "Sending material.specularExponent to shader" );

    // Send texture, if any, to shader.
    if( texture_ != nullptr ){
        texture_->sendToShader();
    }
}


/***
 * 7. Auxiliar methods
 ***/

void Material::print() const
{
    std::cout << "Material" << std::endl
              << "----------------------------------------------" << std::endl
              << "Color: (" << color_[0] << ", " << color_[1] << ", " << color_[2] << ", " << color_[3] << ")" << std::endl
              << "Ambient reflectivity: (" << ambientReflectivity_[0] << ", " << ambientReflectivity_[1] << ", " << ambientReflectivity_[2] << ")" << std::endl
              << "Diffuse reflectivity: (" << diffuseReflectivity_[0] << ", " << diffuseReflectivity_[1] << ", " << diffuseReflectivity_[2] << ")" << std::endl
              << "Specular reflectivity: (" << specularReflectivity_[0] << ", " << specularReflectivity_[1] << ", " << specularReflectivity_[2] << ")" << std::endl
              << "Specular exponent: (" << specularExponent_ << ")" << std::endl
              << "----------------------------------------------" << std::endl;
}


glm::vec3 Material::readVec3( string str ) const
{
    glm::vec3 resVector( 0.0f );

    // http://stackoverflow.com/a/55680
    boost::char_separator<char> separator( " " );
    boost::tokenizer< boost::char_separator< char > > components( str, separator );
    unsigned int i = 0;

    // FIXME: What if a line has more than 3 components?
    for( const auto& component : components ){
        resVector[i] = std::atof( component.c_str() );
        i++;
    }

    return resVector;
}

} // namespace como
