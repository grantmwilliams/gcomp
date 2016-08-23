/*
 *
 * As of now the main function will simply run a test program to see
 * if the compression algorihtm is working as expected.
 *
 */

#include <iostream>
#include <fstream>

#include "arithmetic_compressor.h"
#include "arithmetic_decompressor.h"
#include "model.h"
#include "utility.h"

struct my_model : public model<int>
{
    void pacify(){}
    void frozen(){}
};

int main(int argc, char* argv[])
{
    try
    {
        // Compress
        std::cout << "compressing " << argv[1] << "...\n" << std::flush;
        std::ofstream output1("temp.gcomp", std::ofstream::binary);
        std::ifstream input1(argv[1], std::ifstream::binary);

        my_model cmodel;

        compress(input1, output1, cmodel);
        output1.close();

        // Decompress
        std::ifstream input2("temp.gcomp", std::ifstream::binary);
        std::ofstream output2("temp.out", std::ofstream::binary);

        cmodel.reset(); // make sure to reset the model from the compression algorithm

        decompress(input2, output2, cmodel);
        output2.close();


        // validate results
        double bpb;
        bool verbose = true;
        int result = utility::validate(argv[1], "temp.gcomp", "temp.out", bpb, verbose);

        return 0;
    }
    catch (std::exception& ex)
    {
        std::cerr << "Failed with exception: " << ex.what() << "\n";
    }

    return 0;
}