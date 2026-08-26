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
 * \file mathlib.h
 * \brief General math functions and physical constants for COSMOS.
 * \ingroup cosmos_kernel_math
 * \defgroup cosmos_mathlib Math
 *
 * Provides a collection of mathematical utility functions (angle wrapping,
 * interpolation, statistics) as well as commonly used physical and
 * astronomical constants. This is a foundational header included by most
 * COSMOS computation libraries.
 */

#ifndef _MATHLIB_H
#define _MATHLIB_H 1

/*! \file mathlib.h
    \brief mathlib include file
*/

//! \ingroup support
//!  \defgroup mathlib Matrix and vector math library
//! COSMOS Math Support Library.
//!
//! This library of mathematical operations and the accompanying set of type
//! definitions provides support for all the mathematical computations
//! necessary within COSMOS. Special emphasis is placed on the calculation and
//! representation of Attitude, which requires vector and matrix operations,
//! and the attendant variable types representing it.
//!
//! The broad areas of support are:
//! - Angular conversion
//! - Byte order conversion
//! - Matrix and Vector storage and operations
//! - Coordinate system attitude conversion and rotation
//! - Fitting and Integration
//!
//! Angular Conversion
//!
//! Constants are defined to represent various multiples of Pi in double
//! precision. Constants are also defined to provide conversion factors for
//! conversion between Arc Seconds and Degrees. Finally, macros have been
//! defined to allow conversion both ways between Radians and Degrees.
//!
//! Byte Order Conversion
//!
//! This set of functions and constants allows the automatic sensing and
//! correction of various architecture byte orders. Constants are provided to
//! represent the 2 possible orders. The function ::local_byte_order() is
//! provided to determine the byte order locally. Finally various functions
//! are provided to swap floating point and interger variables between
//! local and other byte orders.
//!
//! Matrix and Vector Math
//!
//! Support for vectors and matrices up to rank and order 4 has been provided.
//! Various vector types have been defined to support various conventions.
//! Various matrix type are then built on top of these. Finally, a universal
//! vector type is defined to allow quick conversion between types. Matrices
//! are all considered to be Row Major, in that their data is stored
//! internally by row first, and their first index always indicates row.
//! Vectors are also considered to be rows, except that when they are
//! submitted for multiplication by a matrix, they are first considered to
//! have been transposed.
//!
//! Coordinate System Attitude Conversion and Rotation
//!
//! In support of rotations, types have been defined to support quaternions,
//! direction cosine matrices, euler angles, and simple direction and
//! magnitude. In the interest of conciseness, the following conventions have
//! been adopted:
//! - Quaternions represent Left Side Multiplication Rotation.
//! - Euler angles represent a Right Handed, Intrinsic, ZYX, Tait-Bryan Rotation.
//! - Direction and Magnitude uses radians.
//!
//! Due to the multiplicity of quaternion representations, a number of naming
//! conventions are defined in parallel in COSMOS. These conventions are:
//! - Vector and Scalar: Vector first, expressed as x, y, z (default)
//! - Complex: Imaginary first, expressed as i, j, k
//! - Array with Scalar First: q1, q2, q3 and scalar q4
//! - Array with Scalar Last: scalar q0 and q1, q2, q3
//!
//! Universal Vector Type
//!
//! A union has been defined that contains all vector and quaternion types.
//! This allows for the quick equating of one to the other for easy switching
//! between conventions. As an example, one can define a Vector/Scalar
//! quaternion, and then use an equation that expects Scalar Last.

#include "support/configCosmos.h"
//#include "support/cosmos-errno.h"

// include all math modules
//#include "constants.h"
#include "vector.h"
#include "matrix.h"
#include "rotation.h"

#include <cmath>
#include <iostream>

//#define true 1
//#define false 0


//! \ingroup cosmos_mathlib
//! \defgroup mathlib_typedefs Math library typedefs
//! @{

