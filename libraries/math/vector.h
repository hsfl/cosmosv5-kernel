/********************************************************************
* Copyright (C) 2015 by Interstel Technologies, Inc.
*   and Hawaii Space Flight Laboratory.
*
* This file is part of the COSMOS/core that is the central
* module for COSMOS. For more information on COSMOS go to
* <http://cosmos-project.com>
*
* The COSMOS/core software is licenced under the
* GNU Lesser General Public License (LGPL) version 3 licence.
*
* You should have received a copy of the
* GNU Lesser General Public License
* If not, go to <http://www.gnu.org/licenses/>
*
* COSMOS/core is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3 of
* the License, or (at your option) any later version.
*
* COSMOS/core is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* Refer to the "licences" folder for further information on the
* condititons and terms to use this software.
********************************************************************/

/*!
 * \file vector.h
 * \brief 3-D vector arithmetic used throughout COSMOS.
 * \ingroup cosmos_kernel_math
 * \defgroup cosmos_vector Vector
 *
 * Provides a compact 3-D (and 4-D homogeneous) vector type with the standard
 * arithmetic operations: add, subtract, scalar multiply/divide, dot product,
 * cross product, and norm. Used by the matrix, rotation, physics, and
 * coordinate conversion libraries.
 */

#ifndef _MATH_VECTOR_H
#define _MATH_VECTOR_H

//#include "types.h"
#include "support/configCosmos.h"
#include "constants.h"

#include <iostream>
#include <iomanip>      // std::setw
#include <cmath>
#include <vector>

#ifdef _MSC_BUILD
#include <stdint.h> // for uint16_t et all.
#endif

//! 3 element generic row vector
/*! 3 double precision numbers representing a 3 element row major vector.
*/
// TODO: rename to row_vector
// TODO: create a generic type of vector like: vector3 that can be accessed wither by .x, .y, .z or with [0], [1], [2]
// TODO: consider using Eigen, no because Eigen is a heavily templated library and takes a long time to compile and has way more than needed
// it can be used for specific projects that require it
struct rvector
{
	double col[3] = {0.};

	rvector() {}
	rvector(double d0, double d1, double d2) {
		col[0] = d0;
		col[1] = d1;
		col[2] = d2;
    }
    rvector operator * (const double scale) const; // multiply vector by scalar operator
    rvector &operator += (const rvector &v2); // add vector by vector operator in place
    rvector &operator -= (const rvector &v2); // add vector by vector operator in place
    rvector &operator *= (const double &scale); // add vector by vector operator in place
    rvector operator - () const; // negate vector in place

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		vector<double> v_col = vector<double>(col, col+sizeof(col)/sizeof(col[0]));
		return json11::Json::object {
			{ "col" , v_col }
		};
	}
	/// Set class contents from JSON string
	/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
		\param	s	JSON-formatted string to set class contents to
		\return n/a
	*/
	void from_json(const string& s) {
		string error;
		json11::Json parsed = json11::Json::parse(s,error);
		if(error.empty()) {
			auto p_col = parsed["col"].array_items();
			for(size_t i = 0; i != p_col.size(); ++i) {
				if(!p_col[i].is_null())	col[i] = p_col[i].number_value();
			}
		} else	{
			cerr<<"ERROR: <"<<error<<">"<<endl;
		}
		return;
	}
};

::std::ostream& operator << (::std::ostream& out, const rvector& a);
::std::ostream& operator << (::std::ostream& out, const vector<rvector>& a);
::std::istream& operator >> (::std::istream& out, rvector& a);
rvector operator + (rvector v1, rvector v2); // add two vector vector operator
rvector operator - (rvector v1, rvector v2); // subtract two vector vector operator
//rvector operator * (rvector v, double scalar); // multiply vector by vector operator
rvector operator * (const double scale, const rvector v);
rvector operator * (rvector v1, rvector v2); // multiply vector by vector operator
rvector operator / (rvector v, double scalar); // divide vector by scalar operator
bool operator == (rvector a, rvector b); // Compares two vectors
bool operator != (rvector a, rvector b); // Compares two vectors

//! 3 element cartesian vector
/*! 3 double precision numbers representing a vector in a right handed
* cartesian space */
class cvector
{

public:
	//! X value
	double x = 0.;
	//! Y value
	double y = 0.;
	//! Z value
	double z = 0.;

	cvector() {}
	cvector(double tx, double ty, double tz) {
		x = tx;
		y = ty;
		z = tz;
	}

