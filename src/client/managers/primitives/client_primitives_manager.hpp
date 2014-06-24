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

#ifndef CLIENT_PRIMITIVES_MANAGER_HPP
#define CLIENT_PRIMITIVES_MANAGER_HPP

#include <string>
#include <client/managers/drawables_manager/drawables_manager.hpp>
#include <common/utilities/log.hpp>
#include <common/commands/primitive_category_commands/primitive_category_commands.hpp>
#include <common/commands/primitive_commands/primitive_commands.hpp>
#include <map>
#include <common/managers/primitives/abstract_primitives_manager.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <QObject>
#include <client/managers/materials/materials_manager.hpp>
#include <cstdio>
#include <fstream>

namespace como {

class ClientPrimitivesManager : public QObject, public AbstractPrimitivesManager
{
    Q_OBJECT

    private:
        ServerInterfacePtr server_;

        DrawablesManagerPtr drawablesManager_;
        MaterialsManagerPtr materialsManager_;

    public:
        /***
         * 1. Construction
         ***/
        ClientPrimitivesManager() = delete;
        ClientPrimitivesManager( std::string sceneDirPath, ServerInterfacePtr server, DrawablesManagerPtr drawablesManager, MaterialsManagerPtr materialsManager, LogPtr log );
        ClientPrimitivesManager( const ClientPrimitivesManager& ) = delete;
        ClientPrimitivesManager( ClientPrimitivesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ClientPrimitivesManager() = default;


        /***
         * 3. Primitives management
         ***/
        std::string createPrimitive( std::string filePath, ResourceID categoryID );
    private:
        ResourceID importMeshFile( std::string oldFilePath, ResourceID categoryID );
    public:
        void instantiatePrimitive( ResourceID primitiveID );
    private:
        void instantiatePrimitive( UserID userID, ResourceID primitiveID, PackableDrawableID meshID, MaterialID materialID );
    public:


        /***
         * 4. Remote command execution
         ***/
        void executeRemoteCommand( PrimitiveCategoryCommandConstPtr command );
        void executeRemoteCommand( PrimitiveCommandConstPtr command );


        /***
         * 5. Operators
         ***/
        ClientPrimitivesManager& operator = ( const ClientPrimitivesManager& ) = delete;
        ClientPrimitivesManager& operator = ( ClientPrimitivesManager&& ) = delete;


        /***
         * 6. Signals
         ***/
    signals:
        void primitiveAdded( ResourceID id, std::string relativePath );
};

typedef std::shared_ptr< ClientPrimitivesManager > ClientPrimitivesManagerPtr;

} // namespace como

#endif // CLIENT_PRIMITIVES_MANAGER_HPP
