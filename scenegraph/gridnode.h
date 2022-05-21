#ifndef GRIDNODE_H
#define GRIDNODE_H

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

class GridNode : public QSGGeometryNode
{
  // The QSGGeometryNode consists of geometry and material. The geometry defines the mesh, the
  // vertices and their structure, to be drawn. The Material defines how the shape is filled.

public:
  GridNode();

  void set_rect(const QRectF &rect);

private:
  // allocate geometry and material
  // QSGGeometry provides low-level storage for graphics primitives
  QSGGeometry m_geometry;
  // QSGFlatColorMaterialprovides a convenient way of rendering solid colored geometry
  QSGFlatColorMaterial m_material;
};

#endif // GRIDNODE_H
