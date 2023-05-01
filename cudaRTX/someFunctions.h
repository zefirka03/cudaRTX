#pragma once
double toDouble(std::string s) {
	double out;
	std::stringstream ss;
	ss << s;
	ss >> out;
	return out;
}

int toInt(std::string s) {
	int out;
	std::stringstream ss;
	ss << s;
	ss >> out;
	return out;
}