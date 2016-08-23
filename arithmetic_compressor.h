//
// Created by Grant Williams on 8/12/16.
//

#ifndef GCOMP_COMPRESSOR_H
#define GCOMP_COMPRESSOR_H

#include <stdexcept>

#include "bitio.h"
#include "byteio.h"

#ifdef LOG
#include <iomanip>
#include <iostream>
#endif

/*
 *
 * The arithmetic arithmetic_compressor is parameterized on the types of input, output, and modeled objects.
 * The arithmetic_compressor can be called easily using the compress() function at the bottom of the header file.
 *
 */

template<typename INPUT, typename OUTPUT, typename MODEL>
class arithmetic_compressor
{
    typedef typename MODEL::CODE_VALUE CODE_VALUE;
    typedef typename MODEL::prob prob;

public:
    arithmetic_compressor(INPUT& input, OUTPUT& output, MODEL& model):
            m_input(input),
            m_output(output),
            m_model(model) {}

    int operator()()
    {
#ifdef LOG
        std::ostream log("arithmetic_compressor.log\n");
        log << std::hex;
#endif
        int pending_bits = 0;
        CODE_VALUE low = 0;
        CODE_VALUE high = MODEL::MAX_CODE;

        for(; ;)
        {
            int c = m_input.getByte();

            if (c == -1)
            {
                c = 256;
            }
#ifdef LOG
            log << std::hex << "0x" << std::setw(2) << std::setfill('0') << c;
            if (c > 0x20 && c <= 0x7f)
            {
                log << "(" << char(c) << ")";
            }

            log << " 0x" << low << " 0x" << high << " => ";
#endif
            prob p = m_model.getProbability(c);
            CODE_VALUE range = high - low + 1;
            high = low + (range * p.high / p.count) - 1;
            low = low + (range * p.low / p.count);
#ifdef LOG
            log << "0x" << low << " 0x" << high << "\n";
#endif

            /*
             *
             * On each pass there are 6 possible configurations of high and low.
             *
             * When high or low is converging, we output the Most Significant Bit
             * and upshift high and low.
             *
             * When they are in a near-convergent state, we upshift over the
             * next-to-Most Significant bit, increment pending count, leave the
             * Most-Significant-Bit intact, and dont output anything.
             *
             * If we are not converging, we do no shifting and have no output.
             *
             * high: 0xxx, low: anything : converging (output 0)
             * low: 1xxx, high: anything : converging (output 1)
             *
             * high: 10xxx, low: 01xxx : near converging
             *
             * high: 11xxx, low: 01xxx : not converging
             * high: 11xxx, low: 00xxx : not converging
             * high: 10xxx, low: 00xxx : not converging
             *
             */

            for (;;)
            {
                if (high < MODEL::ONE_HALF)
                {
                    put_bit_plus_pending(0, pending_bits);
                }
                else if (low >= MODEL::ONE_HALF)
                {
                    put_bit_plus_pending(1, pending_bits);
                }
                else if (low >= MODEL::ONE_FOURTH && high < MODEL::THREE_FOURTHS)
                {
                    pending_bits++;
                    low -= MODEL::ONE_FOURTH;
                    high -= MODEL::ONE_FOURTH;
                }
                else
                {
                    break;
                }

                high <<= 1;
                high++;

                low <<= 1;

                high &= MODEL::MAX_CODE;
                low &= MODEL::MAX_CODE;
            } // end second loop

            // 256 is our EOF code
            if (c == 256)
            {
                break;
            }

        } // end first loop

        pending_bits++;

        if (low < MODEL::ONE_FOURTH)
        {
            put_bit_plus_pending(0, pending_bits);
        }
        else
        {
            put_bit_plus_pending(1, pending_bits);
        }

#ifdef LOG
        log.close();
#endif
        return 0;
    }

    inline void put_bit_plus_pending(bool bit, int& pending_bits)
    {
        m_output.put_bit(bit);
        for (int i = 0; i < pending_bits; i++)
        {
            m_output.put_bit(!bit);
        }

        pending_bits = 0;
    }

private:
    INPUT& m_input;
    OUTPUT& m_output;
    MODEL& m_model;
};

/*
 *
 * This convencience function takes care of constructing the arithmetic_compressor
 * and the input and output objects, then calling the arithmetic_compressor.
 *
 * Letting the user of the class call a template function instead of instantating
 * the template class object erases syntax requirement bit.
 *
 */

template <typename INPUT, typename OUTPUT, typename MODEL>
int compress (INPUT& source, OUTPUT& target, MODEL& model)
{
    input_bytes<INPUT> in(source);
    output_bits<OUTPUT> out(target);
    arithmetic_compressor<input_bytes<INPUT>, output_bits<OUTPUT>, MODEL> c(in, out, model);
    return c();
}

#endif //GCOMP_COMPRESSOR_H
