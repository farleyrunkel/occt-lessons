#include "Viewer.h"

#include <BRepTools.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRep_Builder.hxx>
#include <vector>
#include <Geom2d_Line.hxx>
#include <TColGeom_SequenceOfCurve.hxx>
#include <Geom_Line.hxx>
#include <gp_Lin2d.hxx>
#include <Standard_Handle.hxx>
#include <gp_Lin.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Cylinder.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <iostream>
#include <BRepPrimAPI_MakePrism.hxx> 
#include <TopExp_Explorer.hxx>
#include <set>
#include <TopoDS_TVertex.hxx>

// make a box shape from eight points to edges, then to faces, then to solid
using namespace std;


int main(int argc, char** argv)
{
    Viewer vout(50, 50, 500, 500);

    TColgp_Array1OfPnt nodes(1, 8);

     nodes(1) = gp_Pnt(0, 0, 0);
     nodes(2) = gp_Pnt(1, 0, 0);
     nodes(3) = gp_Pnt(1, 1, 0);
     nodes(4) = gp_Pnt(0, 1, 0);
     nodes(5) = gp_Pnt(0, 0, 1);
     nodes(6) = gp_Pnt(1, 0, 1);
     nodes(7) = gp_Pnt(1, 1, 1);
     nodes(8) = gp_Pnt(0, 1, 1);

  std::vector<TopoDS_Vertex> vertices(nodes.Size() + 1, TopoDS_Vertex());
  for ( int i = 1; i <= 8; ++i ) {
      BRep_Builder builder;
      builder.MakeVertex(vertices[i]);
      builder.UpdateVertex(vertices[i], nodes[i], 0.001);
  }
  for (auto a : vertices) { vout << a;}

  int nodesOfLines[12][2] = {
	{1, 2}, {1, 4}, {1, 5},
	{3, 2}, {3, 4}, {3, 7},
	{6, 2}, {6, 5}, {6, 7},
	{8, 4}, {8, 5}, {8, 7}
  };


  std::vector<TopoDS_Edge> edges(12, TopoDS_Edge());

  for ( int i = 0; i < 12; ++i ) {
	  BRepBuilderAPI_MakeEdge edgeBuilder(vertices[nodesOfLines[i][0]], vertices[nodesOfLines[i][1]]);
	  edges[i] = edgeBuilder.Edge();
  }

 // for (auto a : edges) { vout << a; }

  TopoDS_Wire wire;

  BRepBuilderAPI_MakeWire wireBuilder;
  for ( int i = 0; i < 12; ++i ) {
	  wireBuilder.Add(edges[i]);
  }
  wire = wireBuilder.Wire();

  // vout << wire;

  TopoDS_Face face;
  BRepBuilderAPI_MakeFace faceBuilder(wire);
  face = faceBuilder.Face();
  vout << face;

  {
    // make box
      BRepPrimAPI_MakeBox boxMaker(1, 1, 1);
      TopoDS_Shape box = boxMaker.Shape();

     // get all edge
      TopExp_Explorer edgeExplorer(box, TopAbs_EDGE);
      cout << "edge.Orientation() ";
      int i = 0;
      std::set<Handle(TopoDS_TShape)> aSet;
	  for (; edgeExplorer.More(); edgeExplorer.Next()) {
		  TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());
		  vout << edge;
          cout << edge.Orientation() << " ";

          aSet.insert(edge.TShape());
          i++;
	  }
      cout << "total edge: " << i << "aSet.size" << aSet.size() <<  endl;
	  // get all face

      i = 0;
	  TopExp_Explorer faceExplorer(box, TopAbs_FACE);
	  for (; faceExplorer.More(); faceExplorer.Next()) {
		  TopoDS_Face face = TopoDS::Face(faceExplorer.Current());
		  vout << face;
          cout << face.Orientation() << " ";

          i++;
	  }
      cout << "total face: " << i << endl;
      // get all face
      i = 0;
	  // get all vertex
	  TopExp_Explorer vertexExplorer(box, TopAbs_VERTEX);
      std::set<Handle(TopoDS_TShape)> bSet;

	  for (; vertexExplorer.More(); vertexExplorer.Next()) {
		  TopoDS_Vertex vertex = TopoDS::Vertex(vertexExplorer.Current());

          gp_Pnt point = BRep_Tool::Pnt(vertex);
          gp_Trsf theTrsf = gp_Trsf();
          theTrsf.SetTranslation(gp_Vec(0, 2, 1));
          point.Transform(theTrsf);
          BRep_Builder aBuilder;
          aBuilder.UpdateVertex(vertex, point, Precision::Confusion());

          auto a = BRep_Tool::Pnt(vertex);
          auto b = BRep_Tool::Pnt(TopoDS::Vertex(vertexExplorer.Current()));

          break;
	  }

      vout << box;
      cout << "box" << endl;
      cout << box.Orientation() << endl;

      cout << "total vertex: " << i << " bSet.size " << bSet.size() << endl;
  }

  if ( argc > 1 )
  {
    BRep_Builder bb;
    TopoDS_Shape fromFile;
    //
    if ( !BRepTools::Read(fromFile, argv[1], bb) )
    {
      std::cout << "Failed to read BREP shape from file " << argv[1] << std::endl;
      return 1;
    }

    vout << fromFile;
  }

  vout.StartMessageLoop();

  return 0;
}