	void normalize(double scale=1.);
	double length();
	double norm();
	double norm2();
	cvector normalized(double scale=1.);
	double& operator[] (const int index);

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		return json11::Json::object {
			{ "x" , x },
			{ "y" , y },
			{ "z" , z }
		};
	}

	/// Set class contents from JSON string
	/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
		\param	s	JSON-formatted string to set class contents to
		\return n/a
	*/
	void from_json(const string& s) {
		string error;
		json11::Json parsed = json11::Json::parse(s,error);
		if(error.empty()) {
			if(!parsed["x"].is_null())	x = parsed["x"].number_value();
			if(!parsed["y"].is_null())	y = parsed["y"].number_value();
			if(!parsed["z"].is_null())	z = parsed["z"].number_value();
		} else	{
			cerr<<"ERROR: <"<<error<<">"<<endl;
		}
		return;
	}
};

//! 3 element spherical vector
/*! 3 double precision numbers representing a vector in a spherical
* space. Lambda increases east. */
struct svector
{
	//! N/S in radians
	double phi = 0.;
	//! E/W in radians
	double lambda = 0.;
	//! Radius in meters
	double r = 0.;

	svector() {}
	svector(double tphi, double tlambda, double tr) {
		phi = tphi;
		lambda = tlambda;
		r = tr;
	}

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		return json11::Json::object {
			{ "phi" , phi },
			{ "lambda" , lambda },
			{ "r"   , r }
		};
	}

	/// Set class contents from JSON string
	/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
		\param	s	JSON-formatted string to set class contents to
		\return n/a
	*/
	void from_json(const string& s) {
		string error;
		json11::Json parsed = json11::Json::parse(s,error);
		if(error.empty()) {
			if(!parsed["phi"].is_null())	phi = parsed["phi"].number_value();
			if(!parsed["lambda"].is_null())	lambda = parsed["lambda"].number_value();
			if(!parsed["r"].is_null())		r = parsed["r"].number_value();
		} else	{
			cerr<<"ERROR: <"<<error<<">"<<endl;
		}
		return;
	}
} ;

::std::ostream& operator << (::std::ostream& out, const svector& a);
::std::istream& operator >> (::std::istream& out, svector& a);

//! 3 element geodetic vector
/*! 3 double precision numbers representing a vector in a WGS84
* based geodetic space. Longitude increases east. */
// TODO: replace name gvector->geodetic
struct gvector
{
	//! Latitude in radians
	double lat = 0.;
	//! Longitude in radians
	double lon = 0.;
	//! Height in meters
	double h = 0.;

	gvector() {}
	gvector(double tlat, double tlon, double th) {
		lat = tlat;
		lon = tlon;
		h = th;
	}

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		return json11::Json::object {
			{ "lat" , lat },
			{ "lon" , lon },
			{ "h"   , h }
		};
	}

	/// Set class contents from JSON string
	/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
		\param	s	JSON-formatted string to set class contents to
		\return n/a
	*/
	void from_json(const string& s) {
		string error;
		json11::Json parsed = json11::Json::parse(s,error);
		if(error.empty()) {
			if(!parsed["lat"].is_null())	lat = parsed["lat"].number_value();
			if(!parsed["lon"].is_null())	lon = parsed["lon"].number_value();
			if(!parsed["h"].is_null())		h = parsed["h"].number_value();
		} else	{
			cerr<<"ERROR: <"<<error<<">"<<endl;
		}
		return;
	}
};

::std::ostream& operator << (::std::ostream& out, const gvector& a);
::std::istream& operator >> (::std::istream& out, gvector& a);

//! 3 element attitude vector.
/*! Uses Tait-Bryan representation in a  zyx, right handed order of
* rotation */
// TODO: change name to something more explicit
struct avector
{
	//! Heading
	double h = 0.;
	//! Elevation
	double e = 0.;
	//! Bank
	double b = 0.;

	avector() {}
	avector(double th, double te, double tb) {
		h = th;
		e = te;
		b = tb;
	}

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		return json11::Json::object {
			{ "h" , h },
			{ "e" , e },
			{ "b" , b }
		};
	}

	/// Set class contents from JSON string
	/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
		\param	s	JSON-formatted string to set class contents to
		\return n/a
	*/
	void from_json(const string& s) {
		string error;
		json11::Json parsed = json11::Json::parse(s,error);
		if(error.empty()) {
			if(!parsed["h"].is_null())	h = parsed["h"].number_value();
			if(!parsed["e"].is_null())	e = parsed["e"].number_value();
			if(!parsed["b"].is_null())	b = parsed["b"].number_value();
		} else	{
			cerr<<"ERROR = "<<error<<endl;
		}
		return;
	}
} ;

