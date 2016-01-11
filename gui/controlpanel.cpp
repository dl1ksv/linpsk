
#include "controlpanel.h"

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "spectrumdisplay.h"
#include "qsodata.h"

/* 
 *  Constructs a ControlPanel which is a child of 'parent', with the 
 *  name 'name'.' 
 */
ControlPanel::ControlPanel( QWidget* parent)
  : QFrame( parent ), Ui::ControlPanel()
{
  setupUi(this);
  connect(Display,SIGNAL(FrequencyChanged(double)),this,SIGNAL(FrequencyChanged(double)));
  connect(macroControl,SIGNAL(executeMacro(int)),this,SIGNAL(executeMacro(int)));
}

/*
 *  Destroys the object and frees any allocated resources
 */
ControlPanel::~ControlPanel()
{
 if(QSO)
  delete QSO;
 QSO=0;
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */


void ControlPanel::setQsoData(QsoData value,QString s)
{
  QSO->setQsoData(value,s);
}

void ControlPanel::setPhasePointer(std::complex<float> *p)
{
  Display->setPhasePointer(p);
}
void ControlPanel::setColorList(QList<QColor> *c)
{
  Display->setColorList(c);
}
void ControlPanel::startPlot ( double *d, bool b)
{
  Display->startPlot(d,b);
}
void ControlPanel::present(bool p)
{
  Display->showSpectren(p);
}
void ControlPanel::newChannel()
{
  QSO->newChannel();
}
void ControlPanel::updateMacroWindow(int macroNumber)
{
  macroControl->updateMacroWindow(macroNumber);
}

void ControlPanel::insertMacros(QVector<Macro> *macroList)
{
  macroControl->insertMacros(macroList);
}
void ControlPanel::enableSaveData()
{
  QSO->enableSaveData();
}
void ControlPanel::setAutoDate()
{
  QSO->setAutoDate();
}

void ControlPanel::restoreSplitterStates(const QByteArray & controlState,const QByteArray & spectrumState)
{
  controlSplitter->restoreState(controlState);
  Display->restoreSplitterState(spectrumState);
}
QByteArray ControlPanel::controlSplitterState() const
{
  return controlSplitter->saveState();
}
QByteArray ControlPanel::spectrumSplitterState() const
{
  return Display->spectrumSplitterState();
}
