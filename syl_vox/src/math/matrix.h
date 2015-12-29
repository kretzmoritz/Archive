#ifndef MATRIX_H
#define MATRIX_H

#pragma warning(push)
#pragma warning(disable: 4351) //http://msdn.microsoft.com/de-de/library/1ywe7hcy%28v=vs.80%29.aspx

template<class T, unsigned int r, unsigned int c, bool row_major>
class MatrixBase; //prototype

template<class T, unsigned int r, unsigned int c, bool row_major>
class MatrixFunctionality
{
	template<class T, unsigned int r, unsigned int c, bool row_major> friend class MatrixFunctionality; //force all friend

public:
	MatrixFunctionality()
		: base() //default initialization
	{

	}

	virtual ~MatrixFunctionality()
	{

	}

	template<bool either_major> bool operator==(const MatrixFunctionality<T, r, c, either_major>& _m) const
	{
		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				if(get(i, j) != _m.get(i, j))
				{
					return false;
				}
			}
		}

		return true;
	}

	template<bool either_major> bool operator!=(const MatrixFunctionality<T, r, c, either_major>& _m) const
	{
		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				if(get(i, j) != _m.get(i, j))
				{
					return true;
				}
			}
		}

		return false;
	}

protected:
	T base[r * c];

	virtual T& get(unsigned int _r, unsigned int _c) = 0;
	virtual const T& get(unsigned int _r, unsigned int _c) const = 0;

	template<bool either_major> MatrixBase<T, r, c, row_major> add(const MatrixFunctionality<T, r, c, either_major>& _m) const
	{
		MatrixBase<T, r, c, row_major> result;

		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				static_cast<MatrixFunctionality*>(&result)->get(i, j) = get(i, j) + _m.get(i, j);
			}
		}
		
		return result;
	}

	template<bool either_major> MatrixFunctionality<T, r, c, row_major>& add_equals(const MatrixFunctionality<T, r, c, either_major>& _m)
	{
		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				get(i, j) += _m.get(i, j);
			}
		}

		return *this;
	}

	MatrixBase<T, r, c, row_major> negative() const
	{
		MatrixBase<T, r, c, row_major> result;

		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				static_cast<MatrixFunctionality*>(&result)->get(i, j) = -get(i, j);
			}
		}
		
		return result;
	}

	template<bool either_major> MatrixBase<T, r, c, row_major> subtract(const MatrixFunctionality<T, r, c, either_major>& _m) const
	{
		MatrixBase<T, r, c, row_major> result;

		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				static_cast<MatrixFunctionality*>(&result)->get(i, j) = get(i, j) - _m.get(i, j);
			}
		}
		
		return result;
	}

	template<bool either_major> MatrixFunctionality<T, r, c, row_major>& subtract_equals(const MatrixFunctionality<T, r, c, either_major>& _m)
	{
		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				get(i, j) -= _m.get(i, j);
			}
		}

		return *this;
	}

	MatrixBase<T, r, c, row_major> multiply_value(T _val) const
	{
		MatrixBase<T, r, c, row_major> result;

		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				static_cast<MatrixFunctionality*>(&result)->get(i, j) = get(i, j) * _val;
			}
		}
		
		return result;
	}

	MatrixFunctionality<T, r, c, row_major>& multiply_value_equals(T _val)
	{
		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				get(i, j) *= _val;
			}
		}

		return *this;
	}

	MatrixBase<T, r, c, row_major> divide_value(T _val) const
	{
		MatrixBase<T, r, c, row_major> result;

		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				static_cast<MatrixFunctionality*>(&result)->get(i, j) = get(i, j) / _val;
			}
		}
		
		return result;
	}

	MatrixFunctionality<T, r, c, row_major>& divide_value_equals(T _val)
	{
		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				get(i, j) /= _val;
			}
		}

		return *this;
	}

	template<bool either_major, unsigned int s> MatrixBase<T, r, s, row_major> multiply_matrix(const MatrixFunctionality<T, c, s, either_major>& _m) const
	{
		MatrixBase<T, r, s, row_major> result;

		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < s; ++j)
			{
				for(unsigned int k = 0; k < c; ++k)
				{
					static_cast<MatrixFunctionality*>(&result)->get(i, j) += get(i, k) * _m.get(k, j);
				}
			}
		}
		
		return result;
	}
};