::std::ostream& operator << (::std::ostream& out, const avector& a);
::std::istream& operator >> (::std::istream& out, avector& a);

::std::ostream& operator << (::std::ostream& out, const cvector& a);
::std::istream& operator >> (::std::istream& in, cvector& a);

// Geodetic Vector operations
//! \brief Return a zero-initialised gvector.
gvector gv_zero();

// Row Vector operations
//! \brief Return a zero-initialised rvector {0, 0, 0}.
rvector rv_zero();
//! \brief Return the shortest-path equivalent of \p v (wrap each component to [-π, π]).
rvector rv_shortest(rvector v);
//! \brief Return the shortest-path equivalent of \p v, alternative wrapping convention.
rvector rv_shortest2(rvector v);
//! \brief Return a unit vector along the X axis, optionally scaled.
//! \param scale Scale factor applied to the unit vector; default 1.
rvector rv_unitx(double scale=1.);
//! \brief Return a unit vector along the Y axis, optionally scaled.
rvector rv_unity(double scale=1.);
//! \brief Return a unit vector along the Z axis, optionally scaled.
rvector rv_unitz(double scale=1.);
//! \brief Return an rvector with all components set to 1.
rvector rv_one();
//! \brief Return an rvector with the given component values.
//! \param x X component. \param y Y component. \param z Z component.
rvector rv_one(double x, double y, double z);
//! \brief Multiply rvector \p b by scalar \p a component-wise.
//! \param a Scalar multiplier. \param b Input vector.
//! \return Scaled rvector.
rvector rv_smult(double a, rvector b);
//! \brief Return a unit vector in the direction of \p v.
//! \param v Input vector (must be non-zero).
rvector rv_normal(rvector v);
//! \brief Return the unit normal to the plane through three points.
//! \param p0 First point. \param p1 Second point. \param p2 Third point.
rvector rv_normalto(rvector p0, rvector p1, rvector p2);
//! \brief Add scalar \p a to every component of \p b.
rvector rv_sadd(double a, rvector b);
//! \brief Component-wise rvector addition.
rvector rv_add(rvector a, rvector b);
//! \brief Component-wise rvector subtraction (a − b).
rvector rv_sub(rvector a, rvector b);
//! \brief Component-wise rvector multiplication.
rvector rv_mult(rvector a, rvector b);
//! \brief Component-wise rvector division (a / b).
rvector rv_div(rvector a, rvector b);
//! \brief Component-wise square root of an rvector.
rvector rv_sqrt(rvector a);
//! \brief Cross product of two rvectors (a × b).
rvector rv_cross(rvector a, rvector b);
//! \brief Convert a spherical vector to an rvector (Cartesian).
//! \param from Spherical vector (azimuth, elevation, radius).
rvector rv_convert(svector from);
//! \brief Rotate rvector \p point about \p axis by \p theta radians.
//! \param point Point to rotate. \param axis Rotation axis (unit vector). \param theta Angle in radians.
rvector rv_rotate(rvector point, rvector axis, double theta);

//! \brief Euclidean norm (magnitude) of an rvector.
//! \param a Input vector.
//! \return ||a||.
double norm_rv(rvector a);

//! \brief Normalise an rvector to unit length in place.
//! \param v Vector to normalise (must be non-zero).
void normalize_rv(rvector &v);

//! \brief Angular separation in radians between two rvectors.
//! \param v1 First vector. \param v2 Second vector.
//! \return Angle between them in radians [0, π].
double sep_rv(rvector v1, rvector v2);

//! \brief Dot product of two rvectors.
double dot_rv(rvector a, rvector b);
//! \brief Euclidean length of an rvector (alias for norm_rv).
double length_rv(rvector v);
//! \brief Component-wise equality test for two rvectors.
bool equal_rv(rvector v1, rvector v2);
//! \brief Sum of all components of an rvector.
double sum_rv(rvector a);

//! \brief Convert an rvector to a spherical vector.
svector s_convert(rvector from);


