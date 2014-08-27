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

#ifndef MATERIALS_EDITOR_HPP
#define MATERIALS_EDITOR_HPP

#include <QFrame>
#include <client/managers/managers/materials/materials_manager.hpp>

#include "materials_list.hpp"
#include "material_editor.hpp"

namespace como {

class MaterialsEditor : public QFrame, public ContainerObserver<ResourceID>
{
    Q_OBJECT

    private:
        MaterialsManagerPtr materialsManager_;

        MaterialsList* materialsList_;
        MaterialEditor* materialEditor_;

    public:
        /***
         * 1. Construction
         ***/
        MaterialsEditor( MaterialsManagerPtr materialsManager );
        MaterialsEditor( const MaterialsEditor& ) = delete;
        MaterialsEditor( MaterialsEditor&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~MaterialsEditor(){}


        /***
         * 3. Updating (Observer pattern).
         ***/
        virtual void update( ContainerAction lastContainerAction, ResourceID lastElementModified );


        /***
         * 4. Operators
         ***/
        MaterialsEditor& operator = ( const MaterialsEditor& ) = delete;
        MaterialsEditor& operator = ( MaterialsEditor&& ) = delete;
};

}

#endif // MATERIALS_EDITOR_HPP
