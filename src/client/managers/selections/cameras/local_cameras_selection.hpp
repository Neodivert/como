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

#ifndef LOCAL_CAMERAS_SELECTION_HPP
#define LOCAL_CAMERAS_SELECTION_HPP

#include "cameras_selection.hpp"

namespace como {

class LocalCamerasSelection : public CamerasSelection, public ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        LocalCamerasSelection( ServerInterfacePtr server );
        LocalCamerasSelection() = delete;
        LocalCamerasSelection( const LocalCamerasSelection& ) = delete;
        LocalCamerasSelection( LocalCamerasSelection&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LocalCamerasSelection() = default;


        /***
         * 3. Operators
         ***/
        LocalCamerasSelection& operator = ( const LocalCamerasSelection& ) = delete;
        LocalCamerasSelection& operator = ( LocalCamerasSelection&& ) = delete;
};

} // namespace como

#endif // LOCAL_CAMERAS_SELECTION_HPP