// Column Vector operations
//! \brief Return a zero-initialised cvector {0, 0, 0}.
cvector cv_zero();
//! \brief Return a unit cvector along the X axis.
cvector cv_unitx();
//! \brief Return a unit cvector along the Y axis.
cvector cv_unity();
//! \brief Return a unit cvector along the Z axis.
cvector cv_unitz();
//! \brief Return a cvector with all components set to 1.
cvector cv_one();
//! \brief Return a unit vector in the direction of \p v.
cvector cv_normal(cvector v);
//! \brief Cross product of two cvectors (a × b).
cvector cv_cross(cvector a, cvector b);
//! \brief Add scalar \p a to every component of \p b.
cvector cv_sadd(double a, cvector b);
//! \brief Component-wise cvector addition.
cvector cv_add(cvector a, cvector b);
//! \brief Component-wise cvector subtraction (a − b).
cvector cv_sub(cvector a, cvector b);
//! \brief Component-wise cvector multiplication.
cvector cv_mult(cvector a, cvector b);
//! \brief Component-wise cvector division (a / b).
cvector cv_div(cvector a, cvector b);
//! \brief Multiply cvector \p a component-wise by scalar \p a.
cvector cv_smult(double a, cvector b);
//! \brief Component-wise square root of a cvector.
cvector cv_sqrt(cvector a);
//! \brief Normalise a cvector to unit length in place.
void normalize_cv(cvector &v);

//! \brief Angular separation in radians between two cvectors.
double sep_cv(cvector v1, cvector v2);
//! \brief Dot product of two cvectors.
double dot_cv(cvector a, cvector b);
//! \brief Euclidean length of a cvector.
double length_cv(cvector v);
//! \brief Euclidean norm of a cvector (alias for length_cv).
double norm_cv(cvector v);
//! \brief Euclidean norm of a cvector (alternate naming; alias for length_cv).
double cv_norm(cvector v);
//! \brief Sum of all components of a cvector.
double sum_cv(cvector a);

//! Quaternion, scalar last, using x, y, z.
/*! Can be thought of as ::rvector with scalar last. One can be set equal to other.
* First 3 elements are the scaled orientation axis. Fourth element is the scaled
* amount of rotation. Can alternatively be thought of as a ::cvector,
* followed by a scalar.
*/
// TODO: replace cvector with x,y,z
struct quaternion
{
	//! Orientation
	cvector d;
	//! Rotation
    double w = 1.;

	quaternion() {}
	quaternion(cvector td, double tw) {
		d = td;
		w = tw;
	}
    quaternion operator + (const quaternion &q2); // add vector by vector operator
    quaternion &operator += (const quaternion &v2); // add quaternion by quaternion operator in place
    quaternion operator - (const quaternion &q2) const; // Subtract vector by vector operator
    quaternion &operator -= (const quaternion &v2); // add quaternion by quaternion operator in place
    quaternion operator - () const; // negate quaternion in place
    quaternion operator * (const double &scale) const; // multiply quaternion by scalar operator
    quaternion &operator *= (const double &scale); // add quaternion by quaternion operator in place
    quaternion operator * (const quaternion &q2) const;

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		return json11::Json::object {
			{ "d" , d },
			{ "w" , w }
		};
	}
	/// Set class contents from JSON string
	/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
		\param	s	JSON-formatted string to set class contents to
		\return n/a
	*/
	void from_json(const string& s) {
		string error;
		json11::Json parsed = json11::Json::parse(s,error);
		if(error.empty()) {
			if(!parsed["d"].is_null())	d.from_json(parsed["d"].dump());
			if(!parsed["w"].is_null())	w = parsed["w"].number_value();
		} else	{
			cerr<<"ERROR: <"<<error<<">"<<endl;
		}
		return;
	}
} ;

::std::ostream& operator << (::std::ostream& out, const quaternion& a);
::std::istream& operator >> (::std::istream& out, quaternion& a);

//! Quaternion, scalar last, using imaginary elements.
/*! Can be thought of as i, j, k elements, followed by scalar.
*/
struct qcomplex
{
	double i = 0.;
	double j = 0.;
	double k = 0.;
	double r = 0.;

	qcomplex() {}
	qcomplex(double ti, double tj, double tk, double tr) {
		i = ti;
		j = tj;
		k = tk;
		r = tr;
	}

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		return json11::Json::object {
			{ "i" , i },
			{ "j" , j },
			{ "k" , k },
			{ "r" , r }
		};
	}

	/// Set class contents from JSON string
	/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
		\param	s	JSON-formatted string to set class contents to
		\return n/a
	*/
	void from_json(const string& s) {
		string error;
		json11::Json parsed = json11::Json::parse(s,error);
		if(error.empty()) {
			if(!parsed["i"].is_null())	i = parsed["i"].number_value();
			if(!parsed["j"].is_null())	j = parsed["j"].number_value();
			if(!parsed["k"].is_null())	k = parsed["k"].number_value();
			if(!parsed["r"].is_null())	r = parsed["r"].number_value();
		} else	{
			cerr<<"ERROR: <"<<error<<">"<<endl;
		}
		return;
	}
};