//! Scalar value type Union
/*! A union of double, float, int32, int16, unit32, uint16 that allows
 * manipulating all.
*/
// TODO: explain why this union is used for
typedef union
{
    double d;
    float f;
    int32_t i32;
    int16_t i16;
    uint32_t u32;
    uint16_t u16;
} utype;

//! Quaternion/Rvector Union
/*! A union of a ::cvector, ::rvector, ::matrix1d, and a ::quaternion that allows manipulating all.
*/
// TODO: explain why this union is used for
union uvector
{
    quaternion q;
    qcomplex qc;
    qfirst qf;
    qlast ql;
    rvector r;
    cvector c;
    svector s;
    gvector g;
    matrix1d m1;
    avector a;
    double a4[4];
} ;


//! pxnxm element cube
//typedef double*** matrix3d;

//! Gauss-Jackson Integration Kernel
/*! Contains parameters that can be reused by any instance of a Gauss-Jackson integration of the given order and time step.
*/
// TODO: create seperate class (and file) for gauss jackson functions
struct gj_kernel
{
    int32_t order;
    int32_t horder;
    double dvi;
    double dvi2;
    int32_t **binom;
    double **alpha;
    double **beta;
    double *c;
    double *gam;
    double *q;
    double *lam;
} ;

//! Gauss-Jackson Integration Step
/*! Contains the variables specific to a single step of a particular integration of a given order
*/
struct gj_step
{
    //! Dependent variable
    double vd0;
    //! Dependent variable 1st derivative
    double vd1;
    //! Dependent variable 2nd derivative
    double vd2;
    double *a;
    double *b;
    //! Current guess of dependent variable
    double s;
    double ss;
    double sa;
    double sb;
} ;

//! Gauss-Jackson Integration Instance
/*! Contains the kernel and all steps necessary for a particular integration, order, time step.
*/
struct gj_instance
{
    //! Kernel Pointer
    gj_kernel *kern;
    //! Number of axes of integration
    int32_t axes;
    //! Pointer to array of steps; one for each axis, order+2 for the orders
    gj_step **steps;
    //! Pointer to array of independent variables; order+2
    double *vi;
    //! Pointer to a function that will calculate the 2nd derivative given a axes dependent and one independent variable.
    void (*calc_vd2)(double vi, double *vd0, double *vd2, int32_t axes);
} ;

//! Gauss-Jackson 3D Integration Instance
/*! Contains the kernel and all steps necessary for a particular integration, order, time step.
*/
struct gj_instance3d
{
    //! Kernel Pointer
    gj_kernel *kern;
    //! Pointer to array of x steps
    gj_step *stepx;
    //! Pointer to array of y steps
    gj_step *stepy;
    //! Pointer to array of z steps
    gj_step *stepz;
    //! Pointer to array of independent variables
    double *vi;
    //! Pointer to a function that will calculate the 2nd derivative given a dependent and independent variable.
    double (*calc_vd2)(double vi, double vdx0, double vdy0, double vdz0);
} ;

//! Estimator structure
/*! Contains an estimate returned by the estimator. This includes the
 * zeroth, first, and second derivatives of the dependent value, as
 * well as estimated errors for each.
*/
struct estimatorstruc
{
    double value[3];
    double error[3];
    vector<double> a;
    vector<double> x;
    vector<double> y;
};

//! Estimator handle
/*! Contains storage elements for a string of 2N dependent and
 * independent variables, plus polynomial coefficients for N
 * consecutive fits. This structure can then be used to either return
 * dependent values for an arbitrary independent value, or to update
 * the estimator with new pairs.
*/
struct estimatorhandle
{
    vector<estimatorstruc> r;
    int32_t index;
    uint32_t size;
    uint32_t degree;
    double xbase;
    double ybase;
} ;

//! @}

//! \ingroup cosmos_mathlib
//! \defgroup mathlib_functions Math library functions
//! @{

//! \brief Draw a sample from a Gaussian distribution.
//! \param mean  Distribution mean.
//! \param stdev Distribution standard deviation.
//! \return Random double drawn from N(mean, stdev²).
double gaussian_random(double mean, double stdev);

