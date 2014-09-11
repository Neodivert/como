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

#include "specialized_system_primitives_factory.hpp"

namespace como {

/***
 * 1. Construction
 ***/

SpecializedSystemPrimitivesFactory::SpecializedSystemPrimitivesFactory(ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager ) :
    ServerWriter( server ),
    meshesManager_( meshesManager ),
    materialsManager_( materialsManager )
{}


/***
 * 4. SystemPrimitiveData generation
 ***/

SystemPrimitiveData SpecializedSystemPrimitivesFactory::generatePrimitiveData()
{
    SystemPrimitiveData primitiveData;

    generateVertexData( primitiveData.vertexData );
    generateNormalData( primitiveData.vertexData, primitiveData.normalData );
    generateUVData( primitiveData.uvData );
    generateOGLData( primitiveData );

    return primitiveData;
}


void SpecializedSystemPrimitivesFactory::generateNormalData(const MeshVertexData &vertexData, MeshNormalData &normalData)
{
    normalData.initFromMeshVertexData( vertexData );
}


void SpecializedSystemPrimitivesFactory::generateOGLData( SystemPrimitiveData &primitiveData )
{
    primitiveData.generateOGLData();
}

} // namespace como
