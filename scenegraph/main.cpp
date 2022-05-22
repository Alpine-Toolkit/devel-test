/**************************************************************************************************/

#include <QGuiApplication>
#include <QQuickView>
#include <QtDebug>

#include "scenegraph.h"

/**************************************************************************************************/

int main(int argc, char *argv[])
{
  QGuiApplication application(argc, argv);
  // for logging check $HOME/.config/QtProject/qtlogging.ini !!!
  qInfo() << "Start ...";

  QQuickView view;
  const int width = 1920/2;
  const int height = 1080/2;
  view.resize(width, height);
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setSource(QUrl("qrc:///scenegraph/scene/main.qml"));
  view.show();

  return QGuiApplication::exec();
}
