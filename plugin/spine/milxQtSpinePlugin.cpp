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
#include "milxQtSpinePlugin.h"

//SMILI
#include <milxQtFile.h>


#include <qplugin.h>

milxQtSpinePlugin::milxQtSpinePlugin(QObject *theParent) : milxQtPluginInterface(theParent)
{
    ///Up cast parent to milxQtMain
    MainWindow = qobject_cast<milxQtMain *>(theParent);

    threaded = false;
    dockable = false;
    consoleWindow = false;
    extension = true;
    pluginName = "Spine";
    dataName = "";

    //createConnections();
}

milxQtSpinePlugin::~milxQtSpinePlugin()
{
    if(isRunning() && threaded)
        quit();
    cout << "Spine Plugin Destroyed." << endl;
}

QString milxQtSpinePlugin::name()
{
    return pluginName;
}

QString milxQtSpinePlugin::openFileSupport()
{
    QString openSpineExt = "NII Images (*.nii *.nii.gz)";

    return openSpineExt;
}

QStringList milxQtSpinePlugin::openExtensions()
{
    QStringList exts;
    exts.append(".nii.gz");
    exts.append(".nii");

    return exts;
}

QStringList milxQtSpinePlugin::saveExtensions()
{
    QStringList exts;

    return exts;
}

QString milxQtSpinePlugin::saveFileSupport()
{
    QString savePythonExt = "";

    return savePythonExt;
}

void milxQtSpinePlugin::SetInputCollection(vtkPolyDataCollection* collection, QStringList &filenames)
{

}

void milxQtSpinePlugin::open(QString filename)
{
    if(filename.contains(".nii", Qt::CaseInsensitive))
    {
        cout << "Loading as Spine Image" << endl;
        milxQtFile imageFile;
        if(imageFile.openImage(filename,spineImage))
        {
          spineImage->generateImage();
          spineImage->disableInterpolateDisplay();

          //QObject::connect(spineImage, SIGNAL(resultAvailable(milxQtModel*)), MainWindow, SLOT(display(milxQtModel*)));
          //QObject::connect(spineImage, SIGNAL(resultAvailable(milxQtRenderWindow*)), MainWindow, SLOT(display(milxQtRenderWindow*)));

          //display the spine image
          MainWindow->predisplay(spineImage);
        }
    }
    else
    {
        cout << "File extension not supported." << endl;
        return;
    }

    dataName = filename;
    cout << "Loaded Spine Image File." << endl;

}

void milxQtSpinePlugin::save(QString filename)
{

}

milxQtRenderWindow* milxQtSpinePlugin::genericResult()
{
    return NULL;
} //No render result

milxQtModel* milxQtSpinePlugin::modelResult()
{
    spineModel = new milxQtSpineModel;

    return spineModel;
}

milxQtImage* milxQtSpinePlugin::imageResult()
{
    spineImage = new milxQtSpineImage;

    return spineImage;
}

QDockWidget* milxQtSpinePlugin::dockWidget()
{
    return NULL;
} //No Dock result

bool milxQtSpinePlugin::isPluginWindow(QWidget *window)
{
    if(pluginWindow(window) == 0)
        return false;
    else
        return true;
}

milxQtSpineModel* milxQtSpinePlugin::pluginWindow(QWidget *window)
{
    if(window)
        return qobject_cast<milxQtSpineModel *>(window);
    return 0;
}

void milxQtSpinePlugin::loadExtension()
{
    if(MainWindow->isActiveModel())
    {
        milxQtModel *currentWin = MainWindow->activeModel();
        spineModel = new milxQtSpineModel; //hierarchical deletion
            spineModel->setName(currentWin->getName());
            spineModel->SetInput(currentWin->GetOutput());
            spineModel->generateModel();

        MainWindow->display(spineModel);
    }
    else if(MainWindow->isActiveImage())
    {
        milxQtImage *currentWin = MainWindow->activeImage();
        spineImage = new milxQtSpineImage; //hierarchical deletion
            spineImage->setName(currentWin->getName());
            spineImage->setData(currentWin->GetVectorImage(), false); ///\todo assuming vector image there, checks needed
            spineImage->generateImage();
            spineImage->disableInterpolateDisplay();

        //QObject::connect(spineImage, SIGNAL(resultAvailable(milxQtSpineImageModel*)), MainWindow, SLOT(display(milxQtImage*)));
        //QObject::connect(spineImage, SIGNAL(resultAvailable(milxQtRenderWindow*)), MainWindow, SLOT(display(milxQtRenderWindow*)));

        MainWindow->display(spineImage);
    }
    else
        return;
}

//~ void milxQtSpinePlugin::run()
//~ {
    //~ QMutexLocker locker(&mutex); //Lock memory

    //~ ///Execute own thread work here

    //~ //exec();
//~ }

//~ void milxQtSpinePlugin::createConnections()
//~ {
    //~ //QObject::connect(denoiseAct, SIGNAL(triggered(bool)), denoiseModel, SLOT(denoise()));
//~ }

Q_EXPORT_PLUGIN2(SpinePlugin, milxQtSpinePluginFactory);
