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

#ifndef MATERIALS_SELECTION_HPP
#define MATERIALS_SELECTION_HPP

#include <client/managers/selections/resources/resources_selection.hpp>
#include <client/models/3d/materials/material.hpp>

namespace como {

class MaterialsSelection : public virtual ResourcesSelection<Material>
{
public:
    MaterialsSelection();
};

} // namespace como

#endif // MATERIALS_SELECTION_HPP
