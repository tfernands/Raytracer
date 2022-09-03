#ifndef VEC3HPP
#define VEC3HPP

#include <math.h>
#include <stdlib.h>
#include <iostream>

class Vec3{
public:

	double e[3];

	Vec3();
	Vec3(double e0);
	Vec3(double e0, double e1, double e2);

	virtual ~Vec3();

	Vec3& set(double e0, double e1, double e2);

	double operator[](int i) const;

	double& operator[](int i);

	double x() const;
	double y() const;
	double z() const;

	double r() const;
	double g() const;
	double b() const;

	double length() const;
	double squared_length() const;

	double mean() const;

	Vec3& operator+=(const Vec3 &v);
	Vec3& operator-=(const Vec3 &v);
	Vec3& operator*=(const Vec3 &v);
	Vec3& operator/=(const Vec3 &v);
	Vec3& operator*=(const double s);
	Vec3& operator/=(const double s);

	Vec3& clip(double min_value, double max_value);

	friend inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
		return Vec3(v1.e[0]+v2.e[0], v1.e[1]+v2.e[1], v1.e[2]+v2.e[2]);
	}

	friend inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
		return Vec3(v1.e[0]-v2.e[0], v1.e[1]-v2.e[1], v1.e[2]-v2.e[2]);
	}

	friend inline Vec3 operator-(const Vec3 &v) {
		return Vec3(-v.e[0], -v.e[1], -v.e[2]);
	}

	friend inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2){
		return Vec3(v1.e[0]*v2.e[0], v1.e[1]*v2.e[1], v1.e[2]*v2.e[2]);
	}

	friend inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2){
		return Vec3(v1.e[0]/v2.e[0], v1.e[1]/v2.e[1], v1.e[2]/v2.e[2]);
	}

	//double
	friend inline Vec3 operator*(const double s, const Vec3 &v2) {
		return Vec3(s*v2.e[0], s*v2.e[1], s*v2.e[2]);
	}
	friend inline Vec3 operator*(const Vec3 &v1, const double s) {
		return Vec3(v1.e[0]*s, v1.e[1]*s, v1.e[2]*s);
	}
	friend inline Vec3 operator/(const Vec3 &v1, const double s) {
		return Vec3(v1.e[0]/s, v1.e[1]/s, v1.e[2]/s);
	}

	friend std::istream& operator>>(std::istream &in, Vec3 &v){
		in >> v.e[0] >> v.e[1] >> v.e[2];
		return in;
	}

	friend std::ostream& operator<<(std::ostream &out, Vec3 &v){
		out << "[" << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << "]";
		return out;
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
};

#endif