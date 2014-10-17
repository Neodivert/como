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

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <common/resources/resource_header.hpp>

namespace como {

class Resource
{
    public:
        /***
         * 1. Construction
         ***/
        Resource( const ResourceID& id, const std::string& name );
        Resource() = delete;
        Resource( const Resource& ) = delete;
        Resource( Resource&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~Resource() = default;


        /***
         * 3. Getters
         ***/
        ResourceHeader header() const;
        ResourceID id() const;
        std::string name() const;
        virtual std::string typeName() const = 0;


        /***
         * 4. Operators
         ***/
        Resource& operator = ( const Resource& ) = delete;
        Resource& operator = ( Resource&& ) = default;


    private:
        ResourceHeader header_;
};

} // namespace como

#endif // RESOURCE_HPP
