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

    // Create widgets.
    fileInput_ = new QPushButton( "Select file" );
    primitiveCategorySelector_ = new ResourceSelector( primitivesManager->getCategoriesList() );
    okButton_ = new QPushButton( "OK" );
    cancelButton_ = new QPushButton( "Cancel" );

    // When fileInput button is clicked, open a QFileDialog.
    QObject::connect( fileInput_, &QPushButton::clicked, [this](){
        const QString filePath =
                QFileDialog::getOpenFileName( this,
                                              tr("Open file"),
                                              "data/local/primitives",
                                              tr("Primitive Files (*.obj)" ) );

        fileInfo_.setFile( filePath );
        if( filePath.size() > 0 ){
            fileInput_->setText( fileInfo_.fileName() );
        }else{
            fileInput_->setText( "Select file" );
        }
    });

    // Set accept and reject buttons behaviour.
    QObject::connect( okButton_, &QPushButton::clicked, this, &PrimitiveImportDialog::validate );
    QObject::connect( cancelButton_, &QPushButton::clicked, this, &PrimitiveImportDialog::reject );

    layout->addRow( "Mesh file: ", fileInput_ );
    layout->addRow( "Category: ", primitiveCategorySelector_ );
    layout->addRow( cancelButton_, okButton_ );

    setLayout( layout );
}


/***
 * 3. Data validation
 ***/

void PrimitiveImportDialog::validate()
{
    std::string filePath = fileInfo_.absoluteFilePath().toStdString();
    if( boost::filesystem::exists( filePath ) ){
        try {
            std::string primitiveRelativePath =
                    primitivesManager_->createPrimitive( filePath, primitiveCategorySelector_->getCurrentResourceID() );

            QMessageBox::information( nullptr, "Primitive saved", primitiveRelativePath.c_str() );

            accept();
        }catch( std::exception& ex ){
            std::string errorMessage =
                    "Error importing file [" +
                    filePath + "]: " +
                    ex.what() + "\n";
            QMessageBox::critical( nullptr, "Import error", errorMessage.c_str() );
        }
    }else{
        std::string errorMessage = std::string( "File [" ) + filePath + "] NOT FOUND";
        QMessageBox::critical( nullptr, "Input error", errorMessage.c_str() );
    }
}

} // namespace como
