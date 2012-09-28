
#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qframe.h>
#include <vector>

using namespace std;


class SpectrumDisplay;
class QSOData;
class MacroWindow;
class Macros;

class ControlPanel : public QFrame
{
    Q_OBJECT

public:
    ControlPanel(Macros *M, QWidget* parent = 0);
    ~ControlPanel();

    SpectrumDisplay* Display;
    QSOData *QSO;
    MacroWindow  *MacroBox;

 
protected:
    void resizeEvent( QResizeEvent * );
public slots:
 
protected slots:

private:
 
    void calculateSizeofComponents();

};

#endif // CONTROLPANEL_H
