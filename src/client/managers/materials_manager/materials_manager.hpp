/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef MATERIALS_MANAGER_HPP
#define MATERIALS_MANAGER_HPP

#include <list>
#include <map>
#include <client/models/3d/materials/material.hpp>

namespace como {

typedef std::map< MaterialID, Material > MaterialsOwnershipMap;

class MaterialsManager
{
    private:
        std::list< Material > materials_;
        MaterialsOwnershipMap materialsOwners_;

    public:
        MaterialsManager();
};

} // namespace como

#endif // MATERIALS_MANAGER_HPP
