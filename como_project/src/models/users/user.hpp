/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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

#ifndef USER_HPP
#define USER_HPP

#include "../3d/drawable.hpp"

namespace como {

typedef shared_ptr<Drawable> DrawablePtr;
typedef std::list< std::shared_ptr< Drawable > > DrawablesList;

class User
{
    private:
        GLfloat color[4];
        DrawablesList drawablesSelection;

    public:
        /***
         * 1. Initialization
         ***/
        User( const GLfloat& r, const GLfloat& g, const GLfloat& b );
};

} // namespace como

#endif // USER_HPP
