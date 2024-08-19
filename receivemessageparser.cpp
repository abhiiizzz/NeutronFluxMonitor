#include "receivemessageparser.h"

char* FLtoArr(float& Val) // Floating point number to IEEE-754 32 bit number array
{
    char *Temp_Ptr ;
    float *test ;
    test = &Val ;
    Temp_Ptr = (char *)test;
    return (Temp_Ptr) ; // Return the pointer; Values in Temp_Ptr[0] to Temp_Ptr[3]
}

char FLtoArrIndex(float val, char index)
{
    char temp[4];
    float  *temp1;
    temp1=(float*)&temp;
    *temp1=val;
    return temp[index];
}

float ARRtoFlo(char* Temp)  // IEEE-754 32 bit number array to Floating point number
{
    float *Val ;
    Val = (float*) Temp ;
    return (*Val) ; // Return the value
}

ReceiveMessageParser::ReceiveMessageParser(QByteArray data, Ui::MainWindow *ui)
{
    qDebug() << data.toHex();
    qDebug() << data[0];
    qDebug() << data[1];
    if( data.size() == 0 ) return;
    if( data[0].operator==(':') ) {
        if( data[1].operator==(HOSTID) ) {
            int byteCount = data[2];
            if( byteCount != 0x0E ) { QMessageBox::critical(nullptr, "Wrong Message Received", "Wrong message length"); return; }
            char DoseVal[4], AlarmVal[4];
            for( int i=0; i<4; i++ ) {
                DoseVal[0+i]=data[5+i];
                AlarmVal[0+i]=data[9+i];
            }
            float dose = ARRtoFlo(DoseVal);
            float alarm = ARRtoFlo(AlarmVal);
            std::string doseStr,alarmStr;
            sprintf(&doseStr[0], "%f", dose);
            dynamic_cast<Ui::MainWindow*>(ui)->m_doseValue_lineEdit->setText(doseStr.c_str());
            dynamic_cast<Ui::MainWindow*>(ui)->m_alarmLevel_SpinBox->setValue(alarm);
            char hvStatus = data[14];
            std::string HV_STA= std::string("")+hvStatus;
            char alarmStatus = data[13];
            std::string ALARM_STA = std::string("")+alarmStatus;
            dynamic_cast<Ui::MainWindow*>(ui)->m_hvStaus_lineEdit->setText(HV_STA.c_str());
            dynamic_cast<Ui::MainWindow*>(ui)->m_alarmStatus_lineEdit->setText(ALARM_STA.c_str());
        }else {
            char host = data[1];
            std::string msg= "Message received for host ID: ";
            msg = msg+host;
            QMessageBox::critical(nullptr, "Wrong Message Received", msg.c_str());
        }
    }
}
