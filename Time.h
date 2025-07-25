#pragma once

//Done by Brendan Tjung Yew Wang 2401765

class Time {
private:
	// Time is represented as a 24-hour format
	int hour, minute;

public:
	Time();
	Time(int time);

	int operator- (const Time& other) const;
	
	int getRawTime() const;

	bool isWithinLimit(const Time& other) const;
};