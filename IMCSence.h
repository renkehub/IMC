#ifndef IMCSENCE_H
#define IMCSENCE_H

#include <QGraphicsScene>
class QGraphicsPixmapItem;
class CForgegroungImageItem;
class QUndoStack;


enum DrawType
{
    PAN,
    PEN_STATE,
    CLEAR_STATE,
    MODEFY_STATE
};

class IMCSence : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit IMCSence(QObject* parent = Q_NULLPTR);
    virtual ~IMCSence();
    void setCenterImage(const QImage& image);
    void setCenterImage(const QPixmap& pixmap);
    void setState(DrawType state);
    DrawType getState();

    void setBrushColor(const QColor& cr);
    void setBrushSize(int sz);
    void setClearSize(int sz);
    void addBoundItem(QGraphicsItem* item);
    void removeBoundItem(QGraphicsItem* item);
    QVector<QGraphicsItem*> getBoundItems();

    int getBrushSize();
    int getClearSize();
    QColor getBrushColor();

    void clearBrush();
    void addBox();
    void addCircle();
    void modefyItem(QGraphicsItem* item,const QRectF& rect);
    void modefyItem(QGraphicsItem* item,const QImage& img);
    QUndoStack* getUndoStack();
//    QGraphicsItem* createItem(DrawType type,QVector<QPointF>& pts);
    QImage getForgegroundImage();
    void computeIMC();

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;


signals:
    void backgroundChanged(const QImage& img, CForgegroungImageItem* item);

private:
    QGraphicsPixmapItem* m_imageItem;
    CForgegroungImageItem* m_maskItem;
    QImage m_oldImage;
    QUndoStack* m_undoStack;
    DrawType m_state;
    QVector<QGraphicsItem*> m_showItems;
};


#endif // IMCSENCE_H
