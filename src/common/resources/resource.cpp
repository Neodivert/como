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

#include "resource.hpp"

namespace como {

/***
 * 1. Construction
 ***/

Resource::Resource( const ResourceID &id, const std::string &name ) :
    header_({ id, name })
{}


/***
 * 3. Getters
 ***/

ResourceHeader Resource::header() const
{
    return header_;
}


ResourceID Resource::id() const
{
    return header_.id;
}


std::string Resource::name() const
{
    return header_.name;
}

} // namespace como
