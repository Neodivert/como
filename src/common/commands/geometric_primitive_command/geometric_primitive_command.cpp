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

#include "geometric_primitive_command.hpp"

namespace como {

GeometricPrimitiveCommand::GeometricPrimitiveCommand( GeometricPrimitiveCommandType commandType,
                                                      const ResourceID& meshID,
                                                      const ResourceID& materialID,
                                                      const ResourceID& firstTextureWallID,
                                                      std::uint8_t nTextureWalls,
                                                      const glm::vec3& centroid ) :
    TypeCommand( CommandTarget::GEOMETRIC_PRIMITIVE, commandType, meshID.getCreatorID() ),
    meshID_( meshID ),
    materialID_( materialID ),
    firstTextureWallID_( firstTextureWallID ),
    nTextureWalls_( nTextureWalls ),
    centroid_( centroid.x, centroid.y, centroid.z )
{
    addPackable( &meshID_ );
    addPackable( &materialID_ );
    addPackable( &firstTextureWallID_ );
    addPackable( &nTextureWalls_ );
    addPackable( &centroid_ );
}


GeometricPrimitiveCommand::GeometricPrimitiveCommand( const GeometricPrimitiveCommand& b ) :
    TypeCommand( b ),
    meshID_( b.meshID_ ),
    materialID_( b.materialID_ ),
    firstTextureWallID_( b.firstTextureWallID_ ),
    nTextureWalls_( b.nTextureWalls_ ),
    centroid_( b.centroid_ )
{
    addPackable( &meshID_ );
    addPackable( &materialID_ );
    addPackable( &firstTextureWallID_ );
    addPackable( &nTextureWalls_ );
    addPackable( &centroid_ );
}


/***
 * 3. Getters
 ***/

ResourceID GeometricPrimitiveCommand::getMeshID() const
{
    return meshID_.getValue();
}


ResourceID GeometricPrimitiveCommand::getMaterialID() const
{
    return materialID_.getValue();
}


ResourceID GeometricPrimitiveCommand::getFirstTextureWallID() const
{
    return firstTextureWallID_.getValue();
}


std::uint8_t GeometricPrimitiveCommand::nTextureWalls() const
{
    return nTextureWalls_.getValue();
}


glm::vec3 GeometricPrimitiveCommand::centroid() const
{
    return glm::vec3(
            centroid_.getValues()[0],
            centroid_.getValues()[1],
            centroid_.getValues()[2]
            );
}


} // namespace como
