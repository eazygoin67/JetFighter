/// /////////////////////////////////////// ///
///	Vector Class							///
///											///
/// Author: 		Troy Owen				///
/// Last Edited:	23rd, October, 2014		///
/// /////////////////////////////////////// ///

#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#ifdef VECTOR_2D_STREAM_OPERATORS
#include <iostream>
#endif // VECTOR_2D_STREAM_OPERATORS

#if defined(SFML_VECTOR)
#include <SFML/System/Vector2.hpp>
#elif defined(SFML_STATIC)
#include <SFML/System/Vector2.hpp>
#endif // SFML_VECTOR or SFML_STATIC

class Vector2D
{
public:

	//**********************//
	// Fields				//
	//**********************//

	float 					x;
	float 					y;

	//**********************//
	// Functions			//
	//**********************//

	Vector2D();
	Vector2D( float _x, float _y );
	Vector2D( const Vector2D& other );
	~Vector2D();

	Vector2D& 				zero();
	float 					length() const;
	void 					normalise();
	void 					truncate( float max_value );
	float 					dot( const Vector2D& v2 ) const;

	static Vector2D 		lerp( const Vector2D& start, const Vector2D& end, const float& percent );
	static Vector2D			nlerp( const Vector2D& start, const Vector2D& end, const float& percent );

	//**********************//
	// Operators			//
	//**********************//

	Vector2D 				operator+(const Vector2D &v2) const;
	Vector2D& 				operator+=(const Vector2D &v2);

	Vector2D 				operator-(const Vector2D &v2) const;
	Vector2D& 				operator-=(const Vector2D &v2);

	Vector2D 				operator*(const Vector2D &v2) const;
	Vector2D& 				operator*=(const Vector2D &v2);

	Vector2D 				operator*(float scalar) const;
	Vector2D& 				operator*=(float scalar);

	Vector2D 				operator/(const Vector2D &v2) const;
	Vector2D& 				operator/=(const Vector2D &v2);

	Vector2D 				operator/(float scalar) const;
	Vector2D& 				operator/=(float scalar);

	Vector2D& 				operator = 	( const Vector2D& other );
	bool 					operator == ( const Vector2D& other ) const;
	bool 					operator != ( const Vector2D& other ) const;

	//**********************//
	// Stream Operators		//
	//**********************//

	#ifdef VECTOR_2D_STREAM_OPERATORS
	friend std::ostream&	operator<<(std::ostream& os, const Vector2D& vec)
	{
		os << '(' << vec.x << ',' << vec.y << ')';
		return os;
	}

	friend std::istream&	operator>>(std::istream& is, Vector2D& vec)
	{
		is >> vec.x >> vec.y;
		return is;
	}
	#endif // VECTOR_2D_STREAM_OPERATORS

	//**********************//
	// SFML Vector Cast		//
	//**********************//

	#if defined(SFML_VECTOR)
	operator sf::Vector2f() { return sf::Vector2f( x, y ); }
	#elif defined(SFML_STATIC)
	operator sf::Vector2f() { return sf::Vector2f( x, y ); }
	#endif // SFML_VECTOR or SFML_STATIC

};

#endif // VECTOR_2D_H
