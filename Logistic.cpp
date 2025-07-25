#include "Logistic.h"
#include <string>

//Done by Ryan Ang Rui Heng 2400522

using namespace std;

// Constructor initializes the ID member variable
Logistic::Logistic(const string& id) : ID(id) {}

Logistic::~Logistic() {}

// Returns the ID of the logistic object
string Logistic::getID() const {
    return ID;
}