::std::ostream& operator << (::std::ostream& out, const qcomplex& a);
::std::istream& operator >> (::std::istream& out, qcomplex& a);

//! Quaternion, scalar last, using std::vector elements.
/*! Can be thought of as vector elements, q1, q2, q3, followed by
* scalar q4.
*/
struct qlast
{
	double q1 = 0.; // x
	double q2 = 0.; // y
	double q3 = 0.; // z
	double q4 = 0.; // w

	qlast() {}
	qlast(double tq1, double tq2, double tq3, double tq4) {
		q1 = tq1;
		q2 = tq2;
		q3 = tq3;
		q4 = tq4;
	}

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		return json11::Json::object {
			{ "q1" , q1 },
			{ "q2" , q2 },
			{ "q3" , q3 },
			{ "q4" , q4 }
		};
	}

	/// Set class contents from JSON string
	/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
		\param	s	JSON-formatted string to set class contents to
		\return n/a
	*/
	void from_json(const string& s) {
		string error;
		json11::Json parsed = json11::Json::parse(s,error);
		if(error.empty()) {
			if(!parsed["q1"].is_null())	q1 = parsed["q1"].number_value();
			if(!parsed["q2"].is_null())	q2 = parsed["q2"].number_value();
			if(!parsed["q3"].is_null())	q3 = parsed["q3"].number_value();
			if(!parsed["q4"].is_null())	q4 = parsed["q4"].number_value();
		} else	{
			cerr<<"ERROR: <"<<error<<">"<<endl;
		}
		return;
	}
};

::std::ostream& operator << (::std::ostream& out, const qlast& a);
::std::istream& operator >> (::std::istream& out, qlast& a);

//! Quaternion, scalar first using std::vector elements.
/*! Can be thought of as scalar element, q0, followed by vector
* elements, q1, q2, q3.
*/
struct qfirst
{
	double q0; // w
	double q1; // x
	double q2; // y
	double q3; // z

	qfirst() {}
	qfirst(double tq0, double tq1, double tq2, double tq3) {
		q0 = tq0;
		q1 = tq1;
		q2 = tq2;
		q3 = tq3;
	}

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		return json11::Json::object {
			{ "q0" , q0 },
			{ "q1" , q1 },
			{ "q2" , q2 },
			{ "q3" , q3 }
		};
	}

	/// Set class contents from JSON string
	/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
		\param	s	JSON-formatted string to set class contents to
		\return n/a
	*/
	void from_json(const string& s) {
		string error;
		json11::Json parsed = json11::Json::parse(s,error);
		if(error.empty()) {
			if(!parsed["q0"].is_null())	q0 = parsed["q0"].number_value();
			if(!parsed["q1"].is_null())	q1 = parsed["q1"].number_value();
			if(!parsed["q2"].is_null())	q2 = parsed["q2"].number_value();
			if(!parsed["q3"].is_null())	q3 = parsed["q3"].number_value();
		} else	{
			cerr<<"ERROR: <"<<error<<">"<<endl;
		}
		return;
	}
};

::std::ostream& operator << (::std::ostream& out, const qfirst& a);
::std::istream& operator >> (::std::istream& out, qfirst& a);

//! \brief Extract the rotation axis from a unit quaternion as a cvector.
//! \param q Unit quaternion.
//! \return cvector whose direction is the rotation axis; magnitude is the rotation angle in radians.
cvector cv_quaternion2axis(quaternion q);

//! \brief Normalise a quaternion to unit length in place.
//! \param q Pointer to the quaternion to normalise.
void normalize_q(quaternion *q);

//! \brief Return the zero quaternion {0, 0, 0, 0}.
quaternion q_zero();

//! \brief Return the conjugate of a quaternion (negate the vector part).
//! \param q Input quaternion.
//! \return Conjugate quaternion.
quaternion q_conjugate(quaternion q);

//! \brief Hamilton product of two quaternions (q1 × q2).
quaternion q_times(quaternion q1, quaternion q2);

//! \brief Square root of a quaternion.
//! \param q1 Input quaternion.
//! \return Quaternion r such that r × r ≈ q1.
quaternion q_sqrt(quaternion q1);

//! \brief Multiply an rvector (pure quaternion) by quaternion q2 on the right.
//! \param r1 Pure quaternion (rvector with w=0). \param q2 Right-side quaternion.
quaternion q_fmult(rvector r1, quaternion q2);

