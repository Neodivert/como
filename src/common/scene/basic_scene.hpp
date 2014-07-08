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

#ifndef BASIC_SCENE_HPP
#define BASIC_SCENE_HPP

#include <string>
#include <common/utilities/log.hpp>
#include <boost/filesystem.hpp>

namespace como {

class BasicScene
{
    private:
        std::string sceneName_;
        std::string sceneDirPath_;
        std::string sceneTempDirPath_;

    protected:
        LogPtr log_;

    public:
        /***
         * 1. Construction
         ***/
        BasicScene() = delete;
        BasicScene( LogPtr log  );
        BasicScene( const std::string& sceneName );
        BasicScene( const BasicScene& ) = delete;
        BasicScene( BasicScene&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~BasicScene();


        /***
         * 3. Initialization
         ***/
        void createSceneDirectory();
    protected:
        void initScene( const std::string& sceneName );
    public:


        /***
         * 4. Getters
         ***/
        std::string getName() const;
        std::string getDirPath() const;
        std::string getTempDirPath() const;


        /***
         * 5. Operators
         ***/
        BasicScene& operator = ( const BasicScene& ) = delete;
        BasicScene& operator = ( BasicScene&& ) = delete;
};

} // namespace como

#endif // BASIC_SCENE_HPP
