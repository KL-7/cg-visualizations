#ifndef REGENERATEDIALOG_H
#define REGENERATEDIALOG_H

#include <QDialog>

namespace Ui {
    class RegenerateDialog;
}

class RegenerateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegenerateDialog(int pointsCount, QSize rectSize, QWidget *parent = 0);
    ~RegenerateDialog();

    QSize rectSize() const;
    int pointsCount() const;

private:
    Ui::RegenerateDialog *ui;
};

#endif // REGENERATEDIALOG_H
