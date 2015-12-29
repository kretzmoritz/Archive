#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

#include "matrix.h"

template<unsigned int e, class T>
class Vector; //prototype

template<unsigned int e, class T>
class VectorBase : public MatrixBase<T, 1, e, true>
{
public:
	Vector<e, T> operator+(const Vector<e, T>& _v) const
	{
		return add(_v);
	}

	Vector<e, T>& operator+=(const Vector<e, T>& _v)
	{
		return static_cast<Vector<e, T>&>(add_equals(_v));
	}

	Vector<e, T> operator-() const
	{
		return negative();
	}

	Vector<e, T> operator-(const Vector<e, T>& _v) const
	{
		return subtract(_v);
	}

	Vector<e, T>& operator-=(const Vector<e, T>& _v)
	{
		return static_cast<Vector<e, T>&>(subtract_equals(_v));
	}

	Vector<e, T> operator*(T _val) const
	{
		return multiply_value(_val);
	}

	Vector<e, T>& operator*=(T _val)
	{
		return static_cast<Vector<e, T>&>(multiply_value_equals(_val));
	}

	Vector<e, T> operator/(T _val) const
	{
		return divide_value(_val);
	}

	Vector<e, T>& operator/=(T _val)
	{
		return static_cast<Vector<e, T>&>(divide_value_equals(_val));
	}

	template<bool either_major, unsigned int s> Vector<e, T> operator*(const MatrixBase<T, e, s, either_major>& _m) const
	{
		return multiply_matrix(_m);
	}

	float length() const
	{
		return sqrtf(length_sqr());
	}

	float length_sqr() const
	{
		float result = 0.0f;

		for(unsigned int i = 0; i < e; ++i)
		{
			result += pow(base[i], 2);
		}

		return result;
	}

	void normalize()
	{
		*this /= length();
	}

	Vector<e, T> normalized() const
	{
		return *this / length();
	}

	T dot(const Vector<e, T>& _v) const
	{
		T result = T(0);

		for(unsigned int i = 0; i < e; ++i)
		{
			result += base[i] * _v.base[i];
		}

		return result;
	}
};

template<unsigned int e, class T> Vector<e, T> operator*(const T& _lhs, const Vector<e, T>& _rhs)
{
	return _rhs * _lhs;
}

template<unsigned int e, class T>
class Vector : public VectorBase<e, T>
{
public:
	Vector()
	{

	}

	Vector(const Vector& _v)
	{
		*this = _v;
	}

	Vector(const MatrixBase<T, 1, e, true>& _m)
	{
		*this = *static_cast<const Vector*>(&_m);
	}

	T& operator[](unsigned int _e)
	{
		return get(_e, 0);
	}
	
	const T& operator[](unsigned int _e) const
	{
		return get(_e, 0);
	}
};

template<class T>
class Vector<2, T> : public VectorBase<2, T>
{
public:
	Vector()
		: x(base[0]), y(base[1])
	{

	}

	Vector(T _x, T _y)
		: x(base[0]), y(base[1])
	{
		base[0] = _x;
		base[1] = _y;
	}

	Vector(const Vector& _v)
		: x(base[0]), y(base[1])
	{
		base[0] = _v.x;
		base[1] = _v.y;
	}

	Vector(const MatrixBase<T, 1, 2, true>& _m)
		: x(base[0]), y(base[1])
	{
		const Vector* v = static_cast<const Vector*>(&_m);

		base[0] = v->base[0];
		base[1] = v->base[1];
	}

	Vector& operator=(const Vector& _v) //non-static data member of reference type -> no default provided
	{
		x = _v.x;
		y = _v.y;

		return *this;
	}

	void operator()(T _x, T _y)
	{
		x = _x;
		y = _y;
	}

	T& x;
	T& y;
};

template<class T>
class Vector<3, T> : public VectorBase<3, T>
{
public:
	Vector()
		: x(base[0]), y(base[1]), z(base[2])
	{

	}

	Vector(T _x, T _y, T _z)
		: x(base[0]), y(base[1]), z(base[2])
	{
		base[0] = _x;
		base[1] = _y;
		base[2] = _z;
	}

	Vector(const Vector& _v)
		: x(base[0]), y(base[1]), z(base[2])
	{
		base[0] = _v.x;
		base[1] = _v.y;
		base[2] = _v.z;
	}

	Vector(const MatrixBase<T, 1, 3, true>& _m)
		: x(base[0]), y(base[1]), z(base[2])
	{
		const Vector* v = static_cast<const Vector*>(&_m);

		base[0] = v->base[0];
		base[1] = v->base[1];
		base[2] = v->base[2];
	}

	Vector& operator=(const Vector& _v)
	{
		x = _v.x;
		y = _v.y;
		z = _v.z;

		return *this;
	}

	void operator()(T _x, T _y, T _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector cross(const Vector& _v) const
	{
		return Vector(	y * _v.z - z * _v.y,
						z * _v.x - x * _v.z,
						x * _v.y - y * _v.x);
	}

	T& x;
	T& y;
	T& z;
};

template<class T>
class Vector<4, T> : public VectorBase<4, T>
{
public:
	Vector()
		: x(base[0]), y(base[1]), z(base[2]), w(base[3])
	{

	}

	Vector(T _x, T _y, T _z, T _w)
		: x(base[0]), y(base[1]), z(base[2]), w(base[3])
	{
		base[0] = _x;
		base[1] = _y;
		base[2] = _z;
		base[3] = _w;
	}

	Vector(const Vector& _v)
		: x(base[0]), y(base[1]), z(base[2]), w(base[3])
	{
		base[0] = _v.x;
		base[1] = _v.y;
		base[2] = _v.z;
		base[3] = _v.w;
	}

	Vector(const MatrixBase<T, 1, 4, true>& _m)
		: x(base[0]), y(base[1]), z(base[2]), w(base[3])
	{
		const Vector* v = static_cast<const Vector*>(&_m);

		base[0] = v->base[0];
		base[1] = v->base[1];
		base[2] = v->base[2];
		base[3] = v->base[3];
	}

	Vector& operator=(const Vector& _v)
	{
		x = _v.x;
		y = _v.y;
		z = _v.z;
		w = _v.w;

		return *this;
	}

	void operator()(T _x, T _y, T _z, T _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	T& x;
	T& y;
	T& z;
	T& w;
};

typedef Vector<2, float> Vec2f; //ease of use
typedef Vector<3, float> Vec3f;
typedef Vector<4, float> Vec4f;

typedef Vector<2, int> Vec2i;
typedef Vector<3, int> Vec3i;
typedef Vector<4, int> Vec4i;

#endif