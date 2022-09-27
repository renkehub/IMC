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
    m_centerImage = image;
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
    m_undoStack->clear();
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
    computeIMC();
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
        computeIMC();
    }
    if (event->key() == Qt::Key_Y && event->modifiers() == Qt::ControlModifier)
    {
        m_undoStack->redo();
        computeIMC();
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

void IMCSence::addBox(const QRectF &boundRect)
{
    CGraphicsRectItem* rectItem = new CGraphicsRectItem(boundRect);
    rectItem->setZValue(50);
    rectItem->setSelected(true);
    QUndoCommand* addCommand = new AddCommand(this, rectItem);
    m_undoStack->push(addCommand);
}

void IMCSence::addCircle(const QRectF &boundRect)
{
    CGraphicsCirCleItem* circleItem = new CGraphicsCirCleItem(boundRect);
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

void IMCSence::thresIMC(int lhs,int rhs,bool isMove)
{
    for (auto item : m_showItems)
    {
        QRect rect = item->shape().boundingRect().toRect().normalized();
        QImage retImage = m_centerImage.copy(rect).convertToFormat(QImage::Format_Grayscale8);
        unsigned char* buffer = retImage.bits();
        QImage alpImage(retImage.size(),QImage::Format_Alpha8);
        unsigned char* albuffer = alpImage.bits();
        for(int row=0;row<retImage.height();row++)
        {
            for(int col = 0;col<retImage.width();col++)
            {
                unsigned char val = buffer[row*retImage.bytesPerLine() + col];
                if(val>lhs && val<rhs){
                   albuffer[row*retImage.bytesPerLine() + col] = 255;
                }
                else{
                    albuffer[row*retImage.bytesPerLine() + col] = 0;
                }
            }
        }
        m_maskItem->updatePath(item->shape(),alpImage,isMove);
    }
    computeIMC();
}

void IMCSence::setMaskOpy(qreal opy)
{
    m_maskItem->setoOacity(opy);
}

