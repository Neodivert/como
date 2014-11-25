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

#ifndef PLAINMATERIALDATA_HPP
#define PLAINMATERIALDATA_HPP

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace como {

struct PlainMaterialData {
    glm::vec4 color;

    glm::vec3 ambientReflectivity;  // ("Ka" in a *.mtl file).
    glm::vec3 diffuseReflectivity;  // ("Kd" in a *.mtl file).
    glm::vec3 specularReflectivity; // ("Ks" in a *.mtl file).
    float specularExponent;         // ("Ns" in a *.mtl file).

    PlainMaterialData() :
        color( 1.0f ),
        ambientReflectivity( 1.0f ),
        diffuseReflectivity( 0.5f ),
        specularReflectivity( 0.4f ),
        specularExponent( 30.0f )
    {}
    PlainMaterialData( const PlainMaterialData& ) = default;
};

} // namespace como

#endif // PLAINMATERIALDATA_HPP