//! \brief Perpendicular distance from point \p p0 to line through \p p1 and \p p2.
//! \param p0 The query point.
//! \param p1 First point on the line.
//! \param p2 Second point on the line.
//! \return Distance in the same units as the input vectors.
double distance_rv(rvector p0, rvector p1, rvector p2);

//! \brief Area of the triangle defined by three position vectors.
//! \param p0 First vertex.
//! \param p1 Second vertex.
//! \param p2 Third vertex.
//! \return Triangle area in the squared units of the input vectors.
double area_rv(rvector p0, rvector p1, rvector p2);

//! \brief Evaluate a polynomial at scalar \p x.
//! \param x     Independent variable.
//! \param parms Coefficient vector (index 0 = constant term).
//! \return Polynomial value at \p x.
double evaluate_poly(double x, rvector parms);

//! \brief Evaluate the first derivative of a polynomial at scalar \p x.
//! \param x     Independent variable.
//! \param parms Coefficient vector.
//! \return First derivative at \p x.
double evaluate_poly_slope(double x, rvector parms);

//! \brief Evaluate the second derivative of a polynomial at scalar \p x.
//! \param x     Independent variable.
//! \param parms Coefficient vector.
//! \return Second derivative at \p x.
double evaluate_poly_accel(double x, rvector parms);

//! \brief Evaluate the third derivative of a polynomial at scalar \p x.
//! \param x     Independent variable.
//! \param parms Coefficient vector.
//! \return Third derivative at \p x.
double evaluate_poly_jerk(double x, rvector parms);

//! \brief Evaluate a vector polynomial (gvector) at scalar \p x.
//! \param x     Independent variable.
//! \param parms Per-axis coefficient vectors.
//! \return gvector result at \p x.
gvector gv_evaluate_poly(double x, vector< vector<double> > parms);

//! \brief First derivative of a vector polynomial (gvector) at scalar \p x.
gvector gv_evaluate_poly_slope(double x, vector< vector<double> > parms);

//! \brief Second derivative of a vector polynomial (gvector) at scalar \p x.
gvector gv_evaluate_poly_accel(double x, vector< vector<double> > parms);

//! \brief Third derivative of a vector polynomial (gvector) at scalar \p x.
gvector gv_evaluate_poly_jerk(double x, vector< vector<double> > parms);

//! \brief Evaluate a vector polynomial (rvector) at scalar \p x.
//! \param x     Independent variable.
//! \param parms Per-axis coefficient vectors.
//! \return rvector result at \p x.
rvector rv_evaluate_poly(double x, vector< vector<double> > parms);

//! \brief First derivative of a vector polynomial (rvector) at scalar \p x.
rvector rv_evaluate_poly_slope(double x, vector< vector<double> > parms);

//! \brief Second derivative of a vector polynomial (rvector) at scalar \p x.
rvector rv_evaluate_poly_accel(double x, vector< vector<double> > parms);

//! \brief Third derivative of a vector polynomial (rvector) at scalar \p x.
rvector rv_evaluate_poly_jerk(double x, vector< vector<double> > parms);

//! \brief Arbitrary-order derivative of a vector polynomial (rvector) at scalar \p x.
//! \param x     Independent variable.
//! \param parms Per-axis coefficient vectors.
//! \param order Derivative order (0 = value, 1 = slope, …).
//! \return rvector derivative at \p x.
rvector rv_evaluate_poly_deriv(double x, vector< vector<double> > parms, uint16_t order);

//! \brief Rotate rvector \p v by quaternion \p q (left-side multiplication).
//! \param q Rotation quaternion (unit quaternion).
//! \param v Vector to rotate.
//! \return Rotated rvector.
rvector rotate_q(quaternion q,rvector v);

//! \brief Rotate cvector \p v by quaternion \p q.
//! \param q Rotation quaternion (unit quaternion).
//! \param v Vector to rotate.
//! \return Rotated cvector.
cvector rotate_q(quaternion q, cvector v);

