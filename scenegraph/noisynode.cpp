#include "noisynode.h"

#include <QtCore/QRandomGenerator>

#include <QtQuick/QSGTexture>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGMaterial>

/**************************************************************************************************/

class NoisyShader : public QSGMaterialShader
{
public:
  NoisyShader() {
    setShaderFileName(VertexStage,   QLatin1String(":/scenegraph/scene/shaders/noisy.vert.qsb"));
    setShaderFileName(FragmentStage, QLatin1String(":/scenegraph/scene/shaders/noisy.frag.qsb"));
  }

  bool updateUniformData(RenderState &state, QSGMaterial *new_material, QSGMaterial *old_material) override;
  void updateSampledImage(RenderState &state, int binding, QSGTexture **texture,
                          QSGMaterial *new_material, QSGMaterial *old_material) override;
};

/**************************************************************************************************/

class NoisyMaterial : public QSGMaterial
{
public:
  NoisyMaterial()
  {
    setFlag(Blending);
  }

  ~NoisyMaterial()
  {
    delete state.texture;
  }

  QSGMaterialType *type() const override
  {
    static QSGMaterialType type;
    return &type;
  }

  QSGMaterialShader *createShader(QSGRendererInterface::RenderMode) const override
  {
    return new NoisyShader;
  }

  int compare(const QSGMaterial *material) const override
  {
    const NoisyMaterial *other = static_cast<const NoisyMaterial *>(material);

    if (int diff = int(state.color.rgb()) - int(other->state.color.rgb()))
      return diff;

    if (!state.texture || !other->state.texture)
      return state.texture ? 1 : -1;

    const qint64 diff = state.texture->comparisonKey() - other->state.texture->comparisonKey();
    return diff < 0 ? -1 : (diff > 0 ? 1 : 0);
  }

  struct {
    QColor color;
    QSGTexture *texture;
  } state;
};

/**************************************************************************************************/

bool
NoisyShader::updateUniformData(RenderState &state, QSGMaterial *new_material, QSGMaterial *)
{
  // layout(std140, binding = 0) uniform buf {
  //     mat4 qt_Matrix;
  //     vec4 color;
  //     vec2 texture_size;
  //     float qt_Opacity;
  // };

  auto material = static_cast<NoisyMaterial *>(new_material);

  QByteArray *buffer = state.uniformData();
  Q_ASSERT(buffer->size() >= 92);

  if (state.isMatrixDirty()) {
    const QMatrix4x4 m = state.combinedMatrix();
    memcpy(buffer->data(), m.constData(), 64);
  }

  float c[4];
  material->state.color.getRgbF(&c[0], &c[1], &c[2], &c[3]);
  memcpy(buffer->data() + 64, c, 16);

  const QSize s = material->state.texture->textureSize();
  float texture_size[2] = { 1.0f / s.width(), 1.0f / s.height() };
  memcpy(buffer->data() + 80, texture_size, 8);

  if (state.isOpacityDirty()) {
    const float opacity = state.opacity();
    memcpy(buffer->data() + 88, &opacity, 4);
  }

  return true;
}

/**************************************************************************************************/

void NoisyShader::updateSampledImage(RenderState &state,
                                     int binding,
                                     QSGTexture **texture,
                                     QSGMaterial *new_material,
                                     QSGMaterial *
                                     )
{
  Q_UNUSED(state);
  Q_UNUSED(binding);

  auto material = static_cast<NoisyMaterial *>(new_material);
  *texture = material->state.texture;
}

/**************************************************************************************************/

NoisyNode::NoisyNode(QQuickWindow *window)
{
  // Make some noise...
  const int NOISE_SIZE = 64;
  QImage image(NOISE_SIZE, NOISE_SIZE, QImage::Format_RGB32);
  auto data = (uint *) image.bits();
  for (int i = 0; i < NOISE_SIZE * NOISE_SIZE; ++i) {
    uint g = QRandomGenerator::global()->bounded(0xff);
    data[i] = 0xff000000 | (g << 16) | (g << 8) | g;
  }

  QSGTexture *texture = window->createTextureFromImage(image);
  texture->setFiltering(QSGTexture::Nearest);
  texture->setHorizontalWrapMode(QSGTexture::Repeat);
  texture->setVerticalWrapMode(QSGTexture::Repeat);

  auto material = new NoisyMaterial;
  material->state.texture = texture;
  material->state.color = QColor::fromRgbF(0.95, 0.95, 0.97);
  setMaterial(material);
  setFlag(OwnsMaterial, true);

  // TexturedPoint2D = vec2 position, vec2 texture_coordinate
  QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
  QSGGeometry::updateTexturedRectGeometry(geometry, QRect(), QRect());
  setGeometry(geometry);
  setFlag(OwnsGeometry, true);
}

/**************************************************************************************************/

void
NoisyNode::set_rect(const QRectF &bounds)
{
  QSGGeometry::updateTexturedRectGeometry(geometry(), bounds, QRectF(0, 0, 1, 1));
  markDirty(QSGNode::DirtyGeometry);
}
