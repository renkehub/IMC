#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool eventFilter(QObject *target, QEvent *event) override;

private slots:
    void on_pushButton_load_clicked();

    void on_pushButton_pen_clicked();

    void on_pushButton_eraser_clicked();

    void on_pushButton_pan_clicked();

    void on_pushButton_color_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_addCircle_clicked();

    void on_pushButton_compute_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
