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

#ifndef RESOURCE_SELECTION_RESPONSE_HPP
#define RESOURCE_SELECTION_RESPONSE_HPP

#include <common/commands/resource_commands/resource_command.hpp>

namespace como {

// TODO: Rename to ResourceSelectionResponseCommand.
class ResourceSelectionResponse : public ResourceCommand
{
    private:
        PackableUint8< bool > selectionResponse_;

    public:
        /***
         * 1. Construction
         ***/
        ResourceSelectionResponse();
        ResourceSelectionResponse( const ResourceID& resourceID, bool response );
        ResourceSelectionResponse( const ResourceSelectionResponse& );
        ResourceSelectionResponse( ResourceSelectionResponse&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourceSelectionResponse() = default;


        /***
         * 3. Getters
         ***/
        bool getResponse() const;


        /***
         * 4. Operators
         ***/
        ResourceSelectionResponse& operator = ( const ResourceSelectionResponse& ) = delete;
        ResourceSelectionResponse& operator = ( ResourceSelectionResponse&& ) = delete;
};

} // namespace como

#endif // RESOURCE_SELECTION_RESPONSE_HPP
