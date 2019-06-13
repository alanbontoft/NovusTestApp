#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "modbus/modbus.h"

#include <QDebug>
#include <QSerialPortInfo>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_btnOpen_clicked();

    void on_btnRead_clicked();

    void on_btnClose_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::Dialog *ui;

    modbus_t *_ctx;
};

#endif // DIALOG_H
