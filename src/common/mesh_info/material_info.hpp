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

#ifndef MATERIAL_INFO_HPP
#define MATERIAL_INFO_HPP

#include <string>
#include <glm/glm.hpp>
#include "texture_info.hpp"
#include <memory>

namespace como {

struct MaterialInfo
{
    std::string name;

    glm::vec4 color;

    glm::vec3 ambientReflectivity;
    glm::vec3 diffuseReflectivity;
    glm::vec3 specularReflectivity;
    float specularExponent;

    std::unique_ptr< TextureInfo > textureInfo;

    MaterialInfo() : textureInfo( nullptr ) {}
    inline MaterialInfo( const MaterialInfo& b );
};


MaterialInfo::MaterialInfo( const MaterialInfo& b ) :
    name( b.name ),
    color( b.color ),
    ambientReflectivity( b.ambientReflectivity ),
    diffuseReflectivity( b.diffuseReflectivity ),
    specularReflectivity( b.specularReflectivity ),
    specularExponent( b.specularExponent )
{
    if( b.textureInfo ){
        textureInfo = std::unique_ptr< TextureInfo >( new TextureInfo( *( b.textureInfo ) ) );
    }
}


} // namespace como

#endif // MATERIAL_INFO_HPP