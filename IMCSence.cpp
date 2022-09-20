#include "IMCSence.h"
#include <QGraphicsPixmapItem>
#include "CForgegroungImageItem.h"
#include <QUndoCommand>
#include "commands.h"
#include <QUndoStack>
#include <QKeyEvent>
#include <QDebug>
#include "CGraphicsRectItem.h"
#include "CGraphicsCirCleItem.h"

IMCSence::IMCSence(QObject* parent):
    QGraphicsScene(parent),
    m_imageItem(nullptr),
    m_maskItem(new CForgegroungImageItem),
    m_undoStack(new QUndoStack(this)),
    m_state(PAN)
{
    m_undoStack->setUndoLimit(20);
    addItem(m_maskItem);
    m_maskItem->setZValue(30);
}


IMCSence::~IMCSence()
{

}

void IMCSence::setCenterImage(const QImage& image)
{
    QPixmap pix = QPixmap::fromImage(image);
    setCenterImage(pix);
}

void IMCSence::setCenterImage(const QPixmap& pixmap)
{
    setSceneRect(pixmap.rect());
    if (m_imageItem == nullptr)
    {
        m_imageItem = new QGraphicsPixmapItem;
        m_imageItem->setZValue(20);
        addItem(m_imageItem);
    }
    m_imageItem->setPixmap(pixmap);

    m_maskItem->setSize(pixmap.size());

}

void IMCSence::setState(DrawType state)
{
//    PAN,
//    PEN_STATE,
//    CLEAR_STATE,
//    MODEFY_STATE
    switch (state)
    {
    case PEN_STATE:
    case CLEAR_STATE:
    {
        for (QGraphicsItem* item : m_showItems)
        {
            item->setAcceptHoverEvents(false);
        }
        break;
    }
    case MODEFY_STATE:
    {
        for (QGraphicsItem* item : m_showItems)
        {
            item->setAcceptHoverEvents(true);
        }
        break;
    }
    }
    m_state = state;
}

DrawType IMCSence::getState()
{
    return m_state;
}

void IMCSence::setBrushColor(const QColor& cr)
{
    m_maskItem->setBrushColor(cr);
}

void IMCSence::setBrushSize(int sz)
{
    m_maskItem->setBrushSize(sz);
}

void IMCSence::setClearSize(int sz)
{
    m_maskItem->setClearSize(sz);
}

int IMCSence::getBrushSize()
{
    return m_maskItem->getBrushSize();
}


int IMCSence::getClearSize()
{
    return m_maskItem->getClearSize();
}


QColor IMCSence::getBrushColor()
{
    return m_maskItem->getBrushColor();
}


void IMCSence::clearBrush()
{
    if (m_maskItem)
    {
        m_maskItem->clearBrush();
    }
}

QImage IMCSence::getForgegroundImage()
{
    QImage img;
    if (m_maskItem)
    {
        img = m_maskItem->image();
    }
    return img;
}


void IMCSence::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event);
}

void IMCSence::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseReleaseEvent(event);

}

void IMCSence::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseMoveEvent(event);
}

void IMCSence::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Z && event->modifiers() == Qt::ControlModifier)
    {
        m_undoStack->undo();
    }
    if (event->key() == Qt::Key_Y && event->modifiers() == Qt::ControlModifier)
    {
        m_undoStack->redo();
    }
    if (event->key() == Qt::Key_Space)
    {
        m_maskItem->setVisible(!m_maskItem->isVisible());
    }
    if (event->key() == Qt::Key_Delete)
    {
        for (QGraphicsItem* item : m_showItems)
        {
            if (item->isSelected())
            {
                m_undoStack->push(new RemoveCommand(this, item));
            }
        }
    }
}

void IMCSence::modefyItem(QGraphicsItem* item, const QRectF& rect)
{
    CGraphicsRectItem*  rectItem =  dynamic_cast<CGraphicsRectItem*>(item);
    if (rectItem)
    {
        m_undoStack->push(new RectModefyCommand(rectItem, rect));
        return;
    }
    CGraphicsCirCleItem*  circleItem =  dynamic_cast<CGraphicsCirCleItem*>(item);
    if (circleItem)
    {
        m_undoStack->push(new CircleModefyCommand(circleItem, rect));
        return;
    }

}

void IMCSence::modefyItem(QGraphicsItem* item, const QImage& img)
{
    CForgegroungImageItem*  imgItem =  dynamic_cast<CForgegroungImageItem*>(item);
    if (imgItem)
    {
        m_undoStack->push(new ImageModefyCommand(imgItem, img));
        return;
    }
}

QUndoStack* IMCSence::getUndoStack()
{
    return m_undoStack;
}

void IMCSence::addBox()
{
    CGraphicsRectItem* rectItem = new CGraphicsRectItem(QRectF(50, 50, 100, 100));
    rectItem->setZValue(50);
    rectItem->setSelected(true);
    QUndoCommand* addCommand = new AddCommand(this, rectItem);
    m_undoStack->push(addCommand);
}

void IMCSence::addCircle()
{
    CGraphicsCirCleItem* circleItem = new CGraphicsCirCleItem(QRectF(50, 50, 100, 100));
    circleItem->setZValue(50);
    circleItem->setSelected(true);
    QUndoCommand* addCommand = new AddCommand(this, circleItem);
    m_undoStack->push(addCommand);
}

void IMCSence::addBoundItem(QGraphicsItem* item)
{
    m_showItems.append(item);
}

void IMCSence::removeBoundItem(QGraphicsItem* item)
{
    m_showItems.removeOne(item);
}

QVector<QGraphicsItem*> IMCSence::getBoundItems()
{
    return m_showItems;
}


void IMCSence::computeIMC()
{
    for (auto item : m_showItems)
    {

        qreal imc = m_maskItem->getPathIMC(item->shape());
        QString showString = QString("IMC: ") +  QString::number(imc,'f',2) + QString("%");
        CGraphicsCirCleItem* circleItem = dynamic_cast<CGraphicsCirCleItem*>(item);
        if (circleItem)
        {
            circleItem->setShowString(showString);
        }

        CGraphicsRectItem* ritem = dynamic_cast<CGraphicsRectItem*>(item);
        if (ritem)
        {
            ritem->setShowString(showString);
        }

    }
}

