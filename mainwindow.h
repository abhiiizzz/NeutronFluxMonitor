#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_m_setAlarm_pushButton_clicked();

    void on_m_remote_checkBox_stateChanged(int arg1);

    void on_m_Read_pushButton_clicked();

    void getErrorCode(QAbstractSocket::SocketError e);

    void disconnectStatus();

    void recieveMsg();

    void checkMsgReceived();

    void on_m_ACK_pushButton_clicked();

    void on_m_default_pushButton_clicked();

private:
    QTcpSocket m_client_socket;
    Ui::MainWindow *ui;
    bool m_MsgSent;
};
#endif // MAINWINDOW_H
