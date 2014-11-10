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

#include <QFormLayout>
#include <QIntValidator>
#include <QMessageBox>

#include "page_ids.hpp"
#include "create_server_page.hpp"

#include <QProcess>
#include <QFileDialog>

namespace como {

const std::string NO_SCENE_FILE_SELECTED = "No scene file selected";

/***
 * 1. Initialization and destruction
 ***/

CreateServerPage::CreateServerPage( ScenePtr& scene, LogPtr log ) :
    scene_( scene ),
    portInput_( nullptr ),
    maxUsersInput_( nullptr ),
    log_( log )
{
    QFormLayout* layout = nullptr;

    // Create a scene name input.
    sceneNameInput_ = new QLineEdit( tr( "Unnamed scene" ) );

    // Create a port input.
    portInput_ = new QLineEdit;
    portInput_->setText( tr( "7777") );
    portInput_->setValidator( new QIntValidator( 0, 65535, portInput_ ) );

    // Create a max users input.
    maxUsersInput_ = new QSpinBox;
    maxUsersInput_->setMinimum( 1 );
    maxUsersInput_->setMaximum( 16 );
    maxUsersInput_->setValue( 5 );

    // Create a scene's file input.
    sceneFileInput_ = new QPushButton( NO_SCENE_FILE_SELECTED.c_str() );
    QObject::connect( sceneFileInput_, &QPushButton::clicked, [this](){
        const QString sceneFilePath =
                QFileDialog::getOpenFileName( this,
                                              tr("Open file"),
                                              "../server/data/save",
                                              tr( "COMO Scene files (*.csf)" ) );

                if( sceneFilePath.size() > 0 ){
                    sceneFileInfo_.setFile( sceneFilePath );
                    sceneFileInput_->setText( sceneFileInfo_.fileName() );
                }else{
                    sceneFileInfo_.setFile( "" );
                    sceneFileInput_->setText( NO_SCENE_FILE_SELECTED.c_str() );
                }
    });

    // Create the page's layout
    layout = new QFormLayout;
    layout->addRow( tr( "Scene name: " ), sceneNameInput_ );
    layout->addRow( tr( "Port (0 - 65535): " ), portInput_ );
    layout->addRow( tr( "Max users (1 - 16): " ), maxUsersInput_ );
    layout->addRow( tr( "Load scene from file: " ), sceneFileInput_ );

    // Set the page's title and layout.
    setTitle( tr( "Create server" ) );
    setLayout( layout );
}


/***
 * 2. Validators
 ***/

bool CreateServerPage::validatePage()
{
    log_->debug( "Creating server with port(",
                 portInput_->text().toLocal8Bit().data(),
                 ") and maxUsers(",
                 maxUsersInput_->text().toLocal8Bit().data(), ")\n" );

    char serverCommand[512];

    std::string sceneFilePath;
    if( sceneFileInfo_.isFile() ){
        sceneFilePath =
                    sceneFileInfo_.absoluteFilePath().toStdString();
    }

    sprintf( serverCommand,
             "gnome-terminal -e \" \"%s\" %d %d \"\"\"%s\"\"\" \"\"\"%s\"\"\" \"",
             SERVER_PATH,                                       // Server bin.
             atoi( portInput_->text().toLocal8Bit().data() ),   // Port.
             maxUsersInput_->value(),                           // Max. users.
             sceneNameInput_->text().toLocal8Bit().data(),      // Scene name.
             sceneFilePath.c_str() );                           // Scene file.

    log_->debug( serverCommand );

    QProcess::startDetached( serverCommand );

    QMessageBox::information( nullptr, "Server created", "The server has been created" );

    return true;
}


/***
 * 3. Auxiliar methods
 ***/

int CreateServerPage::nextId() const
{
    return PAGE_CONNECT_SERVER;
}

} // namespace como