//! \brief Left-multiply two quaternions (alias for q_times).
quaternion q_fmult(quaternion q1, quaternion q2);

//! \brief Standard quaternion multiplication (alias for q_times).
quaternion q_mult(quaternion q1, quaternion q2);

//! \brief Right-multiply: return q2 × q1.
quaternion q_rmult(quaternion q1, quaternion q2);

//! \brief Scale all components of quaternion \p q by scalar \p a.
//! \param a Scalar. \param q Quaternion.
quaternion q_smult(double a, quaternion q);

//! \brief Component-wise quaternion addition.
quaternion q_add(quaternion q1, quaternion q2);

//! \brief Component-wise quaternion subtraction (q1 − q2).
quaternion q_sub(quaternion q1, quaternion q2);

//! \brief Convert ZYX Tait-Bryan Euler angles to a unit quaternion.
//! \param rpw avector of roll, pitch, yaw angles in radians.
//! \return Corresponding unit quaternion.
quaternion q_euler2quaternion(avector rpw);

//! \brief Convert an axis-angle cvector to a unit quaternion.
//! \param v Axis-angle cvector (direction = axis, magnitude = angle in radians).
quaternion q_axis2quaternion_cv(cvector v);

//! \brief Convert an axis-angle rvector to a unit quaternion.
quaternion q_axis2quaternion_rv(rvector v);

//! \brief Quaternion that directly rotates rvector \p from onto rvector \p to.
quaternion q_drotate_between_rv(rvector from, rvector to);

//! \brief Unit quaternion for a rotation of \p angle radians about axis \p around (cvector).
quaternion q_change_around_cv(cvector around, double angle);

//! \brief Unit quaternion for a rotation of \p angle radians about axis \p around (rvector).
quaternion q_change_around_rv(rvector around, double angle);

//! \brief Unit quaternion for a rotation of \p angle radians about the X axis.
quaternion q_change_around_x(double angle);
//! \brief Unit quaternion for a rotation of \p angle radians about the Y axis.
quaternion q_change_around_y(double angle);
//! \brief Unit quaternion for a rotation of \p angle radians about the Z axis.
quaternion q_change_around_z(double angle);

//! \brief Quaternion that inverse-rotates sourcea→targeta and sourceb→targetb simultaneously.
//! \param sourcea First source direction. \param sourceb Second source direction.
//! \param targeta First target direction. \param targetb Second target direction.
//! \return Best-fit unit quaternion.
quaternion q_irotate_for(rvector sourcea, rvector sourceb, rvector targeta, rvector targetb);

//! \brief Return the identity quaternion {0, 0, 0, 1} (alias for q_identity).
quaternion q_eye();
//! \brief Return the identity quaternion {0, 0, 0, 1}.
quaternion q_identity();

//! \brief Evaluate a quaternion-valued polynomial at scalar \p x.
quaternion q_evaluate_poly(double x, vector< vector<double> > parms);
//! \brief First derivative of a quaternion-valued polynomial at scalar \p x.
quaternion q_evaluate_poly_slope(double x, vector< vector<double> > parms);
//! \brief Second derivative of a quaternion-valued polynomial at scalar \p x.
quaternion q_evaluate_poly_accel(double x, vector< vector<double> > parms);
//! \brief Third derivative of a quaternion-valued polynomial at scalar \p x.
quaternion q_evaluate_poly_jerk(double x, vector< vector<double> > parms);

//! \brief Euclidean length (norm) of a quaternion.
double length_q(quaternion q);
//! \brief Euclidean norm of a quaternion (alias for length_q).
double norm_q(quaternion q);
//! \brief Angular separation in radians between two unit quaternions.
double sep_q(quaternion q1, quaternion q2);
//! \brief Inner (dot) product of two quaternions (sum of component products).
double inner_q(quaternion q1, quaternion q2);

//! \brief Rotate position \p ipos about axis \p rpos by \p angle radians, storing result in \p opos.
//! \param ipos  Input position array [3].
//! \param rpos  Rotation axis array [3].
//! \param angle Rotation angle in radians.
//! \param opos  Output position array [3].
void qrotate(double ipos[3], double rpos[3], double angle, double *opos);

//! \brief Convert a unit quaternion to ZYX Tait-Bryan Euler angles.
//! \param q Unit quaternion.
//! \return avector of roll, pitch, yaw in radians.
avector a_quaternion2euler(quaternion q);

