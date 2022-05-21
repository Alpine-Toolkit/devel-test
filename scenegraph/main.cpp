#include <QGuiApplication>
#include <QQuickView>
#include <QtDebug>

#include "scenegraph.h"

int main(int argc, char *argv[])
{
  QGuiApplication application(argc, argv);
  // check ~/.config/QtProject/qtlogging.ini !!!
  qInfo() << "Start ...";

  QQuickView view;
  view.resize(1920/2, 1080/2);
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setSource(QUrl("qrc:///scenegraph/scene/main.qml"));
  view.show();

  return QGuiApplication::exec();
}
