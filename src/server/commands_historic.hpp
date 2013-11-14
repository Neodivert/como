/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef COMMANDS_HISTORIC_HPP
#define COMMANDS_HISTORIC_HPP

#include <thread>
#include <list>
#include "../common/packets/packets.hpp"

namespace como {

typedef std::list< std::shared_ptr< const SceneCommand > > CommandsList;

class CommandsHistoric
{
    private:
        // List of commands in the historic.
        CommandsList commands_;

        // Mutex for exclusive access to the previous list of commands.
        // The mutable keyword allow us to modify the mutex (by calling lock()
        // and unlock()) while preserving the logical constness of a given
        // const instance of CommandsHistoric class.
        // http://stackoverflow.com/questions/105014/does-the-mutable-keyword-have-any-purpose-other-than-allowing-the-variable-to
        mutable std::mutex commandsMutex_;

    public:
        /***
         * 2. Getters
         ***/
        unsigned int getSize() const ;


        /***
         * 3. Historic modification
         ***/
        void addCommand( SceneCommandConstPtr command );


        /***
         * 4. Auxiliar methods
         ***/
        bool fillSceneUpdatePacket( SceneUpdate& packet,
                                    const unsigned int firstCommand,
                                    const unsigned int nCommands ) const ;
};

typedef std::shared_ptr< CommandsHistoric > CommandsHistoricPtr;
typedef std::shared_ptr< const CommandsHistoric > CommandsHistoricConstPtr;

}

#endif // COMMANDS_HISTORIC_HPP
