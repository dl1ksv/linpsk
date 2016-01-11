
#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qframe.h>
#include <vector>
#include "ui_controlpanel.h"
#include <complex>
#include "constants.h"

class Macros;

class ControlPanel : public QFrame, private Ui::ControlPanel
{
    Q_OBJECT

public:
    ControlPanel(QWidget* parent = 0);
    void insertMacros(Macros *);
    void setPhasePointer(std::complex<float> *);
    void setColorList(QList<QColor> *c);
    void present(bool p);
    void newChannel();
    void updateMacroWindow(int macroNumber);
    void insertMacros(QVector<Macro> *macroList);
    void enableSaveData();
    void setAutoDate();
    void restoreSplitterStates(const QByteArray & controlState,const QByteArray & spectrumState);
    QByteArray controlSplitterState() const;
    QByteArray spectrumSplitterState() const;
    ~ControlPanel();


 
protected:
public slots:
    void startPlot ( double *, bool );
    void setQsoData(QsoData,QString);
protected slots:

private:

signals:
    void FrequencyChanged ( double );
    void executeMacro ( int );
};

#endif // CONTROLPANEL_H
