//
// Created by Grant Williams on 8/12/16.
//

#ifndef GCOMP_BITIO_H
#define GCOMP_BITIO_H

#include <stdexcept>
#include "byteio.h"

template<typename OUTPUT>
class output_bits
{
public:
    output_bits(OUTPUT& output) : m_Output(output), m_NextByte(0), m_Mask(0x80){}

    ~output_bits()
    {
        if (m_Mask != 0x80)
        {
            m_Output.putByte(m_NextByte);
        }
    }

    void put_bit(bool val)
    {
        if (val)
        {
            m_NextByte |= m_Mask;
        }

        m_Mask >>= 1;

        if (!m_Mask)
        {
            m_Output.putByte(m_NextByte);
            m_Mask = 0x80;
            m_NextByte = 0;
        }
    }

private:
    output_bytes<OUTPUT> m_Output;
    char m_NextByte;
    unsigned char m_Mask;
};

template<typename INPUT>
class input_bits
{
public:
    input_bits(INPUT& input, int code_value_bits) :
            m_Input(input),
            m_CurrentByte(0),
            m_LastMask(1),
            m_CodeValueBits(code_value_bits) {}

    bool get_bit()
    {
        if (m_LastMask == 1)
        {
            m_CurrentByte = m_Input.getByte();

            if (m_CurrentByte < 0)
            {
                if (m_CodeValueBits <= 0)
                {
                    throw std::logic_error("EOF on input\n");
                }
                else
                {
                    m_CodeValueBits -= 8;
                }
            }
            m_LastMask = 0x80;
        }
        else
        {
            m_LastMask >>= 1;
        }

        return (m_CurrentByte& m_LastMask) != 0;
    }

private:
    input_bytes<INPUT> m_Input;
    int m_CurrentByte;
    unsigned char m_LastMask;
    int m_CodeValueBits;
};

#endif //GCOMP_BITIO_H
