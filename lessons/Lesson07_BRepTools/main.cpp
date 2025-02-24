
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepTools_ReShape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Pnt.hxx>
#include <TopoDS.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

#include "Viewer.h"

int main() {
    // Create a box
    TopoDS_Solid box = BRepPrimAPI_MakeBox(100, 100, 100);

    // New vertex to replace with
    gp_Pnt pNew(50, 50, 50);
    TopoDS_Vertex newVertex = BRepBuilderAPI_MakeVertex(pNew);

    // Reshape tool
    BRepTools_ReShape reshaper;
    TopoDS_Vertex oldVertex;

    // Traverse through all vertices of the box
    for (TopExp_Explorer explorer(box, TopAbs_VERTEX); explorer.More(); explorer.Next()) {
         oldVertex = TopoDS::Vertex(explorer.Current());
        gp_Pnt pOld = BRep_Tool::Pnt(oldVertex);

        // Compare with the point you want to replace (e.g., original (0,0,0))
        //gp_Pnt(100, 100, 100) OK
        //gp_Pnt(0, 0, 0) OK
        //gp_Pnt(100, 0, 100) OK
        if (pOld.IsEqual(gp_Pnt(0, 100, 100), Precision::Confusion())) {
            // Replace the vertex
            reshaper.Replace(oldVertex, newVertex);
            break;  // Assuming only one replacement is needed
        }
    }

    // Apply reshaping to the box
    TopoDS_Shape reshapedBox = reshaper.Apply(box);

    BRepTools::Write(reshapedBox, "reshapedBox.brep");
    // Display the modified box
    Viewer vout(50, 50, 500, 500);
    vout << reshapedBox;
    box.DumpJson(std::cout);
    std::cout << std::endl;
    box.TShape()->DumpJson(std::cout);
    vout << newVertex;
    vout << oldVertex;

    vout.StartMessageLoop();

    return 0;
}
