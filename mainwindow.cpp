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
    QObject::connect(ui->spinBox_brush, SIGNAL(valueChanged(int)), ui->horizontalSlider_brush, SLOT(setValue(int)));  //连接spinbox和slider
    QObject::connect(ui->horizontalSlider_brush,  &QSlider::valueChanged, [&](int val)
    {
        ui->spinBox_brush->setValue(val);
        ui->graphicsView->setBrushSize(val);
    });

    QObject::connect(ui->spinBox_eraser, SIGNAL(valueChanged(int)), ui->horizontalSlider_eraser, SLOT(setValue(int)));
    QObject::connect(ui->horizontalSlider_eraser, &QSlider::valueChanged, [&](int val)
    {
        ui->spinBox_eraser->setValue(val);
        ui->graphicsView->setClearSize(val);
    });
    ui->horizontalRangeSlider->setValues(0,255);
    ui->spinBox_lhs->setValue(0);
    ui->spinBox_rhs->setValue(255);
    ui->horizontalRangeSlider->setRange(0,255);

    connect(ui->horizontalRangeSlider, &CRangeSlider::valuesChanged, [&](int min, int max)
    {
        ui->spinBox_lhs->blockSignals(true);
        ui->spinBox_rhs->blockSignals(true);
        ui->spinBox_lhs->setValue(min);
        ui->spinBox_rhs->setValue(max);
        ui->spinBox_lhs->blockSignals(false);
        ui->spinBox_rhs->blockSignals(false);
        ui->graphicsView->thresIMC(min,max);
    });
    connect(ui->spinBox_lhs, QOverload<int>::of(&QSpinBox::valueChanged), [&](int val)
    {
        if (val < ui->horizontalRangeSlider->maximumPosition())
        {
            ui->horizontalRangeSlider->setMinimumPosition(val);
        }
    });

    connect(ui->spinBox_rhs, QOverload<int>::of(&QSpinBox::valueChanged), [&](int val)
    {
        if (val > ui->horizontalRangeSlider->minimumPosition())
        {
            ui->horizontalRangeSlider->setMaximumPosition(val);
        }
    });
    connect(ui->horizontalOpySlider,&QSlider::valueChanged,[&](int val){
        qreal opy = val/100.0;
        ui->graphicsView->setMaskOpy(opy);
    });

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(ui->graphicsView->getUndoAct());
    editMenu->addAction(ui->graphicsView->getRedoAct());
    QButtonGroup* box = new QButtonGroup(this);
    box->addButton(ui->pushButton_pan);
    box->addButton(ui->pushButton_pen);
    box->addButton(ui->pushButton_eraser);
    box->addButton(ui->pushButton_edit);
    box->setExclusive(true);
    ui->pushButton_pan->setChecked(true);
    ui->graphicsView->setBrushSize(ui->spinBox_brush->value());
    ui->graphicsView->setClearSize(ui->spinBox_eraser->value());

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_load_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    if (!fileName.isEmpty())
    {
        QImage img(fileName);
        ui->graphicsView->setCenterImage(std::move(img));
    }
}

void MainWindow::on_pushButton_pen_clicked()
{
    ui->graphicsView->setState(PEN_STATE);
}

void MainWindow::on_pushButton_eraser_clicked()
{
    ui->graphicsView->setState(CLEAR_STATE);
}

void MainWindow::on_pushButton_pan_clicked()
{
    ui->graphicsView->setState(PAN);
}

void MainWindow::on_pushButton_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black);
    if (color.isValid())ui->graphicsView->setBrushColor(color);
}

void MainWindow::on_pushButton_clicked()
{
    ui->graphicsView->clearBrush();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->addBox();
    ui->pushButton_edit->setChecked(true);
    on_pushButton_edit_clicked();
}

void MainWindow::on_pushButton_edit_clicked()
{
    ui->graphicsView->setState(MODEFY_STATE);
}

void MainWindow::on_pushButton_addCircle_clicked()
{
    ui->graphicsView->addCircle();
    ui->pushButton_edit->setChecked(true);
    on_pushButton_edit_clicked();
}

void MainWindow::on_pushButton_compute_clicked()
{
    ui->graphicsView->computeIMC();
}
