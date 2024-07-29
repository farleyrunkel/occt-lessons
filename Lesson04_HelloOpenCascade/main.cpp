#include "Viewer.h"

#include <BRepTools.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>

int main(int argc, char** argv)
{
  Viewer vout(50, 50, 500, 500);
  
   vout << BRepPrimAPI_MakeBox(10, 10, 20);

   vout << BRepPrimAPI_MakeBox(20, 30, 10);

   vout << BRepPrimAPI_MakeSphere(10.0);

   vout << BRepPrimAPI_MakeCylinder(5.0, 10.0);

   vout << BRepPrimAPI_MakeCone(5.0, 2.0, 10.0);

   vout << BRepPrimAPI_MakeTorus(3.0, 1.0);

   vout << BRepPrimAPI_MakeWedge(10.0, 5.0, 2.0, 3.0);

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
