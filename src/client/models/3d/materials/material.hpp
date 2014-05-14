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

#include <glm/glm.hpp>

namespace como {

struct Material
{
    glm::vec3 ambientReflexivity;   // ("Ka" in a *.mtl file).
    glm::vec3 diffuseReflectivity;  // ("Kd" in a *.mtl file).
    glm::vec3 specularReflectivity; // ("Ks" in a *.mtl file).
    float specularExponent;         // ("Ns" in a *.mtl file).


    Material() :
        ambientReflexivity( 0.5f ),
        diffuseReflectivity( 0.5f ),
        specularReflectivity( 0.5f ),
        specularExponent( 0.5f )
    {}
};

} // namespace como

#endif // MATERIAL_HPP
