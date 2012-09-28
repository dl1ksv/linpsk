
#include "ctrigger.h"

#include <QLineEdit>
#include <QRadioButton>



/* 
 *  Constructs a CTrigger which is a child of 'parent', with the 
 *  name 'name'.' 
 */
CTrigger::CTrigger(const  QString name, QWidget* parent )
    : QGroupBox(name, parent)
{
//    setFrameShape( QGroupBox::WinPanel );
//    setFrameShadow( QGroupBox::Raised );
    setAlignment(  Qt::AlignHCenter  );

    TriggerText = new QLineEdit( this );
//    TriggerText->setMaxLength( 80 );

    Activate = new QRadioButton( "Activate", this );
    languageChange();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CTrigger::~CTrigger()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CTrigger::languageChange()
{
    setTitle( tr( "Trigger" ) );
    TriggerText->setText( tr( "CQ CQ" ) );
    Activate->setText( tr( "Activate" ) );
}

void CTrigger::resizeEvent( QResizeEvent * )
{
calculateSizeofComponents();
}

void CTrigger::calculateSizeofComponents()
{
#define LEFTANDRIGHTMARGIN 5
#define TOP 30
#define LABELHEIGHT 30
int xpos,ypos,width,height,innerwidth,innerheight;
width=this->width();
height=this->height();
xpos=width*LEFTANDRIGHTMARGIN/100;
ypos=height*TOP/100;
innerwidth=width-2*xpos;
innerheight=height*LABELHEIGHT/100;
TriggerText->setGeometry(xpos,ypos,innerwidth,innerheight);
ypos=ypos+innerheight;
Activate->setGeometry(xpos+1,ypos,innerwidth-2,innerheight);
//this->setFrameRect(QRect(0,0,width,height));
resize(width,height);
}