template<class T, unsigned int r, unsigned int c, bool row_major>
class MatrixBase : public MatrixFunctionality<T, r, c, row_major>
{

};

template<class T, unsigned int r, unsigned int c>
class MatrixBase<T, r, c, true> : public MatrixFunctionality<T, r, c, true>
{
protected:
	T& get(unsigned int _r, unsigned int _c)
	{
		return base[_r * c + _c]; //row major order
	}

	const T& get(unsigned int _r, unsigned int _c) const
	{
		return base[_r * c + _c];
	}
};

template<class T, unsigned int r, unsigned int c>
class MatrixBase<T, r, c, false> : public MatrixFunctionality<T, r, c, false>
{
protected:
	T& get(unsigned int _r, unsigned int _c)
	{
		return base[_c * r + _r]; //column major order
	}

	const T& get(unsigned int _r, unsigned int _c) const
	{
		return base[_c * r + _r];
	}
};

template<class T, unsigned int r, unsigned int c, bool row_major>
class Matrix : public MatrixBase<T, r, c, row_major>
{
public:
	Matrix()
	{

	}

	Matrix(const Matrix& _m) //compiler generated -> calls base copy constructor; user defined -> calls base default constructor
	{
		*this = _m;
	}

	Matrix(const MatrixBase<T, r, c, row_major>& _m)
	{
		*this = static_cast<const Matrix&>(_m);
	}

	Matrix(const Matrix<T, r, c, !row_major>& _m)
	{
		type_conversion(_m);
	}

	Matrix& operator=(const Matrix<T, r, c, !row_major>& _m)
	{
		type_conversion(_m);

		return *this;
	}

	const T* operator*() const
	{
		return &base[0];
	}

	T& operator()(unsigned int _r, unsigned int _c)
	{
		return get(_r, _c);
	}
	
	const T& operator()(unsigned int _r, unsigned int _c) const
	{
		return get(_r, _c);
	}

	template<bool either_major> Matrix operator+(const Matrix<T, r, c, either_major>& _m) const
	{
		return add(_m);
	}

	template<bool either_major> Matrix& operator+=(const Matrix<T, r, c, either_major>& _m)
	{
		return static_cast<Matrix&>(add_equals(_m));
	}

	Matrix operator-() const
	{
		return negative();
	}

	template<bool either_major> Matrix operator-(const Matrix<T, r, c, either_major>& _m) const
	{
		return subtract(_m);
	}

	template<bool either_major> Matrix& operator-=(const Matrix<T, r, c, either_major>& _m)
	{
		return static_cast<Matrix&>(subtract_equals(_m));
	}

	Matrix operator*(T _val) const
	{
		return multiply_value(_val);
	}

	Matrix& operator*=(T _val)
	{
		return static_cast<Matrix&>(multiply_value_equals(_val));
	}

	Matrix operator/(T _val) const
	{
		return divide_value(_val);
	}

	Matrix& operator/=(T _val)
	{
		return static_cast<Matrix&>(divide_value_equals(_val));
	}

	template<bool either_major, unsigned int s> Matrix<T, r, s, row_major> operator*(const Matrix<T, c, s, either_major>& _m) const
	{
		return multiply_matrix(_m);
	}

	Matrix<T, c, r, row_major> transposed() const
	{
		Matrix<T, c, r, row_major> result;

		for(unsigned int i = 0; i < c; ++i)
		{
			for(unsigned int j = 0; j < r; ++j)
			{
				result(i, j) = (*this)(j, i);
			}
		}
		
		return result;
	}

private:
	void type_conversion(const Matrix<T, r, c, !row_major>& _m)
	{
		for(unsigned int i = 0; i < r; ++i)
		{
			for(unsigned int j = 0; j < c; ++j)
			{
				(*this)(i, j) = _m(i, j);
			}
		}
	}
};

