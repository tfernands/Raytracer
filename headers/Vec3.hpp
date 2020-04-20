#ifndef VEC3HPP
#define VEC3HPP

#include <math.h>
#include <stdlib.h>
#include <iostream>

class Vec3{
public:

	double e[3];

	Vec3() {}
	Vec3(double e0, double e1, double e2) {
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}
	virtual ~Vec3(){}

	inline Vec3& set(double e0, double e1, double e2){
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
		return *this;
	}

	inline double operator[](int i) const { return e[i]; }
	inline double& operator[](int i) { return e[i]; }

	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }
	inline double r() const { return e[0]; }
	inline double g() const { return e[1]; }
	inline double b() const { return e[2]; }

	inline double length() const {
		return sqrt(squared_length());
	}

	inline double squared_length() const {
		return e[0]*e[0] + e[1] * e[1] + e[2]*e[2];
	}

	static inline double dot(const Vec3 &v1, const Vec3 &v2){
		return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
	}

	static inline Vec3 cross(const Vec3 &v1, const Vec3 &v2){
		return Vec3( v1.e[1]*v2.e[2]-v1.e[2]*v2.e[1],
					-(v1.e[0]*v2.e[2]-v1.e[2]*v2.e[0]),
					 v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0] );
	}

	static inline Vec3 unit_vector(Vec3 v){
		return v / v.length();
	}

	inline Vec3& operator+=(const Vec3 &v){
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	inline Vec3& operator-=(const Vec3 &v){
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];
		return *this;
	}
	inline Vec3& operator*=(const Vec3 &v){
		e[0] *= v.e[0];
		e[1] *= v.e[1];
		e[2] *= v.e[2];
		return *this;
	}
	inline Vec3& operator/=(const Vec3 &v){
		e[0] /= v.e[0];
		e[1] /= v.e[1];
		e[2] /= v.e[2];
		return *this;
	}

	inline Vec3& operator*=(const double s){
		e[0] *= s;
		e[1] *= s;
		e[2] *= s;
		return *this;
	}

	inline Vec3& operator/=(const double s){
		e[0] /= s;
		e[1] /= s;
		e[2] /= s;
		return *this;
	}

	inline Vec3& clip(double min_value, double max_value){
		if (e[0] < min_value)
			e[0] = min_value;
		if (e[1] < min_value)
			e[1] = min_value;
		if (e[2] < min_value)
			e[2] = min_value;
		if (e[0] > max_value)
			e[0] = max_value;
		if (e[1] > max_value)
			e[1] = max_value;
		if (e[2] > max_value)
			e[2] = max_value;
		return *this;		
	}

	friend inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2){
		return Vec3(v1.e[0]+v2.e[0], v1.e[1]+v2.e[1], v1.e[2]+v2.e[2]);
	}

	friend inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2){
		return Vec3(v1.e[0]-v2.e[0], v1.e[1]-v2.e[1], v1.e[2]-v2.e[2]);
	}

	friend inline Vec3 operator-(const Vec3 &v){
		return Vec3(-v.e[0], -v.e[1], -v.e[2]);
	}

	friend inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2){
		return Vec3(v1.e[0]*v2.e[0], v1.e[1]*v2.e[1], v1.e[2]*v2.e[2]);
	}

	friend inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2){
		return Vec3(v1.e[0]/v2.e[0], v1.e[1]/v2.e[1], v1.e[2]/v2.e[2]);
	}

	//double
	friend inline Vec3 operator*(const double s, const Vec3 &v2){
		return Vec3(s*v2.e[0], s*v2.e[1], s*v2.e[2]);
	}
	friend inline Vec3 operator*(const Vec3 &v1, const double s){
		return Vec3(v1.e[0]*s, v1.e[1]*s, v1.e[2]*s);
	}
	friend inline Vec3 operator/(const Vec3 &v1, const double s){
		return Vec3(v1.e[0]/s, v1.e[1]/s, v1.e[2]/s);
	}

	friend inline std::istream& operator>>(std::istream &in, Vec3 &v){
		in >> v.e[0] >> v.e[1] >> v.e[2];
		return in;
	}

	friend std::ostream& operator<<(std::ostream &out, Vec3 &v){
		out << "[" << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << "]";
		return out;
	}
};

#endif