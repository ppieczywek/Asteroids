#pragma once
#include <cmath>
#include <assert.h>

class Vector2{
public:
	float x,y;

	Vector2() : x(0.0f), y(0.0f) {};				//konstruktor z parametrami ijednoczesnie domyslny
	Vector2(const float &xx, const float &yy ) : x(xx), y(yy) {};				//konstruktor z parametrami ijednoczesnie domyslny
	Vector2( const Vector2& V) : x(V.x), y(V.y) {};																//konstruktor kopiuj¹cy 
	~Vector2() {};															//destruktor

	float			length(void) const	{ return sqrt( x*x + y*y); };		//d³ugoœæ wektora
	const Vector2&	normalize(void);
	const Vector2	unit() const;		//zwraca wektor jednostkowy bez zmiany bazowego
		
	bool operator!=(const Vector2 &vec) {
		if( (x != vec.x) && (y != vec.y) ) return true;
			else		
				return false; 
	};

		bool operator==(const Vector2 &vec) {
		if( (x == vec.x) && (y == vec.y) ) return true;
			else		
				return false; 
	};

	Vector2& operator=(const Vector2 &vec) {
		if(this == &vec) return *this;
		x = vec.x;
		y = vec.y;
		return *this; 
	};
	Vector2& operator/=(const float &scalar) {							// dzielenie przez skalar razem z przypisaniem
		assert(scalar!=0.0f);//if( scalar == 0 ) {};
		x/=scalar;
		y/=scalar;
		return *this; 
	};
	Vector2& operator*=(const float &scalar) {							// mno¿enie przez skalar razem z przypisaniem
		if( scalar == 0.0f ) {};
		x*=scalar;
		y*=scalar;
		return *this; 
	};
	Vector2& operator+=(const Vector2 &vec) {							//dodawanie wektorów z przypisaniem
		x+=vec.x;
		y+=vec.y;
		return *this;
	};
	Vector2& operator-=(const Vector2 &vec) {							//odejmowanie wektorów z przypisaniem
		x-=vec.x;
		y-=vec.y;
		return *this;
	};
	
	const Vector2 operator-() const {									//odwrócenie wektora "-"
		return Vector2(-x , -y);
	};
	const Vector2 operator-(const Vector2 &vec) const {				//odejmowanie wektorów
		return Vector2(x - vec.x , y - vec.y);
	};
	const Vector2 operator+(const Vector2 &vec) const {				//dodawanie wektorów
		return Vector2(x + vec.x , y + vec.y);
	};
	const Vector2 operator/(const float &scalar) const {				// dzielenie przez skalar
		assert(scalar!=0.0f);//if( scalar == 0 ) {};
		return Vector2(x / scalar , y / scalar); 
	};
	const Vector2 operator*(const float &scalar) const {				// mno¿enie przez skalar vec * sc
		return Vector2(x * scalar , y * scalar);
	};
	friend inline const Vector2 operator * (const float &scalar , const Vector2 &vec); //mno¿enie przez skalar  sc * vec

	const float operator*(const Vector2 &vec) const {					//iloczyn skalarny
		return ( (x * vec.x) + (y * vec.y) );
	};
}; 

inline const Vector2 operator*(const float &scalar ,  const Vector2 &vec) {  //mno¿enie przez skalar
		return vec*scalar;
};
inline const float VectorProduct(const Vector2 &vec1, const Vector2 &vec2) { return (vec1.x * vec2.y - vec1.y*vec2.x); };	
//const float VectorProduct(const Vector2 &vec1, const Vector2 &vec2);
/*float angle( const Vector2D &vec1, const Vector2D &vec2) {			//k¹t miêdzy wektorami w radianach
	return acos( (vec1*vec2)/(vec1.length()*vec2.length()) );
};*/

void AngleNormalize(float &value);

const Vector2 ZeroVector(0.0f,0.0f);

