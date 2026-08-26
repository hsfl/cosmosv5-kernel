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

// dependencies: vector
/*!
 * \file matrix.h
 * \brief Fixed-size matrix operations for linear algebra.
 * \ingroup cosmos_kernel_math
 * \defgroup cosmos_matrix Matrix
 *
 * Implements fixed-size 2-D matrix types and operations including addition,
 * multiplication, transpose, and inversion. Designed to work with the COSMOS
 * vector and rotation types for attitude and coordinate transform mathematics.
 */

#ifndef _MATH_MATRIX_H
#define _MATH_MATRIX_H

#include "support/configCosmos.h"
#include "math/vector.h"
using namespace Cosmos::Math::Vectors;

//! 3x3 element generic matrix
/*! 3 rvector elements representing 3 rows of a matrix
*/
struct rmatrix
{
	rvector row[3];

	rmatrix() {}
	rmatrix(rvector rv0, rvector rv1, rvector rv2) {
		row[0] = rv0;
		row[1] = rv1;
		row[2] = rv2;
	}

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		vector<rvector> v_row = vector<rvector>(row, row+sizeof(row)/sizeof(row[0]));
		return json11::Json::object {
			{ "row" , v_row }
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
			auto p_row = parsed["row"].array_items();
			for(size_t i = 0; i != p_row.size(); ++i) {
				if(!p_row[i].is_null()) row[i].from_json(p_row[i].dump());
			}
		} else  {
			cerr<<"ERROR: <"<<error<<">"<<endl;
		}
		return;
	}

};

::std::ostream& operator << (::std::ostream& out, const rmatrix& a);
::std::ostream& operator << (::std::ostream& out, const vector<rmatrix>& a);
::std::istream& operator >> (::std::istream& out, rmatrix& a);

rvector operator * (rmatrix m, rvector v); // multiply matrix by vector operator



//! 3x3 element cartesian matrix
/*! 3 ::cvector elements representing 3 rows of a matrix in a right
* handed cartesian system */
struct cmatrix
{
	//! Row 1
	cvector r1;
	//! Row 2
	cvector r2;
	//! Row 3
	cvector r3;

	/// Convert class contents to JSON object
	/** Returns a json11 JSON object of the class
		\return	A json11 JSON object containing every member variable within the class
	*/
	json11::Json to_json() const {
		return json11::Json::object {
			{ "r1" , r1 },
			{ "r2" , r2 },
			{ "r3" , r3 }
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
			if(!parsed["r1"].is_null()) r1.from_json(parsed["r1"].dump());
			if(!parsed["r2"].is_null()) r2.from_json(parsed["r2"].dump());
			if(!parsed["r3"].is_null()) r3.from_json(parsed["r3"].dump());
		} else  {
			cerr<<"ERROR: <"<<error<<">"<<endl;
		}
		return;
	}


} ;

::std::ostream& operator << (::std::ostream& out, const cmatrix& a);
::std::istream& operator >> (::std::istream& in, cmatrix& a);


//! n element row matrix
struct matrix1d
{
	//! Elements
	double vector[4];
	//! Number of elements
	uint16_t cols;
} ;

//! nxm element 2D matrix
struct matrix2d
{
	//! Number of rows
	uint16_t rows;
	//! Number of elements
	uint16_t cols;
	//! Elements
	double array[4][4];
} ;


//! \brief Multiply row-major matrix \p a by rvector \p b (matrix-vector product).
//! \param a 3×3 rmatrix. \param b Column vector.
//! \return Result rvector.
rvector rv_mmult(rmatrix a, rvector b);

//! \brief Extract the diagonal of a row-major matrix as an rvector.
//! \param a Input 3×3 rmatrix.
//! \return rvector {a[0][0], a[1][1], a[2][2]}.
rvector rv_diag(rmatrix a);

//! \brief Extract the axial vector from a skew-symmetric rmatrix (inverse of rm_skew).
//! \param matrix Skew-symmetric 3×3 rmatrix.
//! \return Axial rvector.
rvector rv_unskew(rmatrix matrix);

//! \brief Multiply column-major matrix \p a by cvector \p b (matrix-vector product).
//! \param a 3×3 cmatrix. \param b Column vector.
//! \return Result cvector.
cvector cv_mmult(cmatrix a, cvector b);

//! \brief Extract the diagonal of a column-major matrix as a cvector.
cvector cv_diag(cmatrix a);

