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

EntitiesManager::EntitiesManager( ServerInterfacePtr server, LogPtr log, UsersManagerPtr usersManager, MaterialsManagerPtr materialsManager ) :
    ResourceCommandsExecuter( server ), // TODO: Remove all this duplicity?
    AbstractEntitiesManager( server ),
    usersManager_( usersManager ),
    meshesManager_( new MeshesManager( server, log, materialsManager ) ),
    lightsManager_( new LightsManager( server, log ) )
{
    managers_.push_back( lightsManager_.get() );
    managers_.push_back( meshesManager_.get() );

    entitiesSelections_[NO_USER] =
            std::unique_ptr<EntitiesSelection>( new EntitiesSelection( lightsManager_->getResourcesSelection( NO_USER ), meshesManager_->getResourcesSelection( NO_USER ) ) );

    entitiesSelections_[server->getLocalUserID()] =
            std::unique_ptr<EntitiesSelection>( new LocalEntitiesSelection( server, lightsManager_->getLocalResourcesSelection(), meshesManager_->getLocalResourcesSelection() ) );

    usersManager_->addObserver( this );
}


/***
 * 3. Selections management
 ***/

void EntitiesManager::createUserSelection( UserID userID, const glm::vec4& selectionColor )
{
    // TODO: Apply to all managers uniformly.
    lightsManager_->createResourcesSelection( userID, selectionColor );
    meshesManager_->createResourcesSelection( userID, selectionColor );

    entitiesSelections_[userID] =
            std::unique_ptr<EntitiesSelection>( new EntitiesSelection( lightsManager_->getResourcesSelection( userID ), meshesManager_->getResourcesSelection( userID ) ) );
}


void EntitiesManager::removeUserSelection()
{
    removeUserSelection( localUserID() );
}


void EntitiesManager::removeUserSelection( UserID userID )
{
    lightsManager_->removeResourcesSelection( userID );
    meshesManager_->removeResourcesSelection( userID );

    entitiesSelections_.erase( userID );
}


/***
 * 4. Getters
 ***/

LocalEntitiesSelection* EntitiesManager::getLocalSelection() const
{
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


bool EntitiesManager::containsResource(const ResourceID &resourceID) const
{
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
    std::array< float, 3 > transformationVector;

    switch( command.getType() ){
        case SelectionCommandType::SELECTION_TRANSFORMATION:{
            // Cast to a SELECTION_TRANSFORMATION command.
            const SelectionTransformationCommand& selectionTransformation =
                    dynamic_cast< const SelectionTransformationCommand& >( command );

            // Transform the user's selection.
            transformationVector = selectionTransformation.getTransformationVector();

            // Execute one transformation or another according to the requested
            // type.
            switch( selectionTransformation.getTransformationType() ){
                case SelectionTransformationCommandType::TRANSLATION:
                    entitiesSelections_.at( selectionTransformation.getUserID() )->translate( glm::vec3( transformationVector[0], transformationVector[1], transformationVector[2] ) );
                break;
                case SelectionTransformationCommandType::ROTATION:
                    entitiesSelections_.at( selectionTransformation.getUserID() )->rotate( selectionTransformation.getTransformationAngle(), glm::vec3( transformationVector[0], transformationVector[1], transformationVector[2] ) );
                break;
                case SelectionTransformationCommandType::SCALE:
                    entitiesSelections_.at( selectionTransformation.getUserID() )->scale( glm::vec3( transformationVector[0], transformationVector[1], transformationVector[2] ) );
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
