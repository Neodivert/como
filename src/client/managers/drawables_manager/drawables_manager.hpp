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

#ifndef DRAWABLES_MANAGER_HPP
#define DRAWABLES_MANAGER_HPP

#include "../../managers/drawables_selection/drawables_selection.hpp"
#include "../../managers/server_interface/server_interface.hpp"
#include "../../models/utilities/changeable.hpp"
#include <map>

namespace como {

typedef std::map< UserID, DrawablesSelection > DrawablesSelections;

class DrawablesManager : public Changeable
{
    private:
        DrawablesSelection nonSelectedDrawables_;
        DrawablesSelections drawablesSelections_;

        UserID localUserID_;

        ServerInterfacePtr server_;

        // TODO: move this to ServerInterface.
        std::queue< PackableDrawableID > localUserPendingSelections_;

        LogPtr log_;

    public:
        /***
         * 1. Construction
         ***/
        DrawablesManager( UserID localUserID, ServerInterfacePtr server, LogPtr log );
        DrawablesManager( const DrawablesManager& ) = delete;
        DrawablesManager( DrawablesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~DrawablesManager() = default;


        /***
         * 3. Selections management
         ***/
        void addDrawablesSelection( UserID userID );


        /***
         * 4. Drawables (de)seletion.
         ***/
        void selectDrawable( PackableDrawableID drawableID );
        void selectDrawable( PackableDrawableID drawableID, UserID userID );

        void unselectAll();
        void unselectAll( UserID userId );

        PackableDrawableID selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection );


        /***
         * 5. Operators
         ***/
        DrawablesManager& operator=( const DrawablesManager& ) = delete ;
        DrawablesManager& operator=( DrawablesManager&& ) = delete;
};

} // namespace como

#endif // DRAWABLES_MANAGER_HPP