template<class T, unsigned int r, unsigned int c, bool row_major> Matrix<T, r, c, row_major> operator*(T _lhs, const Matrix<T, r, c, row_major>& _rhs)
{
	return _rhs * _lhs;
}

namespace MatHelper //special functions
{
	template<class T, unsigned int s, bool row_major> Matrix<T, s, s, row_major>& make_identity(Matrix<T, s, s, row_major>& _m)
	{
		for(unsigned int i = 0; i < s; ++i)
		{
			for(unsigned int j = 0; j < s; ++j)
			{
				if(i == j)
					_m(i, j) = T(1);
				else
					_m(i, j) = T(0);
			}
		}

		return _m;
	}

	template<class T, bool row_major> Matrix<float, 2, 2, row_major> inverse(const Matrix<T, 2, 2, row_major>& _m)
	{
		Matrix<float, 2, 2, row_major> result;
		T det = determinant(_m);

		if(det == T(0))
			return make_identity(result);

		result(0, 0) = _m(1, 1);
		result(0, 1) = -_m(0, 1);
		
		result(1, 0) = -_m(1, 0);
		result(1, 1) = _m(0, 0);

		return (1.0f / det) * result;
	}

	template<class T, bool row_major> Matrix<float, 3, 3, row_major> inverse(const Matrix<T, 3, 3, row_major>& _m)
	{
		Matrix<float, 3, 3, row_major> result;
		T det = determinant(_m);

		if(det == T(0))
			return make_identity(result);

		result(0, 0) = determinant(_m(1, 1), _m(1, 2), _m(2, 1), _m(2, 2));
		result(0, 1) = determinant(_m(0, 2), _m(0, 1), _m(2, 2), _m(2, 1));
		result(0, 2) = determinant(_m(0, 1), _m(0, 2), _m(1, 1), _m(1, 2));
		
		result(1, 0) = determinant(_m(1, 2), _m(1, 0), _m(2, 2), _m(2, 0));
		result(1, 1) = determinant(_m(0, 0), _m(0, 2), _m(2, 0), _m(2, 2));
		result(1, 2) = determinant(_m(0, 2), _m(0, 0), _m(1, 2), _m(1, 0));
		
		result(2, 0) = determinant(_m(1, 0), _m(1, 1), _m(2, 0), _m(2, 1));
		result(2, 1) = determinant(_m(0, 1), _m(0, 0), _m(2, 1), _m(2, 0));
		result(2, 2) = determinant(_m(0, 0), _m(0, 1), _m(1, 0), _m(1, 1));

		return (1.0f / det) * result;
	}

