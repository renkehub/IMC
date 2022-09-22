#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QProcess>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QButtonGroup>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//bool MainWindow::eventFilter(QObject* target, QEvent* event)
//{
//    if (event->type() == QEvent::KeyPress)
//    {
//        QApplication::sendEvent(ui->graphicsView, event);
//    }
//    return QMainWindow::eventFilter(target, event);
//}

