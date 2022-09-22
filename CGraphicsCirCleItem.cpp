#include "CGraphicsCirCleItem.h"
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "IMCSence.h"
#include <QDebug>
#include <QtMath>

CGraphicsCirCleItem::CGraphicsCirCleItem(const QRectF& rect, bool isCircle, QGraphicsItem* parent):
    QGraphicsRectItem(rect, parent),
    m_color(Qt::green),
    m_penWidth(2),
    m_lod(1.0),
    m_contralSize(5),
    m_direction(OUTSIDE),
    m_isMove(false),
    m_prePt(0, 0),
    m_isCircle(isCircle)
{
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
    m_font = QFont("Arial");
//    m_showStr = QObject::tr("Auto Expose Area");
}

CGraphicsCirCleItem::~CGraphicsCirCleItem()
{
}

void CGraphicsCirCleItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    IMCSence* imcScene =  dynamic_cast<IMCSence*>(scene());
    if (imcScene && imcScene->getState() == MODEFY_STATE)
    {
        setSelected(true);
        m_isMove = true;
        m_oldRect = this->rect();
        m_prePt = event->pos();
    }
    else
    {
        event->setAccepted(false);
        QGraphicsItem::mousePressEvent(event);
        setSelected(false);
    }

}

void CGraphicsCirCleItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    IMCSence* imcScene =  dynamic_cast<IMCSence*>(scene());
    if (imcScene && imcScene->getState() == MODEFY_STATE)
    {
        QPointF scenePos = event->pos();
        QPointF senceTl = this->scene()->sceneRect().topLeft();
        QPointF senceBr = this->scene()->sceneRect().bottomRight();
        if (scenePos.x() < senceTl.x())
        {
            scenePos.setX(senceTl.x());
        }
        if (scenePos.y() < senceTl.y())
        {
            scenePos.setY(senceTl.y());
        }
        if (scenePos.x() > senceBr.x())
        {
            scenePos.setX(senceBr.x());
        }
        if (scenePos.y() > senceBr.y())
        {
            scenePos.setY(senceBr.y());
        }

        if (m_isMove)
        {
            prepareGeometryChange();
            QPointF delta = scenePos - m_prePt;
            QRectF rt = this->rect();

            switch (m_direction)
            {
            case INSIDE:
            {
                rt = rt.translated(delta);
            }
            break;
            case TOP:
            {
                if (m_isCircle)
                {
                    qreal hei = rt.bottom() - scenePos.y();
                    qreal xPos = rt.right() - hei;
                    if (hei < 0)
                    {
                        xPos = rt.right() + hei;
                    }
                    rt.setTopLeft(QPointF(xPos, scenePos.y()));

                }
                else
                {
                    rt.setY(scenePos.y());
                }

            }
            break;
            case BOTTOM:
            {
                if (m_isCircle)
                {
                    qreal hei = scenePos.y() - rt.top();
                    qreal xPos = rt.left() + hei;
                    if (hei < 0)
                    {
                        xPos = rt.left() - hei;
                    }
                    rt.setBottomRight(QPointF(xPos, scenePos.y()));

                }
                else
                {
                    rt.setBottom(qMin(scenePos.y(), senceBr.y()));
                }
            }
            break;
            case LEFT:
            {
                if (m_isCircle)
                {
                    qreal wid =  rt.right() - scenePos.x();
                    qreal ypos = rt.top() + wid;
                    if (wid < 0)
                    {

                        ypos = rt.top() - wid;
                    }
                    rt.setBottomLeft(QPointF(scenePos.x(), ypos));

                }
                else
                {
                    rt.setLeft(qMax(scenePos.x(), senceTl.x()));
                }
            }
            break;
            case RIGHT:
            {
                if (m_isCircle)
                {
                    qreal wid =  scenePos.x() - rt.left();
                    qreal ypos = rt.top() + wid;
                    if (wid < 0)
                    {
                        ypos = rt.top() - wid;
                    }
                    rt.setBottomRight(QPointF(scenePos.x(), ypos));
                }
                else
                {
                    rt.setRight(qMin(scenePos.x(), senceBr.x()));
                }
            }
            break;

            case TOPLEFT:
            {
                if (m_isCircle)
                {
                    qreal hei = rt.bottom() - scenePos.y();
                    qreal xPos = rt.right() - hei;
                    rt.setTopLeft(QPointF(xPos, scenePos.y()));
                }
                else
                {
                    rt.setTopLeft(scenePos);
                }
            }
            break;
            case TOPRIGHT:
            {
                if (m_isCircle)
                {
                    qreal hei = rt.bottom() - scenePos.y();
                    qreal xPos = rt.left() + hei;
                    rt.setTopRight(QPointF(xPos, scenePos.y()));
                }
                else
                {
                    rt.setTopRight(scenePos);
                }
            }
            break;
            case BOTTOMLEFT:
            {
                if (m_isCircle)
                {
                    qreal hei = scenePos.y() - rt.top();
                    qreal xPos = rt.right() - hei;
                    rt.setBottomLeft(QPointF(xPos, scenePos.y()));
                }
                else
                {
                    rt.setBottomLeft(scenePos);
                }
            }
            break;
            case BOTTOMRIGHT:
            {
                if (m_isCircle)
                {
                    qreal hei = scenePos.y() - rt.top();
                    qreal xPos = rt.left() + hei;
                    rt.setBottomRight(QPointF(xPos, scenePos.y()));
                }
                else
                {
                    rt.setBottomRight(scenePos);
                }
            }
            break;
            default:
                break;
            }

            QPointF senceTlDelta = rt.topLeft() - senceTl;
            QPointF senceBrDelta = rt.bottomRight() - senceBr;

            if (senceTlDelta.x() < 0)
            {
                rt.translate(-senceTlDelta.x(), 0);
            }
            if (senceTlDelta.y() < 0)
            {
                rt.translate(0, -senceTlDelta.y());
            }
            if (senceBrDelta.x() > 0)
            {
                rt.translate(-senceBrDelta.x(), 0);
            }
            if (senceBrDelta.y() > 0)
            {
                rt.translate(0, -senceBrDelta.y());
            }
            this->setRect(rt);
            m_prePt = scenePos;
        }
        imcScene->computeIMC();
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(event);
    }

}

void CGraphicsCirCleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_isMove)
    {
        m_isMove = false;
        QRectF norRect = rect().normalized();
        emit locChanged(norRect);
        this->setRect(norRect);
        IMCSence* imcScene =  dynamic_cast<IMCSence*>(scene());
        if (imcScene)
        {
            imcScene->modefyItem(this, m_oldRect);
        }
    }
}

void CGraphicsCirCleItem::setDrawColor(const QColor& cr)
{
    m_color = cr;
}

QRectF CGraphicsCirCleItem::boundingRect() const
{
    QFontMetricsF fontMetrics(m_font);
    QRectF rect2 = fontMetrics.boundingRect(m_showStr).adjusted(-4, -4, 4, 4);
    QRectF itemRect = rect().normalized();
    QRectF textRect = QRectF(rect2.topLeft() / m_lod * 1.5, rect2.bottomRight() / m_lod * 1.5).translated(itemRect.center()).normalized();
    textRect.adjust(0, - rect2.width(), 0, 0);
    QRectF boundRect = itemRect.adjusted(-m_contralSize / m_lod, -m_contralSize / m_lod, m_contralSize / m_lod, m_contralSize / m_lod);
    return  textRect.united(boundRect).normalized();
}

void CGraphicsCirCleItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    m_preCursor = scene()->views()[0]->cursor();
    scene()->views()[0]->setCursor(Qt::SizeAllCursor);
}

void CGraphicsCirCleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    scene()->views()[0]->setCursor(m_preCursor);
    m_direction = direction::OUTSIDE;
}

bool CGraphicsCirCleItem::getHitStatus()
{
    if (m_direction == direction::OUTSIDE)
    {
        return  false;
    }
    return  true;
}

void CGraphicsCirCleItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    QPointF delta(m_contralSize / m_lod, m_contralSize / m_lod);
    QRectF rect = this->rect().normalized();
    QPointF scenePos = event->pos();

    QRectF topLeftContral((rect.topLeft() - delta), (rect.topLeft() + delta));
    QRectF topRightContral((rect.topRight() - delta), (rect.topRight() + delta));
    QRectF bottomLeftContral((rect.bottomLeft() - delta), (rect.bottomLeft() + delta));
    QRectF bottomRightContral((rect.bottomRight() - delta), (rect.bottomRight() + delta));

    QRectF topCenter((rect.topLeft() + rect.topRight()) / 2 - delta, (rect.topLeft() + rect.topRight()) / 2 + delta);
    QRectF rightCenter((rect.bottomRight() + rect.topRight()) / 2 - delta, (rect.bottomRight() + rect.topRight()) / 2 + delta);
    QRectF bottomCenter((rect.bottomRight() + rect.bottomLeft()) / 2 - delta, (rect.bottomRight() + rect.bottomLeft()) / 2 + delta);
    QRectF leftCenter((rect.topLeft() + rect.bottomLeft()) / 2 - delta, (rect.topLeft() + rect.bottomLeft()) / 2 + delta);

    if (topLeftContral.contains(scenePos))
    {
        scene()->views()[0]->setCursor(Qt::SizeFDiagCursor);
        m_direction = TOPLEFT;
    }
    else if (topRightContral.contains(scenePos))
    {
        scene()->views()[0]->setCursor(Qt::SizeBDiagCursor);
        m_direction = TOPRIGHT;
    }
    else if (bottomLeftContral.contains(scenePos))
    {
        scene()->views()[0]->setCursor(Qt::SizeBDiagCursor);
        m_direction = BOTTOMLEFT;
    }
    else if (bottomRightContral.contains(scenePos))
    {
        scene()->views()[0]->setCursor(Qt::SizeFDiagCursor);
        m_direction = BOTTOMRIGHT;
    }
    else if (topCenter.contains(scenePos))
    {
        scene()->views()[0]->setCursor(Qt::SizeVerCursor);
        m_direction = TOP;
    }
    else if (bottomCenter.contains(scenePos))
    {
        scene()->views()[0]->setCursor(Qt::SizeVerCursor);
        m_direction = BOTTOM;
    }
    else if (leftCenter.contains(scenePos))
    {
        scene()->views()[0]->setCursor(Qt::SizeHorCursor);
        m_direction = LEFT;
    }
    else if (rightCenter.contains(scenePos))
    {
        scene()->views()[0]->setCursor(Qt::SizeHorCursor);
        m_direction = RIGHT;
    }
    else
    {
        scene()->views()[0]->setCursor(Qt::SizeAllCursor);
        m_direction = INSIDE;
    }

}

void CGraphicsCirCleItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                                QWidget* widget)
{
    QRectF rect = this->rect().normalized();

    m_lod = option->levelOfDetailFromTransform(painter->worldTransform());


    QPainterPath circle;
    circle.addEllipse(rect);
    painter->setPen(QPen(QBrush(m_color), m_penWidth / m_lod, Qt::DotLine));
    painter->drawPath(circle);

    QPointF delta(m_contralSize / m_lod, m_contralSize / m_lod);



    if (isSelected())
    {
        painter->setPen(QPen(QBrush(Qt::black), m_penWidth / m_lod, Qt::SolidLine));
        painter->drawRect(rect);

        painter->setPen(QPen(QBrush(Qt::white), m_penWidth / m_lod, Qt::DotLine));
        painter->drawRect(rect);

        QRectF topLeftContral((rect.topLeft() - delta), (rect.topLeft() + delta));
        QRectF topRightContral((rect.topRight() - delta), (rect.topRight() + delta));
        QRectF bottomLeftContral((rect.bottomLeft() - delta), (rect.bottomLeft() + delta));
        QRectF bottomRightContral((rect.bottomRight() - delta), (rect.bottomRight() + delta));

        QRectF topCenter((rect.topLeft() + rect.topRight()) / 2 - delta, (rect.topLeft() + rect.topRight()) / 2 + delta);
        QRectF rightCenter((rect.bottomRight() + rect.topRight()) / 2 - delta, (rect.bottomRight() + rect.topRight()) / 2 + delta);
        QRectF bottomCenter((rect.bottomRight() + rect.bottomLeft()) / 2 - delta, (rect.bottomRight() + rect.bottomLeft()) / 2 + delta);
        QRectF leftCenter((rect.topLeft() + rect.bottomLeft()) / 2 - delta, (rect.topLeft() + rect.bottomLeft()) / 2 + delta);

        painter->setPen(QPen(QBrush(Qt::white), m_penWidth / m_lod));

        QVector<QRectF> rects{topLeftContral, topRightContral, bottomLeftContral, bottomRightContral,
                              topCenter, rightCenter, bottomCenter, leftCenter};
        painter->drawRects(rects);

    }
    if (!m_showStr.isEmpty())
    {
        painter->save();
        double scaleValue = 1.5 / m_lod;
        painter->scale(scaleValue, scaleValue);
        QPointF textPos = rect.center() / scaleValue;
        textPos.setY(textPos.y() - 4);

        QPainterPath textpath;
        textpath.addText(textPos, m_font, m_showStr);
//        QFontMetricsF fontMetrics(m_font);
//        QRectF rect2 = fontMetrics.boundingRect(m_showStr).adjusted(-4, -4, 4, 4);
//        textpath.translate(rect2.center()*-1);

        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(QColor(0, 0, 0, 75)));
        painter->drawRect(textpath.boundingRect().adjusted(-2, -2, 2, 2));
        painter->setBrush(QBrush(m_color));
        painter->setBrush(m_color);
        painter->drawPath(textpath);
        painter->restore();
    }
}

QPainterPath CGraphicsCirCleItem::shape() const
{
    QPainterPath path;
    path.addEllipse(this->rect());
    return path;
}

bool CGraphicsCirCleItem::contains(const QPointF& point) const
{
    return rect().contains(point);
}

bool CGraphicsCirCleItem::collidesWithPath(const QPainterPath& path, Qt::ItemSelectionMode mode) const
{
    return contains(path.currentPosition());
}

bool CGraphicsCirCleItem::isCicle()
{
    return m_isCircle;
}

void CGraphicsCirCleItem::setCircle(bool isCircle)
{
    m_isCircle = isCircle;
}
