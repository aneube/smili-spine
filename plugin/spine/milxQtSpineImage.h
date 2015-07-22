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
#ifndef milxQTSPINEIMAGE_H
#define milxQTSPINEIMAGE_H

#include "milxQtImage.h"
#include "milxQtModel.h"

class MILXQT_PLUGIN_EXPORT milxQtSpineImage : public milxQtImage
{
    Q_OBJECT

public:
    milxQtSpineImage(QWidget *theParent = 0);
    virtual ~milxQtSpineImage();

public slots:

  /*!
  \fn milxQtSpineImage::enableSpline(QString title = "", const bool quiet = false, double minRange = 0.0, double maxRange = 0.0, int noOfLabels = 3)
  \brief Enable spline widget.

  Quiet Boolean is to prevent possible popups to ask user parameters.
  */
  virtual void enableSpline(QString title = "", const bool quiet = false, double minRange = 0.0, double maxRange = 0.0, int noOfLabels = 3);

signals:
    /*!
        \fn milxQtSpineImage::resultAvailable(milxQtModel*)
        \brief Send signal that Resultant model is available for showing.
    */
    //void resultAvailable(milxQtSpineImage*);
    /*!
        \fn milxQtSpineImage::resultAvailable(milxQtRenderWindow*)
        \brief Send signal that Resultant rendering is available for showing.
    */
    //void resultAvailable(milxQtRenderWindow*);

protected:
    QMenu *spineMenu; //!< Context Menu
    QAction *splineAct; //!< show spline
    QAction *saveSplineAct; //!< show spline
    QAction *openSplineAct; //!< show spline

private:
    void createActions();
    void createConnections();
    void contextMenuEvent(QContextMenuEvent *currentEvent);
};

#endif // milxQtSpineIMAGE_H
