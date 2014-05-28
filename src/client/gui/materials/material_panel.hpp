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
#include <QLineEdit>
#include <client/models/3d/materials/material.hpp>

namespace como {

class MaterialPanel : public QFrame
{
    Q_OBJECT

    private:
        MaterialPtr currentMaterial_;

        QLineEdit* nameInput_;

    public:
        /***
         * 1. Construction
         ***/
        MaterialPanel();
        MaterialPanel( const MaterialPanel& ) = delete;
        MaterialPanel( MaterialPanel&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MaterialPanel() = default;


        /***
         * 3. Operators
         ***/
        MaterialPanel& operator = ( const MaterialPanel& ) = delete;
        MaterialPanel& operator = ( MaterialPanel&& ) = delete;


        /***
         * 4. Slots
         ***/
    public slots:
        void openMaterial( MaterialPtr material );
};

} // namespace como

#endif // MATERIAL_PANEL_HPP
