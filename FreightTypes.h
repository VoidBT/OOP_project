#pragma once // Include guard
#include <string>
#include <vector>
#include <memory>    // For std::shared_ptr
#include <stdexcept> // For std::invalid_argument
#include "Freight.h" // FreightExtended inherits from Freight

enum class FreightType { MINI_MOVER, CARGO_CRUISER, MEGA_CARRIER };

class FreightExtended : public Freight {
private:
    FreightType type;
    int maxCapacity;
    std::vector<std::string> assignedCargos; // Stores cargo IDs

public:
    FreightExtended(const std::string& id, int t, const std::string& d, FreightType type);

    FreightType getType() const;
    int getMaxCapacity() const;
    int getCurrentLoad() const;
    bool isFull() const;
    bool canAcceptMore() const;
    bool assignCargo(const std::string& cargoId);
    bool removeCargo(const std::string& cargoId);
    void clearAssignedCargos(); // Added previously
    const std::vector<std::string>& getAssignedCargos() const;

    static std::string typeToString(FreightType type);
    static FreightType stringToType(const std::string& typeStr);
};