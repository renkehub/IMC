#ifndef CFORGEGROUNGIMAGEITEM_H
#define CFORGEGROUNGIMAGEITEM_H

#include <QGraphicsObject>
#include <QImage>


class CForgegroungImageItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit CForgegroungImageItem(QGraphicsItem* parent = nullptr);
    virtual ~CForgegroungImageItem();
    void setImage(const QImage& img);
    QImage image() const;

    QRectF boundingRect() const override;
    void setSize(const QSize& sz);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    void setBrushColor(const QColor& cr)
    {
        m_brushColor = cr;
    }

    void setBrushSize(int sz)
    {
        if (sz == 0)
        {
            return;
        }
        m_brushSize = sz;
    }

    int getBrushSize(){
        return m_brushSize;
    }

    int getClearSize(){
        return m_clearSize;
    }

    QColor getBrushColor()
    {
        return m_brushColor;
    }

    void setClearSize(int sz)
    {
        if (sz == 0)
        {
            return;
        }
        m_clearSize = sz;
    }
    qreal getPathIMC(const QPainterPath& path);

    void clearBrush();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QImage m_image;
    QImage m_oldImage;
    QPointF m_lastPos;
    bool m_isDrawing;

    QColor m_brushColor;
    int m_brushSize;
    int m_clearSize;

};

#endif // CFORGEGROUNGIMAGEITEM_H
