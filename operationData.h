#ifndef OPERATIONDATA_H
#define OPERATIONDATA_H
#include <QColor>
#include <QImage>

class OperationData
{
public:
    enum DataType {Rectangle, Circle, Image};
    explicit OperationData(DataType type = Image, const QColor& color = Qt::red);
    DataType type() const;
    QString name() const;
    QColor color() const;

private:
    DataType m_type;
    QColor m_color;
    QPointF m_pos;
    QVector<QPointF> m_pts;
    QImage m_image;
    QString m_name;

};

#endif // OPERATIONDATA_H
