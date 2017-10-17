#pragma once
#include <eoslib/math.hpp>

namespace bancor {
	uint64_t allocate(uint64_t eos_amount) {
            uint64_t percent_allocated = double_div(i64_to_double(eos_amount), i64_to_double(1000ll*1000ll*1000ll));
            return percent_allocated;
        }
}
