// OpenCascade includes
#include <XmlDrivers.hxx>
#include <TDataStd_Integer.hxx>
#include <TDF_Label.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Document.hxx>

int main()
{
  Handle(TDocStd_Application)
    app = new TDocStd_Application;
  //
  XmlDrivers::DefineFormat(app);

  Handle(TDocStd_Document) doc;
  app->NewDocument("XmlOcaf", doc);
  //
  if ( doc.IsNull() )
  {
    std::cout << "Error: cannot create an OCAF document." << std::endl;
    return 1;
  }

  /* 0
     |
     o-> 1 (0:1) // main label
   */

  TDF_Label mainLab = doc->Main();



  TDataStd_Integer::Set(mainLab, 199);

  PCDM_StoreStatus sstatus = app->SaveAs(doc, "C:/Users/95439/Documents/Visual Studio Code/occt-lessons/Lesson10_OCAF_from-scratch/doc/test.xml");
  //
  if ( sstatus != PCDM_SS_OK )
  {
    app->Close(doc);

    std::cout << "Cannot write OCAF document." << std::endl;
    return 1;
  }

  app->Close(doc);

  std::cout << "OCAF document closed successfully." << std::endl;
  return 0;
}
