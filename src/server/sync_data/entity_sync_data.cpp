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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "entity_sync_data.hpp"
#include <common/commands/selection_commands/selection_transformation_command.hpp>
#include <common/commands/entity_commands/model_matrix_replacement_command.hpp>

namespace como {


/***
 * 1. Construction
 ***/

EntitySyncData::EntitySyncData( const Command *creationCommand, const ResourceID& id, const glm::vec3& centroid ) :
    ResourceSyncData( creationCommand, id ),
    originalCentroid_( centroid, 1.0f ),
    centroid_( 0.0f ),
    modelMatrix_( 1.0f )
{}


/***
 * 3. Getters
 ***/

std::list<CommandConstPtr> EntitySyncData::generateUpdateCommands() const
{
    std::list<CommandConstPtr> updateCommands =
            ResourceSyncData::generateUpdateCommands();

    updateCommands.push_back(
                CommandConstPtr(
                    new ModelMatrixReplacementCommand(
                        resourceID(),
                        NO_USER,
                        modelMatrix_ ) ) );

    return updateCommands;
}


/***
 * 4. Updating
 ***/

void EntitySyncData::processCommand( const Command &command )
{
    if( command.getTarget() == CommandTarget::SELECTION ){
        // FIXME: Duplicated code in EntitiesManager::processCommand().
        // Cast to a SELECTION_TRANSFORMATION command.
        const SelectionTransformationCommand& selectionTransformation =
                dynamic_cast< const SelectionTransformationCommand& >( command );

        // Execute one transformation or another according to the requested
        // type.
        switch( selectionTransformation.getTransformationType() ){
            case SelectionTransformationCommandType::TRANSLATION:
                translate( selectionTransformation.getTransformationVector() );
            break;
            case SelectionTransformationCommandType::ROTATION_AROUND_INDIVIDUAL_CENTROIDS:
                rotateAroundCentroid( selectionTransformation.getTransformationAngle(),
                                      selectionTransformation.getTransformationVector() );
            break;
            case SelectionTransformationCommandType::ROTATION_AROUND_PIVOT:
                rotateAroundPivot( selectionTransformation.getTransformationAngle(),
                                   selectionTransformation.getTransformationVector(),
                                   *( selectionTransformation.getPivotPoint() ) );
            break;
            case SelectionTransformationCommandType::SCALE_AROUND_INDIVIDUAL_CENTROIDS:
                scaleAroundCentroid( selectionTransformation.getTransformationVector() );
            break;
            case SelectionTransformationCommandType::SCALE_AROUND_PIVOT:
                scaleAroundPivot( selectionTransformation.getTransformationVector(),
                                  *( selectionTransformation.getPivotPoint() ) );
            break;
        }
    }else if( command.getTarget() == CommandTarget::ENTITY ){
        // Cast to an entity command.
        const EntityCommand& entityCommand =
                dynamic_cast< const EntityCommand& >( command );

        switch( entityCommand.getType() ){
            case EntityCommandType::MODEL_MATRIX_REPLACEMENT:{
                const ModelMatrixReplacementCommand& matrixCommand =
                        dynamic_cast< const ModelMatrixReplacementCommand& >( command );
                setModelMatrix( matrixCommand.modelMatrix() );
            }break;
        }
    }else{
        ResourceSyncData::processCommand( command );
    }
}


/***
 * 5. Transformations
 ***/

glm::vec3 EntitySyncData::centroid() const
{
    return centroid_;
}


void EntitySyncData::applyTransformationMatrix(const glm::mat4 &transformation)
{
    setModelMatrix( transformation * modelMatrix_ );
}


void EntitySyncData::setModelMatrix(const glm::mat4 &modelMatrix)
{
    modelMatrix_ = modelMatrix;

    centroid_ = glm::vec3( modelMatrix_ * originalCentroid_ );
}

} // namespace como
