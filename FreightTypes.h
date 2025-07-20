#pragma once // Add this include guard
#include <vector>
#include <string> // Required for std::string
#include <memory> // Required for std::shared_ptr and std::make_shared
#include <stdexcept> // For std::invalid_argument (used in stringToType)
#include "Freight.h" // Assuming Freight.h defines Freight

// Removed: using namespace std; // Avoid in header files

enum class FreightType { MINI_MOVER, CARGO_CRUISER, MEGA_CARRIER };

class FreightExtended : public Freight {
private:
    FreightType type;
    int maxCapacity;
    std::vector<std::string> assignedCargos; // Qualify std::

public:
    FreightExtended(const std::string& id, int t, const std::string& d, FreightType type); // Qualify std::

    FreightType getType() const;
    int getMaxCapacity() const;
    int getCurrentLoad() const;
    bool isFull() const;
    bool canAcceptMore() const;
    bool assignCargo(const std::string& cargoId); // Qualify std::
    bool removeCargo(const std::string& cargoId); // Qualify std::
    const std::vector<std::string>& getAssignedCargos() const; // Qualify std::

    static std::string typeToString(FreightType type); // Qualify std::
    static FreightType stringToType(const std::string& typeStr); // Qualify std::
};