//! \brief Return the transpose of a row-major matrix.
//! \param a Input 3×3 rmatrix.
//! \return Transposed rmatrix.
rmatrix rm_transpose(rmatrix a);

//! \brief Return a × aᵀ (outer product / element-wise square) of a row-major matrix.
rmatrix rm_square(rmatrix a);

//! \brief Return the 3×3 identity rmatrix.
rmatrix rm_eye();

//! \brief Return the 3×3 zero rmatrix.
rmatrix rm_zero();

//! \brief Multiply two row-major matrices (a × b).
rmatrix rm_mmult(rmatrix a, rmatrix b);

//! \brief Multiply two row-major matrices (alias for rm_mmult).
rmatrix rm_mult(rmatrix a, rmatrix b);

//! \brief Multiply row-major matrix \p b by scalar \p a.
rmatrix rm_smult(double a, rmatrix b);

//! \brief Component-wise addition of two row-major matrices.
rmatrix rm_add(rmatrix a, rmatrix b);

//! \brief Component-wise subtraction of two row-major matrices (a − b).
rmatrix rm_sub(rmatrix a, rmatrix b);

//! \brief Rotation matrix that rotates rvector \p from onto rvector \p to.
//! \param from Source direction. \param to Target direction.
//! \return 3×3 rotation rmatrix.
rmatrix rm_change_between_rv(rvector from, rvector to);

//! \brief Rotation matrix for \p angle radians about the X axis (row-major).
rmatrix rm_change_around_x(double angle);
//! \brief Rotation matrix for \p angle radians about the Y axis (row-major).
rmatrix rm_change_around_y(double angle);
//! \brief Rotation matrix for \p angle radians about the Z axis (row-major).
rmatrix rm_change_around_z(double angle);

//! \brief Rotation matrix for \p angle radians about axis \p axis (0=X, 1=Y, 2=Z).
rmatrix rm_change_around(int axis, double angle);

//! \brief Construct a diagonal rmatrix from an rvector.
//! \param a Diagonal element vector.
rmatrix rm_diag(rvector a);

//! \brief Convert a column-major cmatrix to a row-major rmatrix.
rmatrix rm_from_cm(cmatrix matrix);

//! \brief Build a row or column of an rmatrix from an rvector.
//! \param vector Source rvector. \param direction 0=row, 1=column.
rmatrix rm_from_rv(rvector vector,int direction);

//! \brief Build the skew-symmetric cross-product matrix from an rvector.
//! \param row1 Axial vector [x, y, z].
//! \return 3×3 skew-symmetric rmatrix.
rmatrix rm_skew(rvector row1);

//! \brief Return the inverse of a 3×3 row-major matrix (assumes it is invertible).
//! \param m Input rmatrix.
//! \return Inverse rmatrix.
rmatrix rm_inverse(rmatrix m);

//! \brief Convert a general 2-D matrix to an rmatrix (upper-left 3×3 block).
rmatrix rm_from_m2(matrix2d m);

//! \brief Frobenius norm of a row-major matrix (√sum of squared elements).
//! \param a Input rmatrix.
//! \return Frobenius norm.
double norm_rm(rmatrix a);

//! \brief Trace (sum of diagonal elements) of a row-major matrix.
double trace_rm(rmatrix a);

//! \brief Determinant of a 3×3 row-major matrix.
double determinant_rm(rmatrix a);

//! \brief Frobenius norm of a column-major matrix.
double norm_cm(cmatrix a);

//! \brief Trace of a column-major matrix.
double trace_cm(cmatrix a);

//! \brief Return the transpose of a column-major matrix.
cmatrix cm_transpose(cmatrix a);

//! \brief Return a × aᵀ of a column-major matrix.
cmatrix cm_square(cmatrix a);

//! \brief Return the 3×3 identity cmatrix.
cmatrix cm_eye();

//! \brief Return the 3×3 zero cmatrix.
cmatrix cm_zero();

//! \brief Multiply two column-major matrices (a × b).
cmatrix cm_mmult(cmatrix a, cmatrix b);

//! \brief Multiply two column-major matrices (alias for cm_mmult).
cmatrix cm_mult(cmatrix a, cmatrix b);

//! \brief Multiply column-major matrix \p b by scalar \p a.
cmatrix cm_smult(double a, cmatrix b);

//! \brief Component-wise addition of two column-major matrices.
cmatrix cm_add(cmatrix a, cmatrix b);

//! \brief Component-wise subtraction of two column-major matrices (a − b).
cmatrix cm_sub(cmatrix a, cmatrix b);

