/**************************************************************************************************/

#include <QtGlobal>

#include <QSGImageNode>
#include <QSGSimpleTextureNode>
#include <QTextDocument>

// Qt Private API
// .../Qt/6.3.0/gcc_64/include/QtQuick/6.3.0/QtQuick/private/qquicktextnode_p.h
#include "private/qquicktextnode_p.h"
/*
 * class Q_QUICK_PRIVATE_EXPORT QQuickTextNode : public QSGTransformNode
 * {
 * public:
 *     QQuickTextNode(QQuickItem *ownerElement);
 *     ~QQuickTextNode();
 *
 *     static bool isComplexRichText(QTextDocument *);
 *
 *     void deleteContent();
 *     void addTextLayout(const QPointF &position, QTextLayout *textLayout, const QColor &color = QColor(),
 *                        QQuickText::TextStyle style = QQuickText::Normal, const QColor &styleColor = QColor(),
 *                        const QColor &anchorColor = QColor(),
 *                        const QColor &selectionColor = QColor(), const QColor &selectedTextColor = QColor(),
 *                        int selectionStart = -1, int selectionEnd = -1,
 *                        int lineStart = 0, int lineCount = -1);
 *
 *     void addTextDocument(
 *         const QPointF &position,
 *         QTextDocument *textDocument,
 *         const QColor &color = QColor(),
 *         QQuickText::TextStyle style = QQuickText::Normal,  // Outline Raised Sunken
 *         const QColor &styleColor = QColor(),
 *         const QColor &anchorColor = QColor(),
 *         const QColor &selectionColor = QColor(),
 *         const QColor &selectedTextColor = QColor(),
 *         int selectionStart = -1,
 *         int selectionEnd = -1
 *     );
 *
 *     void setCursor(const QRectF &rect, const QColor &color);
 *     void clearCursor();
 *     QSGInternalRectangleNode *cursorNode() const { return m_cursorNode; }
 *
 *     QSGGlyphNode *addGlyphs(const QPointF &position, const QGlyphRun &glyphs, const QColor &color,
 *                             QQuickText::TextStyle style = QQuickText::Normal, const QColor &styleColor = QColor(),
 *                             QSGNode *parentNode = 0);
 *     void addImage(const QRectF &rect, const QImage &image);
 *     void addRectangleNode(const QRectF &rect, const QColor &color);
 *
 *     bool useNativeRenderer() const { return m_useNativeRenderer; }
 *     void setUseNativeRenderer(bool on) { m_useNativeRenderer = on; }
 *
 *     void setRenderTypeQuality(int renderTypeQuality) { m_renderTypeQuality = renderTypeQuality; }
 *     int renderTypeQuality() const { return m_renderTypeQuality; }
 */

#include "scenegraph.h"

#include "noisynode.h"
#include "gridnode.h"
#include "linenode.h"

/**************************************************************************************************/

class SceneNode : public QSGNode
{
public:
  NoisyNode *background;
  GridNode *grid;
  LineNode *line;
  LineNode *shadow;
  QSGSimpleTextureNode *texture_node;
  QSGImageNode *image_node;
  QQuickTextNode *text_node;
};

/**************************************************************************************************/

Scene::Scene()
  : m_geometry_changed(false),
    m_samples_changed(false)
{
  setFlag(ItemHasContents, true);
}

void
Scene::geometryChange(const QRectF &new_geometry, const QRectF &old_geometry)
{
  qInfo() << new_geometry << "->" << old_geometry;
  m_geometry_changed = true;
  // update(); // useless ??? done in supra ???
  QQuickItem::geometryChange(new_geometry, old_geometry);
}

void
Scene::append_sample(qreal value)
{
  qInfo() << "append_sample" << value;
  m_samples << value;
  m_samples_changed = true;
  update();
}

void
Scene::remove_first_sample()
{
  qInfo() << "remove_first_sample";
  m_samples.removeFirst();
  m_samples_changed = true;
  update();
}

