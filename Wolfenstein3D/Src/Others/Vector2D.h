#pragma once

#include <iostream>
#include <math.h>

using namespace std;

/// <summary>
/// Class used to store the x & y coordinates and the possible typical operations we might need to use
/// </summary>
class Vector2D {
	float x_;  // first coordinate
	float y_;  // second coordinate
public:
	Vector2D();
	Vector2D(const Vector2D &v);
	Vector2D(float x, float y);
	virtual ~Vector2D();

	inline float getX() const {
		return x_;
	}
	inline float getY() const {
		return y_;
	}
	inline void setX(float x) {
		x_ = x;
	}
	inline void setY(float y) {
		y_ = y;
	}
	inline void set(const Vector2D &v) {
		x_ = v.x_;
		y_ = v.y_;
	}
	inline void set(float x, float y) {
		x_ = x;
		y_ = y;
	}
	inline float magnitude() const {
		return (float)sqrt(pow(x_, 2) + pow(y_, 2));
	}

	Vector2D rotate(float degrees) const;
	double angle(const Vector2D &v) const;
	Vector2D normalize() const;

	inline Vector2D operator-(const Vector2D &v) const {
		return Vector2D(x_ - v.x_, y_ - v.y_);
	}

	inline Vector2D operator+(const Vector2D &v) const {
		return Vector2D(x_ + v.x_, y_ + v.y_);
	}
	inline Vector2D operator*(float d) const {
		return Vector2D(x_ * d, y_ * d);
	}
	inline float operator *(const Vector2D &d) const {
		return d.x_ * x_ + d.y_ * y_;
	}
	Vector2D operator/(float d) const {
		return Vector2D(x_ / d, y_ / d);
	}
	friend ostream& operator<<(ostream &os, const Vector2D &v);
};
