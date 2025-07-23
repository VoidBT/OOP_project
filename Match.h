#pragma once // Include guard
#include <string>
#include "Freight.h" // For basic Freight object
#include "Cargo.h"   // For Cargo object

struct Match {
    Freight freight;
    Cargo cargo;

    Match(const Freight& f, const Cargo& c) : freight(f), cargo(c) {}
};