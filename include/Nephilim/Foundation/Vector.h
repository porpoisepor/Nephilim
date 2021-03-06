#ifndef NephilimVectors_h__
#define NephilimVectors_h__

#include <Nephilim/Platform.h>

#include <cmath>

NEPHILIM_NS_BEGIN

/**
	\class Vector2<T> 
	\brief 2D Vector

	This class defines a bidimensional vector (x,y) and its operations.

	Vec2<float> specialization is typedef'd to vec2 for a cleaner coding style in the common float computations.
*/
template <class T>
class NEPHILIM_API Vector2
{
public:
	/// Creates an uninitialized Vec2
	Vector2();

	/// Creates a Vec2 with initial values
	Vector2(T x, T y);

	/// Get the length of the vector
	float length() const;

	/// Makes the vector unit-length
	void normalize();

	/// Returns a normalized copy of this vector
	Vector2<T> normalized();

	/// Get this vector as a radian angle, must be normalized
	float asRadianAngle();

	/// Rotates the vector by radians. Must be normalized.
	void rotate(float radians);
	
	/// Sets all values of the 2D vector
	void set(T x, T y);

	/// Sets all values of the 2D vector as x
	void setAll(T x);

	/// Manhattan distance between the vectors
	float manhattanDistance(const Vector2<T> &vec) const;

	/// Get the squared distance between the vectors, apply sqrt to get a final distance
	float squaredDistance(const Vector2<T> &vec) const;

	/// Comparison operator overload
	bool operator==(const Vector2<T> &vec) const;

	/// Comparison operator overload
	bool operator!=(const Vector2<T> &vec) const;

	/// Assignment operator overload
	Vector2<T>& operator+=(const Vector2<T> &vec);
	/// Assignment operator overload
	Vector2<T>& operator+=(const T &scalar);
	/// Assignment operator overload
	Vector2<T>& operator-=(const Vector2<T> &vec);
	/// Assignment operator overload
	Vector2<T>& operator-=(const T &scalar);
	/// Assignment operator overload
	Vector2<T>& operator*=(const Vector2<T> &vec);
	/// Assignment operator overload
	Vector2<T>& operator*=(const T &scalar);
	/// Assignment operator overload
	Vector2<T>& operator/=(const Vector2<T> &vec);
	/// Assignment operator overload
	Vector2<T>& operator/=(const T &scalar);

	/// Addition operator overload that sums with another 3D Vector of the same type.
	Vector2<T> operator+(const Vector2<T> &vec) const;
	/// Addition operator overload that sums all members with a scalar value
	Vector2<T> operator+(const T &scalar) const;

	/// Subtraction operator overload that sums with another 3D Vector of the same type.
	Vector2<T> operator-(const Vector2<T> &vec) const;
	/// Subtraction operator overload that sums all members with a scalar value
	Vector2<T> operator-(const T &scalar) const;

	/// Multiplication operator overload that sums with another 3D Vector of the same type.
	Vector2<T> operator*(const Vector2<T> &vec);
	/// Multiplication operator overload that sums all members with a scalar value
	Vector2<T> operator*(const T &scalar);

	/// Division operator overload that sums with another 3D Vector of the same type.
	Vector2<T> operator/(const Vector2<T> &vec);
	/// Division operator overload that sums all members with a scalar value
	Vector2<T> operator/(const T &scalar);

	/// Direct accessors
	T x;
	/// Direct accessors
	T y;
};

/// Vector2D and vec2 are the notations for floating precision 2D vectors, without the need for template notation
typedef Vector2<float> vec2;
typedef Vector2<float> Vector2D;
typedef Vector2<int> vec2i;
typedef Vector2<float> Vec2f;
typedef Vector2<int> Vec2i;

/**
	\class Vector3<T>
	\brief 3D Vector
*/
template<class T>
class NEPHILIM_API Vector3
{
public:
	/// Creates an uninitialized Vec3
	Vector3();

	/// Creates a Vec3 with initial values
	Vector3(T x, T y, T z);

	/// Creates a vec3 from a vec2 and a z value
	Vector3(Vector2<T> val, T z);

	/// Return the first two components as (x,y)
	Vector2<T> xy();

	/// Sets all values of the 3D vector
	void set(T x, T y, T z);

	/// Sets all values of the 3D vector as x
	void setAll(T x);

	/// Interpolate linearly two vectors
	static Vector3<T> lerp(Vector3<T> a, Vector3<T> b, float blend);

	/// Get the length of the vector
	float length() const;

	/// Makes the vector unit-length
	void normalize();

	/// Compute the cross product
	Vector3<T> cross(const Vector3<T> &vec);

	/// Compute the dot product
	float dot(const Vector3<T> &vec);

	/// Calculate the distance between vectors
	float distance(const Vector3<T> &vec);

	/// Calculate the squared distance between vectors
	float squaredDistance(const Vector3<T> &vec);

	/// Comparison operator overload
	bool operator==(const Vector3<T> &vec);
	/// Comparison operator overload
	bool operator!=(const Vector3<T> &vec);