//! \brief Rotation matrix that rotates cvector \p from onto cvector \p to (column-major).
cmatrix cm_change_between_cv(cvector from, cvector to);

//! \brief Rotation matrix for \p angle radians about the X axis (column-major).
cmatrix cm_change_around_x(double angle);
//! \brief Rotation matrix for \p angle radians about the Y axis (column-major).
cmatrix cm_change_around_y(double angle);
//! \brief Rotation matrix for \p angle radians about the Z axis (column-major).
cmatrix cm_change_around_z(double angle);

//! \brief Rotation matrix for \p angle radians about axis \p axis (0=X, 1=Y, 2=Z) (column-major).
cmatrix cm_change_around(int axis, double angle);

//! \brief Convert a row-major rmatrix to a column-major cmatrix.
cmatrix cm_from_rm(rmatrix matrix);

//! \brief Construct a diagonal cmatrix from a cvector.
cmatrix cm_diag(cvector a);

//! \brief Return the inverse of a general 2-D matrix.
//! \param m Input matrix2d (must be square and invertible).
//! \return Inverse matrix2d.
matrix2d m2_inverse(matrix2d m);

//! \brief Determinant of a 2-D matrix.
double m2_determinant(matrix2d m);

//! \brief Return a \p rows × \p cols zero matrix2d.
matrix2d m2_zero(uint16_t rows, uint16_t cols);

//! \brief Return a \p rows × \p rows identity matrix2d.
matrix2d m2_eye(uint16_t rows);

//! \brief Matrix product of two matrix2d objects.
matrix2d m2_cross(matrix2d matrix1, matrix2d matrix2);

//! \brief Multiply all elements of a matrix2d by scalar \p number.
matrix2d m2_smult(double number, matrix2d matrix);

//! \brief Component-wise subtraction of two matrix2d objects.
matrix2d m2_sub(matrix2d matrix1, matrix2d matrix2);

//! \brief Component-wise addition of two matrix2d objects.
matrix2d m2_add(matrix2d matrix1, matrix2d matrix2);

//! \brief Return the transpose of a matrix2d.
matrix2d m2_transpose(matrix2d matrix);

//! \brief Extract the axial vector from a 3×3 skew-symmetric matrix2d.
matrix1d m2_unskew(matrix2d matrix);
double m2_trace(matrix2d matrix);
matrix2d m2_mmult(matrix2d matrix1, matrix2d matrix2);
matrix1d m2_eig2x2(matrix2d matrix);
double m2_snorm2x2(matrix2d matrix);
matrix2d cv_to_m2(cvector vector,int direction);
matrix1d cv_to_m1(cvector vector);
matrix2d cm3x3_to_m2(cmatrix matrix);
matrix2d m1_to_m2(matrix1d vector,int direction);
matrix2d m2_from_rm(rmatrix matrix);

matrix1d m1_zero(uint16_t cols);
matrix1d m1_smult(double number, matrix1d row);
matrix1d m1_mmult(matrix2d matrix, matrix1d row);
matrix1d m1_sub(matrix1d row1, matrix1d row2);
matrix1d m1_add(matrix1d row1, matrix1d row2);
matrix1d m1_cross(matrix1d row1, matrix1d row2);
matrix2d m2_skew(matrix1d row1);
matrix2d m2_diag(matrix1d row1);
double m1_norm(matrix1d matrix);
double m1_dot(matrix1d a, matrix1d b);

namespace  Cosmos {
	namespace Math {
		namespace Matrices {
			class Matrix
			{

			public:
				//! Constructor, initialize to zero.
				explicit Matrix(Vector r00=Vector(), Vector r10=Vector(), Vector r20=Vector())
				{
					r0 = r00;
					r1 = r10;
					r2 = r20;
				}

				explicit Matrix(rmatrix m0)
				{
					r0 = Vector(m0.row[0]);
					r1 = Vector(m0.row[1]);
					r2 = Vector(m0.row[2]);
				}

				explicit Matrix(matrix2d m)
				{
					r0[0] = m.array[0][0];
					r0[1] = m.array[0][1];
					r0[2] = m.array[0][2];
					r1[0] = m.array[1][0];
					r1[1] = m.array[1][1];
					r1[2] = m.array[1][2];
					r2[0] = m.array[2][0];
					r2[1] = m.array[2][1];
					r2[2] = m.array[2][2];
				}

