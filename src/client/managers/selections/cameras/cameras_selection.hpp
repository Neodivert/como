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

#ifndef CAMERAS_SELECTION_HPP
#define CAMERAS_SELECTION_HPP

#include <client/models/3d/entities_set.hpp>
#include <client/models/3d/camera.hpp>

namespace como {

class CamerasSelection : public virtual EntitiesSet< Camera >
{
    public:
        /***
         * 1. Construction
         ***/
        CamerasSelection( glm::vec4 borderColor = DEFAULT_BORDER_COLOR );
        CamerasSelection( const CamerasSelection& ) = delete;
        CamerasSelection( CamerasSelection&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~CamerasSelection() = default;


        /***
         * 3. Operators
         ***/
        CamerasSelection& operator = ( const CamerasSelection& ) = delete;
        CamerasSelection& operator = ( CamerasSelection&& ) = delete;
};

} // namespace como

#endif // CAMERAS_SELECTION_HPP
