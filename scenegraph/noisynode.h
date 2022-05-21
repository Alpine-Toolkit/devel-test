#ifndef NOISYNODE_H
#define NOISYNODE_H

#include <QSGGeometryNode>
#include <QQuickWindow>

class NoisyNode : public QSGGeometryNode
{
public:
  NoisyNode(QQuickWindow *window);

  void set_rect(const QRectF &bounds);
};

#endif // NOISYNODE_H
