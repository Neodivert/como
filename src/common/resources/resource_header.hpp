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

#ifndef RESOURCE_HEADER_HPP
#define RESOURCE_HEADER_HPP

#include <common/ids/resource_id.hpp>
#include <string>
#include <list>

namespace como {

struct ResourceHeader {
    const ResourceID id;
    std::string name;

    ResourceHeader( const ResourceID& id, const std::string& name ) :
        id( id ),
        name( name )
    {}
};

typedef std::list< ResourceHeader > ResourceHeadersList;

} // namespace como

#endif // RESOURCE_HEADER_HPP
