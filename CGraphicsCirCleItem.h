#ifndef CGRAPHICSCIRCLEITEM_H
#define CGRAPHICSCIRCLEITEM_H

#include <QFont>
#include <QCursor>
#include <QGraphicsRectItem>

class CGraphicsCirCleItem :  public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum direction {TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT, TOP, LEFT, RIGHT, BOTTOM, INSIDE, OUTSIDE};

    explicit CGraphicsCirCleItem(const QRectF& rect,bool isCircle = true, QGraphicsItem* parent = nullptr);
    ~CGraphicsCirCleItem();
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    void setDrawColor(const QColor& cr);
    void setPenWidth(qreal wid)
    {
        m_penWidth = wid;
    }
    QRectF boundingRect() const override;
    bool getHitStatus();
    bool isCicle();
    void setCircle(bool isCircle);
    void setShowString(const QString& str)
    {
        prepareGeometryChange();
        m_showStr = str;
        update();
    }
    void setName(const QString& name){
        prepareGeometryChange();
        m_name = name;
        update();
    }
    QPainterPath shape() const override;

    bool contains(const QPointF& point) const override;
    bool collidesWithPath(const QPainterPath& path, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const override;


signals:
    void locChanged(const QRectF& rect);

private:
    qreal m_lod;
    qreal m_contralSize;
    bool m_isCircle;
    QColor m_color;
    qreal m_penWidth;
    QFont m_font;
    bool m_isMove;
    direction m_direction;
    QPointF m_prePt;
    QCursor m_preCursor;
    QString m_showStr;
    QRectF m_oldRect; //用于撤销
    QString m_name = "IMC";
    QRectF m_textRect;
};

#endif // CGRAPHICSCIRCLEITEM_H
