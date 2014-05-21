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

#ifndef MATERIAL_PANEL_HPP
#define MATERIAL_PANEL_HPP

#include <QFrame>
#include <client/managers/drawables_selection/local_drawables_selection.hpp>
#include <common/packables/array/packable_color.hpp>

namespace como {

class MaterialPanel : public QFrame
{
    private:
        /*! Local user's (drawables) selection */
        LocalDrawablesSelectionPtr userSelection_;

    public:
        /***
         * 1. Construction
         ***/
        MaterialPanel( LocalDrawablesSelectionPtr userSelection );
        MaterialPanel( const MaterialPanel& ) = delete;
        MaterialPanel( MaterialPanel&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MaterialPanel() = default;


        /***
         * 4. Signals
         ***/
    signals:
        void MaterialDiffuseReflexivityChanged( const PackableColor& );


        /***
         * 3. Operators
         ***/
        MaterialPanel& operator = ( const MaterialPanel& ) = delete;
        MaterialPanel& operator = ( MaterialPanel&& ) = delete;
};

} // namespace como

#endif // MATERIAL_PANEL_HPP
