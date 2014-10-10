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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <common/scene/basic_scene.hpp>

namespace como {

class Scene : public BasicScene
{
    public:
        /***
         * 1. Construction
         ***/
        Scene( const std::string& sceneName );
        Scene() = delete;
        Scene( const Scene& ) = delete;
        Scene( Scene&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~Scene() = default;


        /***
         * 3. Operators
         ***/
        Scene& operator = ( const Scene& ) = delete;
        Scene& operator = ( Scene&& ) = delete;
};

} // namespace como

#endif // SCENE_HPP