//! \brief Transform rvector \p v into the frame defined by quaternion \p q.
//! \param q Frame quaternion.
//! \param v Vector in the original frame.
//! \return Vector expressed in the rotated frame.
rvector transform_q(quaternion q,rvector v);

//! \brief Direct rotation of rvector \p v by quaternion \p q (rotates the vector).
//! \param q Rotation quaternion.
//! \param v Vector to rotate.
//! \return Rotated rvector.
rvector drotate(quaternion q,rvector v);

//! \brief Direct rotation of cvector \p v by quaternion \p q.
cvector drotate(quaternion q, cvector v);

//! \brief Inverse rotation of rvector \p v by quaternion \p q (rotates the coordinate axes).
//! \param q Rotation quaternion.
//! \param v Vector to inverse-rotate.
//! \return Inverse-rotated rvector.
rvector irotate(quaternion q,rvector v);

//! \brief Inverse rotation of cvector \p v by quaternion \p q.
cvector irotate(quaternion q, cvector v);

//! \brief Convert a quaternion to an axis-angle rvector.
//! \param q Unit quaternion.
//! \return rvector whose direction is the rotation axis and magnitude is the rotation angle in radians.
rvector rv_quaternion2axis(quaternion q);

//! \brief Fit a polynomial of given \p order through scalar data points.
//! \param x     Independent variable values packed in a uvector.
//! \param y     Dependent variable values packed in a uvector.
//! \param order Polynomial order.
//! \return uvector of polynomial coefficients.
uvector rv_fitpoly(uvector x, uvector y, uint32_t order);

//! \brief Least-squares polynomial fit (degree 1) through data points.
//! \param x Independent variable values.
//! \param y Dependent variable values.
//! \return Coefficient vector [intercept, slope].
vector<double> polyfit(vector<double> &x, vector<double> &y);

//! \brief Solve an over-determined linear system Xa = y by least squares.
//! \param x Design matrix (rows = observations, cols = parameters).
//! \param y Observation vector.
//! \param a Output coefficient vector.
void multisolve(vector< vector<double> > x, vector<double> y, vector<double>& a);

//! \brief Initialise an estimator handle for sliding-window polynomial fitting.
//! \param estimate Pointer to the handle to initialise.
//! \param size     Window size (number of samples to retain).
//! \param degree   Polynomial degree for the fit.
void open_estimate(estimatorhandle *estimate, uint32_t size, uint32_t degree);

//! \brief Feed a new data point into a sliding-window estimator.
//! \param estimate   Estimator handle.
//! \param independent Independent variable value (e.g. time).
//! \param dependent   Dependent variable value.
//! \return 0 on success, negative error code on failure.
int16_t set_estimate(estimatorhandle *estimate, double independent, double dependent);

//! \brief Query an estimator for value and derivatives at a given point.
//! \param estimate   Estimator handle.
//! \param independent Independent variable value at which to evaluate.
//! \return estimatorstruc containing value, first derivative, second derivative, and error estimates.
estimatorstruc get_estimate(estimatorhandle *estimate, double independent);

//! \brief Allocate and initialise a Gauss-Jackson integration kernel.
//! \param order Integration order (number of corrector steps).
//! \param dvi   Independent variable step size.
//! \return Pointer to the allocated kernel; caller must free with gauss_jackson_dekernel().
gj_kernel *gauss_jackson_kernel(int32_t order, double dvi);

//! \brief Free a Gauss-Jackson kernel and all its internal arrays.
//! \param gjk Pointer to the kernel to free.
void gauss_jackson_dekernel(gj_kernel *gjk);

//! \brief Create a Gauss-Jackson integration instance.
//! \param gjk       Kernel to use.
//! \param axes      Number of dependent variable axes.
//! \param calc_vd2  Callback that computes the second derivative given the current state.
//! \return Pointer to the integration instance.
gj_instance *gauss_jackson_instance(gj_kernel *gjk,int32_t axes,void (*calc_vd2)(double vi, double *vd0, double *vd2, int32_t axes));

