#include "commands.h"
#include "CForgegroungImageItem.h"
#include "CGraphicsRectItem.h"
#include "CGraphicsCirCleItem.h"
#include <QDebug>

AddCommand::AddCommand(IMCSence* scene, QGraphicsItem* item,
                       QUndoCommand* parent ):
    QUndoCommand(parent),
    m_sence(scene)
{
    m_item = item;
    scene->update();
}

AddCommand::~AddCommand()
{
    if (!m_item->scene())
        delete m_item;
}

void AddCommand::undo()
{
    m_sence->removeBoundItem(m_item);
    m_sence->removeItem(m_item);
    m_sence->update();
}

void AddCommand::redo()
{
    m_sence->addBoundItem(m_item);
    m_sence->addItem(m_item);
//    m_sence->clearSelection();
    m_sence->update();
}


RemoveCommand::RemoveCommand(IMCSence* scene, QGraphicsItem* item,
                             QUndoCommand* parent ):
    QUndoCommand(parent),
    m_sence(scene)
{
    m_item = item;
    scene->update();
}


void RemoveCommand::undo()
{
    m_sence->addBoundItem(m_item);
    m_item->setVisible(true);
//    m_sence->addItem(m_item);
//    m_sence->clearSelection();
    m_sence->update();
}

void RemoveCommand::redo()
{
    m_sence->removeBoundItem(m_item);
//    m_sence->removeItem(m_item);
    m_item->setVisible(false);
    m_item->update();
    m_sence->update();
}

ImageModefyCommand::ImageModefyCommand(CForgegroungImageItem* item, const QImage& img,
                                       QUndoCommand* parent):
    QUndoCommand(parent),
    m_item(item),
    m_oldImage(img)
{
    m_curImage = m_item->image();
}

void ImageModefyCommand::undo()
{
    m_item->updateAlphaChannel(m_oldImage);
    m_item->setImage(m_oldImage);
}

void ImageModefyCommand::redo()
{
    m_item->updateAlphaChannel(m_curImage);
    m_item->setImage(m_curImage);
}


RectModefyCommand::RectModefyCommand(CGraphicsRectItem* item, const QRectF& rect, QUndoCommand* parent):
    QUndoCommand(parent),
    m_item(item),
    m_oldRect(rect)
{
    m_curRect = m_item->rect();
}


void RectModefyCommand::undo()
{
    m_item->setRect(m_oldRect);
}

void RectModefyCommand::redo()
{
    m_item->setRect(m_curRect);
}



CircleModefyCommand::CircleModefyCommand(CGraphicsCirCleItem* item, const QRectF& rect, QUndoCommand* parent):
    QUndoCommand(parent),
    m_item(item),
    m_oldRect(rect)
{
    m_curRect = m_item->rect();
}


void CircleModefyCommand::undo()
{
    m_item->setRect(m_oldRect);
}

void CircleModefyCommand::redo()
{
    m_item->setRect(m_curRect);
}


