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

namespace como {

class LightsManager : public QObject, public Changeable
{
    Q_OBJECT

    private:
        // Lights vector.
        std::map< LightID, LightPropertiesSharedPtr > lights_;

        // Lights are drawables, so the drawables manager is also implied in
        // lights management.
        DrawablesManagerPtr drawablesManager_;

        // Interface with the server.
        ServerInterfacePtr server_;

        // A stack holding all the free light indices allowed in shader.
        std::stack< GLint > freeLightIndices_;

        // A stack holding all the free directional light indices allowed in shader.
        std::stack< GLuint > freeDirectionalLightIndices_;


    public:
        /***
         * 1. Construction
         ***/
        LightsManager();
        LightsManager( DrawablesManagerPtr drawablesManager, ServerInterfacePtr server );
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
        void addDirectionalLight( const LightID& lightID, const PackableColor& lightColor );
    public:
        void selectLight( const LightID lightID );


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
        void lightCreated( const LightID& id, const std::string& name );
        void lightSelected( LightHandlerPtr light );
        void lightRemoved( const PackableDrawableID& id );


        /***
         * 7. Updating
         ***/
    private:
        virtual void onChange(){}


        /***
         * 8. Auxiliar methods
         ***/
        unsigned int getNextFreeLightIndex( LightType lightType );
};

typedef std::shared_ptr< LightsManager > LightsManagerPtr;

} // namespace como

#endif // LIGHTS_MANAGER_HPP
