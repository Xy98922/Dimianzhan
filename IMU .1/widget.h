#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QDateTime>
#include "include/HCNetSDK.h"
#include <QMessageBox>
#include <windows.h>
#include <QGamepad>
#include <QGamepadManager>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include "SwitchControl.h"
#include "Handle.h"
//BaiduVoice Use
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMainWindow>
#include "QNetworkAccessManager"
#include "QAudioDeviceInfo"
#include "QAudioInput"
#include "QBuffer"
#include "QMessageBox"
#include "baiduvoice.h"
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QTime>
#include <QSettings>
#include <QAxWidget>
#include <QtGui>
#include <QVBoxLayout>
#include <QPainter>
#include <QHBoxLayout>
#include <QMovie>
// 引用命名空间
QT_CHARTS_USE_NAMESPACE
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void QStringtoHex(QByteArray& sendData,QString str);
    char ConvertHexChar(char c);
    long play(HWND hWnd,NET_DVR_PREVIEWINFO struPlayInfo);
    long play1(HWND hWnd,NET_DVR_PREVIEWINFO struPlayInfo);
    bool initSDK();
    bool uninitSDK();
    void paintEvent(QPaintEvent *);
    void setUIString(QString str);
    void AudioInit();
    void TokenInit();
    void getMacAddress();
    void changeBaiduAudioAns(QString str);
    bool JudgeTokenTime();
    baiduVoice *p_BaiduVoice;
    QString TulingURL;
    QMediaPlayer* media_player;
    QAudioDeviceInfo SpeechCurrentDevice;
    QAudioInput* audio_input=NULL;
    QBuffer* JsonBuffer=NULL;
    QAxWidget *flash;
    QVBoxLayout *bl;

signals:
    void changeV2T();
    void changeUIANS();
protected:
    //覆写虚函数bool eventFilter(QObject *obj, QEvent *event)
    bool eventFilter(QObject *obj, QEvent *event);   //Qlable全屏与退出
private slots:
    void on_up_pushButton_clicked();
    void on_down_pushButton_clicked();
    void on_stop_pushButton_clicked();
    void on_toolButton_clicked();
    void readMyCom();//用于读取数据
    void port_init();
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged();
    void Sleep(int msec);
    void handleSerialError(QSerialPort::SerialPortError error) ;
    void OnClickedLogin();
    void OnClickedLogout();
    void ClickedPreview();
    void ClickedPreview1();
    void OnClickedStopPreview();
    void OnClickedCapture();
    void xboxcontorl();
    void timerxbox();
    void onToggled(bool);
    void speedlevel();
    void on_voiceBtn_pressed();
    void on_voiceBtn_released();
    void VoiceControl();
    void HandleControl();
    void on_radioButton_4_clicked();
    void Multimachinedeformation();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
public slots:
    //直接文字识别
        void Tuling_replyFinish(QNetworkReply* reply);
        //语音识别过程获得回答
        void voice_Tuling_replyFinish(QNetworkReply* reply);
        //识别声音获得文字
        void Baidu_VoiceToText_replyFinish(QNetworkReply* reply);
        void get_Token_slot(QNetworkReply* reply);
        //文本转语音槽函数
        void Baidu_TextToVoice_replyFinish();
        //获得声音后
        void voice_post_Tuling_slot();

private:
    Ui::Widget *ui;
    QSerialPort *my_serialPort;//(实例化一个指向串口的指针，可以用于访问串口)
    QByteArray requestData;//（用于存储从串口那读取的数据）
    QTimer *timer;     // xbox control
    QTimer *timer2;    //horizontal&vertical slider control
    QTimer *timer3;    //voice control
    QTimer *timer4;   // virtual xbox control
    QTimer *timer5;   // voice transform control
    QTimer *timer6;   //  Multi-transform control
    QTimer *timer7;   // IMU read my com();
    int HEX_str_flag;
    int send_HEX_str_flag;
    int vlevel=1;
    int imuflag=0;
    QString file_text;
    long userID;
    long userID1;
    long previewID;
    long previewID1;
    QGamepad *m_gamepad;
    SwitchControl *pGreenSwitchControl;
    Handle *handle;
    Handle *handle2;
    QLineSeries *series,*seriesw,*series2,*seriesw2,*Gyrowx,*Gyrowy,*Gyrowz;
    QChart *chart,*chart2,*chart3;
    QValueAxis *axisX,*axisY,*axisX2,*axisY2,*axisX3,*axisY3;
    QChartView *view,*view2,*view3;
    int countx=0,leftarea=0,rightarea=500,trans=0;
    float voicespeedx=0,voicespeedw=0;
    QString tuling_get_ans;
    QString voice_get_ans;
    QString UI_ANS_String;
    QLabel* sirilabel;
    QMovie *movie;    
};

#endif // WIDGET_H
