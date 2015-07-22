/*=========================================================================
  The Software is copyright (c) Commonwealth Scientific and Industrial Research Organisation (CSIRO)
  ABN 41 687 119 230.
  All rights reserved.

  Licensed under the CSIRO BSD 3-Clause License
  You may not use this file except in compliance with the License.
  You may obtain a copy of the License in the file LICENSE.md or at

  https://stash.csiro.au/projects/SMILI/repos/smili/browse/license.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
=========================================================================*/
#include "milxQtSpineImage.h"

#include <vtkSphereSource.h>
#include <vtkMath.h>
#include <vtkFloatArray.h>
#include <vtkPolyDataNormals.h>
#include <vtkAppendPolyData.h>
#include <vtkSplineRepresentation.h>

milxQtSpineImage::milxQtSpineImage(QWidget *theParent) : milxQtImage(theParent)
{
    milxQtWindow::prefix = "Spine Img: ";

    createActions();

    createConnections();
}

milxQtSpineImage::~milxQtSpineImage()
{
    //dtor
}

void milxQtSpineImage::createActions()
{
  spineMenu = new QMenu(this); //!< Only exists for the duration of the context selection
  spineMenu->setTitle(QApplication::translate("MainWindow", "Spine plugin extensions", 0, QApplication::UnicodeUTF8));

  splineAct = new QAction(this);
      splineAct->setText(QApplication::translate("Spine", "&Manual initialisation", 0, QApplication::UnicodeUTF8));
      //splineAct->setShortcut(tr("Alt+l"));
      splineAct->setCheckable(true);
      splineAct->setChecked(false);
  spineMenu->addAction(splineAct);

  openSplineAct = new QAction(this);
      openSplineAct->setText(QApplication::translate("Spine", "&Open initialisation", 0, QApplication::UnicodeUTF8));
      openSplineAct->setCheckable(false);
  spineMenu->addAction(openSplineAct);

  saveSplineAct = new QAction(this);
      saveSplineAct->setText(QApplication::translate("Spine", "&Save initialisation", 0, QApplication::UnicodeUTF8));
      saveSplineAct->setCheckable(false);
      saveSplineAct->setEnabled(false);
  spineMenu->addAction(saveSplineAct);

      menusToAdd.append(spineMenu);
}

void milxQtSpineImage::createConnections()
{
  //Operations
  connect(splineAct, SIGNAL(triggered()), this, SLOT(enableSpline()));


  //milxQtImage::createConnections();
}

void milxQtSpineImage::contextMenuEvent(QContextMenuEvent *currentEvent)
{
    contextMenu = milxQtImage::basicContextMenu(); //!< Only exists for the duration of the context selection

    contextMenu->addSeparator();
    contextMenu->addMenu(spineMenu);
    contextMenu->addSeparator();
    contextMenu->addMenu(milxQtRenderWindow::contourMenu);
    contextMenu->addMenu(milxQtRenderWindow::windowPropertiesMenu);
    contextMenu->addAction(milxQtRenderWindow::refreshAct);
    contextMenu->addAction(milxQtRenderWindow::resetAct);

    contextMenu->exec(currentEvent->globalPos());
}

void milxQtSpineImage::enableSpline(QString title, const bool quiet, double minRange, double maxRange, int noOfHandles)
{
	if (!milxQtRenderWindow::spline)
	{
		milxQtRenderWindow::spline = vtkSmartPointer<vtkSplineWidget2>::New();
		milxQtRenderWindow::spline->EnabledOff();
	}

	// Check if visualised already, if so - disable
	if(milxQtRenderWindow::spline->GetEnabled())
	{
	  milxQtRenderWindow::spline->EnabledOff();
    splineAct->setChecked(false);
    saveSplineAct->setEnabled(false);
    return;
	}

	bool ok1 = false;

    noOfHandles = QInputDialog::getInt(this, tr("How many handles to show"),
                                        tr("Labels:"), noOfHandles, 0, 99, 1, &ok1);
 
    if(!ok1)
        return;

    vtkSmartPointer<vtkSplineRepresentation> rep1 = vtkSmartPointer<vtkSplineRepresentation>::New();
    rep1->SetNumberOfHandles(3);
    double position[3];

    position[0] = -6;
    position[1] = -58;
    position[2] = -80;
    rep1->SetHandlePosition(0, position);

    position[0] = -6;
    position[1] = -40;
    position[2] = 20;

    rep1->SetHandlePosition(1, position);

    position[0] = -6;
    position[1] = -65;
    position[2] = 125;
    rep1->SetHandlePosition(2, position);

    //rep1->ProjectToPlaneOn();

    milxQtRenderWindow::spline->SetRepresentation(rep1);


    //Add scale to scale widget
    milxQtRenderWindow::spline->SetInteractor(QVTKWidget::GetInteractor());
    milxQtRenderWindow::spline->EnabledOn();

    splineAct->setChecked(true);
    saveSplineAct->setEnabled(true);
}
