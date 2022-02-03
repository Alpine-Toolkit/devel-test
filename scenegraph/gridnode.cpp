#include "gridnode.h"

#include "qmath.h"

#define GRID_SIZE 32

GridNode::GridNode()
  // QSGGeometry(const QSGGeometry::AttributeSet &attributes, int vertexCount, int indexCount = 0, int indexType = UnsignedShortType)
  : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0)
{
  // m_geometry ? versus
  //   QSGGeometry* QSGBasicGeometryNode::geometry()
  setGeometry(&m_geometry);
  m_geometry.setDrawingMode(QSGGeometry::DrawLines);

  // void QSGGeometryNode::setMaterial(QSGMaterial *material)
  setMaterial(&m_material);
  m_material.setColor(Qt::gray);
}

/*
 * The function hardcodes a fixed set of grid lines and scales
 * those to the bounding rect.
 */
void
GridNode::set_rect(const QRectF &rect)
{
  float x = rect.x();
  float y = rect.y();
  float w = rect.width();
  float h = rect.height();

  int v_count = int((w - 1) / GRID_SIZE);
  int h_count = int((h - 1) / GRID_SIZE);
  int line_count = v_count + h_count;

  QSGGeometry *g = geometry();
  g->allocate(line_count * 2);

  QSGGeometry::Point2D *v = g->vertexDataAsPoint2D();

  // write the vertical lines
  size_t k = 0;
  for (size_t i=0; i<v_count; ++i)  {
    float dx = (i + 1) * GRID_SIZE;
    v[k++].set(dx, y);
    v[k++].set(dx, y + h);
  }

  // write the horizontal lines
  for (size_t i=0; i<h_count; ++i)  {
    float dy = (i + 1) * GRID_SIZE;
    v[k++].set(x, dy);
    v[k++].set(x + w, dy);
  }

  // Tell the scenegraph we updated the geometry
  markDirty(QSGNode::DirtyGeometry);
}
