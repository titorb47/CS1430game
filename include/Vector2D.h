#ifndef vector2D_h
#define vector2D_h

#include <vector>
#include <iostream>
using namespace std;

struct Vector2D {
	double x;
	double y;
	Vector2D() : x(0), y(0){};
	Vector2D(double x, double y) : x(x) , y(y){};

	Vector2D& Add(const Vector2D& vector2);
	Vector2D& Subtract(const Vector2D& vector2);
	Vector2D& Multiply (const Vector2D& vector2);
	Vector2D& Divide(const Vector2D& vector2);

	friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

	Vector2D& operator+=(const Vector2D& v2);
	Vector2D& operator-=(const Vector2D& v2);
	Vector2D& operator*=(const Vector2D& v2);
	Vector2D& operator/=(const Vector2D& v2);

	//The dev made this a const int& i for some reason.
	//I did not find that necessary
	Vector2D& operator*(int i);


	Vector2D& Zero();

	friend ostream& operator<< (ostream& stream, const Vector2D& vecc);
};

#endif
