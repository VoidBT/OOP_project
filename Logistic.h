#pragma once
#include <string>

//Done by Ryan Ang Rui Heng 2400522

using namespace std;

class Logistic {
protected:
	// Unique identifier for the logistic object
    string ID;
public:
	// Constructor and destructor
    Logistic(const string& id);
    virtual ~Logistic();
    string getID() const;
};