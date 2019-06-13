#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    for (int i=0; i < ports.count(); i++)
    {
        ui->cboPort->addItem(ports.at(i).portName());
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnOpen_clicked()
{
    int baud = ui->cboBaud->currentText().toInt();
    QString port = ui->cboPort->currentText();

    port = "/dev/" + port;

    if (_ctx == nullptr)
    {
        _ctx = modbus_new_rtu(port.toUtf8().constData(), baud, 'N', 8, 1);

        if (_ctx == nullptr)
        {
            ui->listWidget->addItem("Error creating context");
            return;
        }

        if (modbus_connect(_ctx) == -1)
        {
            ui->listWidget->addItem("Error connecting to context");
            return;
        }

        if (modbus_set_slave(_ctx, 247) == -1)
        {
            ui->listWidget->addItem("Error setting slave address");
            return;
        }
    }
}

void Dialog::on_btnRead_clicked()
{
    uint16_t buff[10] = {0};
    QString line;

    if (_ctx == nullptr)
    {
        ui->listWidget->addItem("Port not open");
    }
    else
    {
        int ret = modbus_read_registers(_ctx, 7, 10, buff);
        if (ret == -1)
        {
            ui->listWidget->addItem("Read error");
        }
        else
        {
            line = "";
            for (int i=0; i < ret; i++)
            {
                line += QVariant(buff[i]).toString() + " ";
            }

            ui->listWidget->addItem(line);
        }
    }
}

void Dialog::on_btnClose_clicked()
{
    if (_ctx != nullptr)
    {
        modbus_close(_ctx);
        modbus_free(_ctx);
        _ctx = nullptr;
    }
}

void Dialog::on_listWidget_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    ui->listWidget->clear();
}
