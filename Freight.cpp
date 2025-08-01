#include "Freight.h"

//Done by Ryan Ang Rui Heng 2400522

// Freight.cpp
Freight::Freight() : id(), time(0), dest(), type() {

}

// Constructor with parameters
Freight::Freight(const string& id, int time, const string& dest, FreightType type)
    : id(id), time(time), dest(dest), type(type) {
}

// Getters
string Freight::getID() const { return id; }
Time Freight::getTime() const { return time; }
string Freight::getDest() const { return dest; }
FreightType Freight::getType() const { return type; }
string Freight::getTypeString() const { return typeToString(type); }

// Setters
void Freight::setTime(int newTime) { time = newTime; }
void Freight::setDest(const string& newDest) { dest = newDest; }
void Freight::setType(FreightType newType) { type = newType; }

// Static method to convert FreightType to string
string Freight::typeToString(FreightType type) {
    switch (type) {
    case FreightType::MINI_MOVER: return "MiniMover";
    case FreightType::CARGO_CRUISER: return "CargoCruiser";
    case FreightType::MEGA_CARRIER: return "MegaCarrier";
    default: return "Unknown";
    }
}
