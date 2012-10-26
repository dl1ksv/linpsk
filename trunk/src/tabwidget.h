#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

class TabWidget : public QTabWidget
{
  Q_OBJECT
public:
  explicit TabWidget(QWidget *parent = 0);
  void setTabTextColor(int index, const QColor &color);
  
signals:
  
public slots:
  
};

#endif // TABWIDGET_H