	template<class T, bool row_major> Matrix<float, 4, 4, row_major> inverse(const Matrix<T, 4, 4, row_major>& _m)
	{
		Matrix<float, 4, 4, row_major> result;
		T det = determinant(_m);

		if(det == T(0))
			return make_identity(result);

		result(0, 0) = _m(1, 2) * _m(2, 3) * _m(3, 1) - _m(1, 3) * _m(2, 2) * _m(3, 1) + _m(1, 3) * _m(2, 1) * _m(3, 2) - _m(1, 1) * _m(2, 3) * _m(3, 2) - _m(1, 2) * _m(2, 1) * _m(3, 3) + _m(1, 1) * _m(2, 2) * _m(3, 3);
		result(0, 1) = _m(0, 3) * _m(2, 2) * _m(3, 1) - _m(0, 2) * _m(2, 3) * _m(3, 1) - _m(0, 3) * _m(2, 1) * _m(3, 2) + _m(0, 1) * _m(2, 3) * _m(3, 2) + _m(0, 2) * _m(2, 1) * _m(3, 3) - _m(0, 1) * _m(2, 2) * _m(3, 3);
		result(0, 2) = _m(0, 2) * _m(1, 3) * _m(3, 1) - _m(0, 3) * _m(1, 2) * _m(3, 1) + _m(0, 3) * _m(1, 1) * _m(3, 2) - _m(0, 1) * _m(1, 3) * _m(3, 2) - _m(0, 2) * _m(1, 1) * _m(3, 3) + _m(0, 1) * _m(1, 2) * _m(3, 3);
		result(0, 3) = _m(0, 3) * _m(1, 2) * _m(2, 1) - _m(0, 2) * _m(1, 3) * _m(2, 1) - _m(0, 3) * _m(1, 1) * _m(2, 2) + _m(0, 1) * _m(1, 3) * _m(2, 2) + _m(0, 2) * _m(1, 1) * _m(2, 3) - _m(0, 1) * _m(1, 2) * _m(2, 3);
		
		result(1, 0) = _m(1, 3) * _m(2, 2) * _m(3, 0) - _m(1, 2) * _m(2, 3) * _m(3, 0) - _m(1, 3) * _m(2, 0) * _m(3, 2) + _m(1, 0) * _m(2, 3) * _m(3, 2) + _m(1, 2) * _m(2, 0) * _m(3, 3) - _m(1, 0) * _m(2, 2) * _m(3, 3);
		result(1, 1) = _m(0, 2) * _m(2, 3) * _m(3, 0) - _m(0, 3) * _m(2, 2) * _m(3, 0) + _m(0, 3) * _m(2, 0) * _m(3, 2) - _m(0, 0) * _m(2, 3) * _m(3, 2) - _m(0, 2) * _m(2, 0) * _m(3, 3) + _m(0, 0) * _m(2, 2) * _m(3, 3);
		result(1, 2) = _m(0, 3) * _m(1, 2) * _m(3, 0) - _m(0, 2) * _m(1, 3) * _m(3, 0) - _m(0, 3) * _m(1, 0) * _m(3, 2) + _m(0, 0) * _m(1, 3) * _m(3, 2) + _m(0, 2) * _m(1, 0) * _m(3, 3) - _m(0, 0) * _m(1, 2) * _m(3, 3);
		result(1, 3) = _m(0, 2) * _m(1, 3) * _m(2, 0) - _m(0, 3) * _m(1, 2) * _m(2, 0) + _m(0, 3) * _m(1, 0) * _m(2, 2) - _m(0, 0) * _m(1, 3) * _m(2, 2) - _m(0, 2) * _m(1, 0) * _m(2, 3) + _m(0, 0) * _m(1, 2) * _m(2, 3);
		
		result(2, 0) = _m(1, 1) * _m(2, 3) * _m(3, 0) - _m(1, 3) * _m(2, 1) * _m(3, 0) + _m(1, 3) * _m(2, 0) * _m(3, 1) - _m(1, 0) * _m(2, 3) * _m(3, 1) - _m(1, 1) * _m(2, 0) * _m(3, 3) + _m(1, 0) * _m(2, 1) * _m(3, 3);
		result(2, 1) = _m(0, 3) * _m(2, 1) * _m(3, 0) - _m(0, 1) * _m(2, 3) * _m(3, 0) - _m(0, 3) * _m(2, 0) * _m(3, 1) + _m(0, 0) * _m(2, 3) * _m(3, 1) + _m(0, 1) * _m(2, 0) * _m(3, 3) - _m(0, 0) * _m(2, 1) * _m(3, 3);
		result(2, 2) = _m(0, 1) * _m(1, 3) * _m(3, 0) - _m(0, 3) * _m(1, 1) * _m(3, 0) + _m(0, 3) * _m(1, 0) * _m(3, 1) - _m(0, 0) * _m(1, 3) * _m(3, 1) - _m(0, 1) * _m(1, 0) * _m(3, 3) + _m(0, 0) * _m(1, 1) * _m(3, 3);
		result(2, 3) = _m(0, 3) * _m(1, 1) * _m(2, 0) - _m(0, 1) * _m(1, 3) * _m(2, 0) - _m(0, 3) * _m(1, 0) * _m(2, 1) + _m(0, 0) * _m(1, 3) * _m(2, 1) + _m(0, 1) * _m(1, 0) * _m(2, 3) - _m(0, 0) * _m(1, 1) * _m(2, 3);
		
		result(3, 0) = _m(1, 2) * _m(2, 1) * _m(3, 0) - _m(1, 1) * _m(2, 2) * _m(3, 0) - _m(1, 2) * _m(2, 0) * _m(3, 1) + _m(1, 0) * _m(2, 2) * _m(3, 1) + _m(1, 1) * _m(2, 0) * _m(3, 2) - _m(1, 0) * _m(2, 1) * _m(3, 2);
		result(3, 1) = _m(0, 1) * _m(2, 2) * _m(3, 0) - _m(0, 2) * _m(2, 1) * _m(3, 0) + _m(0, 2) * _m(2, 0) * _m(3, 1) - _m(0, 0) * _m(2, 2) * _m(3, 1) - _m(0, 1) * _m(2, 0) * _m(3, 2) + _m(0, 0) * _m(2, 1) * _m(3, 2);
		result(3, 2) = _m(0, 2) * _m(1, 1) * _m(3, 0) - _m(0, 1) * _m(1, 2) * _m(3, 0) - _m(0, 2) * _m(1, 0) * _m(3, 1) + _m(0, 0) * _m(1, 2) * _m(3, 1) + _m(0, 1) * _m(1, 0) * _m(3, 2) - _m(0, 0) * _m(1, 1) * _m(3, 2);
		result(3, 3) = _m(0, 1) * _m(1, 2) * _m(2, 0) - _m(0, 2) * _m(1, 1) * _m(2, 0) + _m(0, 2) * _m(1, 0) * _m(2, 1) - _m(0, 0) * _m(1, 2) * _m(2, 1) - _m(0, 1) * _m(1, 0) * _m(2, 2) + _m(0, 0) * _m(1, 1) * _m(2, 2);

		return (1.0f / det) * result;
	}
	
