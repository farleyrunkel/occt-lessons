// Copyright (c) 2021 OPEN CASCADE SAS
//
// This file is part of the examples of the Open CASCADE Technology software library.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

#include "OcctQtViewer.h"

#include <Standard_WarningsDisable.hxx>
#include <QApplication>
#include <QSurfaceFormat>

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <Standard_WarningsRestore.hxx>

#include "mainwindow.h"


int main (int theNbArgs, char** theArgVec)
{
  QApplication aQApp (theNbArgs, theArgVec);

  QCoreApplication::setApplicationName ("OCCT Qt Viewer sample");
  QCoreApplication::setOrganizationName ("OpenCASCADE");
  QCoreApplication::setApplicationVersion (OCC_VERSION_STRING_EXT);

#ifdef __APPLE__
  // suppress Qt warning "QCocoaGLContext: Falling back to unshared context"
  bool isCoreProfile = true;
  QSurfaceFormat aGlFormat;
  aGlFormat.setDepthBufferSize   (24);
  aGlFormat.setStencilBufferSize (8);
  if (isCoreProfile) { aGlFormat.setVersion (4, 5); }
  aGlFormat.setProfile (isCoreProfile ? QSurfaceFormat::CoreProfile : QSurfaceFormat::CompatibilityProfile);
  QSurfaceFormat::setDefaultFormat (aGlFormat);
#endif

  MyMainWindow aMainWindow;
  aMainWindow.resize (aMainWindow.sizeHint());
  aMainWindow.show();
  return aQApp.exec();
}
