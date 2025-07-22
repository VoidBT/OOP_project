#pragma once // Include guard
#include <vector>
#include <string>
#include <memory> // For std::shared_ptr
#include "FreightTypes.h" // Include FreightType.h for FreightExtended

class FStorage {
private:
    std::vector<std::shared_ptr<FreightExtended>> freights; // Store shared_ptr to FreightExtended

public:
    void addFreight(std::shared_ptr<FreightExtended> freight); // Accepts shared_ptr
    void editFreight(const std::string& id, int newTime, const std::string& newDest);
    void deleteFreight(const std::string& id);
    const std::vector<std::shared_ptr<FreightExtended>>& getAllFreights() const; // Returns shared_ptrs
    void printAll() const;
    std::shared_ptr<FreightExtended> findFreight(const std::string& id); // Returns shared_ptr
};