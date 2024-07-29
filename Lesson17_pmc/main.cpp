//-----------------------------------------------------------------------------
// Copyright (c) 2020-present, Quaoar Studio
// All rights reserved.
//-----------------------------------------------------------------------------
// Local includes
#include "ClassifyPt.h"
#include "Viewer.h"

// OpenCascade includes
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepClass3d_SolidClassifier.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepTools.hxx>
#include <OSD_Timer.hxx>
#include <Poly_CoherentTriangulation.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_ConnectedInteractive.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pnt.hxx>
#include <NCollection_Vec3.hxx>
// Standard includes
#include <unordered_map>

#define TIMER_NEW                                         \
  OSD_Timer __aux_debug_Timer;                            \
  Standard_Real __aux_debug_Seconds, __aux_debug_CPUTime; \
  Standard_Integer __aux_debug_Minutes, __aux_debug_Hours;

#define TIMER_RESET                                \
  __aux_debug_Seconds = __aux_debug_CPUTime = 0.0; \
  __aux_debug_Minutes = __aux_debug_Hours = 0;     \
  __aux_debug_Timer.Reset();

#define TIMER_GO \
  __aux_debug_Timer.Start();

#define TIMER_FINISH        \
  __aux_debug_Timer.Stop(); \
  __aux_debug_Timer.Show(__aux_debug_Seconds, __aux_debug_Minutes, __aux_debug_Hours, __aux_debug_CPUTime);

#define TIMER_COUT_RESULT_MSG(Msg)                                               \
  {                                                                              \
    std::cout << "\n=============================================" << std::endl; \
    TCollection_AsciiString ascii_msg(Msg);                                      \
    if (!ascii_msg.IsEmpty())                                                    \
    {                                                                            \
      std::cout << Msg << std::endl;                                             \
      std::cout << "---------------------------------------------" << std::endl; \
    }                                                                            \
    std::cout << "Seconds:  " << __aux_debug_Seconds << std::endl;               \
    std::cout << "Minutes:  " << __aux_debug_Minutes << std::endl;               \
    std::cout << "Hours:    " << __aux_debug_Hours << std::endl;                 \
    std::cout << "CPU time: " << __aux_debug_CPUTime << std::endl;               \
    std::cout << "=============================================\n"               \
              << std::endl;                                                      \
  }

