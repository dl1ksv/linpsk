
#include "controlpanel.h"

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "spectrumdisplay.h"
#include "qsodata.h"
#include "macrowindow.h"

/* 
 *  Constructs a ControlPanel which is a child of 'parent', with the 
 *  name 'name'.' 
 */
ControlPanel::ControlPanel( QWidget* parent)
  : QFrame( parent ), Ui::ControlPanel()
{
  setupUi(this);
  connect(Display,SIGNAL(FrequencyChanged(double)),this,SIGNAL(FrequencyChanged(double)));
  connect(MacroBox,SIGNAL(callMacro(int)),this,SIGNAL(callMacro(int)));
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

void ControlPanel::insertMacros(Macros *M)
{
   MacroBox->setMacroWindow ( M );
}

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
void ControlPanel::display()
{
  Display->show();
}

void ControlPanel::undisplay()
{
  Display->hide();
}
void ControlPanel::newChannel()
{
  QSO->newChannel();
}
void ControlPanel::updateMacroWindow(Macros *M)
{
  MacroBox->updateMacroWindow(M);
}


