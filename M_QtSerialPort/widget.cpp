#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    on_pushButton_fre_com_clicked();

    ui->label_message->setStyleSheet("color:red;");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_fre_com_clicked()
{
    ui->comboBox_com->clear();
    foreach(const QSerialPortInfo &Info, QSerialPortInfo::availablePorts()){
        ui->comboBox_com->addItem(Info.portName());
    }
}

void Widget::on_pushButton_onoff_com_clicked()
{
    if(ui->pushButton_onoff_com->text()=="打开串口"){
        //设置端口
        m_Serial.setPortName(ui->comboBox_com->currentText());
        m_Serial.setBaudRate(ui->comboBox_baud->currentText().toUInt());
        //打开端口
        m_Serial.open(QIODevice::ReadWrite);
        if(m_Serial.isOpen())
        {
            connect(&m_Serial,&QSerialPort::readyRead,this,&Widget::message_Recevied);
            ui->pushButton_led->setStyleSheet("background-color:red;");
            ui->pushButton_onoff_com->setText("关闭串口");
            ui->label_message->setText("串口打开成功！");
        }else{
            ui->label_message->setText("串口打开失败！");
        }
    }else{
        m_Serial.close();
        if(m_Serial.isOpen())
        {
            ui->label_message->setText("串口关闭失败！");
        }else{
            ui->pushButton_led->setStyleSheet("background-color:rgb(232,232,232);");
            ui->pushButton_onoff_com->setText("打开串口");
            ui->label_message->setText("串口已经关闭！");
        }
    }
}

void Widget::on_pushButton_clear_rx_clicked()
{
    ui->textBrowser_rx->clear();
    this->rx_num=0;
    ui->label_rx_num->setText("");
}

void Widget::on_pushButton_clear_tx_clicked()
{
    ui->textEdit_tx->clear();
    this->tx_num=0;
    ui->label_tx_num->setText("");
}

void Widget::message_Recevied()
{
    QByteArray buf=m_Serial.readAll();

    this->rx_num+=buf.size();//实现接受字节数量的自增
    QString str=tr(buf);
    if(this->rx_mode==0)//字符串显示
    {
        ui->textBrowser_rx->insertPlainText(str);
    }else//hex显示
    {
        for(int i=0;i<buf.size();i++){
            str+=QString("%1").arg(buf[i] & 0xff,2,16,QLatin1Char('0'));
        }
        ui->textBrowser_rx->insertPlainText(str);
        ui->textBrowser_rx->moveCursor(QTextCursor::End);//光标移到最后
    }
    ui->label_rx_num->setText(QString::number(this->rx_num));
}

void Widget::on_pushButton_hex_rx_clicked()
{
    ui->pushButton_hex_rx->setStyleSheet("background-color:rgb(149,231,85);");
    this->rx_mode=1;
    ui->pushButton_str_rx->setStyleSheet("background-color:rgb(232,232,232);");

}

void Widget::on_pushButton_str_rx_clicked()
{
    ui->pushButton_hex_rx->setStyleSheet("background-color:rgb(232,232,232);");
    this->rx_mode=0;
    ui->pushButton_str_rx->setStyleSheet("background-color:rgb(149,231,85);");

}

void Widget::on_pushButton_hex_tx_clicked()
{
    ui->pushButton_hex_tx->setStyleSheet("background-color:rgb(149,231,85);");
    this->tx_mode=1;
    ui->pushButton_str_tx->setStyleSheet("background-color:rgb(232,232,232);");

}

void Widget::on_pushButton_str_tx_clicked()
{
    ui->pushButton_hex_tx->setStyleSheet("background-color:rgb(232,232,232);");
    this->tx_mode=0;
    ui->pushButton_str_tx->setStyleSheet("background-color:rgb(149,231,85);");

}

void Widget::on_pushButton_tx_clicked()
{
    if(m_Serial.isOpen()){
        if(this->tx_mode==0)//字符串发送
        {
            this->tx_num+= m_Serial.write(ui->textEdit_tx->toPlainText().toLatin1());

        }else//hex发送
        {
            this->tx_num+=m_Serial.write(QByteArray::fromHex(ui->textEdit_tx->toPlainText().toLatin1()));
            ui->label_tx_num->setText(QString::number(this->tx_num));
        }
    }

}
