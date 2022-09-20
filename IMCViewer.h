#ifndef IMCVIEWER_H
#define IMCVIEWER_H

#include <QGraphicsView>
#include "IMCSence.h"



class IMCSence;
class QUndoStack;
class IMCViewer : public QGraphicsView
{
    Q_OBJECT

public:
    explicit IMCViewer(QWidget* parent = nullptr);
    virtual ~IMCViewer() override;

    void setCenterImage(const QImage& image);
    void setState(DrawType state);
    void setBrushColor(const QColor& cr);
    void setBrushSize(int sz);
    void setClearSize(int sz);
    void clearBrush();
    void addBox();
    void addCircle();
    void computeIMC();

    QAction * getUndoAct();

    QAction * getRedoAct();

    void updateCursor();
//    undoAction =
//    undoAction->setShortcuts(QKeySequence::Undo);

//    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
//    redoAction->setShortcuts(QKeySequence::Redo);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void togglePan(bool pan,const QPoint& startPos);
    void panTo(const QPoint& pos);

public slots:
    void onSceneRectChanged(const QRectF&rect);

private:
    IMCSence* imcscene;
    qreal m_minZoomRatio;

    bool m_isDarging;
    QPoint m_dragPrePoint;
};


#endif // IMCVIEWER_H