//! \brief Quaternion that rotates cvector \p from onto cvector \p to.
quaternion q_change_between_cv(cvector from, cvector to);
//! \brief Quaternion that rotates rvector \p from onto rvector \p to.
quaternion q_change_between_rv(rvector from, rvector to);
//cvector rotate_q(quaternion q, rvector v);
//cvector rotate_q(quaternion q, cvector v);
//cvector irotate(quaternion q,cvector v); // doesn't exist in .cpp?

namespace Cosmos {
	namespace Math {
		namespace Vectors {
		//! Vector Class
		/*! eventually this is where all row vector stuff would come?
*/
		class Vector
		{

		public:
			//! Constructor and initializer.
			//! Initialize ::Vector to zero.

			explicit Vector(double x0=0., double y0=0., double z0=0., double w0=0.)
			{
				x = x0;
				y = y0;
				z = z0;
				w = w0;
			}

			explicit Vector(rvector rv)
			{
				x = rv.col[0];
				y = rv.col[1];
				z = rv.col[2];
				w = 0.;
			}

			explicit Vector(quaternion q)
			{
				x = q.d.x;
				y = q.d.y;
				z = q.d.z;
				w = q.w;
			}

			explicit Vector(cvector cv)
			{
				x = cv.x;
				y = cv.y;
				z = cv.z;
				w = 0.;
			}

			double x;
			double y;
			double z;
			double w;

			// TODO: check if we can iterated the vector
			double at(int i);
			// convert from cartesian vector to row vector
//            Vector from_cv(cvector cv);
//            Vector from_rv(rvector rv);
//            Vector from_q(quaternion q);
			rvector to_rv();
			cvector to_cv();
			quaternion to_q();
			Vector cross(Vector b);
			double dot(Vector b);
            Vector &normalize(double size=1.);
            Vector &maxmag(double size);
            Vector &minmag(double size);
            Vector &clear(double x0=0., double y0=0., double z0=0., double w0=0.);
            double norm();
            double norm2();
            double separation(Vector b);
			double area(Vector b);
			Vector vector();
			Vector flattenx();
			Vector flatteny();
			Vector flattenz();
			double sum();
            Vector rotate(Vector point, double theta);

			Vector &operator = (const rvector rv)
			{
				*this = Vector(rv);
				return *this;
			}

			double &operator [] (const int &index);

			Vector operator *(const double scale) const; // multiply vector by scalar operator
			Vector &operator *=( const double &scale); // multiply vector by scalar operator in place
			Vector operator * (const Vector &v2) const; // multiply vector by vector operator

			Vector operator / (const double scale) const; // divide vector by scalar operator
			Vector &operator /= (const double &scale); // divide vector by scalar operator in place
            Vector operator / (const Vector &v2) const; // divide vector by vector operator

			Vector operator - (const Vector &v2) const; // Subtract vector by vector operator
			Vector &operator -= (const Vector &v2); // subtract vector by vector operator in place
			Vector operator - () const; // Negate vector operator

			Vector operator + (const Vector &v2) const; // add vector by vector operator
			Vector &operator += (const Vector &v2); // add vector by vector operator in place

			bool operator == (const Vector &v2) const; // Compares two vectors
			bool operator != (const Vector &v2) const; // Compares two vectors
		
			/// Convert class contents to JSON object
			/** Returns a json11 JSON object of the class
				\return	A json11 JSON object containing every member variable within the class
			*/
			json11::Json to_json() const {
				return json11::Json::object {
					{ "x" , x },
					{ "y" , y },
                    { "z" , z }
				};
			}

			/// Set class contents from JSON string
			/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
				\param	s	JSON-formatted string to set class contents to
				\return n/a
			*/
			void from_json(const string& s) {
				string error;
				json11::Json parsed = json11::Json::parse(s,error);
				if(error.empty()) {
					if(!parsed["x"].is_null())	x = parsed["x"].number_value();
					if(!parsed["y"].is_null())	y = parsed["y"].number_value();
					if(!parsed["z"].is_null())	z = parsed["z"].number_value();
					if(!parsed["w"].is_null())	w = parsed["w"].number_value();
				} else	{
					cerr<<"ERROR: <"<<error<<">"<<endl;
				}
				return;
			}
		};

		Vector operator * (const double scale, const Vector &v);
		::std::ostream& operator << (::std::ostream& out, const Vector& v);
		Vector eye(double scale=1.);
		Vector unitxV(double scale=1.);
		Vector unityV(double scale=1.);
		Vector unitzV(double scale=1.);

		class geodeticVector : public Vector {};

		class sphericalVector : public Vector {};

		class eulerVector : public Vector {}; 
		}


