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

        if (modbus_set_slave(_ctx, ui->spnSlave->value()) == -1)
        {
            ui->listWidget->addItem("Error setting slave address");
            return;
        }
    }
}

void Dialog::on_btnRead_clicked()
{
    uint16_t *buff;
    QString line;
    int ret, numRegs, start;


    if (_ctx == nullptr)
    {
        ui->listWidget->addItem("Port not open");
    }
    else
    {
        start = ui->spnStartReg->value();

        numRegs = ui->spnNumber->value();

        buff = new uint16_t[numRegs];

        if (ui->chkInputReg->isChecked())
        {
            ret = modbus_read_input_registers(_ctx, start, numRegs, buff);
        }
        else
        {
            ret = modbus_read_registers(_ctx, start, numRegs, buff);
        }

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

        delete[] buff;
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
