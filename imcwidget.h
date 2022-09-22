#ifndef IMCWIDGET_H
#define IMCWIDGET_H

#include <QWidget>

namespace Ui {
class IMCWidget;
}

class IMCWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IMCWidget(QWidget* parent = nullptr);
    ~IMCWidget();
    bool eventFilter(QObject* target, QEvent* event) override;

private slots:
    void on_pushButton_load_clicked();

    void on_pushButton_pen_clicked();

    void on_pushButton_eraser_clicked();

    void on_pushButton_pan_clicked();

    void on_pushButton_color_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_addCircle_clicked();

    void on_pushButton_compute_clicked();
    void on_pushButton_clear_clicked();

    void on_pushButton_addBox_clicked();

private:
    Ui::IMCWidget* ui;
};

#endif // IMCWIDGET_H
