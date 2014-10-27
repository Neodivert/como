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

#ifndef LIGHT_DATA_HPP
#define LIGHT_DATA_HPP

#include <glm/vec3.hpp>

namespace como {

const glm::vec3 DEFAULT_LIGHT_COLOR( 1.0f );
const float DEFAULT_LIGHT_AMBIENT_COEFFICIENT = 0.3f;

struct LightData {
    glm::vec3 color;
    float ambientCoefficient;


    /***
     * 1. Construction
     ***/
    LightData() :
        color( DEFAULT_LIGHT_COLOR ),
        ambientCoefficient( DEFAULT_LIGHT_AMBIENT_COEFFICIENT ){}
};

} // namespace como

#endif // LIGHT_DATA_HPP
