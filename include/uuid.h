#pragma once

#include <stdint.h>
#include <random>

struct UUID {
    // NOT CORRECT
    uint64_t lo;
    uint64_t hi;

    bool operator==(const UUID& other) const {
        return lo == other.lo && hi == other.hi;
    }

    bool operator!=(const UUID& other) const {
        return !(*this == other);
    }

    static UUID random() {
        // super temporary
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dis;

        UUID uuid;
        uuid.lo = dis(gen);
        uuid.hi = dis(gen);

        return uuid;
    }
};

// hash function for UUID

namespace std {
    template<>
    struct hash<UUID> {
        size_t operator()(const UUID& uuid) const {
            return uuid.lo ^ uuid.hi * 31;
        }
    };
}