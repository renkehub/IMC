#include "IMCViewer.h"
#include "IMCSence.h"
#include <QWheelEvent>
#include <QScrollBar>
#include <QUndoStack>
#include <QDebug>
#include "commands.h"
#include "CForgegroungImageItem.h"

IMCViewer::IMCViewer(QWidget* parent):
    QGraphicsView(parent),
    imcscene(nullptr),
    m_minZoomRatio(0.1),
    m_isDarging(false),
    m_dragPrePoint(0, 0)
{
    imcscene = new IMCSence;
    setScene(imcscene);
    connect(imcscene, SIGNAL(sceneRectChanged(const QRectF&)), this, SLOT(onSceneRectChanged(const QRectF&)));
    setMouseTracking(true);
}


IMCViewer::~IMCViewer()
{

}


void IMCViewer::setCenterImage(const QImage& image)
{
    imcscene->setCenterImage(image);
}

void IMCViewer::mousePressEvent(QMouseEvent* event)
{
    if (imcscene->getState() == PAN || (event->buttons() & Qt::MiddleButton) || (event->buttons() & Qt::RightButton))
    {
        togglePan(true, event->pos());
        event->accept();
    }
    else
    {
        QGraphicsView::mousePressEvent(event);
    }

}

void IMCViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (imcscene->getState() == PAN || (event->button() & Qt::MiddleButton) || (event->button() & Qt::RightButton))
    {
        togglePan(false, event->pos());
        event->accept();
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }

}

