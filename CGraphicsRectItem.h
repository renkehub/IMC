#ifndef CGRAPHICSRECTITEM_H
#define CGRAPHICSRECTITEM_H
#include <QGraphicsRectItem>
#include <QFont>
#include <QCursor>

class CGraphicsRectItem :  public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum direction {TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT, TOP, LEFT, RIGHT, BOTTOM, INSIDE, OUTSIDE};

    explicit CGraphicsRectItem(const QRectF& rect, QGraphicsItem* parent = nullptr);
    ~CGraphicsRectItem();
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
    void setShowString(const QString& str)
    {
        prepareGeometryChange();
        m_showStr = str;
        update();
    }
    QPainterPath shape() const override;

    void setName(const QString& name){
        m_name = name;
    }

signals:
    void locChanged(const QRectF& rect);

private:
    qreal m_lod;
    qreal m_contralSize;
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
};

#endif // CGRAPHICSRECTITEM_H
