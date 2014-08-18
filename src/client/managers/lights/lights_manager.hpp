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
#include <client/managers/abstract_entities_manager.hpp>
#include <client/managers/lights/local_lights_selection.hpp>

namespace como {

typedef std::map< ResourceID, LightSharedPtr > LightsMap;

class LightsManager : public AbstractEntitiesManager< Light, LightsSelection, LocalLightsSelection >
{
    private:
        // Lights vector.
        //LightsMap lights_;

        // A stack holding all the free light indices allowed in shader.
        std::stack< GLint > freeLightIndices_;

        // A stack holding all the free directional light indices allowed in shader.
        std::stack< GLuint > freeDirectionalLightIndices_;


        LightHandlerPtr currentLight_;


    public:
        /***
         * 1. Construction
         ***/
        LightsManager() = delete;
        LightsManager( ServerInterfacePtr server, LogPtr log );
        LightsManager( const LightsManager& ) = delete;
        LightsManager( LightsManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsManager() = default;


        /***
         * 3. Getters
         ***/
        LightHandlerPtr getCurrentLight() const;
        std::string getResourceName( const ResourceID& lightID ) const;


        /***
         * 4. Lights management
         ***/
        void createDirectionalLight();
    private:
        void addDirectionalLight( const ResourceID& lightID, const PackableColor& lightColor );
    public:
        void selectLight( const ResourceID lightID );
    private:
        void removeLight( ResourceID lightID );


        /***
         * 5. Remote command execution
         ***/
    public:
        void executeRemoteCommand( LightCommandConstPtr command );


        /***
         * 6. Operators
         ***/
        LightsManager& operator = ( const LightsManager& ) = delete;
        LightsManager& operator = ( LightsManager&& ) = delete;


        /***
         * 8. Updating
         ***/
    private:
        virtual void update();


        /***
         * 9. Auxiliar methods
         ***/
        unsigned int getNextFreeLightIndex( LightType lightType );
    public:
        void highlightLight( ResourceID lightID );
        void removeHighlights(); 
};

typedef std::shared_ptr< LightsManager > LightsManagerPtr;

} // namespace como

#endif // LIGHTS_MANAGER_HPP