void IMCViewer::mouseMoveEvent(QMouseEvent* event)
{
    if (imcscene->getState() == PAN || event->buttons() & Qt::MiddleButton || (event->buttons() & Qt::RightButton))
    {
        panTo(event->pos());
        event->accept();
    }
    else
    {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void IMCViewer::setState(DrawType state)
{
    imcscene->setState(state);
    updateCursor();
}

void IMCViewer::updateCursor()
{
    QColor curColor = imcscene->getBrushColor();
    if (imcscene->getState() == PAN || imcscene->getState() == MODEFY_STATE)
    {
        setCursor(Qt::ArrowCursor);
        return;
    }
    else if (imcscene->getState() == PEN_STATE)
    {
        QPixmap pixmap(QSize(1, 1)* imcscene->getBrushSize()*transform().m11());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setBrush(curColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(pixmap.rect());
        painter.end();
        QCursor cursor(pixmap);
        setCursor(cursor);
    }
    else if (imcscene->getState() == CLEAR_STATE)
    {
        QPixmap pixmap(QSize(1, 1)* imcscene->getClearSize()*transform().m11());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(Qt::white);
        painter.drawRect(pixmap.rect());
        painter.end();
        QCursor cursor(pixmap);
        setCursor(cursor);
    }

}

void IMCViewer::resizeEvent(QResizeEvent* event)
{
    QRectF srect = sceneRect();
    qreal widRatio = viewport()->width() * 0.6 / srect.width();
    qreal heiRatio = viewport()->height() * 0.6 / srect.height();
    m_minZoomRatio = qMin(widRatio, heiRatio);
    QGraphicsView::resizeEvent(event);
}

void IMCViewer::onSceneRectChanged(const QRectF& rect)
{
    qreal widRatio = viewport()->width() * 0.6 / rect.width();
    qreal heiRatio = viewport()->height() * 0.6 / rect.height();
    m_minZoomRatio = qMin(widRatio, heiRatio);
}

void IMCViewer::wheelEvent(QWheelEvent* event)
{
    int wheelDelta = event->delta();
    if(event->modifiers() == Qt::ControlModifier)
    {
        int valStep = event->angleDelta().y() / 8 / 15;
        emit valChanged(imcscene->getState(),valStep);
        return;
    }
    QPointF curPos = event->pos();
    QPointF scenePos = this->mapToScene(curPos.toPoint());
    qreal viewWidth = this->viewport()->width();
    qreal viewHeight = this->viewport()->height();

    qreal hScale = curPos.x() / viewWidth;
    qreal vScale = curPos.y() / viewHeight;



    qreal scaleVal = transform().m11();
    if (wheelDelta > 0)
    {
        if (scaleVal * 1.2 > 5.0)
        {
            scale(5.0 / scaleVal, 5.0 / scaleVal);
        }
        else
        {
            scale(1.2, 1.2);
        }
    }
    else
    {
        if (scaleVal / 1.2 < m_minZoomRatio)
        {
            scale(m_minZoomRatio / scaleVal, m_minZoomRatio / scaleVal);
        }
        else
        {
            scale(0.8, 0.8);
        }
    }

    QPointF viewPointPos = this->transform().map(scenePos);
    horizontalScrollBar()->setValue(int(viewPointPos.x() - viewWidth * hScale));
    verticalScrollBar()->setValue(int(viewPointPos.y() - viewHeight * vScale));
    update();
    updateCursor();
}

void IMCViewer::togglePan(bool pan, const QPoint& startPos)
{
    if (pan)
    {
        if (m_isDarging)
        {
            return;
        }
        m_isDarging = true;
        m_dragPrePoint = startPos;
        m_cursor = cursor();
        setCursor(Qt::ClosedHandCursor);
    }
    else
    {
        if (m_dragPrePoint != QPoint(0, 0))
        {
            QScrollBar* hBar = horizontalScrollBar();
            QScrollBar* vBar = verticalScrollBar();
            QPoint delta = startPos - m_dragPrePoint;
            hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
            vBar->setValue(vBar->value()  - delta.y());

        }
        if (!m_isDarging)
        {
            return;
        }
        m_isDarging = false;
        m_dragPrePoint = QPoint(0, 0);
        setCursor(m_cursor);
    }
}

void IMCViewer::panTo(const QPoint& pos)
{
    if (m_isDarging)
    {
        QScrollBar* hBar = horizontalScrollBar();
        QScrollBar* vBar = verticalScrollBar();
        QPoint delta = pos - m_dragPrePoint;
        m_dragPrePoint = pos;
        hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
        vBar->setValue(vBar->value() - delta.y());
    }
    update();
}


void IMCViewer::setBrushColor(const QColor& cr)
{
    if (imcscene)imcscene->setBrushColor(cr);
    updateCursor();
}

void IMCViewer::setBrushSize(int sz)
{
    if (imcscene)imcscene->setBrushSize(sz);
    updateCursor();
}

void IMCViewer::setClearSize(int sz)
{
    if (imcscene)imcscene->setClearSize(sz);
    updateCursor();
}

void IMCViewer::clearBrush()
{
    if (imcscene)imcscene->clearBrush();
}

void IMCViewer::addBox()
{
    QRect viewCenter = viewport()->rect();
    QRectF senceRect =  mapToScene(viewCenter).boundingRect();

    qreal length = qMin(senceRect.width(), senceRect.height()) / 3;
    QPointF tl = QPointF(senceRect.center().x() - length / 2, senceRect.center().y() - length / 2);
    QRectF itemRect(tl, QSizeF(length, length));
    if (imcscene)imcscene->addBox(itemRect);
}

void IMCViewer::addCircle()
{
    QRect viewCenter = viewport()->rect();
    QRectF senceRect =  mapToScene(viewCenter).boundingRect();

    qreal length = qMin(senceRect.width(), senceRect.height()) / 3;
    QPointF tl = QPointF(senceRect.center().x() - length / 2, senceRect.center().y() - length / 2);
    QRectF itemRect(tl, QSizeF(length, length));
    if (imcscene)imcscene->addCircle(itemRect);
}

QAction* IMCViewer::getUndoAct()
{
    return imcscene->getUndoStack()->createUndoAction(this, tr("&Undo"));
}

QAction* IMCViewer::getRedoAct()
{
    return imcscene->getUndoStack()->createRedoAction(this, tr("&Redo"));
}

void IMCViewer::computeIMC()
{
    imcscene->computeIMC();
}

void IMCViewer::thresIMC(int lhs, int rhs, bool isMove)
{
    imcscene->thresIMC(lhs, rhs, isMove);
}

void  IMCViewer::setMaskOpy(qreal opy)
{
    imcscene->setMaskOpy(opy);
}

void IMCViewer::updateCurrentItemName(const QString& name)
{
    imcscene->updateCurrentItemName(name);
    imcscene->update();
}


