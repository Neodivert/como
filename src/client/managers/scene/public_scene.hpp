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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef PUBLIC_SCENE_HPP
#define PUBLIC_SCENE_HPP

#include "scene.hpp"

namespace como {

class PublicScene : public Scene
{
    Q_OBJECT

    public:
        /***
         * 1. Initialization and destruction
         ***/
        PublicScene( LogPtr log );
        virtual void connect( const char* host, const char* port, const char* userName );


        /***
         * 2. Users administration
         ***/
        virtual void addUser( std::shared_ptr< const UserConnected > userConnectedCommand );
};

} // namespace como

#endif // PUBLIC_SCENE_HPP
