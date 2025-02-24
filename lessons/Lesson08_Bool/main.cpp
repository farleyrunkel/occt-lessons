
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepTools_ReShape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Pnt.hxx>
#include <TopoDS.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <AIS_Shape.hxx>

#include "Viewer.h"

// make Menger sponge

float boxSize = 100.0f;
Viewer vout(150,150, 700, 700);

TopoDS_Compound modifySingleBoxToMengerSponge(const TopoDS_Solid& theBox) {
    
    TopoDS_Compound aCompound;
    TopoDS_Builder aBuilder;
    aBuilder.MakeCompound(aCompound);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (int(i == 1) + int(j == 1) + int(k == 1) > 1) {
                    continue;
                }

                // get the size of the box using the bounding box
                Bnd_Box aBndBox;
                BRepBndLib::Add(theBox, aBndBox);
                Standard_Real aXmin, aYmin, aZmin, aXmax, aYmax, aZmax;
                aBndBox.Get(aXmin, aYmin, aZmin, aXmax, aYmax, aZmax);

                float aSize = (aXmax - aXmin) / 3.0;

                gp_Trsf aScaleTrsf;
                aScaleTrsf.SetScale(gp_Pnt(aXmin, aYmin, aZmin), 1.0 / 3.0);
                gp_Trsf aTransTrsf;
                aTransTrsf.SetTranslation(gp_Vec(i * aSize, j * aSize, k * aSize));

                BRepBuilderAPI_Transform aTransform(theBox, aTransTrsf * aScaleTrsf);
                TopoDS_Shape scaledBox = aTransform.Shape();

                // vout << scaledBox;
                aBuilder.Add(aCompound, scaledBox);
            }
        }
    }
 
    return aCompound;
}

TopoDS_Compound CreateMengerSponge(const TopoDS_Compound& theCompound) {
    TopoDS_Compound aCompound;
    TopoDS_Builder aBuilder;
    aBuilder.MakeCompound(aCompound);
    for (TopExp_Explorer aIter(theCompound, TopAbs_SOLID); aIter.More(); aIter.Next()) {

        auto aModifiedShape = modifySingleBoxToMengerSponge(TopoDS::Solid(aIter.Value()));      

        aBuilder.Add(aCompound, aModifiedShape);
    }

    return aCompound;
}


int main() {

 // Create a box
    TopoDS_Solid aBox = BRepPrimAPI_MakeBox(100, 100, 100).Solid();

    TopoDS_Compound aCompound;
    TopoDS_Builder aBuilder;
    aBuilder.MakeCompound(aCompound);
    aBuilder.Add(aCompound, aBox);

    TopoDS_Compound aNewCompound  = CreateMengerSponge(aCompound);
    TopoDS_Compound aNewCompound1 = CreateMengerSponge(aNewCompound);
    TopoDS_Compound aNewCompound2 = CreateMengerSponge(aNewCompound1);

    vout.Display(new AIS_Shape(aNewCompound2));

    vout.StartMessageLoop();
    return 0;
}
