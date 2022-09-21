#include "CForgegroungImageItem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtDebug>
#include <QGraphicsSceneMouseEvent>
#include "IMCSence.h"


CForgegroungImageItem::CForgegroungImageItem(QGraphicsItem* parent):
    QGraphicsObject(parent),
    m_isDrawing(false),
    m_brushColor(Qt::black),
    m_brushSize(2),
    m_clearSize(10)
{
    QGraphicsEllipseItem ff;
}


CForgegroungImageItem::~CForgegroungImageItem()
{

}


QImage CForgegroungImageItem::image() const
{
    return m_image;
}

void CForgegroungImageItem::setSize(const QSize& sz)
{
    m_image = QImage(sz, QImage::Format_Alpha8);
//    m_image = QImage(sz, QImage::Format_ARGB32);
    m_image.fill(Qt::transparent);
}

QRectF CForgegroungImageItem::boundingRect() const
{
    if (m_image.isNull())return QRectF();
    return QRectF(QPointF(0, 0), m_image.size() / m_image.devicePixelRatioF());
}


void CForgegroungImageItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if (m_image.isNull())
    {
        return;
    }
    painter->drawImage(option->exposedRect, m_image, m_image.rect());
}


void CForgegroungImageItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        IMCSence* imcScene =  dynamic_cast<IMCSence*>(scene());
        if (imcScene)
        {
            m_isDrawing = true;
            m_lastPos = event->pos();
            m_oldImage = m_image;
            QPainter painter(&m_image);
            if (imcScene->getState() == CLEAR_STATE )
            {

                painter.setPen(QPen(m_brushColor, m_clearSize, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
                painter.save();
                painter.setCompositionMode(QPainter::CompositionMode_Clear);
                painter.drawLine(m_lastPos, event->pos());
                painter.restore();
            }
            else if (imcScene->getState() == PEN_STATE)
            {
                QPainterPath clipPath;
                for (QGraphicsItem* bitem : imcScene->getBoundItems())
                {
                    clipPath.addPath(bitem->shape());
                }
                if (!clipPath.isEmpty())
                {
                    painter.setClipPath(clipPath);
                }
                painter.setPen(QPen(m_brushColor, m_brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.drawPoint(event->pos());
            }
            else
            {
                QGraphicsItem::mousePressEvent(event);
            }
            painter.end();
            update();
        }
        else
        {
            QGraphicsItem::mousePressEvent(event);
        }
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}


void CForgegroungImageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isDrawing = false;
        IMCSence* imcScene =  dynamic_cast<IMCSence*>(scene());
        if (imcScene)
        {
            imcScene->modefyItem(this, m_oldImage);
        }
        update();

    }
}

void CForgegroungImageItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    IMCSence* imcScene =  dynamic_cast<IMCSence*>(scene());
    if (imcScene)
    {
        if (event->buttons() & Qt::LeftButton && m_isDrawing)
        {
            QPainter painter(&m_image);

            if (imcScene->getState() == CLEAR_STATE)
            {
                painter.setPen(QPen(m_brushColor, m_clearSize, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
                painter.save();
                painter.setCompositionMode(QPainter::CompositionMode_Clear);
                painter.drawLine(m_lastPos, event->pos());
                painter.restore();
            }
            else if (imcScene->getState() == PEN_STATE)
            {
                QPainterPath clipPath;
                for (QGraphicsItem* bitem : imcScene->getBoundItems())
                {
                    clipPath.addPath(bitem->shape());
                }
                if (!clipPath.isEmpty())
                {
                    painter.setClipPath(clipPath);
                }
                painter.setPen(QPen(m_brushColor, m_brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.drawLine(m_lastPos, event->pos());
            }
            painter.end();
            m_lastPos = event->pos();
            update();
        }
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(event);
    }

}

void CForgegroungImageItem::clearBrush()
{
    if (!m_image.isNull())
    {
        prepareGeometryChange();
        m_image.fill(Qt::transparent);
        update();
    }
}

void CForgegroungImageItem::setImage(const QImage& img)
{
    m_image = img;
    update();
}

qreal CForgegroungImageItem::getPathIMC(const QPainterPath& path)
{
    QRect pathBr = path.boundingRect().toRect();
    QImage img(pathBr.size(), QImage::Format_Grayscale8);
    img.fill(128);
    QPainter painter(&img);
    QPainterPath cp = path.translated(pathBr.topLeft() * -1);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white));
    painter.drawPath(cp);

    painter.setClipPath(cp);
    painter.drawImage(img.rect(), m_image, pathBr);
    unsigned int num = 0;
    unsigned int imc = 0;
    for (int row = 0; row < img.height(); row++)
    {
        for (int col = 0; col < img.width(); col++)
            if (img.scanLine(row)[col] == 128)
            {
                continue;
            }
            else if (img.scanLine(row)[col] == 255)
            {
                imc++;
            }
            else
            {
                num++;
            }
    }
    qreal imcPercentage = 1.0 - num / qreal(imc + num);
    return imcPercentage * 100;
}

void CForgegroungImageItem::updatePath(const QPainterPath& path,const QImage& img)
{
    IMCSence* imcScene =  dynamic_cast<IMCSence*>(scene());
    if (imcScene)
    {
        QPainter painter(&m_image);
        if (!path.isEmpty())
        {
            painter.setClipPath(path);
        }
        painter.setPen(Qt::NoPen);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.drawImage(path.boundingRect(),img,img.rect());
        painter.end();
        update();
    }
}
