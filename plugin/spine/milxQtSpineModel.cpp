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
#include "milxQtSpineModel.h"

#include <time.h>

#include <vtkMath.h>
#include <vtkLine.h>
#include <vtkSphereSource.h>


milxQtSpineModel::milxQtSpineModel(QWidget *theParent) : milxQtModel(theParent)
{
    milxQtWindow::prefix = "Spine: ";

    createActions();

    createConnections();
}

milxQtSpineModel::~milxQtSpineModel()
{
    //dtor
}

void milxQtSpineModel::createActions()
{
 /*   colourDirectionAct = new QAction(this);
        colourDirectionAct->setText(QApplication::translate("Model", "Colour By Direction", 0, QApplication::UnicodeUTF8));
        colourDirectionAct->setShortcut(tr("Alt+c"));
    harmonicsAct = new QAction(this);
        harmonicsAct->setText(QApplication::translate("Model", "Show Spherical Harmonic ...", 0, QApplication::UnicodeUTF8));
        harmonicsAct->setShortcut(tr("Alt+s"));
		*/
		}

void milxQtSpineModel::createConnections()
{
    //Operations
//connect(colourDirectionAct, SIGNAL(triggered()), this, SLOT(colourByDirection()));
  //  connect(harmonicsAct, SIGNAL(triggered()), this, SLOT(harmonics()));

    //milxQtModel::createConnections();
}

void milxQtSpineModel::contextMenuEvent(QContextMenuEvent *currentEvent)
{
    contextMenu = milxQtModel::basicContextMenu(); //!< Only exists for the duration of the context selection

    contextMenu->addSeparator()->setText(tr("Spine"));
//    contextMenu->addAction(colourDirectionAct);
  //  contextMenu->addAction(harmonicsAct);
    contextMenu->addSeparator()->setText(tr("Extensions"));
    foreach(QAction *currAct, extActionsToAdd)
    {
        contextMenu->addAction(currAct);
    }
    contextMenu->addSeparator();
    ///Dont display extensions
    contextMenu->addAction(milxQtModel::scaleAct);
    contextMenu->addAction(milxQtRenderWindow::axesAct);
    contextMenu->addAction(milxQtRenderWindow::refreshAct);

    contextMenu->exec(currentEvent->globalPos());
}
