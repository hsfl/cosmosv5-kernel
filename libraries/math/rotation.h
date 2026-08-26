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
 * \file rotation.h
 * \brief Quaternion and rotation matrix operations.
 * \ingroup cosmos_kernel_math
 * \defgroup cosmos_rotation Rotation
 *
 * Implements unit quaternion and Direction Cosine Matrix (DCM) types for
 * representing 3-D rotations. Provides conversion between the two
 * representations and composition (multiplication) of rotations. Required by
 * attitude determination and coordinate frame transformation code.
 */

#ifndef _MATH_ROTATE_H
#define _MATH_ROTATE_H

#include "support/configCosmos.h"
#include "vector.h"
#include "matrix.h"
// #include "quaternion.h"

//! \brief Convert a unit quaternion to a column-major (complex) Direction Cosine Matrix.
//! \param q Unit quaternion representing the rotation.
//! \return Corresponding 3×3 DCM in cmatrix form.
cmatrix cm_quaternion2dcm(quaternion q);

//! \brief Convert a column-major DCM to a unit quaternion.
//! \param dcm 3×3 direction cosine matrix in cmatrix form.
//! \return Corresponding unit quaternion.
quaternion q_dcm2quaternion_cm(cmatrix dcm);

//! \brief Convert a row-major DCM to a unit quaternion.
//! \param m 3×3 direction cosine matrix in rmatrix form.
//! \return Corresponding unit quaternion.
quaternion q_dcm2quaternion_rm(rmatrix m);

//! \brief Convert a unit quaternion to a row-major Direction Cosine Matrix.
//! \param q Unit quaternion representing the rotation.
//! \return Corresponding 3×3 DCM in rmatrix form.
rmatrix rm_quaternion2dcm(quaternion q);

//! Orthonormal basis
/*! Used to crease frame basis such as inertial, body and sensor frames
 * The DCM class uses this struct as default
*/
class basisOrthonormal {
public:

    cvector i; // = {1,0,0}
    cvector j; // = {0,1,0}
    cvector k; // = {0,0,1}

    //! \brief Orthonormalise all three basis vectors in place.
    void normalize();
};

// TODO: rename to DirectionCosineMatrix
class DCM {
private:

public:
    //! \brief Compute the DCM that transforms vectors from frame 1 to frame 2.
    //! \param base2 Target orthonormal basis (frame 2).
    //! \param base1 Source orthonormal basis (frame 1).
    //! \return 3×3 cmatrix rotating frame 1 → frame 2.
    cmatrix base2_from_base1(basisOrthonormal base2,basisOrthonormal base1);

    //! \brief Compute the DCM that transforms vectors from frame 2 to frame 1.
    //! \param base1 Target orthonormal basis (frame 1).
    //! \param base2 Source orthonormal basis (frame 2).
    //! \return 3×3 cmatrix rotating frame 2 → frame 1.
    cmatrix base1_from_base2(basisOrthonormal base1, basisOrthonormal base2);

    //! \brief Compute the dot product of two cvectors.
    //! \param a First vector.
    //! \param b Second vector.
    //! \return Scalar dot product.
    double dotProduct(cvector a, cvector b);

    //! \brief Return the transpose of a cmatrix.
    //! \param a Input 3×3 matrix.
    //! \return Transposed matrix.
    cmatrix transposeMatrix(cmatrix a);
};

#endif