//! \brief Set one integration step's initial conditions.
//! \param gji   Integration instance.
//! \param vi    Independent variable value for this step.
//! \param vd0   Dependent variable values (one per axis).
//! \param vd1   First derivatives (one per axis).
//! \param vd2   Second derivatives (one per axis).
//! \param istep Step index to set.
//! \return 0 on success.
int gauss_jackson_setstep(gj_instance *gji, double vi, double *vd0, double *vd1, double *vd2, int32_t istep);

//! \brief Retrieve the state at one integration step.
//! \param gji   Integration instance.
//! \param vi    Output: independent variable value at \p istep.
//! \param vd0   Output: dependent variable values (one per axis).
//! \param vd1   Output: first derivatives (one per axis).
//! \param vd2   Output: second derivatives (one per axis).
//! \param istep Step index to read.
//! \return 0 on success.
int gauss_jackson_getstep(gj_instance *gji, double *vi, double *vd0, double *vd1, double *vd2, int32_t istep);

//! \brief Allocate a single Gauss-Jackson step array for a given kernel.
//! \param kern Kernel whose order determines the array size.
//! \return Pointer to the allocated step; free with gauss_jackson_destep().
gj_step *gauss_jackson_step(gj_kernel *kern);

//! \brief Free a single Gauss-Jackson step array.
//! \param kern Kernel used when the step was allocated.
//! \param step Step to free.
void gauss_jackson_destep(gj_kernel *kern, gj_step *step);

//! \brief Precompute the predictor/corrector sums for all steps of a GJ instance.
//! \param gji Integration instance with all initial steps already set.
void gauss_jackson_preset(gj_instance *gji);

//! \brief Advance a Gauss-Jackson integration to the target independent variable value.
//! \param gji    Integration instance (must have been preset).
//! \param target Target value of the independent variable.
void gauss_jackson_extrapolate(gj_instance *gji, double target);

//! \brief Wrap an angle to the range [0, 2π) or [-π, π).
//! \param angle Input angle in radians.
//! \param d2pi  If true, wrap to [0, 2π); if false, wrap to [-π, π). Default true.
//! \return Wrapped angle in radians.
double fixangle(double angle, bool d2pi=true);

//! \brief Four-quadrant arctangent, equivalent to atan2(y, x).
//! \param y Numerator (y component).
//! \param x Denominator (x component).
//! \return Angle in radians in the range (-π, π].
double actan(double y, double x);

//! \brief Round a number to a given precision step.
//! \param number    Value to round.
//! \param precision Quantisation step size.
//! \return Nearest multiple of \p precision.
double fixprecision(double number, double precision);

//! \brief Compute the Discrete Cosine Transform of a float vector.
//! \param datain Input data vector.
//! \param dct    Output DCT coefficients.
//! \param start  First element index to transform; default 0.
//! \param end    One-past-last element index; default 0 means use all.
//! \return 0 on success, negative error code on failure.
int32_t dodct(vector<float> datain, vector<float> &dct, uint16_t start=0, uint16_t end=0);


class LsFit
{
private:
    //! Least Squares Fit Element
    /*! Contains the dependent (x) and independent (y) values for a single element of a ::LsFit.
         * The ::uvector allows both quaternions and rvector to be fit.
         */
    struct fitelement
    {
        // Independent variable
        double x;
        // Double, rvector or quaternion dependent variable
        uvector y{};
    } ;

    //! Least Squares Fit Structure
    /*! Contains the data for a second order least squares fit of N elements that are type
         * ::rvector or ::quaternion.
         */
    // Number of elements in fit
    uint16_t element_cnt;
    // Number of axes (double, rvector, quaternion)
    uint16_t depth;
    // Order of fit
    uint32_t order;
    // base level subtracted from independent variable before fitting
    double basex;
    // Of size element_cnt
    deque<fitelement> var;

    vector< vector<double> > parms;

