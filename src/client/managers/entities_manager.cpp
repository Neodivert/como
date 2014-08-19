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

EntitiesManager::EntitiesManager( ServerInterfacePtr server, LogPtr log ) :
    server_( server ),
    meshesManager_( new MeshesManager( server, log ) ),
    lightsManager_( new LightsManager( server, log ) )
{
    managers_.push_back( lightsManager_ );
    // TODO: Push meshesManager_ too.

    entitiesSelections_[NO_USER] = EntitiesSelectionPtr( new EntitiesSelection( lightsManager_->getResourcesSelection( NO_USER ).get() ) );
    entitiesSelections_[server->getLocalUserID()] = EntitiesSelectionPtr( new LocalEntitiesSelection( server, lightsManager_->getLocalResourcesSelection().get() ) );
}


/***
 * 3. Selections management
 ***/

void EntitiesManager::createUserSelection( UserID userID )
{
    // TODO: Apply to all managers.
    lightsManager_->createResourcesSelection( userID );

    entitiesSelections_[userID] = EntitiesSelectionPtr( new EntitiesSelection( lightsManager_->getResourcesSelection( userID ).get() ) );
}


void EntitiesManager::removeUserSelection()
{
    // TODO: Apply to all managers.
    lightsManager_->removeResourcesSelection( server_->getLocalUserID() );
}


void EntitiesManager::removeUserSelection( UserID userID )
{
    // TODO: Apply to all managers.
    lightsManager_->removeResourcesSelection( userID );

    entitiesSelections_.erase( userID );
}


/***
 * 4. Getters
 ***/

LocalEntitiesSelectionPtr EntitiesManager::getLocalSelection() const
{
    return std::dynamic_pointer_cast< LocalEntitiesSelection >( entitiesSelections_.at( server_->getLocalUserID() ) );
}


MeshesManagerPtr EntitiesManager::getMeshesManager()
{
    return meshesManager_;
}


LightsManagerPtr EntitiesManager::getLightsManager()
{
    return lightsManager_;
}


/***
 * 5. Entity picking
 ***/

ResourceID EntitiesManager::selectEntityByRayPicking(glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint )
{
    // TODO: Call also to MeshesManager::selectEntityByRayPicking() and lock
    // only the closest entity between both managers.
    return lightsManager_->selectEntityByRayPicking( r0, r1, addToSelection, worldCollisionPoint );
}


/***
 * 6. Command execution
 ***/

void EntitiesManager::executeRemoteSelectionCommand( SelectionCommandConstPtr command )
{
    const SelectionTransformationCommand* selectionTransformation = nullptr;
    std::array< float, 3 > transformationVector;

    switch( command->getType() ){
        case SelectionCommandType::SELECTION_TRANSFORMATION:
            // Cast to a SELECTION_TRANSFORMATION command.
            selectionTransformation = dynamic_cast< const SelectionTransformationCommand* >( command.get() );

            // Transform the user's selection.
            transformationVector = selectionTransformation->getTransformationVector();

            // Execute one transformation or another according to the requested
            // type.
            switch( selectionTransformation->getTransformationType() ){
                case SelectionTransformationCommandType::TRANSLATION:
                    entitiesSelections_.at( selectionTransformation->getUserID() )->translate( glm::vec3( transformationVector[0], transformationVector[1], transformationVector[2] ) );
                break;
                case SelectionTransformationCommandType::ROTATION:
                    entitiesSelections_.at( selectionTransformation->getUserID() )->rotate( selectionTransformation->getTransformationAngle(), glm::vec3( transformationVector[0], transformationVector[1], transformationVector[2] ) );
                break;
                case SelectionTransformationCommandType::SCALE:
                    entitiesSelections_.at( selectionTransformation->getUserID() )->scale( glm::vec3( transformationVector[0], transformationVector[1], transformationVector[2] ) );
                break;
            }
        break;
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


void EntitiesManager::executeResourceCommand( ResourceCommandConstPtr command )
{
    for( auto& manager : managers_ ){
        manager->executeResourceCommand( command );
    }
}


void EntitiesManager::executeResourcesSelectionCommand( ResourcesSelectionCommandConstPtr command )
{
    for( auto& manager : managers_ ){
        manager->executeResourcesSelectionCommand( command );
    }
}


/***
 * 7. Drawing
 ***/

void EntitiesManager::drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const
{
    // TODO: Apply to all managers.
    lightsManager_->drawAll( openGL, viewMatrix, projectionMatrix );
}

} // namespace como
