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



#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <BRepTools.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Message.hxx>

int main() {
    // ����һ��������
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100, 100, 100).Shape();

    BRepTools::Dump(box, std::cout);

    // ����һ���ֲ�����ϵ��ԭ����(1, 1, 1)
    gp_Ax2 localCoordinateSystem(gp_Pnt(1, 1, 1), gp_Dir(0, 0, 1)); // Z�᷽��

    // ����һ���任�����������ԭ���ƶ���(1, 1, 1)
    gp_Trsf transform;
    transform.SetTransformation(gp_Ax3(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), localCoordinateSystem);

    // Ӧ�ñ任
    BRepBuilderAPI_Transform transformedBox(box, transform, true);
    TopoDS_Shape resultBox = transformedBox.Shape();
    BRepTools::Dump(resultBox, std::cout);

    // ���������һ������
    // BRepTools::Write(resultBox, "box_in_local_coordinate.brep");

    return 0;
}
