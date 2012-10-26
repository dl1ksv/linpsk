#include "tabwidget.h"
#include <QTabBar>

TabWidget::TabWidget(QWidget *parent) :
  QTabWidget(parent)
{
}
void TabWidget::setTabTextColor(int index, const QColor &color)
{
  tabBar()->setTabTextColor(index, color);
}
