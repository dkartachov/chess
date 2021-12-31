#pragma once
#include <tuple>

class Vector {
public:
	Vector() {
		x = y = 0;
	}

	Vector(int x, int y) {
		this->x = x;
		this->y = y;
	}

	static Vector Zero() {
		return Vector(0, 0);
	}

public:
	int x, y;
};

inline Vector operator* (const double& c, const Vector& v) {
	return Vector(c * v.x, c * v.y);
}

inline Vector operator+ (const Vector& a, const Vector& b) {
	return Vector(a.x + b.x, a.y + b.y);
}

inline Vector operator+= (Vector& a, const Vector& b) {
	return Vector(a.x += b.x, a.y += b.y);
}

inline Vector operator- (const Vector& a, const Vector& b) {
	return Vector(a.x - b.x, a.y - b.y);
}

inline bool operator< (const Vector& left, const Vector& right) {
	return std::tie(left.x, left.y) < std::tie(right.x, right.y);
}

inline bool operator!= (const Vector& left, const Vector& right) {
	return left.x != right.x || left.y != right.y;
}