//-----------------------------------------------------------------------------
// Creation date: 13 March 2019
// Author:        Sergey Slyadnev
//-----------------------------------------------------------------------------
// Copyright (c) 2019, Sergey Slyadnev
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of Sergey Slyadnev nor the
//      names of all contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#ifndef OcafEx_BuildOBB_h
#define OcafEx_BuildOBB_h

// asiAlgo includes
#include <OcafEx.h>

// OCCT includes
#include <gp_Ax3.hxx>
#include <Poly_Triangulation.hxx>
#include <TopoDS_Solid.hxx>

//-----------------------------------------------------------------------------

//! Oriented Bounding Box.
struct OcafEx_OBB
{
  gp_Ax3 Placement;      //!< Placement of the local axes of the OBB.
  gp_Pnt LocalCornerMin; //!< Min corner.
  gp_Pnt LocalCornerMax; //!< Max corner.

  void Dump() const
  {
    // Access data
    const gp_Pnt& pos = this->Placement.Location();
    const gp_Dir& OZ  = this->Placement.Direction();
    const gp_Dir& OX  = this->Placement.XDirection();
    //
    const gp_Pnt& MinCorner = this->LocalCornerMin;
    const gp_Pnt& MaxCorner = this->LocalCornerMax;

    // Dump
    std::cout << "Pos       = (" << pos.X()       << ", " << pos.Y()       << ", " << pos.Z()       << ")" << std::endl;
    std::cout << "OZ        = (" << OZ.X()        << ", " << OZ.Y()        << ", " << OZ.Z()        << ")" << std::endl;
    std::cout << "OX        = (" << OX.X()        << ", " << OX.Y()        << ", " << OX.Z()        << ")" << std::endl;
    std::cout << "MinCorner = (" << MinCorner.X() << ", " << MinCorner.Y() << ", " << MinCorner.Z() << ")" << std::endl;
    std::cout << "MaxCorner = (" << MaxCorner.X() << ", " << MaxCorner.Y() << ", " << MaxCorner.Z() << ")" << std::endl;
  }
};

//-----------------------------------------------------------------------------

//! Utility to build Oriented Bounding Box on mesh by finding eigen vectors
//! of a covariance matrix.
class OcafEx_BuildOBB
{
public:

  //! Constructor.
  //! \param[in] mesh triangulation to build OBB for.
  OcafExLib_EXPORT
    OcafEx_BuildOBB(const Handle(Poly_Triangulation)& mesh);

public:

  //! Builds OBB.
  //! \return true in case of success, false -- otherwise.
  OcafExLib_EXPORT bool
    Perform();

  //! \return computed OBB.
  OcafExLib_EXPORT const OcafEx_OBB&
    GetResult() const;

  //! \return resulting transformation matrix which defines the placement
  //!         of the oriented box.
  OcafExLib_EXPORT gp_Trsf
    GetResultTrsf() const;

  //! \return resulting OBB as a B-Rep box.
  OcafExLib_EXPORT TopoDS_Solid
    GetResultBox() const;

protected:

  //! Calculates local axes by covariance analysis on mesh.
  //! \param[out] xAxis      X axis.
  //! \param[out] yAxis      Y axis.
  //! \param[out] zAxis      Z axis.
  //! \param[out] meanVertex central vertex.
  void calculateByCovariance(gp_Ax1& xAxis,
                             gp_Ax1& yAxis,
                             gp_Ax1& zAxis,
                             gp_XYZ& meanVertex) const;

protected:

  Handle(Poly_Triangulation) m_input;  //!< Input triangulation.
  OcafEx_OBB                 m_result; //!< Result.

};

#endif
