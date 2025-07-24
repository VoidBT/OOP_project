#include "Time.h"
#include <cstdlib>

Time::Time() {
	hour = 0;
	minute = 0;
}
 
Time::Time(int time) : hour(time/100), minute(time%100) {

}

int Time::operator- (const Time& other) const {
	int h = 60 * (hour - other.hour),
		m = minute - other.minute;
	m = h + m; // Total minutes difference
	return m;
}

int Time::getRawTime() const {
	return hour * 100 + minute;
}

bool Time::isWithinLimit(const Time& other) const {
	int min_diff = *this - other;
	return abs(min_diff) <= 15;
}