		namespace Quaternions {
		class Quaternion : public Vectors::Vector
		{

		public:

            explicit Quaternion(double qx=0., double qy=0., double qz=0., double qw=1.) : Vector(qx, qy, qz, qw)
			{
				x = qx;
				y = qy;
				z = qz;
				w = qw;
			}

			explicit Quaternion(Vector v)
			{
				x = v.x;
				y = v.y;
				z = v.z;
				w = 0.;
			}

			explicit Quaternion(quaternion q)
			{
				x = q.d.x;
				y = q.d.y;
				z = q.d.z;
				w = q.w;
			}

			explicit Quaternion(rvector rv) : Vector(rv)
			{
				x = rv.col[0];
				y = rv.col[1];
				z = rv.col[2];
				w = 0.;
			}

			quaternion to_q();
			Quaternion getQuaternion();

			// temporary while the new Quaternion class is not finisheds
			Quaternion quaternion2Quaternion(quaternion q);
			quaternion Quaternion2quaternion(Quaternion Q);


			// operators
//            Quaternion &operator = (const Quaternion &q) { return *this; }
			Quaternion &operator = (const quaternion &q2);
            Quaternion operator * (const double &scale) const; // multiply vector by scalar operator
            Quaternion &operator *= (const double &scale); // multiply vector by scalar operator in place
			Quaternion operator * (const Quaternion &q2) const;

//            Quaternion operator / (double scale); // multiply vector by scalar operator
//            Quaternion &operator /= (const double &scale); // multiply vector by scalar operator in place

			Quaternion operator - (const Quaternion &q2) const; // Subtract vector by vector operator
			Quaternion &operator -= (const Quaternion &q2); // subtract vector by vector operator in place
			Quaternion operator - () const; // Negate vector operator

			Quaternion operator + (const Quaternion &q2); // add vector by vector operator
			Quaternion &operator += (const Quaternion &q2); // add vector by vector operator in place
			//::std::ostream& operator<<(::std::ostream& os, const Quaternion& q);
			//friend ::std::ostream& operator << (::std::ostream& os, const Quaternion& q);
			//::std::istream& operator >> (::std::istream& out, Quaternion& a);

			Quaternion multiplyScalar(double a);
			Quaternion conjugate() const;
			Vector vector();
			Vector omegaFromDerivative(Quaternion dq);
//            Quaternion &normalize();
//            double norm();
			Vector toEuler();
			Vector irotate(const Vector &v);
			Vector drotate(const Vector &v);

			/// Convert class contents to JSON object
			/** Returns a json11 JSON object of the class
				\return	A json11 JSON object containing every member variable within the class
			*/
			json11::Json to_json() const {
				return json11::Json::object {
					{ "x" , x },
					{ "y" , y },
					{ "z" , z },
					{ "w" , w }
				};
			}

			/// Set class contents from JSON string
			/** Parses the provided JSON-formatted string and sets the class data. String should be formatted like the string returned from #to_json()
				\param	s	JSON-formatted string to set class contents to
				\return n/a
			*/
			void from_json(const string& s) {
				string error;
				json11::Json parsed = json11::Json::parse(s,error);
				if(error.empty()) {
					if(!parsed["x"].is_null())	x = parsed["x"].number_value();
					if(!parsed["y"].is_null())	y = parsed["y"].number_value();
					if(!parsed["z"].is_null())	z = parsed["z"].number_value();
					if(!parsed["w"].is_null())	w = parsed["w"].number_value();
				} else	{
					cerr<<"ERROR: <"<<error<<">"<<endl;
				}
				return;
			}

		};

		// declared outside class because it does not need to access members of the class Quaternion
		Quaternion operator * (double scale, const Quaternion &q);
		Quaternion operator * (const Vectors::Vector &v, const Quaternion &q);
		::std::ostream& operator << (::std::ostream& os, const Quaternion& q);
		Quaternion irotate_for(Vectors::Vector sourcea, Vectors::Vector sourceb, Vectors::Vector targeta, Vectors::Vector targetb);
		Quaternion drotate_between(Vectors::Vector a, Vectors::Vector b);
        Quaternion drotate_around(Vectors::Vector axis, double angle);
		Quaternion drotate_around_x(double angle);
		Quaternion drotate_around_y(double angle);
		Quaternion drotate_around_z(double angle);
        Quaternion drotate_around(int axis, double angle);
        Quaternion eye(double scale = 1.);
        Quaternion zero();

		}
	} // end namespace Math
} // end namespace COSMOS

#endif // _MATH_VECTOR_H
