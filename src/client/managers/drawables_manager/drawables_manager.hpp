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
#include <QOffscreenSurface>
#include <common/utilities/observer_pattern/observable.hpp>

namespace como {

typedef std::map< UserID, DrawablesSelectionPtr > DrawablesSelections;

enum class MeshEdgesDisplayFrequency {
    ALWAYS,
    ONLY_WHEN_SELECTED
};

// TODO: Remove AbstractChangeable and use only Observable.
class DrawablesManager : public QOffscreenSurface, public AbstractChangeable, public Observable
{
    Q_OBJECT

    private:
        DrawablesSelections drawablesSelections_;

        DrawablesSelectionPtr nonSelectedDrawables_;
        LocalDrawablesSelectionPtr localDrawablesSelection_;

        ServerInterfacePtr server_;

        UserID localUserID_;

        // TODO: move this to ServerInterface?
        std::queue< ResourceID > localUserPendingSelections_;

        // Map ID - PrimitivePath.
        std::map< ResourceID, std::string > primitivePaths_; // TODO: Remove this and related methods.

        // Relative path to the primitives directory.
        std::string primitivesDirPath_;

        shared_ptr< QOpenGLContext > oglContext_;

        LogPtr log_;


    protected:
        MeshEdgesDisplayFrequency meshEdgesDisplayFrequency_;


    public:
        /***
         * 1. Construction
         ***/
        DrawablesManager( ServerInterfacePtr server, UserID localUserID, const PackableColor& localSelectionBorderColor, std::string primitivesDirPath, shared_ptr< QOpenGLContext > oglContext, LogPtr log );
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
        bool existsDrawable( const ResourceID& id ) const;


        /***
         * 4. Setters
         ***/
        void displayEdges( MeshEdgesDisplayFrequency frequency );


        /***
         * 5. Changeable
         ***/
        void onChange(){}


        /***
         * 6. Drawables administration
         ***/
    public:
        ResourceID addDrawable( DrawablePtr drawable );
        void addDrawable( UserID userID, DrawablePtr drawable, ResourceID drawableID );

    public:
        ResourceID createMesh( MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials );
        void createMesh( ResourceID meshID, MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials );

    private:
        //void createRemoteMesh( ResourceID primitiveID, ResourceID drawableID, ResourceID materialID );

    public:


        /***
         * 7. Selections management
         ***/
        void addDrawablesSelection( UserID userID, const PackableColor& selectionBorderColor );

        void deleteSelection();
        void deleteSelection( const unsigned int& userId );


        /***
         * 8. Drawables (de)seletion.
         ***/
        void selectDrawable( ResourceID drawableID );
        void selectDrawable( ResourceID drawableID, UserID userID );

        void unselectAll();
        void unselectAll( UserID userId );

        ResourceID selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint );


        /***
         * 10. Drawing
         ***/
        void drawAll( OpenGLPtr openGL, const glm::mat4& viewProjMatrix ) const ;


        /***
         * 11. Command execution
         ***/
        void executeRemoteDrawableCommand( DrawableCommandConstPtr command );
        void executeRemoteSelectionCommand( SelectionCommandConstPtr command );
        void executeRemoteParameterChangeCommand( UserParameterChangeCommandConstPtr command );


        /***
         * 11. Operators
         ***/
        DrawablesManager& operator=( const DrawablesManager& ) = delete;
        DrawablesManager& operator=( DrawablesManager&& ) = delete;

        // TODO: Move to a new PrimitivesManager class?
        void registerPrimitivePath( ResourceID primitiveID, std::string primitiveRelPath );

        virtual bool hasChangedSinceLastQuery();
        void highlightProperty( const void* property );
};

typedef shared_ptr< DrawablesManager > DrawablesManagerPtr;

} // namespace como

#endif // DRAWABLES_MANAGER_HPP
