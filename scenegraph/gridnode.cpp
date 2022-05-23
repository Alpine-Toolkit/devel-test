#include "gridnode.h"

#include "qmath.h"

/**************************************************************************************************/

#define GRID_SIZE 32

/**************************************************************************************************/

GridNode::GridNode()
  // QSGGeometry(const QSGGeometry::AttributeSet &attributes, int vertexCount, int indexCount = 0, int indexType = UnsignedShortType)
  : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0)
{
  setGeometry(&m_geometry);
  // https://doc.qt.io/qt-6/qsggeometry.html#DrawingMode-enum
  m_geometry.setDrawingMode(QSGGeometry::DrawLines);

  // void QSGGeometryNode::setMaterial(QSGMaterial *material)
  setMaterial(&m_material);
  m_material.setColor(Qt::gray);
}

// The function hardcodes a fixed set of grid lines and scales those to the bounding rect.
void
GridNode::set_rect(const QRectF &rect)
{
  auto x = rect.x();
  auto y = rect.y();
  auto w = rect.width();
  auto h = rect.height();

  auto v_count = int((w - 1) / GRID_SIZE);
  auto h_count = int((h - 1) / GRID_SIZE);
  auto line_count = v_count + h_count;

  m_geometry.allocate(line_count * 2);

  QSGGeometry::Point2D *vertices = m_geometry.vertexDataAsPoint2D();

  // write the vertical lines
  size_t k = 0;
  for (size_t i = 0; i < v_count; ++i)  {
    float dx = (i + 1) * GRID_SIZE;
    vertices[k++].set(dx, y);
    vertices[k++].set(dx, y + h);
  }

  // write the horizontal lines
  for (size_t i = 0; i < h_count; ++i)  {
    float dy = (i + 1) * GRID_SIZE;
    vertices[k++].set(x, dy);
    vertices[k++].set(x + w, dy);
  }

  // Tell the scenegraph we updated the geometry
  markDirty(QSGNode::DirtyGeometry);
}
