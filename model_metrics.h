//
// Created by Grant Williams on 8/12/16.
//

#ifndef GCOMP_MODEL_METRICS_H
#define GCOMP_MODEL_METRICS_H

#include <iostream>
#include <typeinfo>
#include <limits>
#include <stdint.h>

template<typename CODE_VALUE, int CODE_VALUE_BITS_, int FREQUENCY_BITS_>
struct model_metrics
{
    static const int PRECISION              = std::numeric_limits<CODE_VALUE>::digits;
    static const int CODE_VALUE_BITS        = CODE_VALUE_BITS_;
    static const int FREQUENCY_BITS         = FREQUENCY_BITS_;
    static const CODE_VALUE MAX_CODE        = (CODE_VALUE(1) << CODE_VALUE_BITS) - 1;
    static const CODE_VALUE MAX_FREQ        = (CODE_VALUE(1) << FREQUENCY_BITS) - 1;
    static const CODE_VALUE ONE_FOURTH      = CODE_VALUE(1) << (CODE_VALUE_BITS - 2);;
    static const CODE_VALUE ONE_HALF        = 2 * ONE_FOURTH;
    static const CODE_VALUE THREE_FOURTHS   = 3 * ONE_FOURTH;

    static_assert( std::numeric_limits<CODE_VALUE>::digits >= CODE_VALUE_BITS,
                   "CODE_VALUE_BITS is too large to fit in CODE_VALUE type\n");

    static_assert( FREQUENCY_BITS <= (CODE_VALUE_BITS + 2),
                   "FREQUENCY_BITS can be no greater than CODE)_VALUE_BITS - 2\n");

    static_assert( (CODE_VALUE_BITS + FREQUENCY_BITS) <= PRECISION,
                   "CODE_VALUE_BITS + FREQUENCY_BITS cannot exceed precision of CODE_VALUE\n");

    template<typename STRING, typename STREAM>
            static void dump(const STRING& name, STREAM& s)
    {
        s << "Model " << name << " created with:\n"
          << "CODE_VALUE of type "   << typeid(CODE_VALUE).name() << " with " << PRECISION << " bits\n"
          << "CODE_VALUE_BITS " << CODE_VALUE_BITS << " bits giving MAX_CODE of "      << MAX_CODE << "\n"
          << "FREQUENCY_BITS "  << FREQUENCY_BITS  << " bits giving MAX_FREQUENCY of " << MAX_FREQ << "\n"
          << "MAX_CODE: "       << MAX_CODE      << " (0x" << std::hex << MAX_CODE      << std::dec << ")\n"
          << "MAX_FREQ: "       << MAX_FREQ      << " (0x" << std::hex << MAX_FREQ      << std::dec << ")\n"
          << "ONE_FOURTH: "     << ONE_FOURTH    << " (0x" << std::hex << ONE_FOURTH    << std::dec << ")\n"
          << "ONE_HALF: "       << ONE_HALF      << " (0x" << std::hex << ONE_HALF      << std::dec << ")\n"
          << "THREE_FOURTHS: "  << THREE_FOURTHS << " (0x" << std::hex << THREE_FOURTHS << std::dec << ")\n";
    }

    struct prob
    {
        CODE_VALUE low;
        CODE_VALUE high;
        CODE_VALUE count;
    }; // prob

}; // model_metrics

#endif //GCOMP_MODEL_METRICS_H
