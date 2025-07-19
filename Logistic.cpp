#include "Logistic.h"
#include <string>

using namespace std;

Logistic::Logistic(const string& id) : ID(id) {}

Logistic::~Logistic() {}

string Logistic::getID() const {
    return ID;
}

hi Brendan added