//
// Created by Grant Williams on 8/12/16.
//

#ifndef GCOMP_UTILITY_H
#define GCOMP_UTILITY_H

#include <iostream>
#include <fstream>
#include <string>

namespace utility
{
    std::streampos file_size( const std::string& file_name )
    {

        std::streampos fsize = 0;
        std::ifstream file( file_name, std::ios::binary );

        fsize = file.tellg();
        file.seekg( 0, std::ios::end );
        fsize = file.tellg() - fsize;
        file.close();

        return fsize;
    }
}

#endif //GCOMP_UTILITY_H