QSGNode *
Scene::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *)
{
  SceneNode *node = static_cast<SceneNode *>(old_node);

  QRectF rect = boundingRect();
  qInfo() << "updatePaintNode" << rect;
  // QRectF(0,0 920x500)

  // Item is empty ?
  if (rect.isEmpty()) {
    delete node;
    return nullptr;
  }

  // Create graph ?
  if (!node) {
    node = new SceneNode();

    auto _window = window();

    node->background = new NoisyNode(_window);
    node->appendChildNode(node->background);

    node->grid = new GridNode();
    node->appendChildNode(node->grid);

    {
      float size = 10.;
      float spread = .5; // 0 -> 1 => larger => thinner
      QColor color("steelblue");
      node->line = new LineNode(size, spread, color);
      node->appendChildNode(node->line);
      // size = 20.;
      // spread = .2;
      // color = QColor::fromRgbF(.2, .2, .2, .4);
      // node->shadow = new LineNode(size, spread, color);
      // node->appendChildNode(node->shadow);
    }

    QImage image(":/scenegraph/scene/icons/qt-logo.png");
    auto texture = _window->createTextureFromImage(image);

    // This utility class is only functional when running with the default or software backends of the Qt Quick scenegraph.
    // As an alternative, prefer using QSGImageNode via QQuickWindow::createImageNode().
    // However, this standalone class is still useful when used via subclassing
    // and the application knows that no special scenegraph backends will be involved.
    auto texture_node = new QSGSimpleTextureNode();
    // texture_node->setOwnsTexture(false);
    texture_node->setTexture(texture);
    // texture_node->setFiltering(QSGTexture::None);
    // texture_node->setFiltering(QSGTexture::Nearest);
    texture_node->setFiltering(QSGTexture::Linear);
    // texture_node->setTextureCoordinatesTransform(QSGSimpleTextureNode::NoTransform);
    // This is commonly needed when using a third party OpenGL library to render to texture
    // as OpenGL has an inverted y-axis relative to Qt Quick.
    texture_node->setRect(10, 10, image.width(), image.height());
    // texture_node->setSourceRect(0, 0, image.width()/2, image.height()/2);
    node->appendChildNode(texture_node);
    node->texture_node = texture_node;

    auto image_node = _window->createImageNode();
    image_node->setTexture(texture);
    image_node->setFiltering(QSGTexture::Linear);
    image_node->setRect(image.width() + 10*2, 10, image.width(), image.height());
    // image_node->setMipmapFiltering(QSGTexture::Linear);
    // image_node->setAnisotropyLevel(QSGTexture::Anisotropy16x);
    // Specifies the anisotropic filtering level to be used when the texture is not screen aligned.
    node->appendChildNode(image_node);
    node->image_node = image_node;

    auto text_document = new QTextDocument(); // Fixme: new
    {
      auto font = QFont("Roboto", 32);
      qInfo() << "Font is " << font << font.toString();
      text_document->setDefaultFont(font);
      QString text("# Hello World !\nIt is so **cool** to show text!\n* item1\n* item2");
      // MarkdownNoHTML MarkdownDialectCommonMark MarkdownDialectGitHub
      text_document->setMarkdown(text, QTextDocument::MarkdownDialectGitHub);
    }

    auto text_node = new QQuickTextNode(this); // using private Qt API
    {
      QPointF position(50, 50);
      QColor color(QLatin1String("black"));
      text_node->addTextDocument(position, text_document, color);
    }
    node->appendChildNode(text_node);
    node->text_node = text_node;
  }

  // Update geometry ?
  if (m_geometry_changed) {
    node->background->set_rect(rect);
    node->grid->set_rect(rect);
  }
  if (m_geometry_changed || m_samples_changed) {
    node->line->update_geometry(rect, m_samples);
    // We don't need to calculate the geometry twice, so just steal it from the other one...
    if (node->shadow)
      node->shadow->setGeometry(node->line->geometry());
  }

  m_geometry_changed = false;
  m_samples_changed = false;

  return node;
}
