/** CSci-4611 Assignment 3:  Earthquake
 */

#include <sstream>
#include "earthquake.h"


Earthquake::Earthquake() {
}

Earthquake::Earthquake(std::string s) {
    line = s;
}

Date Earthquake::date() {
    int year = ParseInt(line.substr(12,4));
    int month = ParseInt(line.substr(17,2));
    int day = ParseInt(line.substr(20,2));
    int hour = ParseInt(line.substr(24,2));
    int minute = ParseInt(line.substr(27,2));
    double second = ParseFloat(line.substr(30,5));

    return Date(month, day, year, hour, minute, second);
}

double Earthquake::longitude() {
    return ParseFloat(line.substr(44,8));
}

double Earthquake::latitude() {
    return ParseFloat(line.substr(37,7));
}

double Earthquake::magnitude() {
    return ParseFloat(line.substr(66,4));
}

double Earthquake::ParseFloat(std::string s) {
    std::stringstream ss(s);
    double f;
    ss >> f;
    return f;
}

int Earthquake::ParseInt(std::string s) {
    std::stringstream ss(s);
    int i;
    ss >> i;
    return i;
}
bool Earthquake::equals(Date date, double longitude, double latitude, double magnitude, Date date2, double longitude2, double latitude2, double magnitude2) {
	if (date.year() == date2.year()) {
		if (date.month() == date2.month()) {
			if (date.day() == date2.day()) {
				if (longitude == longitude2) {
					if (latitude == latitude2) {
						if (magnitude == magnitude2) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
