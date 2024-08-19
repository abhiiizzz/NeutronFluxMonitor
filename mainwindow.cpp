#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include "receivemessageparser.h"

const char HOSTID=0x02;
const std::string default_ip("192.168.0.7");
const unsigned short default_port(20108);
const unsigned short default_alarm(40);

//char* FLtoArr(float& Val) // Floating point number to IEEE-754 32 bit number array
//{
//    char *Temp_Ptr ;
//    float *test ;
//    test = &Val ;
//    Temp_Ptr = (char *)test;
//    return (Temp_Ptr) ; // Return the pointer; Values in Temp_Ptr[0] to Temp_Ptr[3]
//}

//char FLtoArrIndex(float val, char index)
//{
//    char temp[4];
//    float  *temp1;
//    temp1=(float*)&temp;
//    *temp1=val;
//    return temp[index];
//}

//float ARRtoFlo(char* Temp)  // IEEE-754 32 bit number array to Floating point number
//{
//    float *Val ;
//    Val = (float*) Temp ;
//    return (*Val) ; // Return the value
//}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_MsgSent(false)
{
    ui->setupUi(this);
    connect(&m_client_socket, SIGNAL(connected()),    this,  SLOT(on_m_Read_pushButton_clicked()));
    connect(&m_client_socket, SIGNAL(readyRead()),    this,  SLOT(recieveMsg()));
    connect(&m_client_socket, SIGNAL(disconnected()), this,  SLOT(disconnectStatus()));
    connect(&m_client_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(getErrorCode(QAbstractSocket::SocketError)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_m_setAlarm_pushButton_clicked()
{
    unsigned char buffer[8] = {0x3A, 0x08, 0x05, 0x02, 0x02, 0x00, 0x00, 0x00};
    buffer[1] = ui->m_Instrument_ID_spinBox->value()&0xFF; // buffer[4]==0x02 for write command
    unsigned short value = ui->m_alarmLevel_SpinBox->value();
    buffer[6] = value/256;
    buffer[7] = value%256;
    m_MsgSent = true;
    m_client_socket.write((char*)buffer, sizeof(buffer));
    QTimer::singleShot(2*1000, this, SLOT(checkMsgReceived()));
}

void MainWindow::recieveMsg() {
    QByteArray data = m_client_socket.readAll();
    m_MsgSent = false;
    ReceiveMessageParser parser(data, ui);
}

void MainWindow::on_m_remote_checkBox_stateChanged(int checkState)
{
    if( checkState == Qt::Checked ) {
        ui->m_remote_checkBox->setStyleSheet("color:#00FF00");
        ui->m_remote_checkBox->setChecked(true);
        if( m_client_socket.state() == QTcpSocket::UnconnectedState || m_client_socket.waitForDisconnected(1000))
            m_client_socket.connectToHost(ui->m_NFM_IP_lineEdit->text(), std::stoi(ui->m_portSpinBox->text().toStdString()));
    }else {
        if( m_client_socket.state() == QTcpSocket::ConnectedState )
            m_client_socket.disconnectFromHost();
        ui->m_remote_checkBox->setStyleSheet("color:#FF0000");
        ui->m_remote_checkBox->setChecked(false);
    }
}


void MainWindow::on_m_Read_pushButton_clicked()
{
    unsigned char buffer[6] = {0x3A, 0x08, 0x03, 0x02, 0x01, 0x00};
    buffer[1] = ui->m_Instrument_ID_spinBox->value()&0xFF;
    char chkSum = 0x00;
    for(int i=0; i<sizeof(buffer)-1; i++)
    {
        chkSum += buffer[i];
    }
    buffer[5] = -chkSum;
    m_MsgSent = true;
    m_client_socket.write((char*)buffer, sizeof(buffer));
    QTimer::singleShot(2*1000, this, SLOT(checkMsgReceived()));
}

void MainWindow::checkMsgReceived() {
    if( m_MsgSent == true ) // that means message was not yet received
    {
        QMessageBox::critical(this, "NFM device not available", "Check NFM device address or check if NFM device is working");
    }
}

void MainWindow::getErrorCode(QAbstractSocket::SocketError e)
{
    qDebug() << "Error type : " << m_client_socket.errorString();
    QMessageBox::about(this, QString("Socket Error") ,m_client_socket.errorString());
    //if( ui->m_remote_checkBox->checkState() == Qt::Checked ) {
    ui->m_remote_checkBox->setStyleSheet("color:#FF0000");
    ui->m_remote_checkBox->setChecked(false);
    //}
//    ui->m_remote_checkBox->toggle();
}

void MainWindow::disconnectStatus()
{
    QMessageBox::about(this, QString("Socket Disconnected") ,m_client_socket.errorString());
    //if( ui->m_remote_checkBox->checkState() == Qt::Checked ) {
    ui->m_remote_checkBox->setStyleSheet("color:#FF0000");
    ui->m_remote_checkBox->setChecked(false);
    //}
//    ui->m_remote_checkBox->toggle();
}

void MainWindow::on_m_ACK_pushButton_clicked()
{
    unsigned char buffer[8] = {0x3A, 0x08, 0x05, 0x02, 0x03, 0x01, 0x00, 0x00};
    buffer[1] = ui->m_Instrument_ID_spinBox->value()&0xFF; // buffer[4]==0x02 for write command
    unsigned short value = ui->m_alarmLevel_SpinBox->value();
    buffer[6] = value/256;
    buffer[7] = value%256;
    m_MsgSent = true;
    m_client_socket.write((char*)buffer, sizeof(buffer));
    QTimer::singleShot(2*1000, this, SLOT(checkMsgReceived()));
}


void MainWindow::on_m_default_pushButton_clicked()
{
    ui->m_NFM_IP_lineEdit->setText(default_ip.c_str());
    ui->m_portSpinBox->setValue(default_port);
    ui->m_alarmLevel_SpinBox->setValue(default_alarm);
}

