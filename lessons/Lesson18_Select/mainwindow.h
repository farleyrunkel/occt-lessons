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
#include <QMouseEvent>

#include <Standard_WarningsRestore.hxx>
#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>
#include <Standard_Version.hxx>

//! Main application window.
class MyMainWindow : public QMainWindow
{
  OcctQtViewer* myViewer;
public:
  MyMainWindow() : myViewer (nullptr)
  {
    {
      // menu bar with Quit item
      QMenuBar* aMenuBar = new QMenuBar();
      QMenu* aMenuWindow = aMenuBar->addMenu ("&File");
      {
        QAction* anActionSplit = new QAction(aMenuWindow);
        anActionSplit->setText("Split Views");
        aMenuWindow->addAction(anActionSplit);
        connect(anActionSplit, &QAction::triggered, [this]()
        {
          if (!myViewer->View()->Subviews().IsEmpty())
          {
            // remove subviews
            myViewer->View()->View()->SetSubviewComposer(false);
            NCollection_Sequence<Handle(V3d_View)> aSubviews = myViewer->View()->Subviews();
            for (const Handle(V3d_View)& aSubviewIter : aSubviews)
            {
              aSubviewIter->Remove();
            }
            myViewer->OnSubviewChanged(myViewer->Context(), nullptr, myViewer->View());
          }
          else
          {
            // create two subviews splitting window horizontally
            myViewer->View()->View()->SetSubviewComposer(true);

            Handle(V3d_View) aSubView1 = new V3d_View(myViewer->Viewer());
            aSubView1->SetImmediateUpdate(false);
            aSubView1->SetWindow(myViewer->View(), Graphic3d_Vec2d(0.5, 1.0),
                                 Aspect_TOTP_LEFT_UPPER, Graphic3d_Vec2d(0.0, 0.0));

            Handle(V3d_View) aSubView2 = new V3d_View(myViewer->Viewer());
            aSubView2->SetImmediateUpdate(false);
            aSubView2->SetWindow(myViewer->View(), Graphic3d_Vec2d(0.5, 1.0),
                                 Aspect_TOTP_LEFT_UPPER, Graphic3d_Vec2d(0.5, 0.0));

            myViewer->OnSubviewChanged(myViewer->Context(), nullptr, aSubView1);
          }
          myViewer->View()->Invalidate();
          myViewer->update();
        });
      }
      {
        QAction* anActionQuit = new QAction (aMenuWindow);
        anActionQuit->setText ("Quit");
        aMenuWindow->addAction (anActionQuit);
        connect (anActionQuit, &QAction::triggered, [this]()
        {
          close();
        });
      }
      setMenuBar (aMenuBar);
    }
    {
      // 3D Viewer and some controls on top of it
      myViewer = new OcctQtViewer();
      QVBoxLayout* aLayout = new QVBoxLayout (myViewer);
      aLayout->setDirection (QBoxLayout::BottomToTop);
      aLayout->setAlignment (Qt::AlignBottom);

      setCentralWidget (myViewer);
    }
  }

  void mouseReleaseEvent(QMouseEvent* theEvent)
  {
    QMainWindow::mouseReleaseEvent(theEvent);
    
    myViewer->update();

    std::cout << "click HasDetected " << myViewer->Context()->HasDetected() << std::endl;
    std::cout << "click NbSelected  " << myViewer->Context()->NbSelected() << std::endl;

    for (myViewer->Context()->InitSelected(); myViewer->Context()->MoreSelected(); myViewer->Context()->NextSelected())
    {
        TopoDS_Shape anShape = myViewer->Context()->SelectedShape();
        if (anShape.IsNull())
        {
            QMessageBox::warning(this, "Warning", "Please select a shape first!");
            return;
        }
        gp_Trsf agp_Trsf = gp_Trsf();
        agp_Trsf.SetTranslation(gp_Vec(111, 111, 111));
        anShape.Move(TopLoc_Location(agp_Trsf));

        auto newAisShaple = new AIS_Shape(anShape);
        myViewer->Context()->Display(newAisShaple, Standard_True);
    };
  }

};
