#include "file_not_open_exception.hpp"

namespace como {

/***
 * 1. Construction
 ***/

FileNotOpenException::FileNotOpenException( std::string filePath ) :
    std::runtime_error( std::string( "Could't open file [" ) + filePath + "]" )
{}

} // namespace como
