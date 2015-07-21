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
/*diffusionAct = new QAction(this);
        diffusionAct->setText(QApplication::translate("Model", "Diffusion Glyphs for Slice", 0, QApplication::UnicodeUTF8));
        diffusionAct->setShortcut(tr("Shift+Alt+d"));
    diffusion2Act = new QAction(this);
        diffusion2Act->setText(QApplication::translate("Model", "Diffusion Glyphs for Slice Brute Force", 0, QApplication::UnicodeUTF8));
        diffusion2Act->setShortcut(tr("Ctrl+Shift+d"));
*/}

void milxQtSpineImage::createConnections()
{
    //Operations
//connect(diffusionAct, SIGNAL(triggered()), this, SLOT(diffusionGlyphs()));
  //  connect(diffusion2Act, SIGNAL(triggered()), this, SLOT(diffusionGlyphs2()));

    //milxQtImage::createConnections();
}

void milxQtSpineImage::contextMenuEvent(QContextMenuEvent *currentEvent)
{
    contextMenu = milxQtImage::basicContextMenu(); //!< Only exists for the duration of the context selection

    contextMenu->addSeparator()->setText(tr("Spine"));
//contextMenu->addAction(diffusionAct);
    //contextMenu->addAction(diffusion2Act);
    contextMenu->addSeparator()->setText(tr("Extensions"));
    foreach(QAction *currAct, extActionsToAdd)
    {
        contextMenu->addAction(currAct);
    }
    contextMenu->addSeparator();
    ///Don't display extensions
    contextMenu->addMenu(milxQtRenderWindow::contourMenu);
    contextMenu->addMenu(milxQtRenderWindow::windowPropertiesMenu);
    contextMenu->addAction(milxQtRenderWindow::refreshAct);
    contextMenu->addAction(milxQtRenderWindow::resetAct);

    contextMenu->exec(currentEvent->globalPos());
}

void milxQtSpineImage::enableSpline(QString title, const bool quiet, double minRange, double maxRange, int noOfHandles)
{
	if (!milxQtRenderWindow::spline)
		milxQtRenderWindow::spline = vtkSmartPointer<vtkSplineWidget2>::New();

	bool ok1 = false;

    noOfHandles = QInputDialog::getInt(this, tr("How many handles to show"),
                                        tr("Labels:"), noOfHandles, 0, 99, 1, &ok1);
 
    if(!ok1)
        return;


    //Add scale to scale widget
    milxQtRenderWindow::spline->SetInteractor(QVTKWidget::GetInteractor());
    //milxQtRenderWindow::scalarBar->SetScalarBarActor(milxQtRenderWindow::scale);
    milxQtRenderWindow::spline->EnabledOn();

    //milxQtRenderWindow::scaleBefore = true;
    //milxQtRenderWindow::scaleAct->setChecked(true);
}