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

#ifndef DRAWABLES_MANAGER_HPP
#define DRAWABLES_MANAGER_HPP

#include <client/models/3d/lights/lights.hpp>
#include <client/managers/drawables_selection/drawables_selection.hpp>
#include <client/managers/drawables_selection/local_drawables_selection.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <client/models/utilities/changeable/changeable.hpp>
#include <map>
#include <client/managers/materials_manager/materials_manager.hpp>

namespace como {

typedef std::map< UserID, DrawablesSelectionPtr > DrawablesSelections;

class DrawablesManager : public QOffscreenSurface, public AbstractChangeable
{
    Q_OBJECT

    private:
        DrawablesSelections drawablesSelections_;

        DrawablesSelectionPtr nonSelectedDrawables_;
        LocalDrawablesSelectionPtr localDrawablesSelection_;

        ServerInterfacePtr server_;

        UserID localUserID_;

        MaterialsManagerPtr materialsManager_;

        // TODO: move this to ServerInterface?
        std::queue< PackableDrawableID > localUserPendingSelections_;

        // Map ID - PrimitivePath.
        std::map< PrimitiveID, std::string > primitivePaths_;

        // Relative path to the primitives directory.
        std::string primitivesDirPath_;

        shared_ptr< QOpenGLContext > oglContext_;

        LogPtr log_;

    public:
        /***
         * 1. Construction
         ***/
        DrawablesManager( ServerInterfacePtr server, MaterialsManagerPtr materialsManager, UserID localUserID, const PackableColor& localSelectionBorderColor, std::string primitivesDirPath, shared_ptr< QOpenGLContext > oglContext, LogPtr log );
        DrawablesManager( const DrawablesManager& ) = delete;
        DrawablesManager( DrawablesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~DrawablesManager() = default;


        /***
         * 3. Getters
         ***/
        LocalDrawablesSelectionPtr getLocalUserSelection() const ;
    private:
        DrawablesSelectionPtr getUserSelection( UserID userID ) const ;
        DrawablesSelectionPtr getUserSelection( UserID userID );
    public:
        glm::vec3 getPivotPoint() const ;
        glm::vec3 getPivotPoint( UserID userID ) const ;



        /***
         * 5. Changeable
         ***/
        void onChange(){}


        /***
         * 6. Drawables administration
         ***/
    public:
        PackableDrawableID addDrawable( DrawablePtr drawable );
        void addDrawable( UserID userID, DrawablePtr drawable, PackableDrawableID drawableID );

    public:
        //void addMesh( PrimitiveID primitiveID, QColor color );
        void createMeshAndMaterial( PrimitiveID primitiveID );

    public:
        void createMesh( PrimitiveID primitiveID, MaterialID materialID );
    private:
        void createRemoteMesh( PrimitiveID primitiveID, PackableDrawableID drawableID, MaterialID materialID );

    public:
        void addDirectionalLight( PackableDrawableID lightID, const PackableColor& lightColor, MaterialID materialID );


        /***
         * 7. Selections management
         ***/
        void addDrawablesSelection( UserID userID, const PackableColor& selectionBorderColor );

        void deleteSelection();
        void deleteSelection( const unsigned int& userId );


        /***
         * 8. Drawables (de)seletion.
         ***/
        void selectDrawable( PackableDrawableID drawableID );
        void selectDrawable( PackableDrawableID drawableID, UserID userID );

        void unselectAll();
        void unselectAll( UserID userId );

        PackableDrawableID selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection );


        /***
         * 10. Drawing
         ***/
        void drawAll( const glm::mat4& viewProjMatrix ) const ;


        /***
         * 11. Command execution
         ***/
        void executeRemoteDrawableCommand( DrawableCommandConstPtr command );
        void executeRemoteSelectionCommand( SelectionCommandConstPtr command );
        void executeRemoteParameterChangeCommand( ParameterChangeCommandConstPtr command );


        /***
         * 11. Operators
         ***/
        DrawablesManager& operator=( const DrawablesManager& ) = delete;
        DrawablesManager& operator=( DrawablesManager&& ) = delete;

        // TODO: Move to a new PrimitivesManager class?
        void registerPrimitivePath( PrimitiveID primitiveID, std::string primitiveRelPath );

        virtual bool hasChangedSinceLastQuery();
};

typedef shared_ptr< DrawablesManager > DrawablesManagerPtr;

} // namespace como

#endif // DRAWABLES_MANAGER_HPP
