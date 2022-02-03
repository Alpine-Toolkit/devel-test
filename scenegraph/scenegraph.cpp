#include "scenegraph.h"

#include "noisynode.h"
#include "gridnode.h"
#include "linenode.h"

Scene::Scene()
  : m_samples_changed(false)
  , m_geometry_changed(false)
{
  setFlag(ItemHasContents, true);
}


void
Scene::append_sample(qreal value)
{
  m_samples << value;
  m_samples_changed = true;
  update();
}


void
Scene::remove_first_sample()
{
  m_samples.removeFirst();
  m_samples_changed = true;
  update();
}

void
Scene::geometryChange(const QRectF &new_geometry, const QRectF &old_geometry)
{
  m_geometry_changed = true;
  update();
  QQuickItem::geometryChange(new_geometry, old_geometry);
}


class SceneNode : public QSGNode
{
public:
  NoisyNode *background;
  GridNode *grid;
  LineNode *line;
  LineNode *shadow;
};


QSGNode *
Scene::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *)
{
  SceneNode *node = static_cast<SceneNode *>(old_node);

  QRectF rect = boundingRect();

  // Item is empty ?
  if (rect.isEmpty()) {
    delete node;
    return nullptr;
  }

  // Create graph ?
  if (!node) {
    node = new SceneNode();

    node->background = new NoisyNode(window());
    node->grid = new GridNode();
    node->line = new LineNode(10, 0.5, QColor("steelblue"));
    node->shadow = new LineNode(20, 0.2f, QColor::fromRgbF(0.2, 0.2, 0.2, 0.4));

    node->appendChildNode(node->background);
    node->appendChildNode(node->grid);
    node->appendChildNode(node->shadow);
    node->appendChildNode(node->line);
  }

  // Update geometry ?
  if (m_geometry_changed) {
    node->background->set_rect(rect);
    node->grid->set_rect(rect);
  }
  if (m_geometry_changed || m_samples_changed) {
    node->line->updateGeometry(rect, m_samples);
    // We don't need to calculate the geometry twice, so just steal it from the other one...
    node->shadow->setGeometry(node->line->geometry());
  }

  m_geometry_changed = false;
  m_samples_changed = false;

  return node;
}
