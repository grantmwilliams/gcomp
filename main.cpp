/*
 *
 * As of now the main function will simply run a test program to see
 * if the compression algorihtm is working as expected.
 *
 */

#include <iostream>
#include <fstream>

#include "arithmetic_compressor.h"
#include "model.h"
#include "utility.h"

int main()
{

    try
    {
        std::ifstream input("pride_and_prejudice.txt", std::ifstream::binary);
        std::ofstream output("compressed_output", std::ofstream::binary);

        model<int, 16, 15>cmodel;
        cmodel.dump("cmodel", std::clog);

        std::cout << "Compressing...\n";
        compress(input, output, cmodel);
        std::cout << "bytes processed: " << cmodel.m_bytesProcessed << "\n";
        output.close();

        long long int initial_size = file_size("pride_and_prejudice.txt");
        long long int final_size  = file_size("compressed_output");
        double ratio = (double) final_size /  (double) initial_size;

        std::cout << "\n---------------------------------------\n\n";
        std::cout << "Initial size: " << initial_size << "\n";
        std::cout << "Final size: " << final_size << "\n";
        std::cout << "Compression ratio: " << ratio * 100 << "%\n";

        return 0;
    }
    catch (std::exception& ex)
    {
        std::cerr << "Failed with exception " << ex.what() << "\n";
    }

    return 255;
}