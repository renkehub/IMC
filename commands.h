#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QGraphicsItem>
#include "IMCSence.h"

class CForgegroungImageItem;
class CGraphicsRectItem;
class CGraphicsCirCleItem;

class AddCommand : public QUndoCommand
{
public:
    AddCommand(IMCSence* graphicsScene, QGraphicsItem* item,
               QUndoCommand* parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;

private:
    QGraphicsItem* m_item;
    IMCSence* m_sence;
};


class RemoveCommand : public QUndoCommand
{
public:
    RemoveCommand(IMCSence* graphicsScene, QGraphicsItem* item,
               QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QGraphicsItem* m_item;
    IMCSence* m_sence;
};




class ImageModefyCommand: public QUndoCommand
{
public:
    ImageModefyCommand(CForgegroungImageItem* item, const QImage& img,
                       QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;
//    bool mergeWith(const QUndoCommand *command) override;
//    int id() const override { return Id; }

private:
    CForgegroungImageItem* m_item;
    QImage m_oldImage;
    QImage m_curImage;
};


class RectModefyCommand: public QUndoCommand
{
public:
    RectModefyCommand(CGraphicsRectItem* item, const QRectF& img,
                      QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;

private:
    CGraphicsRectItem* m_item;
    QRectF m_oldRect;
    QRectF m_curRect;
};

class CircleModefyCommand: public QUndoCommand
{
public:
    CircleModefyCommand(CGraphicsCirCleItem* item, const QRectF& img,
                      QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;

private:
    CGraphicsCirCleItem* m_item;
    QRectF m_oldRect;
    QRectF m_curRect;
};

#endif // COMMANDS_H
