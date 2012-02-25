#include "regeneratedialog.h"
#include "ui_regeneratedialog.h"

RegenerateDialog::RegenerateDialog(int pointsCount, QSize rectSize, QWidget *parent) : QDialog(parent), ui(new Ui::RegenerateDialog) {
    ui->setupUi(this);
    ui->rectWidthSpinBox->setValue(rectSize.width());
    ui->rectHeightSpinBox->setValue(rectSize.height());
    ui->pointsCountSpinBox->setValue(pointsCount);
}

RegenerateDialog::~RegenerateDialog() {
    delete ui;
}

QSize RegenerateDialog::rectSize() const {
    return QSize(ui->rectWidthSpinBox->value(), ui->rectHeightSpinBox->value());
}

int RegenerateDialog::pointsCount() const {
    return ui->pointsCountSpinBox->value();
}
