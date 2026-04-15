#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_fre_com_clicked();

    void on_pushButton_onoff_com_clicked();

    void on_pushButton_clear_rx_clicked();

    void on_pushButton_clear_tx_clicked();

    void message_Recevied();

    void on_pushButton_hex_rx_clicked();

    void on_pushButton_str_rx_clicked();

    void on_pushButton_hex_tx_clicked();

    void on_pushButton_str_tx_clicked();

    void on_pushButton_tx_clicked();

private:
    Ui::Widget *ui;
    QSerialPort m_Serial;
    int rx_num;
    int tx_num;
    char rx_mode=0;
    char tx_mode=0;

};
#endif // WIDGET_H