int main(int argc, char **argv)
{
  Viewer vout(50, 50, 1080, 1080);

  /* =======================
   *  Get the working model.
   * ======================= */

  // Read from file.
  BRep_Builder bb;
  TopoDS_Shape shape;
  //
  if (argc > 1)
  {
    if (!BRepTools::Read(shape, argv[1], bb))
    {
      std::cout << "Failed to read BREP shape from file '" << argv[1] << "'." << std::endl;
      return 1;
    }
    // vout << shape;
  }
  else
  {
    std::cout << "Please, pass filename (BREP) as an argument." << std::endl;
    return 1;
  }

  /* ==============================
   *  Prepare visualization meshes.
   * ============================== */

  // Prepare data structure for all triangles with back refs.
  Handle(Poly_CoherentTriangulation)
      tris = new Poly_CoherentTriangulation;

  const double linDefl = 0.01;
  BRepMesh_IncrementalMesh meshGen(shape, linDefl);

  // Add all triangulations from faces to the common collection.
  for (TopExp_Explorer fexp(shape, TopAbs_FACE); fexp.More(); fexp.Next())
  {
    const TopoDS_Face &face = TopoDS::Face(fexp.Current());
    
    Handle(AIS_Shape) aisFace = new AIS_Shape(face);
    Handle(AIS_InteractiveContext) context = vout.GetContext();

    aisFace->SetDisplayMode(AIS_Shaded);
    // // 设置随机种子
    // std::srand(time(nullptr));

    aisFace->SetColor(static_cast<Quantity_NameOfColor>(std::rand() % 501 + 1));

    //  设置随机种子

    // hiStyle->SetColor(static_cast<Quantity_NameOfColor>(std::rand() % 201));

    // context->Display(aisFace, Standard_True);
    //  context->UpdateCurrentViewer();
    //   Poly_MeshPurpose was introduced in OpenCascade 7.6 and remained
    //   undocumented.
    TopLoc_Location L;
    const Handle(Poly_Triangulation) &poly = BRep_Tool::Triangulation(face, L);

    std::unordered_map<int, int> nodesMap;
    for (int iNode = 1; iNode <= poly->NbNodes(); ++iNode)
    {
      // Make sure to apply location, e.g., see the effect in /cad/ANC101.brep
      const int n = tris->SetNode(poly->Node(iNode).Transformed(L).XYZ());

      // Local to global node index mapping.
      nodesMap.insert({iNode, n});
    }

    // Add triangles.
    for (int iTri = 1; iTri <= poly->NbTriangles(); ++iTri)
    {
      const Poly_Triangle &tri = poly->Triangle(iTri);

      int iNodes[3];
      tri.Get(iNodes[0], iNodes[1], iNodes[2]);

      // Try disabling this and check how mesh is visualized.
      if (face.Orientation() == TopAbs_REVERSED)
        std::swap(iNodes[1], iNodes[2]);

      tris->AddTriangle(nodesMap[iNodes[0]], nodesMap[iNodes[1]], nodesMap[iNodes[2]]);

      BRepBuilderAPI_MakePolygon PolyMaker(poly->Node(iNodes[0]).Transformed(L).XYZ(),
                                           poly->Node(iNodes[1]).Transformed(L).XYZ(),
                                           poly->Node(iNodes[2]).Transformed(L).XYZ(), Standard_True);

      BRepBuilderAPI_MakeFace faceMaker(PolyMaker.Wire());

      Handle(AIS_Shape) aisFace = new AIS_Shape(faceMaker.Shape());
      Handle(AIS_InteractiveContext) context = vout.GetContext();
      aisFace->SetDisplayMode(AIS_Shaded);
      aisFace->SetColor(static_cast<Quantity_NameOfColor>(std::rand() % 501 + 1));
      context->Display(aisFace, Standard_True);
    }
  }

  // Display the triangulation to be sure it's consistent.
//vout << tris->GetTriangulation();
  
  // /* =========================
  //  *  Sample the bounding box.
  //  * ========================= */

  // // Get the bounding box.
  // Bnd_Box aabb;
  // BRepBndLib::Add(shape, aabb, true); // Use triangulation.

  // // show the bounding box
  // BRepPrimAPI_MakeBox boxMaker(aabb.CornerMin(), aabb.CornerMax());
  // // vout << boxMaker.Wedge().Wire(BRepPrim_Direction::BRepPrim_XMax);
  // // vout << boxMaker.Wedge().Wire(BRepPrim_Direction::BRepPrim_YMax);
  // // vout << boxMaker.Wedge().Wire(BRepPrim_Direction::BRepPrim_ZMax);
  // // vout << boxMaker.Wedge().Wire(BRepPrim_Direction::BRepPrim_XMin);
  // // vout << boxMaker.Wedge().Wire(BRepPrim_Direction::BRepPrim_YMin);
  // // vout << boxMaker.Wedge().Wire(BRepPrim_Direction::BRepPrim_ZMin);

  // Handle(AIS_Shape) aisBox = new AIS_Shape(boxMaker.Shape());
  // Handle(AIS_InteractiveContext) context = vout.GetContext();
  // aisBox->SetDisplayMode(AIS_WireFrame);
  // context->Display(aisBox, Standard_True);

  // // Handle(AIS_Shape) aisBox = new AIS_Shape(boxMaker.Shape());
  // // Handle(AIS_InteractiveContext) context = vout.GetContext();
  // // Handle(AIS_ConnectedInteractive) brepConnected = new AIS_ConnectedInteractive();
  // // brepConnected->Connect(aisBox);
  // // brepConnected->SetDisplayMode( AIS_WireFrame );
  // // context->Display(brepConnected, Standard_False);

  // // BRepBuilderAPI_MakeWire wireMaker;

  // // // 遍历长方体对象中的边缘并添加到线框中
  // // for (TopExp_Explorer explorer(boxMaker.Shape(), TopAbs_EDGE); explorer.More(); explorer.Next()) {
  // //     const TopoDS_Edge& edge = TopoDS::Edge(explorer.Current());
  // //     wireMaker.Add(edge);
  // // }

  // //   // 获取线框
  // //   TopoDS_Wire wire = wireMaker.Wire();

  // //   vout << wire;

  // // Compute the dimensions.
  // const int density = 10;
  // gp_XYZ Pmin = aabb.CornerMin().XYZ();
  // gp_XYZ Pmax = aabb.CornerMax().XYZ();
  // gp_XYZ D = Pmax - Pmin;
  // double dims[3] = {D.X(), D.Y(), D.Z()};
  // const double mind = Min(dims[0], Min(dims[1], dims[2]));
  // const double d = mind / density;
  // const int nslice[3] = {int(Round(dims[0] / d)) + 1,
  //                        int(Round(dims[1] / d)) + 1,
  //                        int(Round(dims[2] / d)) + 1};

  // std::vector<gp_XYZ> gridPts;
  // for (int i = 0; i <= nslice[0]; ++i)
  //   for (int j = 0; j <= nslice[1]; ++j)
  //     for (int k = 0; k <= nslice[2]; ++k)
  //     {
  //       gp_XYZ P = Pmin + gp_XYZ(d * i, 0, 0) + gp_XYZ(0, d * j, 0) + gp_XYZ(0, 0, d * k);
  //       //
  //       gridPts.push_back(P);

  //       // vout << BRepBuilderAPI_MakeVertex(P);
  //     }

  // // /* ====================
  // //  *  PMC by OpenCascade.
  // //  * ==================== */

  // TIMER_NEW
  // TIMER_GO

  // int numInnerByBrep = 0;
  // const double tolBrep = Precision::Confusion();
  // TColStd_PackedMapOfInteger iPtsBrep;

  // BRepClass3d_SolidClassifier classBrep(shape);

  // for (int i = 0; i < int(gridPts.size()); ++i)
  // {
  //   classBrep.Perform(gridPts[i], tolBrep);
  //   //
  //   if (classBrep.State() == TopAbs_IN)
  //   {
  //     iPtsBrep.Add(i);
  //     numInnerByBrep++;
  //     // vout << BRepBuilderAPI_MakeVertex(gridPts[i]);
  //   }
  // }

  // // TIMER_FINISH
  // // TIMER_COUT_RESULT_MSG("OpenCascade PMC")

  // // /* ====================
  // //  *  PMC by ray casting.
  // //  * ==================== */

  // TIMER_RESET
  // TIMER_GO

  // int numInnerByBvh = 0;
  // const double tolMesh = mind / 50;
  // TColStd_PackedMapOfInteger iPtsMesh;

  // ClassifyPt classMesh(tris->GetTriangulation());

  // for (int i = 0; i < int(gridPts.size()); ++i)
  // {
  //   if (classMesh.IsIn(gridPts[i], tolMesh))
  //   {
  //     iPtsMesh.Add(i);
  //     numInnerByBvh++;
  //     // vout << BRepBuilderAPI_MakeVertex(gridPts[i]);
  //   }
  // }

  // TIMER_FINISH
  // TIMER_COUT_RESULT_MSG("BVH-based PMC")

  // std::cout << "Num. tested points:                          " << gridPts.size() << std::endl;
  // std::cout << "Num. inner points with OpenCascade      PMC: " << numInnerByBrep << std::endl;
  // std::cout << "Num. inner points with BVH-based        PMC: " << numInnerByBvh << std::endl;
  // std::cout << "Tolerance for inner points in BVH-based PMC: " << tolMesh << std::endl;

  // TColStd_PackedMapOfInteger diff;
  // diff.Difference(iPtsBrep, iPtsMesh);

  // for (TColStd_PackedMapOfInteger::Iterator it(diff); it.More(); it.Next())
  // {
  //   // vout << BRepBuilderAPI_MakeVertex(gridPts[it.Key()]);
  // }

  vout.StartMessageLoop();
  return 0;
}