    void fit();

public:
    double meanx;
    uvector meany{};
    double stdevx;
    uvector stdevy{};
    // Minimum reasonable step in dependent variable
    double resolution;

    // constructors
//    LsFit();
//    LsFit(uint16_t element_cnt);
//    LsFit(uint16_t element_cnt, uint16_t ord);
    //! \brief Construct a least-squares fitter with a given window size and polynomial order.
    //! \param cnt Window size (number of data points to retain); default 10.
    //! \param ord Polynomial order; default 2.
    LsFit(uint16_t cnt=10, uint16_t ord=2);

    //! \brief Re-initialise the fitter, discarding all stored data.
    //! \param cnt New window size; default 10.
    //! \param ord New polynomial order; default 2.
    void initialize(uint16_t cnt=10, uint16_t ord=2);
    //! \brief Add a scalar (double) data point to the fit.
    //! \param x Independent variable.
    //! \param y Dependent scalar value.
    void update(double x, double y);
    //! \brief Add an rvector data point to the fit.
    void update(double x, rvector y);
    //! \brief Add a gvector data point to the fit.
    void update(double x, gvector y);
    //! \brief Add a quaternion data point to the fit.
    void update(double x, quaternion y);
    //! \brief Add a pre-built fitelement to the fit.
    void update(fitelement cfit, uint16_t dep);

    //! \brief Return the independent variable of the most recently added point.
    double lastx();
    //! \brief Return the independent variable of the oldest retained point.
    double firstx();
    //! \brief Return the scalar dependent value of the most recently added point.
    double lasty();
    //! \brief Return the rvector dependent value of the most recently added point.
    rvector lastrvector();
    //! \brief Return the gvector dependent value of the most recently added point.
    gvector lastgvector();
    //! \brief Return the quaternion dependent value of the most recently added point.
    quaternion lastquaternion();
    //! \brief Return the number of data points currently stored in the window.
    size_t size();

    //! \brief Evaluate the fitted scalar polynomial at \p x.
    //! \param x Independent variable.
    //! \return Fitted scalar value.
    double eval(double x);
    //! \brief Evaluate the fitted rvector polynomial at \p x.
    rvector evalrvector(double x);
    //! \brief Evaluate the fitted gvector polynomial at \p x.
    gvector evalgvector(double x);
    //! \brief Evaluate the fitted quaternion polynomial at \p x.
    quaternion evalquaternion(double x);

    //! \brief First derivative of the fitted scalar polynomial at \p x.
    double slope(double x);
    //! \brief First derivative of the fitted rvector polynomial at \p x.
    rvector slopervector(double x);
    //! \brief First derivative of the fitted gvector polynomial at \p x.
    gvector slopegvector(double x);
    //! \brief First derivative of the fitted quaternion polynomial at \p x.
    quaternion slopequaternion(double x);

    //! \brief Second derivative of the fitted scalar polynomial at \p x.
    double accel(double x);
    //! \brief Second derivative of the fitted rvector polynomial at \p x.
    rvector accelrvector(double x);
    //! \brief Second derivative of the fitted gvector polynomial at \p x.
    gvector accelgvector(double x);
    //! \brief Second derivative of the fitted quaternion polynomial at \p x.
    quaternion accelquaternion(double x);

    //! \brief Third derivative of the fitted scalar polynomial at \p x.
    double jerk(double x);
    //! \brief Third derivative of the fitted rvector polynomial at \p x.
    rvector jerkrvector(double x);
    //! \brief Third derivative of the fitted gvector polynomial at \p x.
    gvector jerkgvector(double x);
    //! \brief Third derivative of the fitted quaternion polynomial at \p x.
    quaternion jerkquaternion(double x);

    //! \brief Return the current polynomial coefficient matrix (axis × order).
    vector < vector <double> > getparms();
    //! \brief Return the base independent-variable offset used internally to improve numerical stability.
    double getbasex();
    //! \brief Return true if enough data points have been collected to compute a valid fit.
    bool is_ready();
};


//! @}

#endif