				explicit Matrix(cmatrix m)
				{
					r0[0] = m.r1.x;
					r0[1] = m.r1.y;
					r0[2] = m.r1.z;
					r1[0] = m.r2.x;
					r1[1] = m.r2.y;
					r1[2] = m.r2.z;
					r2[0] = m.r3.x;
					r2[1] = m.r3.y;
					r2[2] = m.r3.z;
				}

                explicit Matrix(Quaternions::Quaternion q)
                {
                    double yy, xx, zz, xy, xz, xw, yz, yw, zw;

                    q.normalize();

                    xx = 2. * q.x;
                    xy = xx * q.y;
                    xz = xx * q.z;
                    xw = xx * q.w;
                    xx *= q.x;
                    yy = 2. * q.y;
                    yz = yy * q.z;
                    yw = yy * q.w;
                    yy *= q.y;
                    zz = 2. * q.z;
                    zw = zz * q.w;
                    zz *= q.z;

                    r0[0] = 1. - yy - zz;
                    r0[1] = xy - zw;
                    r0[2] = xz + yw;
                    r1[0] = xy + zw;
                    r1[1] = 1. - xx - zz;
                    r1[2] = yz - xw;
                    r2[0] = xz - yw;
                    r2[1] = yz + xw;
                    r2[2] = 1. - xx - yy;
                }

				//! ::Vector to ::Matrix.
				/*! Convert a row vector to a row order ::Matrix
						\param vector Row vector to be converted
						\param direction Alignment, column order if 1 (DIRECTION_COLUMN), otherwise row
						order.
						\return Single row or column ::Matrix
				*/
				Matrix(Vector vector,int direction)
				{
					switch (direction)
					{
					case DIRECTION_COLUMN:
						r0[0] = vector[0];
						r1[0] = vector[1];
						r2[0] = vector[2];
						break;
					default:
						r0 = vector;
						break;
					}
				}

				Vector r0;
				Vector r1;
				Vector r2;

				Vector c0()
				{
					Vector c;
					c[0] = (*this)[0][0];
					c[1] = (*this)[1][0];
					c[2] = (*this)[2][0];
					return c;
				}

				Vector c1()
				{
					Vector c;
					c[0] = (*this)[0][1];
					c[1] = (*this)[1][1];
					c[2] = (*this)[2][0];
					return c;
				}

				Vector c2()
				{
					Vector c;
					c[0] = (*this)[0][2];
					c[1] = (*this)[1][2];
					c[2] = (*this)[2][2];
					return c;
				}

				Vector &operator [] (const int &index);

				Matrix &operator *= (const double scale);
				Matrix operator * (const double scale) const;
//                Matrix smult(const double scale) const;

				Vector operator * (const Vector v) const;
//                Vector mmult(const Vector v) const;

				Matrix &operator *= (const Matrix &m);
				Matrix operator * (const Matrix &m) const;
				Matrix mmult(const Matrix &m) const;

				Matrix mult(const Matrix &b) const;

				Matrix &operator += (const Matrix &m);
				Matrix operator + (const Matrix &m) const;

				Matrix &operator -= (const Matrix &m);
				Matrix operator - (const Matrix &m) const;

				Vector diag();
				Vector unskew();
				
				Matrix transpose();
				Matrix square();
				Matrix inverse();
				Matrix change_between(Vector from, Vector to);
				Matrix change_around_x(double angle);
				Matrix change_around_y(double angle);
				Matrix change_around_z(double angle);
				Matrix change_around(int axis, double angle);
				Matrix diag(Vector a);
				Matrix skew(Vector row1);

				double determinant();

				/// Convert class contents to JSON object
				/** Returns a json11 JSON object of the class
					\return	A json11 JSON object containing every member variable within the class
				*/
				json11::Json to_json() const {
					return json11::Json::object {
						{ "r0" , r0 },
						{ "r1" , r1 },
						{ "r2" , r2 }
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
						if(!parsed["r0"].is_null()) r0.from_json(parsed["r0"].dump());
						if(!parsed["r1"].is_null()) r1.from_json(parsed["r1"].dump());
						if(!parsed["r2"].is_null()) r2.from_json(parsed["r2"].dump());
					} else {
						cerr<<"ERROR: <"<<error<<">"<<endl;
					}
					return;
				}
			};

			Matrix operator * (const double scale, Matrix &v);
			Matrix eye(double scale = 1.);
		}
	} // end of namespace Nath
} // end of namespace COSMOS

#endif
