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

#include "entities_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***/

EntitiesManager::EntitiesManager( ServerInterfacePtr server, LogPtr log, OpenGL* openGL, UsersManagerPtr usersManager, MaterialsManagerPtr materialsManager, TextureWallsManager *textureWallsManager ) :
    ResourceCommandsExecuter( server ), // TODO: Remove all this duplicity?
    AbstractEntitiesManager( server ),
    usersManager_( usersManager ),
    meshesManager_( new MeshesManager( server, log, materialsManager, textureWallsManager ) ),
    lightsManager_( new LightsManager( server, log, openGL ) ),
    camerasManager_( new CamerasManager( *openGL, server, log ) )
{
    managers_.push_back( lightsManager_.get() );
    managers_.push_back( meshesManager_.get() );
    managers_.push_back( camerasManager_.get() );

    entitiesSelections_[NO_USER] =
            std::unique_ptr<EntitiesSelection>( new EntitiesSelection(
                                                    lightsManager_->getResourcesSelection( NO_USER ),
                                                    meshesManager_->getResourcesSelection( NO_USER ),
                                                    camerasManager_->getResourcesSelection( NO_USER ) ) );

    entitiesSelections_[server->getLocalUserID()] =
            std::unique_ptr<EntitiesSelection>(
                new LocalEntitiesSelection( server,
                                            lightsManager_->getLocalResourcesSelection(),
                                            meshesManager_->getLocalResourcesSelection(),
                                            camerasManager_->getLocalResourcesSelection() ) );

    usersManager_->addObserver( this );
}


/***
 * 3. Selections management
 ***/

void EntitiesManager::createUserSelection( UserID userID, const glm::vec4& selectionColor )
{
    lock();

    // TODO: Apply to all managers uniformly.
    lightsManager_->createResourcesSelection( userID, selectionColor );
    meshesManager_->createResourcesSelection( userID, selectionColor );
    camerasManager_->createResourcesSelection( userID, selectionColor );

    entitiesSelections_[userID] =
            std::unique_ptr<EntitiesSelection>(
                new EntitiesSelection(
                    lightsManager_->getResourcesSelection( userID ),
                    meshesManager_->getResourcesSelection( userID ),
                    camerasManager_->getResourcesSelection( userID ) ) );
}


void EntitiesManager::removeUserSelection()
{
    removeUserSelection( localUserID() );
}


void EntitiesManager::removeUserSelection( UserID userID )
{
    lock();
    lightsManager_->removeResourcesSelection( userID );
    meshesManager_->removeResourcesSelection( userID );
    camerasManager_->removeResourcesSelection( userID );

    entitiesSelections_.erase( userID );
}


/***
 * 4. Getters
 ***/

LocalEntitiesSelection* EntitiesManager::getLocalSelection() const
{
    lock();
    return dynamic_cast< LocalEntitiesSelection* >( entitiesSelections_.at( localUserID() ).get() );
}


MeshesManagerPtr EntitiesManager::getMeshesManager()
{
    return meshesManager_;
}


LightsManagerPtr EntitiesManager::getLightsManager()
{
    return lightsManager_;
}


CamerasManager* EntitiesManager::getCamerasManager()
{
    return camerasManager_.get();
}


bool EntitiesManager::containsResource(const ResourceID &resourceID) const
{
    lock();
    for( const auto& manager : managers_ ){
        if( manager->containsResource( resourceID ) ){
            return true;
        }
    }
    return false;
}


/***
 * 5. Entity picking
 ***/

bool EntitiesManager::pick(const glm::vec3 &rayOrigin, glm::vec3 rayDirection, ResourceID &pickedElement, float &t, const float &MAX_T) const
{
    lock();

    unsigned int nIntersectedManagers = 0;
    float maxT = MAX_T;

    for( const auto& manager : managers_ ){
        if( manager->pick( rayOrigin, rayDirection, pickedElement, t, maxT ) ){
            nIntersectedManagers++;
        }
        // Force the following manager to find a closer element.
        maxT = t;
    }

    return (nIntersectedManagers > 0);
}


