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

#ifndef MATERIAL_EDITOR_HPP
#define MATERIAL_EDITOR_HPP

#include <QFrame>
#include <QLineEdit>
#include <client/managers/managers/materials/material_handler.hpp>
#include <client/gui/utilities/color_button.hpp>

namespace como {

class MaterialEditor : public QFrame
{
    Q_OBJECT

    private:
        MaterialHandlerPtr currentMaterial_;

        QLineEdit* nameInput_;
        ColorButton* colorButton_;
        ColorButton* ambientReflectivityButton_;
        ColorButton* diffuseReflectivityButton_;
        ColorButton* specularReflectivityButton_;
        QDoubleSpinBox* specularExponentSpinBox_;

    public:
        /***
         * 1. Construction
         ***/
        MaterialEditor();
        MaterialEditor( const MaterialEditor& ) = delete;
        MaterialEditor( MaterialEditor&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MaterialEditor() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getMaterialID() const;


        /***
         * 3. Operators
         ***/
        MaterialEditor& operator = ( const MaterialEditor& ) = delete;
        MaterialEditor& operator = ( MaterialEditor&& ) = delete;


        /***
         * 4. Refreshing
         ***/
        void refresh();


        /***
         * 5. Slots
         ***/
    public slots:
        void openMaterial( MaterialHandlerPtr material );
};

} // namespace como

#endif // MATERIAL_EDITOR_HPP
