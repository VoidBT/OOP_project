#pragma once // Include guard
#include <string>
#include "Freight.h" // For basic Freight object
#include "Cargo.h"   // For Cargo object

// This struct is for the basic matchFreightAndCargo function,
// which only finds potential pairings, not actual assignments.
struct Match {
    Freight freight;
    Cargo cargo;

    Match(const Freight& f, const Cargo& c) : freight(f), cargo(c) {}
};