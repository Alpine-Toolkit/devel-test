## QtQuick Scene

# Changes

* https://doc.qt.io/qt-6/qtcore-changes-qt6.html

* https://doc.qt.io/qt-6/quick-changes-qt6.html
* https://doc.qt.io/qt-6/quick-changes-qt6.html#changes-to-qsg-apis

# Scene Graph

* https://doc.qt.io/qt-6/qtquick-visualcanvas-scenegraph.html

# QQuickItem

* https://doc.qt.io/qt-6/qquickitem.html
* https://doc.qt.io/qt-6/qquickitem.html#updatePaintNode
  `QSGNode *QQuickItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)`

# QSG Classes

* [QSGGeometry](https://doc.qt.io/qt-6/qsggeometry.html) provides low-level storage for graphics primitives.
* [QSGGeometry::Attribute](https://doc.qt.io/qt-6/qsggeometry-attribute.html)
* [QSGMaterialShader](https://doc.qt.io/qt-6/qsgmaterial.html) encapsulates rendering state for a
  shader program.
* [QSGMaterialShader](https://doc.qt.io/qt-6/qsgmaterialshader.html) represents a combination of
  vertex and fragment shaders, data that define the graphics pipeline state changes, and logic that
  updates graphics resources, such as uniform buffers and textures.
* [QSGMaterialType](https://doc.qt.io/qt-6/qsgmaterialtype.html) is used as a unique type token in combination with QSGMaterial.

* [QSGDynamicTexture](https://doc.qt.io/qt-6/qsgdynamictexture.html) serves as a baseclass for
  dynamically changing textures, such as content that is rendered to FBO's.
* [QSGTexture](https://doc.qt.io/qt-6/qsgtexture.html) is the base class for textures
* [QSGOpaqueTextureMaterial](https://doc.qt.io/qt-6/qsgopaquetexturematerial.html) provides a
  convenient way of rendering textured geometry.
* [QSGTextureProvider](https://doc.qt.io/qt-6/qsgtextureprovider.html) encapsulates texture based entities in QML.

* [QSGFlatColorMaterial](https://doc.qt.io/qt-6/qsgflatcolormaterial.html) provides a convenient way
  of rendering solid colored geometry in the scene graph. **shader is provided by Qt**
* QSGVertexColorMaterial

* [QSGNode](https://doc.qt.io/qt-6/qsgnode.html) can be used as a child container.
* [QSGBasicGeometryNode](https://doc.qt.io/qt-6/qsgbasicgeometrynode.html) should not be used by
  itself. It is only encapsulates shared functionality between the QSGGeometryNode and QSGClipNode
  classes.
* https://doc.qt.io/qt-6/qsgclipnode.html
* [QSGGeometryNode](https://doc.qt.io/qt-6/qsggeometrynode.html) consists of geometry and
  material. The geometry defines the mesh, the vertices and their structure, to be drawn. The
  Material defines how the shape is filled.
* https://doc.qt.io/qt-6/qsgopacitynode.html
* https://doc.qt.io/qt-6/qsgsimplerectnode.html
* [QSGSimpleTextureNode]( https://doc.qt.io/qt-6/qsgsimpletexturenode.html) is provided for
  convenience to easily draw textured content.
* https://doc.qt.io/qt-6/qsgtransformnode.html
* [QSGImageNode](https://doc.qt.io/qt-6/qsgimagenode.html) is provided for convenience to easily
  draw textured content.
  https://doc.qt.io/qt-6/qquickwindow.html#createImageNode
  `QSGImageNode *QQuickWindow::createImageNode() const`
