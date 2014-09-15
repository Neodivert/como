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

#ifndef TEXTURE_WALL_HPP
#define TEXTURE_WALL_HPP

#include <glm/glm.hpp>
#include <memory>
#include "texture.hpp"

namespace como {

struct TextureWall {
    glm::vec2 textureOffset;
    glm::vec2 textureScale;

    std::unique_ptr< Texture > texture;


    /***
     * 1. Construction
     ***/
    TextureWall() :
        textureOffset( 0.0f, 0.0f ),
        textureScale( 1.0f, 1.0f ),
        texture( nullptr ) {}
};

} // namespace como

#endif // TEXTURE_WALL_HPP
