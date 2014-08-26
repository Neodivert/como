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

#ifndef PRIMITIVE_IMPORT_DIALOG_HPP
#define PRIMITIVE_IMPORT_DIALOG_HPP

#include <QDialog>
#include <client/managers/managers/primitives/client_primitives_manager.hpp>
#include <QFormLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QMessageBox>
#include <client/gui/resources/resource_selector.hpp>

namespace como {

class PrimitiveImportDialog : public QDialog
{
    Q_OBJECT

    private:
        ClientPrimitivesManager* primitivesManager_;

        QPushButton* fileInput_;
        ResourceSelector* primitiveCategorySelector_;

        QPushButton* okButton_;
        QPushButton* cancelButton_;

    public:
        /***
         * 1. Construction
         ***/
        PrimitiveImportDialog() = delete;
        PrimitiveImportDialog( ClientPrimitivesManager* primitivesManager );
        PrimitiveImportDialog( const PrimitiveImportDialog& ) = delete;
        PrimitiveImportDialog( PrimitiveImportDialog&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~PrimitiveImportDialog() = default;


        /***
         * 3. Data validation
         ***/
    public slots:
        void validate();
    public:


        /***
         * 4. Operators
         ***/
        PrimitiveImportDialog& operator = ( const PrimitiveImportDialog& ) = delete;
        PrimitiveImportDialog& operator = ( PrimitiveImportDialog&& ) = delete;
};

} // namespace como

#endif // PRIMITIVE_IMPORT_DIALOG_HPP
