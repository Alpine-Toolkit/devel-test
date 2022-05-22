/**************************************************************************************************/
#ifndef SCENE_H
#define SCENE_H

#include <QQuickItem>

/**************************************************************************************************/

class Scene : public QQuickItem
{
  Q_OBJECT
  QML_ELEMENT
public:
  Scene();

protected:
  QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);
  void geometryChange(const QRectF &new_geometry, const QRectF &old_geometry);

public slots:
  void append_sample(qreal value);
  void remove_first_sample();

private:
  bool m_geometry_changed;

  QList<qreal> m_samples;
  bool m_samples_changed;
};

/**************************************************************************************************/

#endif // SCENE_H
