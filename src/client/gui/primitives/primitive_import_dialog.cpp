#include "primitive_import_dialog.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveImportDialog::PrimitiveImportDialog( ClientPrimitivesManager *primitivesManager ) :
    QDialog(),
    primitivesManager_( primitivesManager )
{
    QFormLayout* layout = new QFormLayout;

    fileInput_ = new QPushButton( "Select file" );

    QObject::connect( fileInput_, &QPushButton::clicked, [this](){
        fileInput_->setText( QFileDialog::getOpenFileName(this,
            tr("Open file"), "/home/", tr("Primitive Files (*.obj)") ) );
    });

    layout->addRow( "Mesh file: ", fileInput_ );

    setLayout( layout );
}

} // namespace como
