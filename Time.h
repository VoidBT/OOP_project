#pragma once

class Time {
private:
	int hour, minute;

public:
	Time();
	Time(int time);

	int operator- (const Time& other) const;
	
	int getRawTime() const;

	bool isWithinMinutes(const Time& other, int minutes) const;
};