	template<class T, bool row_major> T determinant(const Matrix<T, 2, 2, row_major>& _m)
	{
		return determinant(_m(0, 0), _m(0, 1), _m(1, 0), _m(1, 1));
	}

	template<class T> T determinant(T _00, T _01, T _10, T _11)
	{
		return _00 * _11 - _01 * _10;
	}

	template<class T, bool row_major> T determinant(const Matrix<T, 3, 3, row_major>& _m)
	{
		return determinant(_m(0, 0), _m(0, 1), _m(0, 2), _m(1, 0), _m(1, 1), _m(1, 2), _m(2, 0), _m(2, 1), _m(2, 2));
	}

	template<class T> T determinant(T _00, T _01, T _02, T _10, T _11, T _12, T _20, T _21, T _22) //Laplace expansion
	{
		return	_00 * determinant(_11, _12, _21, _22) - 
				_01 * determinant(_10, _12, _20, _22) + 
				_02 * determinant(_10, _11, _20, _21);
	}

	template<class T, bool row_major> T determinant(const Matrix<T, 4, 4, row_major>& _m)
	{
		return determinant(	_m(0, 0), _m(0, 1), _m(0, 2), _m(0, 3), 
							_m(1, 0), _m(1, 1), _m(1, 2), _m(1, 3), 
							_m(2, 0), _m(2, 1), _m(2, 2), _m(2, 3), 
							_m(3, 0), _m(3, 1), _m(3, 2), _m(3, 3));
	}

	template<class T> T determinant(T _00, T _01, T _02, T _03, 
									T _10, T _11, T _12, T _13, 
									T _20, T _21, T _22, T _23, 
									T _30, T _31, T _32, T _33)
	{
		return	_00 * determinant(_11, _12, _13, _21, _22, _23, _31, _32, _33) - 
				_01 * determinant(_10, _12, _13, _20, _22, _23, _30, _32, _33) + 
				_02 * determinant(_10, _11, _13, _20, _21, _23, _30, _31, _33) - 
				_03 * determinant(_10, _11, _12, _20, _21, _22, _30, _31, _32);
	}
}

template<class T, unsigned int r, unsigned int c>
struct Mat
{
	typedef Matrix<T, r, c, true> RM; //ease of use
	typedef Matrix<T, r, c, false> CM;
};

template<unsigned int r, unsigned int c>
struct Matf
{
	typedef Matrix<float, r, c, true> RM;
	typedef Matrix<float, r, c, false> CM;
};

#pragma warning(pop)

#endif