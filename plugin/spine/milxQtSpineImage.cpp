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
  connect(openSplineAct, SIGNAL(triggered()), this, SLOT(openSpline()));
  connect(saveSplineAct, SIGNAL(triggered()), this, SLOT(saveSpline()));


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

    noOfHandles = QInputDialog::getInt(this, tr("Number of handles"),
                                        tr("Handles:"), noOfHandles, 2, 10, 1, &ok1);
	if (!ok1)
		return;

	/* // get the image origin and real size
	floatImageType::PointType origin;
	floatImageType::SpacingType spacing;
	floatImageType::RegionType::SizeType imageSize;
	if (usingVTKImage)
	{
		imageData->GetOrigin(origin.GetDataPointer());
		imageData->GetSpacing(spacing.GetDataPointer());
		imageData->GetExtent(extents);

		imageSize[0] = extents[1] - extents[0];
		imageSize[1] = extents[3] - extents[2];
		imageSize[2] = extents[5] - extents[4];
	}
	else
	{
		if (eightbit)
		{
			origin = imageChar->GetOrigin();
			spacing = imageChar->GetSpacing();
			imageSize = imageChar->GetLargestPossibleRegion().GetSize();
		}
		else if (rgb)
		{
			origin = imageRGB->GetOrigin();
			spacing = imageRGB->GetSpacing();
			imageSize = imageRGB->GetLargestPossibleRegion().GetSize();
		}
		else
		{
			origin = imageFloat->GetOrigin();
			spacing = imageFloat->GetSpacing();
			imageSize = imageFloat->GetLargestPossibleRegion().GetSize();
		}
	}
	float width = imageSize[1] * spacing[1];
	float height = imageSize[2] * spacing[2];
	// got the origin, width and height */

 
    vtkSmartPointer<vtkSplineRepresentation> rep1 = vtkSmartPointer<vtkSplineRepresentation>::New();
	rep1->SetHandleSize(9);
	rep1->SetNumberOfHandles(noOfHandles);
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

// Reading spline
void milxQtSpineImage::openSpline(QString title, const bool quiet, double minRange, double maxRange, int noOfHandles)
{
	if (!milxQtRenderWindow::spline)
	{
		milxQtRenderWindow::spline = vtkSmartPointer<vtkSplineWidget2>::New();
		milxQtRenderWindow::spline->EnabledOff();
	}

	// Check if visualised already, if so - disable
	if (milxQtRenderWindow::spline->GetEnabled())
	{
		milxQtRenderWindow::spline->EnabledOff();
		splineAct->setChecked(false);
		saveSplineAct->setEnabled(false);
	}


	QFileDialog *fileOpener = new QFileDialog(this);
	QSettings settings("Shekhar Chandra", "milxQt");
	QString path = settings.value("recentPath").toString();

	QString filename = fileOpener->getOpenFileName(this,
		tr("Select File to Open"),
		path,
		tr("Spline Files (*.txt)"));

	if (filename.isEmpty())
		return;


		vtkSmartPointer<vtkSplineRepresentation> rep1 = vtkSmartPointer<vtkSplineRepresentation>::New();

		using namespace std;
		ifstream file;
		//file.open("SplineSpine.txt");
		file.open(filename.toLatin1().data());
		string word;
		double position[3];

		// Read the first word from the file
		file >> word;
		// Check if it matches the keyword
		if (word.compare("SplineSpine"))
		{
			cout << "ERROR when reading input file - not the correct format." << endl;
			return;
		}
		else
		{
			cout << "Input file is correct." << endl;
		}

		// Read the second word from the file - number of handles
		file >> word;
		int numberOfHandles = atoi(word.c_str());
		cout << "Number of Handles: " << numberOfHandles << endl << endl;
		rep1->SetNumberOfHandles(numberOfHandles);
		// rep1->SetHandleSize(9);

		for (int i = 0; i < numberOfHandles; i++)
		{
			file >> word;
			double x = std::stod(word);
			file >> word;
			double y = std::stod(word);
			file >> word;
			double z = std::stod(word);

			// cout << x <<" "<< y <<" "<< z << endl;
			rep1->SetHandlePosition(i, x, y, z);
		}
		cout << "Read Successfully \n" << endl;

		//rep1->ProjectToPlaneOn();
		milxQtRenderWindow::spline->SetRepresentation(rep1);

		//Add scale to scale widget
		milxQtRenderWindow::spline->SetInteractor(QVTKWidget::GetInteractor());
		milxQtRenderWindow::spline->EnabledOn();

		splineAct->setChecked(true);
		openSplineAct->setEnabled(true);
		saveSplineAct->setEnabled(true);
	}

//Writing a spine
void milxQtSpineImage::saveSpline(QString title, const bool quiet, double minRange, double maxRange, int noOfHandles)
{
  vtkSplineRepresentation* rep1 = reinterpret_cast<vtkSplineRepresentation*>(milxQtRenderWindow::spline->GetRepresentation());
	 
  using namespace std;
  ofstream myfile;
  myfile.open("SplineSpine.txt");
  myfile <<"SplineSpine" << std::endl;
  myfile << rep1->GetNumberOfHandles() << std::endl;
  double position[3];
  for (int i = 0; i<rep1->GetNumberOfHandles(); ++i)
  {
	  rep1->GetHandlePosition(i, position);
	  myfile << position[0] << "  " << position[1] << "  " << position[2] << std::endl;
  }
  myfile.close();
}
