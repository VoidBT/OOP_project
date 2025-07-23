#include "Freight.h"

Freight::Freight(const string& id, int time, const string& dest, const string& type)
    : id(id), time(time), dest(dest), type(type) {
}

string Freight::getID() const { return id; }
int Freight::getTime() const { return time; }
string Freight::getDest() const { return dest; }
string Freight::getType() const { return type; } // NEW: Getter for type

void Freight::setTime(int newTime) { time = newTime; }
void Freight::setDest(const string& newDest) { dest = newDest; }
void Freight::setType(const string& newType) { type = newType; }
