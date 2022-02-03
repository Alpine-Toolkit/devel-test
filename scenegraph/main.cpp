#include <QGuiApplication>
#include <QQuickView>

#include "scenegraph.h"
#include <QtQml/QQmlContext>

int main(int argc, char *argv[])
{
  QGuiApplication a(argc, argv);

  QQuickView view;
  view.resize(1920/2, 1080/2);
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setSource(QUrl("qrc:///scenegraph/scene/main.qml"));
  view.show();

  return QGuiApplication::exec();
}
