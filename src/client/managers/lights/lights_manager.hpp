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

#ifndef LIGHTS_MANAGER_HPP
#define LIGHTS_MANAGER_HPP

#include <client/managers/drawables_manager/drawables_manager.hpp>
#include <client/models/3d/lights/lights.hpp>
#include <client/managers/lights/light_handler.hpp>
#include <common/commands/commands.hpp>
#include <map>
#include <common/utilities/observer_pattern/observer.hpp>
#include <client/managers/resources_manager.hpp>

namespace como {

typedef std::map< ResourceID, LightPropertiesSharedPtr > LightsMap;

class LightsManager : public QObject, public Changeable, public Observer, public ResourcesManager
{
    Q_OBJECT

    private:
        // Lights vector.
        LightsMap lights_;

        // Lights are drawables, so the drawables manager is also implied in
        // lights management.
        DrawablesManagerPtr drawablesManager_;

        // A stack holding all the free light indices allowed in shader.
        std::stack< GLint > freeLightIndices_;

        // A stack holding all the free directional light indices allowed in shader.
        std::stack< GLuint > freeDirectionalLightIndices_;


        LogPtr log_;


    public:
        /***
         * 1. Construction
         ***/
        LightsManager() = delete;
        LightsManager( DrawablesManagerPtr drawablesManager, ServerInterfacePtr server, LogPtr log );
        LightsManager( const LightsManager& ) = delete;
        LightsManager( LightsManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsManager() = default;


        /***
         * 3. Lights management
         ***/
    public slots:
        void createDirectionalLight();
    private:
        void addDirectionalLight( const ResourceID& lightID, const PackableColor& lightColor );
    public:
        void selectLight( const ResourceID lightID );
    private:
        void removeLight( ResourceID lightID );


        /***
         * 4. Remote command execution
         ***/
    public:
        void executeRemoteCommand( LightCommandConstPtr command );


        /***
         * 5. Operators
         ***/
        LightsManager& operator = ( const LightsManager& ) = delete;
        LightsManager& operator = ( LightsManager&& ) = delete;


        /***
         * 6. Signals
         ***/
    signals:
        void lightCreated( ResourceID id, std::string name );
        void lightSelected( LightHandlerPtr light );
        void lightRemoved( ResourceID id );
        void lightModified( ResourceID id );


        /***
         * 7. Updating
         ***/
    private:
        virtual void onChange(){}
        virtual void update();


        /***
         * 8. Auxiliar methods
         ***/
        unsigned int getNextFreeLightIndex( LightType lightType );
        void print();
    public:
        void highlightLight( ResourceID lightID );
        void removeHighlights();
};

typedef std::shared_ptr< LightsManager > LightsManagerPtr;

} // namespace como

#endif // LIGHTS_MANAGER_HPP
