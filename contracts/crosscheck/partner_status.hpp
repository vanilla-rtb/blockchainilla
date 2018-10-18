#pragma once

#include <cstdint>

namespace blockchainilla {

    enum class PartnerStatus : uint8_t {
        Active,
        Closed,
        Disputed,
        OnHold,
        Undefined
    };
}