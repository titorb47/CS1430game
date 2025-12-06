#include "Vector2D.h"

Vector2D& Vector2D::Add(const Vector2D& vec) {
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}


Vector2D& Vector2D::Subtract(const Vector2D& vec) {
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}


Vector2D& Vector2D::Multiply(const Vector2D& vec) {
	this->x *= vec.x;
	this->y *= vec.y;
	return *this;
}


Vector2D& Vector2D::Divide(const Vector2D& vec) {
	this->x /= vec.x;
	this->y /= vec.y;
	return *this;
}

Vector2D& operator+(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.Add(vec2);
}

Vector2D& operator-(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.Subtract(vec2);
}

Vector2D& operator*(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.Multiply(vec2);
}

Vector2D& operator/(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.Divide(vec2);
}



Vector2D& Vector2D::operator+=(const Vector2D& vec2) {
	return this->Add(vec2);
}

Vector2D& Vector2D::operator-=(const Vector2D& vec2) {
	return this->Subtract(vec2);
}

Vector2D& Vector2D::operator*=(const Vector2D& vec2) {
	return this->Multiply(vec2);
}

Vector2D& Vector2D::operator/=(const Vector2D& vec2) {
	return this->Divide(vec2);
}

Vector2D& Vector2D::operator*(int i) {
	this->x *= i;
	this->y *= i;

	return *this;
}

Vector2D& Vector2D::Zero() {
	this->x = 0;
	this->y = 0;

	return *this;
}

ostream& operator<< (ostream& out, const Vector2D& vec) {
	out << "(" << vec.x << "," << vec.y << ")";
	return out;
}

