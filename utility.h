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
    std::streampos file_size(const std::string& file_name)
    {

        std::streampos fsize = 0;
        std::ifstream file( file_name, std::ios::binary );

        fsize = file.tellg();
        file.seekg( 0, std::ios::end );
        fsize = file.tellg() - fsize;
        file.close();

        return fsize;
    }

    std::string green(const std::string& phrase)
    {
        return "\33[1;32m" + phrase + "\33[0m";
    }

    std::string red(const std::string& phrase)
    {
        return "\33[1;31m" + phrase + "\33[0m";
    }


    template<typename T>
    int validate (const std::string& input_file,
              const std::string& compressed_file,
              const std::string& output_file,
              T& bpb,
              bool verbose)
    {
        std::ifstream in(input_file.c_str(), std::ifstream::binary);

        if (!in)
        {
            std::cout << "Validate error opening input file: " << input_file << "\n";
            return 255;
        }

        std::ifstream compressed(compressed_file.c_str(), std::ifstream::binary);

        if (!compressed)
        {
            std::cout << "Validate error opening compressed file: " << compressed_file << "\n";
            return 255;
        }

        std::ifstream out(output_file.c_str(), std::ifstream::binary);
        if (!out)
        {
            std::cout << "Validate error opening compressed file: " << output_file << "\n";
            return 255;
        }

        in.seekg(0, std::ios::end);
        out.seekg(0, std::ios::end);
        compressed.seekg(0, std::ios::end);

        auto in_length          = in.tellg();
        auto out_length         = out.tellg();
        auto compressed_length  = compressed.tellg();

        in.seekg(0, std::ios::beg);
        out.seekg(0, std::ios::beg);

        if (verbose)
        {
            std::cout << "Input length: " << in_length << "\n"
                      << "Output length: " << out_length << "\n"
                      << "Compressed length: " << compressed_length << "\n"
                      << "Compression ratio: " <<  (100 * ((double)compressed_length / (double) in_length)) << "%\n \n";
        }

        if (in_length != out_length)
        {
            std::cout << "Error, input file and output file have different lengths\n";
            return 255;
        }

        if (static_cast<long long>(in_length) == 0)
        {
            bpb = 8.0;
        }
        else
        {
            bpb = compressed_length * 8.0 / in_length;
        }

        if (verbose)
        {
            std::cout << "Compressed to " << bpb << " bits per byte\n \n";
        }

        int c1, c2;

        while (c1 = in.get(), c2 = out.get(), c1 != -1 || c2 != -1)
        {
            if (c1 != c2)
            {
                int i = 1;
                std::cerr << "Error comparing at position: " << (std::streamoff(in.tellg()) - 1) << "\n";
                return 255;
            }
        }

        if (verbose)
        {
            std::cout << green("All test cases passed.") << "\n \n";

        }

        return 0;
    }
}

#endif //GCOMP_UTILITY_H
