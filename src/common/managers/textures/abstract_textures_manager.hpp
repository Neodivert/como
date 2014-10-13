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

#ifndef ABSTRACT_TEXTURES_MANAGER_HPP
#define ABSTRACT_TEXTURES_MANAGER_HPP

#include <string>
#include <common/utilities/lockable.hpp>

namespace como {

class AbstractTexturesManager : public Lockable
{
    public:
        /***
         * 1. Construction
         ***/
        AbstractTexturesManager( const std::string& scenesDirPath );
        AbstractTexturesManager() = delete;
        AbstractTexturesManager( const AbstractTexturesManager& ) = delete;
        AbstractTexturesManager( AbstractTexturesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~AbstractTexturesManager();


        /***
         * 3. Operators
         ***/
        AbstractTexturesManager& operator = ( const AbstractTexturesManager& ) = delete;
        AbstractTexturesManager& operator = ( AbstractTexturesManager&& ) = delete;


    protected:
        const std::string TEXTURES_DIR_PATH_;
};

} // namespace como

#endif // ABSTRACT_TEXTURES_MANAGER_HPP
