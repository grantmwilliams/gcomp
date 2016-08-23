//
// Created by Grant Williams on 8/22/16.
//

#ifndef GCOMP_COMPRESS_H
#define GCOMP_COMPRESS_H

#include <iostream>
#include <fstream>

#include "model.h"
#include "arithmetic_compressor.h"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "missing command line arguments \n";
        return 255;
    }

    try
    {
        std::cout << "compressing...\n";

        std::ifstream input(argv[1], std::ifstream::binary);
        std::ofstream output(argv[2], std::ofstream::binary);
        model<int, 16, 14> cmodel;
        cmodel.dump("cmodel", std::clog);

        compress(input, output, cmodel);
        std::cout << cmodel.m_bytesProcessed << "\n";

        return 0;
    }
    catch
    {
        
    }



}



#endif //GCOMP_COMPRESS_H
