///*****************************************************************///
/// 						Vector2D Class							///
///*****************************************************************///

#include <math.h>
#include "Vector2D.h"

//////////////////////////////////
// Constructor and Destructor	//
//////////////////////////////////

Vector2D::Vector2D() :	x(0.0f),
						y(0.0f)
{}

Vector2D::Vector2D( float _x, float _y ) :	x(_x),
											y(_y)
{}

Vector2D::Vector2D( const Vector2D& other ) : 	x( other.x ),
												y( other.y )
{}


Vector2D::~Vector2D()
{}

//////////////////////////////////
// Manipulation Functions		//
//////////////////////////////////

Vector2D& Vector2D::zero()
{
	x = 0;
	y = 0;
	return (*this);
}

float Vector2D::length() const
{
	return sqrtf(x*x + y*y);
}

void Vector2D::normalise()
{
	float l = length();
	if ( l > 0)
	{
		(*this) *= 1 / l;
	}
}

void Vector2D::truncate( float max_value )
{
	if ( length() > max_value )
	{
		normalise();
		*this *= max_value;
	}
}

float Vector2D::dot( const Vector2D& v2 ) const
{
	return ( (this->x*v2.x) + (this->y*v2.y) );
}

//////////////////////////////////
// Static Functions				//
//////////////////////////////////

Vector2D Vector2D::lerp( const Vector2D& start, const Vector2D& end, const float& percent )
{
	return (start + (end-start)*percent);
}

Vector2D Vector2D::nlerp( const Vector2D& start, const Vector2D& end, const float& percent )
{
	Vector2D result = ( lerp(start, end, percent) );
	result.normalise();
	return result;
}

//////////////////////////////////
// Operators					//
//////////////////////////////////

/*** ADDITION ***/

Vector2D Vector2D::operator+(const Vector2D &v2) const
{
	return Vector2D(x + v2.x, y + v2.y);
}

Vector2D& Vector2D::operator+=(const Vector2D &v2)
{
	x += v2.x;
	y += v2.y;

	return *this;
}

/*** SUBTRACTION ***/

Vector2D Vector2D::operator-(const Vector2D &v2) const
{
	return Vector2D(x - v2.x, y - v2.y);
}

Vector2D& Vector2D::operator-=(const Vector2D &v2)
{
	x -= v2.x;
	y -= v2.y;

	return *this;
}

/*** MULTIPLICATION ***/

Vector2D Vector2D::operator*(const Vector2D &v2) const
{
	return Vector2D(x * v2.x, y * v2.y);
}

Vector2D& Vector2D::operator*=(const Vector2D &v2)
{
	x *= v2.x;
	y *= v2.y;

	return *this;
}

Vector2D Vector2D::operator*(float scalar) const
{
	return Vector2D(x * scalar, y * scalar);
}

Vector2D& Vector2D::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;

	return *this;
}

/*** DIVISION ***/

Vector2D Vector2D::operator/(const Vector2D &v2) const
{
	if (x == 0 || y == 0 || v2.x == 0 || v2.y == 0)
		return Vector2D(0,0);

	return Vector2D(x / v2.x, y / v2.y);
}

Vector2D& Vector2D::operator/=(const Vector2D &v2)
{
	if (x == 0 || y == 0 || v2.x == 0 || v2.y == 0)
	{
		x = 0;
		y = 0;
		return *this;
	}

	x /= v2.x;
	y /= v2.y;

	return *this;
}

Vector2D Vector2D::operator/(float scalar) const
{
	if (x == 0 || y == 0 || scalar == 0)
		return Vector2D(0,0);

	return Vector2D(x / scalar, y / scalar);
}

Vector2D& Vector2D::operator/=(float scalar)
{
	if (x == 0 || y == 0 || scalar == 0)
	{
		x = 0;
		y = 0;
		return *this;
	}

	x /= scalar;
	y /= scalar;

	return *this;
}

/*** ASSIGNMENT AND EQUALITY ***/

Vector2D& Vector2D::operator=(  const Vector2D& other )
{
	if (*this == other)
		return *this;

	x = other.x;
	y = other.y;
	return *this;
}

bool Vector2D::operator==( const Vector2D& other ) const
{
	if (other.x != x) return false;
	if (other.y != y) return false;
	return true;
}

bool Vector2D::operator!=( const Vector2D& other ) const
{
	if (other.x != x) return true;
	if (other.y != y) return true;
	return false;
}