/***
 * 6. Command execution
 ***/

void EntitiesManager::executeRemoteSelectionCommand( const SelectionCommand& command )
{
    switch( command.getType() ){
        case SelectionCommandType::SELECTION_TRANSFORMATION:{
            // Cast to a SELECTION_TRANSFORMATION command.
            const SelectionTransformationCommand& selectionTransformation =
                    dynamic_cast< const SelectionTransformationCommand& >( command );

            EntitiesSelection& selection = *( entitiesSelections_.at( selectionTransformation.getUserID() ) );

            // Execute one transformation or another according to the requested
            // type.
            switch( selectionTransformation.getTransformationType() ){
                case SelectionTransformationCommandType::TRANSLATION:
                    selection.translate( selectionTransformation.getTransformationVector() );
                break;
                case SelectionTransformationCommandType::ROTATION_AROUND_INDIVIDUAL_CENTROIDS:
                    selection.rotateAroundIndividualCentroids( selectionTransformation.getTransformationAngle(),
                                                               selectionTransformation.getTransformationVector() );
                break;
                case SelectionTransformationCommandType::ROTATION_AROUND_PIVOT:
                    selection.rotateAroundPivot( selectionTransformation.getTransformationAngle(),
                                                 selectionTransformation.getTransformationVector(),
                                                 *( selectionTransformation.getPivotPoint() ) );
                break;
                case SelectionTransformationCommandType::SCALE_AROUND_INDIVIDUAL_CENTROIDS:
                    selection.scaleAroundIndividualCentroids( selectionTransformation.getTransformationVector() );
                break;
                case SelectionTransformationCommandType::SCALE_AROUND_PIVOT:
                    selection.scaleAroundPivot( selectionTransformation.getTransformationVector(),
                                                *( selectionTransformation.getPivotPoint() ) );
                break;
            }
        }break;
    }
}


void EntitiesManager::executeRemoteParameterChangeCommand( UserParameterChangeCommandConstPtr command )
{
    // Change parameter.
    switch( command->getParameterType() ){
        case ParameterType::PIVOT_POINT_MODE:
            entitiesSelections_.at( command->getUserID() )->setPivotPointMode( command->getPivotPointMode() );
        break;
    }
}


/***
 * 7. Drawing
 ***/

void EntitiesManager::drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const
{
    lightsManager_->sendLightsToShader( *openGL, viewMatrix );

    for( const auto& manager : managers_ ){
        manager->drawAll( openGL, viewMatrix, projectionMatrix );
    }
}


/***
 * 8. Updating (observer pattern)
 ***/

void EntitiesManager::update( ContainerAction lastContainerAction, UserID modifiedUser )
{
    if( lastContainerAction == ContainerAction::ELEMENT_INSERTION ){
        createUserSelection( modifiedUser, usersManager_->user( modifiedUser  ).color() );
    }else if( lastContainerAction == ContainerAction::ELEMENT_DELETION ){
        unlockResourcesSelection( modifiedUser );
        removeUserSelection( modifiedUser );
    }
}


/***
 * 10. Resources locking / unlocking
 ***/

void EntitiesManager::lockResource(const ResourceID &resourceID, UserID newOwner)
{
    for( auto& manager : managers_ ){
        if( manager->containsResource( resourceID ) ){
            manager->lockResource( resourceID, newOwner );
        }
    }
}


void EntitiesManager::unlockResourcesSelection(UserID currentOwner)
{
    for( auto& manager : managers_ ){
        manager->unlockResourcesSelection( currentOwner );
    }
}


void EntitiesManager::clearResourcesSelection(UserID currentOwner)
{
    for( auto& manager : managers_ ){
        manager->clearResourcesSelection( currentOwner );
    }
}

} // namespace como
