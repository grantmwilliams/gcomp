//
// Created by Grant Williams on 8/12/16.
//

#ifndef GCOMP_MODEL_H
#define GCOMP_MODEL_H

#include <iostream>
#include <stdexcept>

#include "model_metrics.h"

template<typename CODE_VALUE_ = unsigned int,
         int CODE_VALUE_BITS_ = (std::numeric_limits<CODE_VALUE_>::digits + 3) / 2,
         int FREQUENCY_BITS_  = std::numeric_limits<CODE_VALUE_>::digits - CODE_VALUE_BITS_>

struct model : public model_metrics<CODE_VALUE_, CODE_VALUE_BITS_, FREQUENCY_BITS_>
{
    typedef model_metrics<CODE_VALUE_, CODE_VALUE_BITS_, FREQUENCY_BITS_> metrics;
    typedef typename metrics::prob prob;
    typedef CODE_VALUE_ CODE_VALUE;
    using metrics::MAX_CODE;
    using metrics::MAX_FREQ;
    using metrics::CODE_VALUE_BITS;
    using metrics::ONE_FOURTH;
    using metrics::ONE_HALF;
    using metrics::THREE_FOURTHS;

    // variables used by the model.

    /*
     * Character is defined by the range cumulative_frequency[a],
     * cumulative_frequency[a+1], whith cumulative_frequency[257]
     * containing thde total count for the model.
     * Entry 256 is the EOF.
     */

    CODE_VALUE cumulative_frequency[258];

    unsigned long long m_bytesProcessed;
    static_assert(MAX_FREQ > 257, "Not enough code bits to represent the needed symbol library\n");

    model()
    {
        reset();
    }

    void reset()
    {
        for (int i = 0; i < 258; i++)
        {
            cumulative_frequency[i] = i;
        }
        m_bytesProcessed = 0;
        m_frozen = 0;
    }

    virtual inline void pacify()
    {
        if ( (++m_bytesProcessed % 1000) == 0)
        {
            std::cout << m_bytesProcessed << "\r";
        }
    }

    virtual void frozen()
    {
        std::cout << "Frozen at: " << m_bytesProcessed << "\n";
    }

    void inline update(int c)
    {
        for (int i = c + 1; i < 258; i++)
        {
            cumulative_frequency[i]++;
        }

        if (cumulative_frequency[257] >= MAX_FREQ)
        {
            m_frozen = true;
            frozen();
        }
    }

    prob getProbability(int c)
    {
        prob p = {cumulative_frequency[c], cumulative_frequency[c+1], cumulative_frequency[257]};

        if (! m_frozen)
        {
            update(c);
        }
        pacify();

        return p;
    }

    prob getChar(CODE_VALUE scaled_value, int& c)
    {
        pacify();

        for (int i = 0; i < 257; i++)
        {
            if (scaled_value < cumulative_frequency[i+1])
            {
                c = i;
                prob p = {cumulative_frequency[i], cumulative_frequency[i+1], cumulative_frequency[257]};

                if (! m_frozen)
                {
                    update(c);
                }
                return p;
            }
        }
        throw std::logic_error("error\n");
    }

    CODE_VALUE getCount()
    {
        return cumulative_frequency[257];
    }

    bool m_frozen;
};

#endif //GCOMP_MODEL_H
