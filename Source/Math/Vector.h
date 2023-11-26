#pragma once

#define Vec2i Vec2<int>
#define Vec2f Vec2<float>

#define Vec3i Vec3<int>
#define Vec3f Vec3<float>

#define Vec4i Vec4<int>
#define Vec4f Vec4<float>

#define TypeT template<typename T>

#include <ostream>

TypeT struct Vec3;

TypeT struct Vec2
{
	Vec2<T>() :
		x(0),
		y(0) {}

	Vec2<T>(T xIn, T yIn) :
		x(xIn),
		y(yIn) {}

	operator Vec3<T>() const
	{
		return Vec3<T>(x, y, 0);
	}

	Vec2<T> operator *(const T scalar) const
	{
		return Vec2<T>(x * scalar, y * scalar);
	}
	Vec2<T>& operator *=(const T scalar)
	{
		return *this = *this * scalar;
	}

	Vec2<T> operator /(const T scalar) const
	{
		return Vec2<T>(x / scalar, y / scalar);
	}
	Vec2<T>& operator /=(const T scalar)
	{
		return *this = *this / scalar;
	}

	Vec2<T> operator +(const Vec2<T>& other) const
	{
		return Vec2<T>(x + other.x, y + other.y);
	}
	Vec2<T>& operator +=(const Vec2<T>& other)
	{
		return *this = *this + other;
	}

	T Magnitude()
	{
		return sqrt((T) (x * x + y * y));
	}

	Vec2<T> Normalize()
	{
		return *this /= Magnitude();
	}
	
	T Dot(const Vec2<T>& right) const
	{
		return x * right.x + y * right.y;
	}

	T x, y;
};

TypeT struct Vec3
{
	Vec3<T>() :
		x(0),
		y(0),
		z(0) {}

	Vec3<T>(T xIn, T yIn) :
		x(xIn),
		y(yIn),
		z(0) {}

	Vec3<T>(T xIn, T yIn, T zIn) :
		x(xIn),
		y(yIn),
		z(zIn) {}

	explicit operator Vec2<T>() const
	{
		return Vec2<T>(x, y);
	}

	Vec3<T> operator *(const T scalar) const
	{
		Vec3<T> output(x * scalar, y * scalar, z * scalar);
		return output;
	}
	Vec3<T>& operator *=(const T scalar)
	{
		return *this = *this * scalar;
	}

	Vec3<T> operator /(const T scalar) const
	{
		return Vec3<T>(x / scalar, y / scalar, z / scalar);
	}
	Vec3<T>& operator /=(const T scalar)
	{
		return *this = *this / scalar;
	}

	Vec3<T> operator +(const Vec3<T>& right) const
	{
		return Vec3<T>(x + right.x, y + right.y, z + right.z);
	}
	Vec3<T>& operator +=(const Vec3<T>& right)
	{
		return *this = *this + right;
	}

	Vec3<T> operator -(const Vec3<T>& right) const
	{
		return Vec3<T>(x - right.x, y - right.y, z - right.z);
	}
	Vec3<T>& operator -=(const Vec3<T>& right)
	{
		return *this = *this - right;
	}
	Vec3<T> operator -() const
	{
		return Vec3<T>(-x, -y, -z);
	}

	bool operator ==(Vec3<T> other)
	{
		return x == other.x && y == other.y && z == other.z;
	}

	T operator [](int axis)
	{
		switch(axis)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}

		throw std::invalid_argument("Trying to get a value outside of the 3 dimensions");
	}

	const T Magnitude() const
	{
		return sqrt((T) (x * x + y * y + z * z));
	}

	Vec3<T>& Normalize()
	{
		return *this /= Magnitude();
	}

	Vec3<T> Normalized() const
	{
		Vec3<T> output = Vec3f(x, y, z) / Magnitude();
		return output;
	}

	T Dot(const Vec3<T>& right) const
	{
		return x * right.x + y * right.y + z * right.z;
	}

	Vec3<T> Cross(const Vec3<T>& right) const
	{
		Vec3<T> output(y * right.z - z * right.y,
					   z * right.x - x * right.z,
					   x * right.y - y * right.x);

		return output;
	}

	T x, y, z;
};

TypeT struct Vec4
{
	Vec4() :
		x(0),
		y(0),
		z(0),
		w(0) {}

	Vec4(T xIn, T yIn, T zIn, T wIn) :
		x(xIn),
		y(yIn),
		z(zIn),
		w(wIn) {}

	T x, y, z, w = 0;

	Vec4<T>& operator /(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}
	Vec4<T>& operator /=(const T scalar)
	{
		return *this = *this / scalar;
	}

	T& operator [](int axis)
	{
		switch(axis)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
		}

		throw std::invalid_argument("Trying to get a value outside of the 4 dimensions");
	}

	T Magnitude()
	{
		return sqrt((T) (x * x + y * y + z * z + w * w));
	}

	Vec4<T> Normalize()
	{
		return *this /= Magnitude();
	}

	T Dot(const Vec4<T>& right)
	{
		return x * right.x + y * right.y + z * right.z + w * right.w;
	}
};

TypeT
std::ostream& operator <<(std::ostream& stream, const Vec2<T> vector)
{
	stream << "(" << vector.x << ", " << vector.y << ")";
	return stream;
}

TypeT
std::ostream& operator <<(std::ostream& stream, const Vec3<T> vector)
{
	stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	return stream;
}

TypeT
std::ostream& operator <<(std::ostream& stream, const Vec4<T> vector)
{
	stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
	return stream;
}

TypeT
Vec3<T> operator +(Vec3<T>& vec1, Vec3<T>& vec2)
{
	return Vec3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

#undef TypeT