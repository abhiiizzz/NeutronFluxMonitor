#ifndef RECEIVEMESSAGEPARSER_H
#define RECEIVEMESSAGEPARSER_H

#include <QByteArray>
#include <QByteArray>
#include <QMessageBox>
#include <QDebug>
#include "ui_mainwindow.h"

extern const char HOSTID;

class ReceiveMessageParser
{
public:
    ReceiveMessageParser(QByteArray data, Ui::MainWindow *ui);
};

#endif // RECEIVEMESSAGEPARSER_H
