// OpenCascade includes
#include <XmlDrivers.hxx>
#include <TDataStd_Integer.hxx>
#include <TDF_Label.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Document.hxx>

#include <TDataStd_Real.hxx>
#include <TDataStd_Name.hxx>
#include <TFunction_Function.hxx>
#include <TFunction_DriverTable.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TNaming_NamedShape.hxx>

#include "TOcafFunction_BoxDriver.h"

void addBoxToDoc(Handle(TDocStd_Document)myOcafDoc, std::ostream& myResult)
{
    // Open a new command (for undo)
    myOcafDoc->NewCommand();

    // A data structure for our box:
    // the box itself is attached to the BoxLabel label (as his name and his function attribute)
    // its arguments (dimensions: width, length and height; and position: x, y, z)
    // are attached to the child labels of the box:
    // 0:1 Box Label ---> Name --->  Named shape ---> Function
    //     0:1:1 -- Width Label
    //     0:1:2 -- Length Label
    //     0:1:3 -- Height Label
    //     0:1:4 -- X Label
    //     0:1:5 -- Y Label
    //     0:1:6 -- Z Label

    // Create a new label in the data structure for the box
    TDF_Label aLabel = TDF_TagSource::NewChild(myOcafDoc->Main());

    Standard_Real aBoxWidth(30.0), aBoxLength(20.0), aBoxHeight(10.0);
    Standard_Real aBoxX(0.0), aBoxY(0.0), aBoxZ(0.0);
    Standard_CString aBoxName("OcafBox");
    // Create the data structure : Set the dimensions, position and name attributes
    TDataStd_Real::Set(aLabel.FindChild(1), aBoxWidth);
    TDataStd_Real::Set(aLabel.FindChild(2), aBoxLength);
    TDataStd_Real::Set(aLabel.FindChild(3), aBoxHeight);
    TDataStd_Real::Set(aLabel.FindChild(4), aBoxX);
    TDataStd_Real::Set(aLabel.FindChild(5), aBoxY);
    TDataStd_Real::Set(aLabel.FindChild(6), aBoxZ);
    TDataStd_Name::Set(aLabel, aBoxName); // Name

    // Instantiate a TFunction_Function attribute connected to the current box driver
    // and attach it to the data structure as an attribute of the Box Label
    Handle(TFunction_Function) myFunction = TFunction_Function::Set(aLabel, TOcafFunction_BoxDriver::GetID());

    // Initialize and execute the box driver (look at the "Execute()" code)
    Handle(TFunction_Logbook) aLogBook = TFunction_Logbook::Set(aLabel);

    Handle(TFunction_Driver) myBoxDriver;
    // Find the TOcafFunction_BoxDriver in the TFunction_DriverTable using its GUID
    if (!TFunction_DriverTable::Get()->FindDriver(TOcafFunction_BoxDriver::GetID(), myBoxDriver)) {
        myResult << "Ocaf Box driver not found" << std::endl;
    }

    myBoxDriver->Init(aLabel);
    if (myBoxDriver->Execute(aLogBook)) {
        myResult << "Create Box function execute failed" << std::endl;
    }

    // Get the TPrsStd_AISPresentation of the new box TNaming_NamedShape
    Handle(TPrsStd_AISPresentation) anAisPresentation = TPrsStd_AISPresentation::Set(aLabel, TNaming_NamedShape::GetID());
    // Display it
    anAisPresentation->Display(1);
    // Attach an integer attribute to aLabel to memorize it's displayed
    TDataStd_Integer::Set(aLabel, 1);

    // Close the command (for undo)
    myOcafDoc->CommitCommand();

    myResult << "Created a box with name: " << aBoxName << std::endl;
    myResult << "base coord X: " << aBoxX << " Y: " << aBoxY << " Z: " << aBoxZ << std::endl;
    myResult << "width: " << aBoxWidth << " length: " << aBoxLength << " height: " << aBoxHeight << std::endl;
}

void doUndo(Handle(TDocStd_Document)myOcafDoc, std::ostream& myResult)
{
    if (myOcafDoc->Undo()) {
        myOcafDoc->CommitCommand();
        //myContext->UpdateCurrentViewer();
        myResult << "Undo was done successfully" << std::endl;
    }
    else {
        myResult << "Nothing to undo" << std::endl;
    }
}

int main()
{
    Handle(TDocStd_Application)
        app = new TDocStd_Application;
    //
    XmlDrivers::DefineFormat(app);
    // Instantiate a TOcafFunction_BoxDriver and add it to the TFunction_DriverTable
    TFunction_DriverTable::Get()->AddDriver(TOcafFunction_BoxDriver::GetID(),
                                            new TOcafFunction_BoxDriver());

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

    TDF_Label mainLab = doc->Main();

    // Create a new label in the data structure for the box
    TDF_Label aLabel = TDF_TagSource::NewChild(doc->Main());

    TDataStd_Integer::Set(mainLab, 199);

    addBoxToDoc(doc, std::cout);
    //doUndo(doc, std::cout);

    PCDM_StoreStatus sstatus = app->SaveAs(doc, "E:/Documents/occt-lessons/lessons/Lesson12_Undo/result.xml");
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
