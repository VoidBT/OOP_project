#pragma once
#include "Freight.h"
#include "Cargo.h"

using namespace std;

class Match {
public:
    Freight freight;
    Cargo cargo;
    Match(const Freight& f, const Cargo& c);
};