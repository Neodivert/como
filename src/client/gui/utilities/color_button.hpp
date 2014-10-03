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

#ifndef COLOR_BUTTON_HPP
#define COLOR_BUTTON_HPP

#include <QPushButton>
#include <QColorDialog>
#include <common/packables/packable_color.hpp>

namespace como {

class ColorButton : public QPushButton
{
    Q_OBJECT

    private:
        QColor color_;

    public:
        /***
         * 1. Construction
         ***/
        ColorButton( const QColor& color );
        ColorButton( const ColorButton& ) = delete;
        ColorButton( ColorButton&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ColorButton() = default;


        /***
         * 3. Setters
         ***/
        void setColor( const QColor& color );
        void setColor( const Color& color );


        /***
         * 4. Operators
         ***/
        ColorButton& operator = ( const ColorButton& ) = delete;
        ColorButton& operator = ( ColorButton&& ) = delete;


        /***
         * 5. Signals
         ***/
    signals:
        void colorChanged( Color color );


        /***
         * 6. Slots
         ***/
    public slots:
        void execColorDialog();
};

} // namespace como

#endif // COLOR_BUTTON_HPP
