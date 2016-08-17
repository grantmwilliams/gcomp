//
// Created by Grant Williams on 8/16/16.
//

#ifndef GCOMP_ARITHMETIC_DECOMPRESSOR_H
#define GCOMP_ARITHMETIC_DECOMPRESSOR_H

#include "bitio.h"
#include "byteio.h"

#ifdef LOG
#include <iomanip>
#include <iostream>
#endif

/*
 * Templated integer arithmetic decompressor
 *
 * INPUT: expects get_bit function
 *
 * OUTPUT: expects a put_byte() function
 *
 * both functions require exceptions on errors. We
 * expects EOF to be embedded in compressed stream and
 * thus should be extracted by decoder.
 *
 * If compression went wrong, get_bit() will loop EOF() and
 * needs to return an error.
 *
 */

template<typename INPUT, typename OUTPUT, typename MODEL>
class decompressor
{
    typedef typename MODEL::CODE_VALUE CODE_VALUE;
    typedef typename MODEL::prob prob;

public:
    decompressor(INPUT& input, OUTPUT& output, MODEL& model) : m_input(input), m_output(output), m_model(model) {}

    int operator()()
    {
#ifdef LOG
        std::ofstream log("decompressor.log");
        log << std::hex;
#endif
        CODE_VALUE high = MODEL::MAX_CODE;
        CODE_VALUE low = 0;
        CODE_VALUE value = 0;

        for (int i = 0; i < MODEL::CODE_VALUE_BITS; i++)
        {
            value <<= 1;
            value += m_input.get_bit() ? 1 : 0;
        }

        for (;;)
        {
            CODE_VALUE range = high - low + 1;
            CODE_VALUE scaled_value =  ((value - low + 1) * m_model.getCount() - 1 ) / range;

            int c;
            prob p = m_model.getChar(scaled_value, c);

            if (c == 256)
            {
                break;
            }

            m_output.putByte(c);

#ifdef LOG
            log << std::hex << "0x" << std::setw(2) << std::setfill('0') << c;

            if (c > 0x20 && c < = 0x7f)
            {
                log << "(" << char(c) << ")";
            }

            log << "0x" << low << " 0x" << high << " => ";
#endif

            high = low + (range * p.high) / p.count - 1;
            low = low + (range * p.low) / p.count;

#ifdef LOG
            log << "0x" << low << " 0x" << high << "\n";
#endif

            for (;;)
            {
                if (high < MODEL::ONE_HALF)
                {
                    // nothing is done because bit == 0.
                }
                else if (low >= MODEL::ONE_HALF)
                {
                    value -= MODEL::ONE_HALF;
                    low -= MODEL::ONE_HALF;
                    high -= MODEL::ONE_HALF;
                }
                else if ( low >= MODEL::ONE_FOURTH && high < MODEL::THREE_FOURTHS)
                {
                    value -= MODEL::ONE_FOURTH;
                    low -= MODEL::ONE_FOURTH;
                    high -= MODEL::ONE_FOURTH;
                }
                else
                {
                    break;
                }

                low <<= 1;
                high <<= 1;

                high++;

                value <<= 1;

                value += m_input.get_bit() ? 1 : 0;

            } // for loop
        } // for loop

#ifdef LOG
        log << std::hex << "0x" << std::setw(2) << std::setfill('0') << 256;
        log << " 0x" << low << " 0x" << high << "\n";
#endif

        return 0;
    } // operator

private:
    OUTPUT& m_output;
    INPUT& m_input;
    MODEL& m_model;
};

/*
 * Convenience function to construct the decompressor, input, and output
 * objects and then calls decompressor.
 */

template<typename INPUT, typename OUTPUT, typename MODEL>
int decompress(INPUT& source, OUTPUT& target, MODEL& model)
{
    input_bits<INPUT> in(source, MODEL::CODE_VALUE_BITS);
    output_bytes<OUTPUT> out(target);

    decompressor<input_bits<INPUT>, output_bytes<OUTPUT>, MODEL> d(in,out, model);

    return d();
};

#endif //GCOMP_ARITHMETIC_DECOMPRESSOR_H
