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

#ifndef SESSION_HPP
#define SESSION_HPP

#include <memory>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/bind.hpp>

namespace como {

const unsigned int BUFFER_SIZE = 1024;

class Session : public std::enable_shared_from_this<Session>
{
    private:
        unsigned int id_;
        boost::asio::ip::tcp::socket socket_;

        char buffer_[BUFFER_SIZE];

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Session( unsigned int id, boost::asio::ip::tcp::socket socket );
        ~Session();


        /***
         * 2. Getters
         ***/
        unsigned int getId();


        /***
         * 3. Socket reading
         ***/
        void read();
        void onRead( const boost::system::error_code& errorCode, std::size_t length );
};


typedef std::shared_ptr< Session > SessionPtr;

} // namespace como

#endif // SESSION_HPP
