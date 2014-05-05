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

        ServerInterfacePtr server_;

        UserID localUserID_;

        // TODO: move this to ServerInterface.
        std::queue< PackableDrawableID > localUserPendingSelections_;

        // Map ID - PrimitivePath.
        std::map< PrimitiveID, std::string > primitivePaths_;

        // Relative path to the primitives directory.
        std::string primitivesDirPath_;

        LogPtr log_;

    public:
        /***
         * 1. Construction
         ***/
        DrawablesManager( ServerInterfacePtr server, UserID localUserID, std::string primitivesDirPath, LogPtr log );
        DrawablesManager( const DrawablesManager& ) = delete;
        DrawablesManager( DrawablesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~DrawablesManager() = default;


        /***
         * 3. Getters
         ***/
    private:
        DrawablesSelection* getUserSelection();
        DrawablesSelection* getUserSelection( UserID userID );
    public:

        /***
         * 4. Setters
         ***/
        void setPivotPointMode( PivotPointMode pivotPointMode );
        void setPivotPointMode( PivotPointMode pivotPointMode, UserID userID );


        /***
         * 5. Drawables administration
         ***/
    private:
        void addDrawable( DrawablePtr drawable, PackableDrawableID drawableID );
    public:
        void addMesh( PrimitiveID primitiveID, QColor color );
        void addMesh( PrimitiveID primitiveID, const std::uint8_t* color );
        void addMesh( PrimitiveID primitiveID, const std::uint8_t* color, PackableDrawableID drawableID );


        /***
         * 6. Selections management
         ***/
        void addDrawablesSelection( UserID userID );

        void deleteSelection();
        void deleteSelection( const unsigned int& userId );


        /***
         * 7. Drawables (de)seletion.
         ***/
        void selectDrawable( PackableDrawableID drawableID );
        void selectDrawable( PackableDrawableID drawableID, UserID userID );

        void unselectAll();
        void unselectAll( UserID userId );

        PackableDrawableID selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection );


        /***
         * 8. Drawables selection transformations
         ***/
        void translateSelection( glm::vec3 direction );
        void translateSelection( glm::vec3 direction, UserID userID );

        void rotateSelection( GLfloat angle, glm::vec3 axis );
        void rotateSelection( GLfloat angle, glm::vec3 axis, UserID userID );

        void scaleSelection( glm::vec3 scaleFactors );
        void scaleSelection( glm::vec3 scaleFactors, UserID userID );
        //void rotateSelection( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot );


        /***
         * 9. Auxiliar methods
         ***/
        void roundTransformationMagnitude( float& vx, float& vy, float& vz );
        void roundTransformationMagnitude( float& angle, float& vx, float& vy, float& vz );


        /***
         * 10. Operators
         ***/
        DrawablesManager& operator=( const DrawablesManager& ) = delete ;
        DrawablesManager& operator=( DrawablesManager&& ) = delete;
};

} // namespace como

#endif // DRAWABLES_MANAGER_HPP