	/// Assignment operator overload
	Vector3<T>& operator+=(const Vector3<T> &vec);
	/// Assignment operator overload
	Vector3<T>& operator+=(const T &scalar);
	/// Assignment operator overload
	Vector3<T>& operator-=(const Vector3<T> &vec);
	/// Assignment operator overload
	Vector3<T>& operator-=(const T &scalar);
	/// Assignment operator overload
	Vector3<T>& operator*=(const Vector3<T> &vec);
	/// Assignment operator overload
	Vector3<T>& operator*=(const T &scalar);
	/// Assignment operator overload
	Vector3<T>& operator/=(const Vector3<T> &vec);
	/// Assignment operator overload
	Vector3<T>& operator/=(const T &scalar);

	/// Addition operator overload that sums with another 3D Vector of the same type.
	Vector3<T> operator+(const Vector3<T> &vec);
	/// Addition operator overload that sums all members with a scalar value
	Vector3<T> operator+(const T &scalar);

	/// Subtraction operator overload that sums with another 3D Vector of the same type.
	Vector3<T> operator-(const Vector3<T> &vec);
	/// Subtraction operator overload that sums all members with a scalar value
	Vector3<T> operator-(const T &scalar);

	/// Multiplication operator overload that sums with another 3D Vector of the same type.
	Vector3<T> operator*(const Vector3<T> &vec);
	/// Multiplication operator overload that sums all members with a scalar value
	Vector3<T> operator*(const T &scalar);

	/// Division operator overload that sums with another 3D Vector of the same type.
	Vector3<T> operator/(const Vector3<T> &vec);
	/// Division operator overload that sums all members with a scalar value
	Vector3<T> operator/(const T &scalar);

	/// Direct accessors
	T x;
	/// Direct accessors
	T y;
	/// Direct accessors
	T z;
};
typedef Vector3<float> vec3;
typedef Vector3<float> Vector3D;

typedef Vector3<float> Vec3f;


/**
	\class Vector4<T> 
	\brief 4D Vector
*/
template<class T>
class NEPHILIM_API Vector4
{
public:
	/// Creates a zero vector. w component is 1 by default, defining a "position"
	Vector4();

	/// Creates a Vec4 with initial values
	Vector4(T x, T y, T z, T w);

	/// Creates a Vec4 from a Vec3 and a w component
	Vector4(Vector3<T> vec3, T w);

	/// Sets all values of the 4D vector
	void set(T x, T y, T z, T w);

	/// Sets all values of the 4D vector as x
	void setAll(T x);

	const T* get();

	/// Get the first three components of the vector
	Vector3<T> xyz();

	/// Get the first two components
	Vector2<T> xy();

	/// Normalize the vector
	void normalize();

	/// Get the length of the vector
	float length() const;

	/// Cast the vec4 to another type
	template<class N>	Vector4<N> cast();

	/// Comparison operator overload
	bool operator==(const Vector4<T> &vec);
	/// Comparison operator overload
	bool operator!=(const Vector4<T> &vec);

	/// Assignment operator overload
	Vector4<T>& operator+=(const Vector4<T> &vec);
	/// Assignment operator overload
	Vector4<T>& operator+=(const T &scalar);
	/// Assignment operator overload
	Vector4<T>& operator-=(const Vector4<T> &vec);
	/// Assignment operator overload
	Vector4<T>& operator-=(const T &scalar);
	/// Assignment operator overload
	Vector4<T>& operator*=(const Vector4<T> &vec);
	/// Assignment operator overload
	Vector4<T>& operator*=(const T &scalar);
	/// Assignment operator overload
	Vector4<T>& operator/=(const Vector4<T> &vec);
	/// Assignment operator overload
	Vector4<T>& operator/=(const T &scalar);

	/// Addition operator overload that sums with another 4D Vector of the same type.
	Vector4<T> operator+(const Vector4<T> &vec);
	/// Addition operator overload that sums all members with a scalar value
	Vector4<T> operator+(const T &scalar);
	
	/// Subtraction operator overload that sums with another 4D Vector of the same type.
	Vector4<T> operator-(const Vector4<T> &vec);
	/// Subtraction operator overload that sums all members with a scalar value
	Vector4<T> operator-(const T &scalar);

	/// Multiplication operator overload that sums with another 4D Vector of the same type.
	Vector4<T> operator*(const Vector4<T> &vec);
	/// Multiplication operator overload that sums all members with a scalar value
	Vector4<T> operator*(const T &scalar);

	/// Division operator overload that sums with another 4D Vector of the same type.
	Vector4<T> operator/(const Vector4<T> &vec);
	/// Division operator overload that sums all members with a scalar value
	Vector4<T> operator/(const T &scalar);

	/// Direct accessors
	T x;
	/// Direct accessors
	T y;
	/// Direct accessors
	T z;
	/// Direct accessors
	T w;
};
typedef Vector4<float> vec4;
typedef Vector4<float> Vector4D;

/// Inline definitions
#include "Vector.inl"

NEPHILIM_NS_END
#endif // NephilimVectors_h__