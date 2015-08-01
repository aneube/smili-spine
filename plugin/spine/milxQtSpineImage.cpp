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
	spineMenu->setTitle(QApplication::translate("MainWindow", "Spine Plugin Extensions", 0, QApplication::UnicodeUTF8));
	
	splineAct = new QAction(this);
	splineAct->setText(QApplication::translate("Spine", "&Manual Initialisation", 0, QApplication::UnicodeUTF8));
	//splineAct->setShortcut(tr("Alt+l"));
	splineAct->setCheckable(true);
	splineAct->setChecked(false);
	spineMenu->addAction(splineAct);
	
	openSplineAct = new QAction(this);
	openSplineAct->setText(QApplication::translate("Spine", "&Open Initialisation", 0, QApplication::UnicodeUTF8));
	openSplineAct->setCheckable(false);
	spineMenu->addAction(openSplineAct);
	
	saveSplineAct = new QAction(this);
	saveSplineAct->setText(QApplication::translate("Spine", "&Save Initialisation", 0, QApplication::UnicodeUTF8));
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

//Enabling spline for manual initialisation
void milxQtSpineImage::enableSpline(QString title, const bool quiet, double minRange, double maxRange, int noOfHandles)
{
	if (!milxQtRenderWindow::spline)
	{
		milxQtRenderWindow::spline = vtkSmartPointer<vtkSplineWidget2>::New();
		milxQtRenderWindow::spline->EnabledOff();
	}
	
	//Check if visualised already, if so - disable
	if(milxQtRenderWindow::spline->GetEnabled())
	{
		milxQtRenderWindow::spline->EnabledOff();
		splineAct->setChecked(false);
		saveSplineAct->setEnabled(false);
		return;
	}
	
	bool ok1 = false;
	
	noOfHandles = QInputDialog::getInt(this, tr("Number of handles"),
												tr("Select number of Handles ranges from 2 to 10"), noOfHandles, 2, 10, 1, &ok1);
	
	if (!ok1)
		return;
	
	//get the image origin and real size
	floatImageType::PointType origin;
	floatImageType::SpacingType spacing;
	floatImageType::RegionType::SizeType imageSize;
	int extents[6];
	
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
	//float width = imageSize[1] * spacing[1];
	//float height = imageSize[2] * spacing[2];
	//got the origin, width and height
	
	vtkSmartPointer<vtkSplineRepresentation> rep1 = vtkSmartPointer<vtkSplineRepresentation>::New();
	rep1->SetNumberOfHandles(noOfHandles);
	double x = origin[0]+(imageSize[0]/2)*spacing[0];
	
	//Set the position of handles on the image within a plane
	for (int i = 0; i < noOfHandles; i++)
	{
		double coef = (0.2) + ((0.6*i) / (noOfHandles - 1));
		double y = origin[1] + (0.25*imageSize[1] * spacing[1]);
		double z = origin[2]+(coef*imageSize[2]*spacing[2]);
		rep1->SetHandlePosition(i, x, y, z);
	}
	
	//rep1->ProjectToPlaneOn();
	milxQtRenderWindow::spline->SetRepresentation(rep1);
	
	//Add scale to scale widget
	milxQtRenderWindow::spline->SetInteractor(QVTKWidget::GetInteractor());
	milxQtRenderWindow::spline->EnabledOn();
	
	splineAct->setChecked(true);
	saveSplineAct->setEnabled(true);
}

//Reading a spline
void milxQtSpineImage::openSpline(QString title, const bool quiet, double minRange, double maxRange, int noOfHandles)
{
	if (!milxQtRenderWindow::spline)
	{
		milxQtRenderWindow::spline = vtkSmartPointer<vtkSplineWidget2>::New();
		milxQtRenderWindow::spline->EnabledOff();
	}
	
	//Check if visualised already, if so - disable
	if (milxQtRenderWindow::spline->GetEnabled())
	{
		milxQtRenderWindow::spline->EnabledOff();
		splineAct->setChecked(false);
		saveSplineAct->setEnabled(false);
	}
	
	//Opening a new window to ask for the location of file to open
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
	file.open(filename.toLatin1().data());
	string word;
	double position[3];
	
	//Read the first word from the file
	file >> word;
	
	//Check if it matches the keyword
	if (word.compare("SplineSpine"))
	{
		cout << "ERROR when reading input file - not the correct format." << endl;
		return;
	}
	else
	{
		cout << "Input file is correct." << endl;
	}
	
	//Read the second word from the file - number of handles
	file >> word;
	int numberOfHandles = atoi(word.c_str());
	cout << "Number of Handles: " << numberOfHandles << endl << endl;
	
	//Read the co-ordinates of handles
	rep1->SetNumberOfHandles(numberOfHandles);
	for (int i = 0; i < numberOfHandles; i++)
	{
		file >> word;
		double x = std::stod(word);
		file >> word;
		double y = std::stod(word);
		file >> word;
		double z = std::stod(word);
		cout << x <<" "<< y <<" "<< z << endl;
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
	if (!milxQtRenderWindow::spline)
	{
		milxQtRenderWindow::spline = vtkSmartPointer<vtkSplineWidget2>::New();
		milxQtRenderWindow::spline->EnabledOff();
	}
	
	//Check if visualised already, if so - disable
	if (milxQtRenderWindow::spline->GetEnabled())
	{
		milxQtRenderWindow::spline->EnabledOff();
		splineAct->setChecked(false);
		saveSplineAct->setEnabled(false);
	}
	
	//Opening a new window to ask for the location to save a file	
	QFileDialog *fileSaver = new QFileDialog(this);
	QSettings settings("Shekhar Chandra", "milxQt");
	QString path = settings.value("recentPath").toString();
	
	QString filename = fileSaver->getSaveFileName(this,
													tr("Select File Name to Save"),
													path,
													tr("Text Files (*.txt)"));
	
	if(filename.isEmpty())
		return;
	
	saveView(filename);
	
	vtkSplineRepresentation* rep1 = reinterpret_cast<vtkSplineRepresentation*>(milxQtRenderWindow::spline->GetRepresentation());
	using namespace std;
	ofstream myfile;
	myfile.open(filename.toLatin1().data());
	myfile <<"SplineSpine" << std::endl;
	myfile << rep1->GetNumberOfHandles() << std::endl;
	double position[3];
	
	//Saving the co-ordinates of handles into a file
	for (int i = 0; i<rep1->GetNumberOfHandles(); ++i)
	{
		rep1->GetHandlePosition(i, position);
		myfile << position[0] << "  " << position[1] << "  " << position[2] << std::endl;
	}
	myfile.close();
	
	//rep1->ProjectToPlaneOn();
	milxQtRenderWindow::spline->SetRepresentation(rep1);
	
	//Add scale to scale widget
	milxQtRenderWindow::spline->SetInteractor(QVTKWidget::GetInteractor());
	milxQtRenderWindow::spline->EnabledOn();
	
	splineAct->setChecked(true);
	saveSplineAct->setEnabled(true);
}