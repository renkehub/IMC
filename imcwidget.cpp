#include "imcwidget.h"
#include "ui_imcwidget.h"
#include <QButtonGroup>
#include <QColorDialog>
#include <QFileDialog>

IMCWidget::IMCWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::IMCWidget)
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
    ui->horizontalRangeSlider->setValues(0, 255);
    ui->spinBox_lhs->setValue(0);
    ui->spinBox_rhs->setValue(255);
    ui->horizontalRangeSlider->setRange(0, 255);

    connect(ui->horizontalRangeSlider, &CRangeSlider::valuesChanged, [&](int min, int max, bool isMove)
    {
        ui->spinBox_lhs->blockSignals(true);
        ui->spinBox_rhs->blockSignals(true);
        ui->spinBox_lhs->setValue(min);
        ui->spinBox_rhs->setValue(max);
        ui->spinBox_lhs->blockSignals(false);
        ui->spinBox_rhs->blockSignals(false);
        ui->graphicsView->thresIMC(min, max, isMove);
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
    connect(ui->horizontalOpySlider, &QSlider::valueChanged, [&](int val)
    {
        qreal opy = val / 100.0;
        ui->graphicsView->setMaskOpy(opy);
        ui->doubleSpinBoxOpy->blockSignals(true);
        ui->doubleSpinBoxOpy->setValue(opy);
        ui->doubleSpinBoxOpy->blockSignals(false);
    });

    connect(ui->doubleSpinBoxOpy, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&](double val)
    {
        ui->horizontalOpySlider->setValue(val * 100);
    });

    ui->horizontalOpySlider->setValue(55);
    QButtonGroup* box = new QButtonGroup(this);
    box->addButton(ui->pushButton_pan);
    box->addButton(ui->pushButton_pen);
    box->addButton(ui->pushButton_eraser);
    box->addButton(ui->pushButton_edit);
    box->setExclusive(true);
    ui->pushButton_pan->setChecked(true);
    ui->graphicsView->setBrushSize(ui->spinBox_brush->value());
    ui->graphicsView->setClearSize(ui->spinBox_eraser->value());
    installEventFilter(this);
}

bool IMCWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QApplication::sendEvent(ui->graphicsView, event);
    }
    return QWidget::eventFilter(target, event);
}


void IMCWidget::on_pushButton_load_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    if (!fileName.isEmpty())
    {
        QImage img(fileName);
        ui->graphicsView->setCenterImage(std::move(img));
    }
}

void IMCWidget::on_pushButton_pen_clicked()
{
    ui->graphicsView->setState(PEN_STATE);
}

void IMCWidget::on_pushButton_eraser_clicked()
{
    ui->graphicsView->setState(CLEAR_STATE);
}

void IMCWidget::on_pushButton_pan_clicked()
{
    ui->graphicsView->setState(PAN);
}

void IMCWidget::on_pushButton_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black);
    if (color.isValid())ui->graphicsView->setBrushColor(color);
}

void IMCWidget::on_pushButton_edit_clicked()
{
    ui->graphicsView->setState(MODEFY_STATE);
}

void IMCWidget::on_pushButton_addCircle_clicked()
{
    ui->graphicsView->addCircle();
    ui->pushButton_edit->setChecked(true);
    on_pushButton_edit_clicked();
}

void IMCWidget::on_pushButton_compute_clicked()
{
    ui->graphicsView->computeIMC();
}


IMCWidget::~IMCWidget()
{
    delete ui;
}

void IMCWidget::on_pushButton_clear_clicked()
{
    ui->graphicsView->clearBrush();
}

void IMCWidget::on_pushButton_addBox_clicked()
{
    ui->graphicsView->addBox();
    ui->pushButton_edit->setChecked(true);
    on_pushButton_edit_clicked();
}
