#pragma once
#include <iostream>
using namespace std;
class date
{
private:
	int day;
	int month;
	int year;
public:
	date(int d = 1, int m = 1, int y = 2000) {
		day = d;
		month = m;
		year = y;
	}
	date(const date& other) {
		day = other.day;
		month = other.month;
		year = other.year;
	}
	date& operator=(const date& other) {
		if (this != &other) {
			day = other.day;
			month = other.month;
			year = other.year;
		}
		return *this;
	}
	int GetDay() {
		return day;
	}
	int GetMonth() {
		return month;
	}
	int GetYear() {
		return year;
	}
	void SetDate(int d, int m, int y) {
		day = d;
		month = m;
		year = y;
	}
};