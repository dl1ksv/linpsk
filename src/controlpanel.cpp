
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
ControlPanel::ControlPanel(Macros *Macro, QWidget* parent)
    : QFrame( parent )
{
    setFrameShape( QFrame::StyledPanel );
    setFrameShadow( QFrame::Sunken );

    Display = new SpectrumDisplay( this );
    QSO = new QSOData(this);
    MacroBox = new MacroWindow(Macro,this);

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

void ControlPanel::calculateSizeofComponents()
{
#define TOPMARGIN 2
#define LEFTMARGIN 1
#define SPECTRUMWIDTH 35
#define SPECTRUMHEIGHT 85
#define QSODATAWIDTH 45
#define MACROBOXWIDTH 17
int xpos,ypos,width,height,innerheight,innerwidth,innerwidthqso,innerwidthmacros;
width=this->width();
height=this->height();
xpos=width*LEFTMARGIN/100;
ypos=height*TOPMARGIN/100;
innerwidth=(width-xpos)*SPECTRUMWIDTH/100;
innerwidthqso=(width-xpos)*QSODATAWIDTH/100;
innerwidthmacros=(width-xpos)*MACROBOXWIDTH/100;
innerheight=((height-2*ypos)*SPECTRUMHEIGHT)/100;
Display->setGeometry(xpos,ypos,innerwidth,innerheight);
xpos=xpos+innerwidth+10;
innerheight=(height*(100-2*TOPMARGIN))/100;
QSO->setGeometry(xpos,ypos,innerwidthqso,innerheight);
xpos = xpos+innerwidthqso+10;
MacroBox->setGeometry(xpos,ypos,innerwidthmacros,innerheight);
MacroBox->show();
}

void ControlPanel::resizeEvent( QResizeEvent * )
{
calculateSizeofComponents();
}
