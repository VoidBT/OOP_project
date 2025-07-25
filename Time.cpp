#include "Time.h"
#include <cstdlib>

//Done by Brendan Tjung Yew Wang 2401765

// Default constructor initializes time to 00:00
Time::Time() {
	hour = 0;
	minute = 0;
}

// Constructor that takes an integer in HHMM format
Time::Time(int time) : hour(time/100), minute(time%100) {

}

// Overloaded operator to calculate the difference in minutes between two Time objects
int Time::operator- (const Time& other) const {
	int h = 60 * (hour - other.hour),
		m = minute - other.minute;
	m = h + m; // Total minutes difference
	return m;
}

// Returns the time in HHMM format
int Time::getRawTime() const {
	return hour * 100 + minute;
}

// Checks if the time difference between two Time objects is within 15 minutes
bool Time::isWithinLimit(const Time& other) const {
	int min_diff = *this - other;
	return abs(min_diff) <= 15;
}