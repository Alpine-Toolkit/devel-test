#include "linenode.h"

#include <QtGlobal>
#include <QtGui/QColor>
#include <QtQuick/QSGMaterial>

/**************************************************************************************************/

class LineShader : public QSGMaterialShader
{
public:
  LineShader() {
    setShaderFileName(VertexStage,   QLatin1String(":/scenegraph/scene/shaders/line.vert.qsb"));
    setShaderFileName(FragmentStage, QLatin1String(":/scenegraph/scene/shaders/line.frag.qsb"));
  }

  bool updateUniformData(RenderState &state, QSGMaterial *new_material, QSGMaterial *old_material) override;
};

/**************************************************************************************************/

class LineMaterial : public QSGMaterial
{
public:
  LineMaterial()
  {
    // Set this flag to true if the material requires GL_BLEND to be enabled during rendering.
    setFlag(Blending);
  }

  // This function is called by the scene graph to query an identifier
  //   that is unique to the QSGMaterialShader instantiated by createShader().
  // For many materials, the typical approach will be to return a pointer to a static,
  //   and so globally available, QSGMaterialType instance.
  // The QSGMaterialType is an opaque object.
  // Its purpose is only to serve as a type-safe, simple way to generate unique material identifiers.
  QSGMaterialType *type() const override
  {
    static QSGMaterialType type;
    return &type;
  }

  // This function returns a new instance of a the QSGMaterialShader implementation
  //  used to render geometry for a specific implementation of QSGMaterial.
  // The function will be called only once for each combination of material type
  //  and renderMode and will be cached internally.
  QSGMaterialShader *createShader(QSGRendererInterface::RenderMode) const override
  {
    return new LineShader;
  }

  // Compares this material to other and returns 0 if they are equal;
  //  -1 if this material should sort before other and 1 if other should sort before.
  // The scene graph can reorder geometry nodes to minimize state changes.
  // The compare function is called during the sorting process
  //   so that the materials can be sorted to minimize state changes in each call to QSGMaterialShader::updateState().
  int compare(const QSGMaterial *material) const override
  {
    // The this pointer and other is guaranteed to have the same type().
    auto other = static_cast<const LineMaterial *>(material);

    // example ...

    // if diff is non-zero
    if (int diff = int(state.color.rgb()) - int(other->state.color.rgb()))
      return diff;

    if (int diff = state.size - other->state.size)
      return diff;

    if (int diff = state.spread - other->state.spread)
      return diff;

    return 0; // equal
  }

  // Shader state
  struct {
    QColor color;
    float size;
    float spread;
  } state;
};

/**************************************************************************************************/

// updateUniformData() is expected to update the contents of a QByteArray
// that will then be exposed to the shaders as a uniform buffer.
bool
LineShader::updateUniformData(RenderState &state, QSGMaterial *new_material, QSGMaterial *)
{
  // layout(std140, binding = 0) uniform buf {
  //     mat4 qt_Matrix;   // 4*4*4 = 64
  //     vec4 color;       //   4*4 = 16 @64
  //     float qt_Opacity; //     4      @80
  //     float size;       //     4      @84
  //     float spread;     //     4      @88
  //                                      92
  // };

  auto material = static_cast<LineMaterial *>(new_material);

  QByteArray *buffer = state.uniformData();
  Q_ASSERT(buffer->size() >= 92);

  if (state.isMatrixDirty()) {
    const QMatrix4x4 matrice = state.combinedMatrix();
    memcpy(buffer->data(), matrice.constData(), 64);
  }

  float c[4];
  material->state.color.getRgbF(&c[0], &c[1], &c[2], &c[3]);
  memcpy(buffer->data() + 64, c, 16);

  if (state.isOpacityDirty()) {
    const float opacity = state.opacity();
    memcpy(buffer->data() + 80, &opacity, 4);
  }

  memcpy(buffer->data() + 84, &material->state.size, 4);
  memcpy(buffer->data() + 88, &material->state.spread, 4);

  return true;
}

/**************************************************************************************************/

struct LineVertex {
  float x;
  float y;
  float t;

  inline void set(float xx, float yy, float tt) { x = xx; y = yy; t = tt; }
};

/**************************************************************************************************/

static const
QSGGeometry::AttributeSet & attributes()
{
  // The QSGGeometry::Attribute struct describes a single vertex attribute in a QSGGeometry.
  //    the attribute register position, the size of the attribute tuple and the attribute type.
  // It also contains a hint to the renderer if this attribute is the attribute describing the position.
  //   The scene graph renderer may use this information to perform optimizations.
  // create(int pos, int tupleSize, int primitiveType, bool isPosition = false)
  static QSGGeometry::Attribute attr[] = {
    // layout(location = 0) in vec4 pos;
    QSGGeometry::Attribute::create(0, 2, QSGGeometry::FloatType, true),
    // layout(location = 1) in float t;
    QSGGeometry::Attribute::create(1, 1, QSGGeometry::FloatType)
  };

  // The QSGGeometry::AttributeSet describes how the vertices in a QSGGeometry are built up.
  //   struct AttributeSet {
  //       int count;
  //       int stride;
  //       const Attribute *attributes;
  //   };
  static QSGGeometry::AttributeSet set = {
    2,
    // 3 * sizeof(float),
    sizeof(LineVertex),
    attr
  };

  return set;
}

/**************************************************************************************************/

LineNode::LineNode(float size, float spread, const QColor &color)
  // QSGGeometry::QSGGeometry(const QSGGeometry::AttributeSet &attributes, int vertexCount, int indexCount = 0, int indexType = UnsignedShortType)
  : m_geometry(attributes(), 0)
{
  setGeometry(&m_geometry);
  m_geometry.setDrawingMode(QSGGeometry::DrawTriangleStrip);
  //                2(x1,y1,t=1)                4(x2,y2,t=1)
  //
  //  1(x1,y1,t=0)               3(x2,y2,t=0)                 5(x3,y3,t=0)...

  LineMaterial *material = new LineMaterial;
  material->state.color = color;
  material->state.size = size;
  material->state.spread = spread;

  setMaterial(material);
  setFlag(OwnsMaterial);
}

/**************************************************************************************************/

// Assumes that samples have values in the range of 0 to 1 and scales them to the height of bounds.
// The samples are stretched out horizontally along the width of the bounds.
// The position of each pair of points is identical, but we use the third value
//   "t" to shift the point up or down and to add antialiasing.
void
LineNode::update_geometry(const QRectF &bounds, const QList<qreal> &samples)
{
  m_geometry.allocate(samples.size() * 2);

  // float ?
  auto x = bounds.x();
  auto y = bounds.y();
  auto w = bounds.width();
  auto h = bounds.height();

  float dx = w / (samples.size() - 1);

  auto v = static_cast<LineVertex *>(m_geometry.vertexData());
  size_t k = 0;
  for (size_t i = 0; i < samples.size(); ++i) {
    float xx = x + dx * i;
    float yy = y + samples.at(i) * h;
    // qInfo() << xx << " " << yy;
    v[k++].set(xx, yy, 0);
    v[k++].set(xx, yy, 1);
  }

  markDirty(QSGNode::DirtyGeometry);
}
