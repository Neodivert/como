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

#ifndef TOOLS_MENU_HPP
#define TOOLS_MENU_HPP

#include <QFrame>
#include <memory>
#include "../como_app.hpp"

class ToolsMenu : public QFrame
{
    protected:
        // Pointer to app's current state.
        shared_ptr< ComoApp > comoApp;

    public:
        ToolsMenu( shared_ptr< ComoApp > comoApp );
};

#endif // TOOLS_MENU_HPP
