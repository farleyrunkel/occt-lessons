#include <TDocStd_Document.hxx>
#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TNaming_Builder.hxx>
#include <TNaming_NamedShape.hxx>
#include <TNaming_Selector.hxx>
#include <TNaming_Tool.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <Standard_GUID.hxx>
#include <iostream>
#include <TDocStd_Application.hxx>
#include <XmlDrivers.hxx>

int main()
{
    Handle(TDocStd_Application)
        app = new TDocStd_Application;
    //
    XmlDrivers::DefineFormat(app);

    Handle(TDocStd_Document) doc;
    app->NewDocument("XmlOcaf", doc);
    //
    if (doc.IsNull()) {
        std::cout << "Error: cannot create an OCAF document." << std::endl;
        return 1;
    }

    /* 0
       |
       o-> 1 (0:1) // main label
     */

    TDF_Label root = doc->Main();

    // 2. 创建立方体（Box）
    TopoDS_Shape box = BRepPrimAPI_MakeBox(50.0, 50.0, 50.0).Shape(); // 50x50x50 的立方体
    TDF_Label boxLabel = root.NewChild(); // 为立方体创建标签
    TNaming_Builder boxBuilder(boxLabel);
    boxBuilder.Generated(box); // 标记为 PRIMITIVE
    TColStd_ListOfInteger intergers;
    TDF_Tool::TagList(boxLabel, intergers);

    // 3. 创建圆柱体（Cylinder），位置在立方体内
    TopoDS_Shape cylinder = BRepPrimAPI_MakeCylinder(10.0, 30.0).Shape(); // 半径 10，高 30 的圆柱体
    gp_Trsf trsf;
    trsf.SetTranslation(gp_Vec(25.0, 25.0, 10.0)); // 移动到立方体中心上方
    cylinder.Move(trsf);
    TDF_Label cylinderLabel = root.NewChild(); // 为圆柱体创建标签
    TNaming_Builder cylinderBuilder(cylinderLabel);
    cylinderBuilder.Generated(cylinder); // 标记为 PRIMITIVE

    // 4. 执行布尔切割（Cut）
    BRepAlgoAPI_Cut cutOp(box, cylinder);
    TopoDS_Shape result = cutOp.Shape(); // 切割结果
    TDF_Label resultLabel = root.NewChild(); // 为结果创建标签
    TNaming_Builder resultBuilder(resultLabel);
    resultBuilder.Modify(box, result); // 标记为 MODIFY，旧形状为立方体，新形状为切割结果

    // 5. 注册结果的子形状（所有面）
    TopExp_Explorer faceExplorer(result, TopAbs_FACE);
    while (faceExplorer.More()) {
        TopoDS_Face face = TopoDS::Face(faceExplorer.Current());
        TDF_Label faceLabel = resultLabel.NewChild();
        TNaming_Builder faceBuilder(faceLabel);
        faceBuilder.Generated(face); // 标记为 GENERATED（新生成的面）
        faceExplorer.Next();
    }

    // 6. 选择切割结果中的一个面（例如第一个面）
    TDF_Label selectedLabel = root.NewChild(); // 为选定面创建新标签
    TNaming_Selector selector(selectedLabel);
    TopExp_Explorer firstFaceExplorer(result, TopAbs_FACE);
    if (firstFaceExplorer.More()) {
        TopoDS_Face selectedFace = TopoDS::Face(firstFaceExplorer.Current());
        selector.Select(selectedFace, result); // 选择第一个面，上下文为结果形状
    }

    // 7. 假设修改立方体位置，重新计算选定面
    gp_Trsf newTrsf;
    newTrsf.SetTranslation(gp_Vec(10.0, 0.0, 0.0)); // 移动立方体 10 单位沿 X 轴
    TopoDS_Shape modifiedBox = box.Moved(newTrsf);
    TNaming_Builder modifiedBoxBuilder(boxLabel);
    modifiedBoxBuilder.Modify(box, modifiedBox); // 更新立方体的命名形状

    BRepAlgoAPI_Cut newCutOp(modifiedBox, cylinder);
    TopoDS_Shape newResult = newCutOp.Shape();
    TNaming_Builder newResultBuilder(resultLabel);
    newResultBuilder.Modify(result, newResult); // 更新结果的命名形状

    PCDM_StoreStatus sstatus = app->SaveAs(doc, "E:/Documents/occt-lessons/lessons/Lesson11_TNaming/result.xml");
    //
    if (sstatus != PCDM_SS_OK) {
        app->Close(doc);

        std::cout << "Cannot write OCAF document." << std::endl;
        return 1;
    }

    app->Close(doc);

    std::cout << "OCAF document closed successfully." << std::endl;
    return 0;
}
