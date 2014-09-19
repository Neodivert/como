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

#ifndef TEXTURES_VIEWER_HPP
#define TEXTURES_VIEWER_HPP

#include <QDialog>
#include <client/managers/managers/textures/textures_manager.hpp>

namespace como {

class TexturesViewer : public QDialog
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        TexturesViewer( TexturesManager* texturesManager, QWidget* parent = nullptr );
        TexturesViewer() = delete;
        TexturesViewer( const TexturesViewer& ) = delete;
        TexturesViewer( TexturesViewer&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~TexturesViewer() = default;


        /***
         * 3. Operators
         ***/
        TexturesViewer& operator = ( const TexturesViewer& ) = delete;
        TexturesViewer& operator = ( TexturesViewer&& ) = delete;
};

} // namespace como

#endif // TEXTURES_VIEWER_HPP
