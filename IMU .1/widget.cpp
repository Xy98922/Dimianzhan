#include "widget.h"
#include "ui_widget.h"
#include "SwitchControl.h"
#include "CImageButton.h"
#include "baiduvoice.h"
#include "Handle.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>
#include <QMovie>
#include <QSlider>
#include <QString>
#include <QLabel>
#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QColorDialog>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
   ui->setupUi(this);
   HEX_str_flag = 0; //初始化转换标志位0 （字符串显示）
   send_HEX_str_flag =0; //初始化转换标志位0 （字符发送）
   port_init();
   userID=-1;
   previewID=-1;
   sirilabel=new QLabel(ui->voiceBtn);
   sirilabel->resize(258,45);
   movie = new QMovie(":/siri.gif");
   sirilabel->setMovie(movie);
   m_gamepad = new QGamepad(0, this);
   timer  = new QTimer(this);
   timer2 = new QTimer(this);
   timer3 = new QTimer(this);
   timer4 = new QTimer(this);
   timer5 = new QTimer(this);
   timer6 = new QTimer(this);
   timer7 = new QTimer(this);
   series = new QLineSeries();
   seriesw= new QLineSeries();
   series2 = new QLineSeries();
   seriesw2= new QLineSeries();
   Gyrowx=new QLineSeries();
   Gyrowy=new QLineSeries();
   Gyrowz=new QLineSeries();
   chart  = new QChart();
   chart2  = new QChart();
   chart3  = new QChart();
   view   = new QChartView(chart);
   view2   = new QChartView(chart2);
   view3   = new QChartView(chart3);
   p_BaiduVoice=new baiduVoice();
   chart->legend()->hide(); // 将图例隐藏
   chart2->legend()->hide();
   chart3->legend()->hide();
   chart->addSeries(series); // 关联series，这一步很重要，必须要将series关联到QChart才能将数据渲染出来：
   chart->addSeries(seriesw);
   chart2->addSeries(series2); // 关联series，这一步很重要，必须要将series关联到QChart才能将数据渲染出来：
   chart2->addSeries(seriesw2);
   chart3->addSeries(Gyrowx);
   chart3->addSeries(Gyrowy);
   chart3->addSeries(Gyrowz);
   series->setUseOpenGL(true);
   seriesw->setUseOpenGL(true);// 开启OpenGL，QLineSeries支持GPU绘制，Qt其他有的图表类型是不支持的。
   series2->setUseOpenGL(true);
   seriesw2->setUseOpenGL(true);
   Gyrowx->setUseOpenGL(true);
   Gyrowy->setUseOpenGL(true);
   Gyrowz->setUseOpenGL(true);
   axisX = new QValueAxis;
   axisX2 = new QValueAxis;
   axisX3 = new QValueAxis;
   axisX->setRange(leftarea, rightarea);//设置坐标轴范围
   axisX->setLabelFormat("%.0f"); //标签格式：每个单位保留几位小数
   axisX->setTickCount(11); //主分隔个数：0到10分成20个单位
   axisX->setMinorTickCount(0); //每个单位之间绘制了多少虚网线
   axisX->setGridLineVisible(false);
   axisX2->setRange(leftarea, rightarea);//设置坐标轴范围
   axisX2->setLabelFormat("%.0f"); //标签格式：每个单位保留几位小数
   axisX2->setTickCount(11); //主分隔个数：0到10分成20个单位
   axisX2->setMinorTickCount(0); //每个单位之间绘制了多少虚网线
   axisX2->setGridLineVisible(false);
   axisX3->setRange(leftarea, rightarea);//设置坐标轴范围
   axisX3->setLabelFormat("%.0f"); //标签格式：每个单位保留几位小数
   axisX3->setTickCount(11); //主分隔个数：0到10分成20个单位
   axisX3->setMinorTickCount(0); //每个单位之间绘制了多少虚网线
   axisX3->setGridLineVisible(false);
   axisY = new QValueAxis; //Y 轴
   axisY2 = new QValueAxis;
   axisY3 = new QValueAxis;
   axisY->setRange(-100, 100);
   axisY->setTitleText("(m/s)");
   axisY->setLabelFormat("%.0f"); //标签格式
   axisY->setTickCount(13);
   axisY->setMinorTickCount(0);
   axisY2->setRange(-100, 100);
   axisY2->setTitleText("(m/s)");
   axisY2->setLabelFormat("%.0f"); //标签格式
   axisY2->setTickCount(13);
   axisY2->setMinorTickCount(0);
   axisY3->setRange(-100, 100);
   axisY3->setTitleText("(m/s)");
   axisY3->setLabelFormat("%.0f"); //标签格式
   axisY3->setTickCount(13);
   axisY3->setMinorTickCount(0);
   chart->setAxisX(axisX, series);
   chart->setAxisY(axisY, series);
   chart->setAxisX(axisX, seriesw);
   chart->setAxisY(axisY, seriesw);
   chart2->setAxisX(axisX2, series2);
   chart2->setAxisY(axisY2, series2);
   chart2->setAxisX(axisX2, seriesw2);
   chart2->setAxisY(axisY2, seriesw2);
   chart3->setAxisX(axisX3, Gyrowx);
   chart3->setAxisY(axisY3, Gyrowx);
   chart3->setAxisX(axisX3, Gyrowy);
   chart3->setAxisY(axisY3, Gyrowy);
   chart3->setAxisX(axisX3, Gyrowz);
   chart3->setAxisY(axisY3, Gyrowz);
   view->setRenderHint(QPainter::Antialiasing);// 开启抗锯齿，让显示效果更好
   view->resize(1000,400);
   view2->setRenderHint(QPainter::Antialiasing);// 开启抗锯齿，让显示效果更好
   view2->resize(1000,400);
   view3->setRenderHint(QPainter::Antialiasing);// 开启抗锯齿，让显示效果更好
   view3->resize(1000,400);
   connect(timer2, SIGNAL( timeout() ), this, SLOT(on_horizontalSlider_valueChanged()));
   connect(timer, SIGNAL( timeout() ), this, SLOT(xboxcontorl()));
   connect(timer3, SIGNAL( timeout() ), this, SLOT(VoiceControl()));
   connect(timer4, SIGNAL( timeout() ), this, SLOT(HandleControl()));
   connect(timer5, &QTimer::timeout, [=](){trans=0;});
   connect(timer6, &QTimer::timeout, [=](){Multimachinedeformation();});
   connect(timer7, SIGNAL( timeout() ), this, SLOT(readMyCom()));
   connect(ui->LoginButton,&QPushButton::clicked,this,&Widget::OnClickedLogin);
   connect(ui->LogoutButton,&QPushButton::clicked,this,&Widget::OnClickedLogout);
   connect(ui->LoginButton,&QPushButton::clicked,this,&Widget::ClickedPreview);
   connect(ui->LoginButton,&QPushButton::clicked,this,&Widget::ClickedPreview1);
   connect(ui->StopPreviewButton,&QPushButton::clicked,this,&Widget::OnClickedStopPreview);
   connect(ui->horizontalSlider_2,&QSlider::valueChanged,this,&Widget::on_horizontalSlider_valueChanged);
   connect(ui->radioButton,&QPushButton::clicked,this,&Widget::speedlevel);
   connect(ui->radioButton_2,&QPushButton::clicked,this,&Widget::speedlevel);
   connect(ui->radioButton_3,&QPushButton::clicked,this,&Widget::speedlevel);
   connect(ui->verticalSlider_2,&QSlider::valueChanged,this,&Widget::on_horizontalSlider_valueChanged);
   connect(ui->verticalSlider,&QSlider::valueChanged,this,&Widget::on_horizontalSlider_valueChanged);
   connect(m_gamepad,&QGamepad::buttonStartChanged,this,&Widget::timerxbox);
   connect(ui->toolButton_7,&QPushButton::clicked,[=](){
       timer6->start(10);
       Multimachinedeformation();
   });
   connect(ui->pushButton_13,&QPushButton::clicked,[=](){
        view3->show();
        imuflag=0;
   });
   connect(ui->pushButton_14,&QPushButton::clicked,[=](){
        view3->show();
        imuflag=1;
   });
   connect(ui->pushButton_15,&QPushButton::clicked,[=](){
        view3->show();
        imuflag=2;
   });
   connect(ui->PreviewButton,&QPushButton::clicked,[=](){
          ui-> label_5->setWindowFlags(Qt::Window);
          ui-> label_5->setScaledContents(true);
          ui-> label_5->showFullScreen();
          ui-> label->setWindowFlags(Qt::Window);
          ui-> label->setScaledContents(true);
          ui-> label->showFullScreen();
   });
   connect(this,&Widget::changeUIANS,this,&Widget::Baidu_TextToVoice_replyFinish);
   connect(this,&Widget::changeV2T,this,&Widget::voice_post_Tuling_slot);
   AudioInit();
   media_player =new QMediaPlayer(this);
   getMacAddress();
   TokenInit();
   if(initSDK())
       qDebug()<<"SDK init Success!"<<endl;
   Qt::WindowFlags ture=Qt::Widget;
   ture |=Qt::WindowMinimizeButtonHint;
   ture |=Qt::WindowMaximizeButtonHint;
   ture |= Qt::WindowCloseButtonHint;
   setWindowFlags(ture);
   ui-> label_5->installEventFilter(this);
   ui-> label->installEventFilter(this);  
   ui-> radioButton->setChecked(true);
   pGreenSwitchControl = new SwitchControl(ui->label_6);
   pGreenSwitchControl->setToggle(false);
   pGreenSwitchControl->setCheckedColor(QColor(0, 160, 230));
   pGreenSwitchControl->resize(60,25);
   connect(pGreenSwitchControl, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
   handle = new Handle(ui->pushButton_4);
   handle2 = new Handle(ui->pushButton_5);
   ui->pushButton_3->setStyleSheet(QString("QPushButton{border-radius:70px;backg round-color:rgba(0,255,0,255);}")
//                                   +"QPushButton:hover{background-color:rgba(0,0,255,255);}"
//                                   +"QPushButton:pressed{background-color: rgba (255,0,0,255);}"
                                   );
   ui->pushButton_4->setStyleSheet(QString("QPushButton{border-radius:50px;backg round-color:rgba(0,255,0,255);}")
//                                   +"QPushButton:hover{background-color:rgba(0,0,255,255);}"
//                                   +"QPushButton:pressed{background-color: rgba (255,0,0,255);}"
                                   );
   ui->pushButton_5->setStyleSheet(QString("QPushButton{border-radius:50px;backg round-color:rgba(0,255,0,255);}")
  //                                 +"QPushButton:hover{background-color:rgba(0,0,255,255);}"
//                                   +"QPushButton:pressed{background-color: rgba (255,0,0,255);}"
                                   );
   ui->toolButton_3->setStyleSheet(QString("QToolButton{border-radius:70px;backg round-color:rgba(0,255,0,255);}")
//                                   +"QToolButton:hover{background-color:rgba(0,0,255,255);}"
//                                   +"QToolButton:pressed{background-color: rgba (255,0,0,255);}"
                                   );

   ui->toolButton_5->setStyleSheet(QString("QToolButton{border-radius:70px;backg round-color:rgba(0,255,0,255);}")
//                                   +"QToolButton:hover{background-color:rgba(0,0,255,255);}"
//                                   +"QToolButton:pressed{background-color: rgba (255,0,0,255);}"
                                   );

   ui->toolButton_7->setStyleSheet(QString("QToolButton{border-radius:70px;backg round-color:rgba(0,255,0,255);}")
//                                   +"QToolButton:hover{background-color:rgba(0,0,255,255);}"
//                                   +"QToolButton:pressed{background-color: rgba (255,0,0,255);}"
                                   );
}

Widget::~Widget()     //析构函数，释放窗口时触发
{
    if(uninitSDK())
        qDebug()<<"SDK uninit Success!"<<endl;
    qDebug()<<"山城机甲控制器关闭!"<<endl;
    delete ui;
}
 QSerialPort serial;
void Widget::port_init()  //获取可用的串口号
{
    ui->comboBox_portName_2->clear();
    foreach( const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())//读取串口信息   foereach 函数
        {
            serial.setPort(Info);//串口信息初始化，得到info结构体
             ui->comboBox_portName_2->addItem( Info.portName() );//在comboBox那添加串口号
       //     if( serial.open( QIODevice::ReadWrite) )//如果串口是可以读写方式打开的
         //   {
             //   ui->comboBox_portName_2->addItem( Info.portName() );//在comboBox那添加串口号
         //       serial.close();//然后自动关闭等待人为开启（通过那个打开串口的PushButton）
        //    }
        }
}

void Widget::onToggled(bool) //打开串口
{
    if(pGreenSwitchControl->isToggled() == true)
    {
       // ui->pushButton_comopen->setText("断开连接");
        //对串口进行一些初始化
        my_serialPort = new QSerialPort(this);
        my_serialPort->setPortName( ui->comboBox_portName_2->currentText() );//连接串口，void setPortName(const QString &name) 参数name为所需要连接的串口的名字
        qDebug() << ui->comboBox_portName_2->currentText();
        my_serialPort->setBaudRate(  115200 );//设置波特率
        my_serialPort->setFlowControl( QSerialPort::NoFlowControl );//设置数据流控制,默认无
        my_serialPort->setDataBits(QSerialPort::Data8);//设置数据位
        my_serialPort->setParity(QSerialPort::NoParity);//设置校验位
        my_serialPort->setStopBits(QSerialPort::OneStop);//设置停止位
        my_serialPort->open(QIODevice::ReadWrite);//同时开启串口读写 
        connect(my_serialPort,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(handleSerialError(QSerialPort::SerialPortError)));  //连接打开的串口异常检测
        timer2->start(10);
        timer7->start(2);
        timer->stop();
        timer3->stop();
        timer4->stop();
        timer5->stop();
        timer6->stop();
    }
    else
    {
//        QMovie *movie = new QMovie(":/fliped.gif");
//        ui->label_flag->setMovie(movie);
//        movie->stop();
        my_serialPort->close();
        timer->stop();
        timer2->stop();
        timer3->stop();
        timer4->stop();
        timer5->stop();
        timer6->stop();
        timer7->stop();
        ui->label_4->setText("未连接...");
    }
}

void Widget::handleSerialError(QSerialPort::SerialPortError error) //串口拔出检测
{
    if (error == QSerialPort::ResourceError) {
        my_serialPort->close();
        QMessageBox::critical(this, tr("Error"), "串口连接中断，请检查是否正确连接！");
      //  ui->label_flag->seticon("background-color:rgb(130,130,130)");
        ui->label_6->setText("串口拔出");
        port_init(); //重新检查串口
    }
}

//void Widget::on_pushButton_send_clicked()
//{
//    if(ui->pushButton_comopen->text() == "port_open")//串口开启后才能发送
//    {
//        if(send_HEX_str_flag == 1) //发送HEX数据
//        {
//            QString sendData = ui->lineEdit_write->text();
//            QByteArray sendbuff;
//            QStringtoHex(sendbuff,sendData);
//            my_serialPort->write( sendbuff );
//        }
//        else if(send_HEX_str_flag == 0) //发送STR数据
//        {
//            my_serialPort->write(ui->lineEdit_write->text().toUtf8());
//        }
//    }
//}
void Widget::on_horizontalSlider_valueChanged()
{
    timer->stop();
    timer3->stop();
    QByteArray up1;
    int valuew = ui->horizontalSlider->value();
    int valuex = ui->verticalSlider->value();
    up1.resize(14);
    int value1=valuex*0.5*vlevel;
    int value=valuew*vlevel;
    ui->progressBar->setValue(value1/vlevel);
    ui->progressBar_2->setValue(value/vlevel);
    up1[0]=0x3e;
    up1[1]=0xa2;
    up1[2]=0x02;   //ID2
    up1[3]=0x04;
    up1[4]=0xe6;
    up1[8] =((int_fast32_t)value1 >> 24);
    up1[7] = ((int_fast32_t)value1>>16);
    up1[6] =  ((int_fast32_t)value1>> 8);
    up1[5] =  (int_fast32_t)value1;
    up1[12] = ((int_fast32_t)value >> 24);
    up1[11] =  ((int_fast32_t)value>>16);
    up1[10] = ((int_fast32_t)value>> 8);
    up1[9] =  (int_fast32_t)value;
    up1[13]=up1[5]+up1[6]+up1[7]+up1[8]+up1[9]+up1[10]+up1[11]+up1[12];
    int speedx;
    speedx=(((up1[8]&0xff))<<24 |((up1[7]&0xff)<<16) | ((up1[6]&0xff)<<8) | (up1[5]&0xff));
    int speedw;
    speedw=(((up1[12]&0xff)<<24) |((up1[11]&0xff)<<16) | ((up1[10]&0xff)<<8) | (up1[9]&0xff));
    qDebug()<<"speedx="<<speedx<<"speedw="<<speedw;
    my_serialPort->write(up1);
    ui->lcdNumber_7->display(speedx*0.01);
    ui->lcdNumber_8->display(speedw*0.01);
    ui->lcdNumber_7->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber_8->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    up1.clear();
    Sleep(10);
    int valuew2 = ui->horizontalSlider_2->value();
    int valuex2 = ui->verticalSlider_2->value();
    int value3=valuex2*0.5*vlevel;
    int value2=valuew2*vlevel;
    ui->progressBar_3->setValue(value2/vlevel);
    ui->progressBar_4->setValue(value3/vlevel);
    up1[0]=0x3e;
    up1[1]=0xa2;
    up1[2]=0x01;   //ID1
    up1[3]=0x04;
    up1[4]=0xe6;
    up1[8] = (int)value3 >> 24;
    up1[7] = (int)value3>>16;
    up1[6] = (int)value3>> 8;
    up1[5] = (int)value3;
    up1[12] = (int)value2 >> 24;
    up1[11] = (int)value2>>16;
    up1[10] = (int)value2>> 8;
    up1[9] = (int)value2;
    up1[13]=up1[5]+up1[6]+up1[7]+up1[8]+up1[9]+up1[10]+up1[11]+up1[12];
    int speedx2=(((up1[8]&0xff))<<24 |((up1[7]&0xff)<<16) | ((up1[6]&0xff)<<8) | (up1[5]&0xff));
    int speedw2=(((up1[12]&0xff)<<24) |((up1[11]&0xff)<<16) | ((up1[10]&0xff)<<8) | (up1[9]&0xff));
    qDebug()<<"speedx2="<<speedx<<"speedw2="<<speedw;
    my_serialPort->write(up1);
    up1.clear();
    ui->lcdNumber->display(speedx2*0.01);
    ui->lcdNumber_2->display(speedw2*0.01);
    ui->lcdNumber->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber_2->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    Sleep(10);
    readMyCom();
}



void Widget::on_up_pushButton_clicked()
{

    QByteArray up;
    float setv1 =106.103297*6*6*100*0.2;
    float setv2 =106.103297*6*6*100*0.2;
    up[0]=0x3e;
    up[1]=0xa2;
    up[2]=0x02;
    up[3]=0x04;
    up[4]=0xe6;
    up[8] = ((int)setv1 >> 24) & 0xff;
    up[7] = ((int)setv1 >> 16) & 0xff;
    up[6] = ((int)setv1>> 8)  & 0xff;
    up[5] = (int)setv1&0xff;
    up[9]=(up[5]+up[6]+up[7]+up[8]);

    my_serialPort->write(up);


    qDebug()<<"上槽函数触发";
    ui->lcdNumber->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber->display(0.2);
    Sleep(200);
    QByteArray DATA;
    DATA[0]=0x3E;
    DATA[1]=0xA2;
    DATA[2]=0x01;
    DATA[3]=0x04;
    DATA[4]=0xE5;
    DATA[8] = ((int)setv2 >> 24) & 0xff;
    DATA[7] = ((int)setv2 >> 16) & 0xff;
    DATA[6] = ((int)setv2>> 8)  & 0xff;
    DATA[5] = (int)setv2&0xff;
    //test=(DATA[8]<<24)|(DATA[7]<<16)|(DATA[6]<<8)|DATA[5];
    //UART_send_string(UART4,"v1:");UART_send_intdata(UART4,test);UART_send_char(UART4,'\t');
    DATA[9]=DATA[5]+DATA[6]+DATA[7]+DATA[8];

    my_serialPort->write( DATA );
}
void Widget::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Widget::on_down_pushButton_clicked()
{


    QByteArray up;
    float setv1 =106.103297*6*6*100*-0.2;
    float setv2 =106.103297*6*6*100*-0.2;
    up[0]=0x3e;
    up[1]=0xa2;
    up[2]=0x02;
    up[3]=0x04;
    up[4]=0xe6;
    up[8] = ((int)setv1 >> 24) & 0xff;
    up[7] = ((int)setv1 >> 16) & 0xff;
    up[6] = ((int)setv1>> 8)  & 0xff;
    up[5] = (int)setv1&0xff;
    up[9]=(up[5]+up[6]+up[7]+up[8]);
    qDebug()<<"下槽函数触发";
    ui->lcdNumber->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber->display(-0.2);
    my_serialPort->write( up );
    Sleep(300);
    QByteArray DATA;
    DATA[0]=0x3E;
    DATA[1]=0xA2;
    DATA[2]=0x01;
    DATA[3]=0x04;
    DATA[4]=0xE5;
    DATA[8] = ((int)setv2 >> 24) & 0xff;
    DATA[7] = ((int)setv2 >> 16) & 0xff;
    DATA[6] = ((int)setv2>> 8)  & 0xff;
    DATA[5] = (int)setv2&0xff;
    //test=(DATA[8]<<24)|(DATA[7]<<16)|(DATA[6]<<8)|DATA[5];
    //UART_send_string(UART4,"v1:");UART_send_intdata(UART4,test);UART_send_char(UART4,'\t');
    DATA[9]=DATA[5]+DATA[6]+DATA[7]+DATA[8];

    my_serialPort->write( DATA );


}
void Widget::on_stop_pushButton_clicked(){
      timer->stop();
      timer2->stop();
      timer3->stop();
      timer4->stop();
      ui->horizontalSlider->setValue(0);
      ui->horizontalSlider_2->setValue(0);
      ui->verticalSlider->setValue(0);
      ui->verticalSlider_2->setValue(0);
}
void Widget::readMyCom()//读取缓冲的数据，每5ms读一次
{

     my_serialPort->setPortName( ui->comboBox_portName_2->currentText() );
     my_serialPort->setBaudRate( 115200 );//波特率
     requestData = my_serialPort->readAll();//用requestData存储从串口那读取的数据
     requestData=requestData.toHex();
     //qDebug()<<requestData.size();
     QByteArray Angle,Acc,Gyro,temp;
     QByteArray Anglecopy;
     QString rDate;
     QString angleint;
     rDate=QString(requestData);
     float b;
     qDebug()<<"串口接收数据：";
     qDebug()<<rDate<<endl;
     static int transflag=1;
     if(requestData.size()!=0)
         {
             ui->label_4->setText("连接成功!");
             if(rDate[0] == '5'&&rDate[1] == '5'&&rDate[2] == '5'&&rDate[3] == '1'&&rDate.size()>=80)
             {
                 qDebug()<<"IMU数据：";
                 qDebug()<<rDate<<endl;
                 rDate.remove(0, 4);
                 Angle = rDate.left(4).toLocal8Bit();
                 Anglecopy=Angle;
                 Angle[0]=Anglecopy[2];
                 Angle[1]=Anglecopy[3];
                 Angle[2]=Anglecopy[0];
                 Angle[3]=Anglecopy[1];
                 angleint=QString(Angle);
                 b=angleint.toInt(nullptr,16);
                 b=b/32768*16;
                 if(b>=16)
                     b=b-32;
                 if(imuflag==1)
                     Gyrowx->append(countx,b);
                 ui->lcdNumber_9->display(b);
                 rDate.remove(0, 4);
                 Angle = rDate.left(4).toLocal8Bit();
                 Anglecopy=Angle;
                 Angle[0]=Anglecopy[2];
                 Angle[1]=Anglecopy[3];
                 Angle[2]=Anglecopy[0];
                 Angle[3]=Anglecopy[1];
                 angleint=QString(Angle);
                 b=angleint.toInt(nullptr,16);
                 b=b/32768*16;
                 if(b>=16)
                     b=b-32;
                 if(imuflag==1)
                     Gyrowy->append(countx,b);
                 ui->lcdNumber_10->display(b);
                 rDate.remove(0, 4);
                 Angle = rDate.left(4).toLocal8Bit();
                 Anglecopy=Angle;
                 Angle[0]=Anglecopy[2];
                 Angle[1]=Anglecopy[3];
                 Angle[2]=Anglecopy[0];
                 Angle[3]=Anglecopy[1];
                 angleint=QString(Angle);
                 b=angleint.toInt(nullptr,16);
                 b=b/32768*16;
                 if(b>=16)
                     b=b-32;
                 if(imuflag==1)
                     Gyrowz->append(countx++,b);
                 ui->lcdNumber_11->display(b);
                 rDate.remove(0, 4);
                 Angle = rDate.left(4).toLocal8Bit();
                 Anglecopy=Angle;
                 Angle[0]=Anglecopy[2];
                 Angle[1]=Anglecopy[3];
                 Angle[2]=Anglecopy[0];
                 Angle[3]=Anglecopy[1];
                 angleint=QString(Angle);
                 b=angleint.toInt(nullptr,16);
                 b=b/340+35.53;
                 ui->lcdNumber_6->display(b);

                 rDate.remove(0, 10);
                 Angle = rDate.left(4).toLocal8Bit();
                 Anglecopy=Angle;
                 Angle[0]=Anglecopy[2];
                 Angle[1]=Anglecopy[3];
                 Angle[2]=Anglecopy[0];
                 Angle[3]=Anglecopy[1];
                 angleint=QString(Angle);
                // qDebug()<<angleint;
                 b=angleint.toInt(nullptr,16);
                 b=b/32768*2000;
                 if(b>=2000)
                     b=b-4000;
                 if(imuflag==2)
                     Gyrowx->append(countx,b);
                 rDate.remove(0, 4);
                 Angle = rDate.left(4).toLocal8Bit();
                 Anglecopy=Angle;
                 Angle[0]=Anglecopy[2];
                 Angle[1]=Anglecopy[3];
                 Angle[2]=Anglecopy[0];
                 Angle[3]=Anglecopy[1];
                 angleint=QString(Angle);
                 b=angleint.toInt(nullptr,16);
                 b=b/32768*2000;
                 if(b>=2000)
                     b=b-4000;
                 if(imuflag==2)
                     Gyrowy->append(countx,b);
                 rDate.remove(0, 4);
                 Angle = rDate.left(4).toLocal8Bit();
                 Anglecopy=Angle;
                 Angle[0]=Anglecopy[2];
                 Angle[1]=Anglecopy[3];
                 Angle[2]=Anglecopy[0];
                 Angle[3]=Anglecopy[1];
                 angleint=QString(Angle);
                 b=angleint.toInt(nullptr,16);

                 b=b/32768*2000;
                 if(b>=2000)
                     b=b-4000;
                 if(imuflag==2)
                     Gyrowz->append(countx++,b);
                   rDate.remove(0, 14);
                   Angle = rDate.left(4).toLocal8Bit();
                   Anglecopy=Angle;
                   Angle[0]=Anglecopy[2];
                   Angle[1]=Anglecopy[3];
                   Angle[2]=Anglecopy[0];
                   Angle[3]=Anglecopy[1];
                   angleint=QString(Angle);
                   b=angleint.toInt(nullptr,16);
                   b=b/32768*180;
                   if(b>=180)
                       b=b-360;
                   if(b>20&&transflag==1){
                       timer6->start(10);
                       Multimachinedeformation();
                       transflag=0;
                       timer7->stop();
                   }
                   if(b<-20&&transflag==0){
                       timer6->start(10);
                       Multimachinedeformation();
                       transflag=1;
                       timer7->stop();
                   }
                   if(imuflag==0)
                       Gyrowx->append(countx,b);
                   ui->lcdNumber_3->display(b);
                   rDate.remove(0, 4);
                   Angle = rDate.left(4).toLocal8Bit();
                   Anglecopy=Angle;
                   Angle[0]=Anglecopy[2];
                   Angle[1]=Anglecopy[3];
                   Angle[2]=Anglecopy[0];
                   Angle[3]=Anglecopy[1];
                   angleint=QString(Angle);
                   b=angleint.toInt(nullptr,16);
                   b=b/32768*180;
                   if(b>=180)
                       b=b-360;
                   if(imuflag==0)
                      Gyrowy->append(countx,b);
                   ui->lcdNumber_4->display(b);
                   rDate.remove(0, 4);
                   Angle = rDate.left(4).toLocal8Bit();
                   Anglecopy=Angle;
                   Angle[0]=Anglecopy[2];
                   Angle[1]=Anglecopy[3];
                   Angle[2]=Anglecopy[0];
                   Angle[3]=Anglecopy[1];
                   angleint=QString(Angle);
                   b=angleint.toInt(nullptr,16);
                   b=b/32768*180;
                   if(b>=180)
                       b=b-360;
                   if(imuflag==0)
                     Gyrowz->append(countx++,b);
                   ui->lcdNumber_5->display(b);

             }
             if(countx==rightarea){
                 leftarea=rightarea;
                 rightarea+=500;}
             axisX3 = new QValueAxis;
             axisX3->setRange(leftarea, rightarea);//设置坐标轴范围
             //axisX2->setTitleText("time(secs)");//标题
             axisX3->setLabelFormat("%.0f"); //标签格式：每个单位保留几位小数
             axisX3->setTickCount(11); //主分隔个数：0到10分成20个单位
             axisX3->setMinorTickCount(0); //每个单位之间绘制了多少虚网线
             axisY3 = new QValueAxis; //Y 轴
             if(imuflag==1)
                  axisY3->setRange(-2, 2);
             else if(imuflag==2)
                  axisY3->setRange(-180, 180);
             else axisY3->setRange(-180, 180);
             axisY3->setTitleText("°/S or °");
             axisY3->setLabelFormat("%.0f"); //标签格式
             axisY3->setTickCount(13);
             axisY3->setMinorTickCount(0);
             axisX3->setGridLineVisible(false);
             chart3->setAxisX(axisX3, Gyrowx);
             chart3->setAxisY(axisY3, Gyrowx);
             chart3->setAxisX(axisX3, Gyrowy);
             chart3->setAxisY(axisY3, Gyrowy);
             chart3->setAxisX(axisX3, Gyrowz);
             chart3->setAxisY(axisY3, Gyrowz);
         }


    requestData.clear();    //清除缓冲区
//        for(int i = 0; i< requestData.length(); i++){
//            QString byteStr = QString::number(static_cast<uchar>(requestData[i]), 16);
//            if(byteStr.length() == 1) str += "0" + byteStr;
//            else str += byteStr;
//        }
//        qDebug()<<str;
}

void Widget::on_pushButton_clicked() //刷新串口
{
    port_init();
}

//void Widget::on_pushButton_2_clicked() //清空显示区
//{
//    ui->textEdit_read->clear();
//}

//void Widget::on_pushButton_3_clicked()//清空发送区
//{
//    ui->lineEdit_write->clear();
//}


//void Widget::on_hex_and_str_clicked()
//{
//    if(ui->hex_and_str->text() == "STR_show")
//    {
//        HEX_str_flag = 1;
//        ui->hex_and_str->setText("HEX_show");
//    }
//    else if(ui->hex_and_str->text() == "HEX_show")
//    {
//        HEX_str_flag = 0;
//        ui->hex_and_str->setText("STR_show");
//    }
//}




void Widget::QStringtoHex(QByteArray& sendData,QString str)
{
    char hstr,lstr,hdata,ldata;
    int len = str.length();
    int sendnum = 0;
    QByteArray temp;
    temp.resize(len/2);//设置大小，len/2会大于实际16进制字符
    //sendData.resize(len/2);
    for(int i=0;i<len;)
    {
        //hstr = str[i].toAscii();
        hstr = str[i].toLatin1();
        if(hstr == ' ')
        {
            ++i;
            continue;
        }
        ++i;
        if(i >= len)
        {
            break;
        }
        lstr = str[i].toLatin1();

        hdata = ConvertHexChar(hstr);
        ldata = ConvertHexChar(lstr);
        if(-1 == hdata || -1 == ldata)
        {
            break;
        }
        ++i;
        temp[sendnum] = hdata<<4|ldata;
        sendnum++;
    }
    sendData.reserve(sendnum);
    sendData = temp.left(sendnum);//去掉多余字符
}

char Widget::ConvertHexChar(char c)
{
    if(c>='a'&&c<='f')
    {
        return c-'a'+10;
    }
    else if(c>='A'&&c<='F')
    {
        return c-'A'+10;
    }
    else if(c>='0'&&c<='9')
    {
        return c-'0';
    }
    else{
        return -1;
    }
}

//void Widget::on_str_hex_send_clicked()
//{
//    if(ui->str_hex_send->text() == "STR_send")
//    {
//        send_HEX_str_flag = 1;
//        ui->str_hex_send->setText("HEX_send");
//    }
//    else if(ui->str_hex_send->text() == "HEX_send")
//    {
//        send_HEX_str_flag = 1;
//        ui->str_hex_send->setText("STR_send");
//    }
//}

//void Widget::on_pushButton_take_notes_clicked()//日志记录控制
//{
//    if(file_text.isNull())
//    {
//        qDebug() << "file_path is NULL" << endl;
//        return;
//    }
//    if(ui->pushButton_take_notes->text() == "stop")
//    {
//        ui->pushButton_take_notes->setText("start");
//        ui->pushButton_take_notes->setStyleSheet("background-color:red");
//    }
//    else if(ui->pushButton_take_notes->text() == "start")
//    {
//        ui->pushButton_take_notes->setText("stop");
//        ui->pushButton_take_notes->setStyleSheet("background-color:rgb(130,130,130)");
//    }
//}

//void Widget::on_pushButton_4_clicked() //日志保存位置
//{
//        QDateTime current_date_time =QDateTime::currentDateTime();   // 获取当前日期
//        QString current_date =current_date_time.toString("yyyy_MM_dd hh.mm.ss");

//        QString file_paht = QFileDialog::QFileDialog::getSaveFileName(this,  tr("Save File"), ".", tr("Text Files(*.txt)"));
//        if(file_paht.isEmpty())
//            return;
//        qDebug() << file_paht << endl;
//        file_text = file_paht;  //
//}
long Widget::play(HWND hWnd, NET_DVR_PREVIEWINFO struPlayInfo)
{
    struPlayInfo={0};
    struPlayInfo.hPlayWnd = hWnd;//需要 SDK 解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel = 1;//预览通道号
    struPlayInfo.dwStreamType = 0;//码流类型：0-主码流，1-子码流，2-三码流，3-虚拟码流，以此类推
    struPlayInfo.dwLinkMode = 0;//0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked = 1;//0- 非阻塞取流，1- 阻塞取流

    long IRealPlayHandle = NET_DVR_RealPlay_V40(userID,&struPlayInfo,NULL,NULL);

    if(IRealPlayHandle <0)
    {
        qDebug() << "NET_DVR_RealPlay_V40 error;error number " <<NET_DVR_GetLastError();
        QMessageBox::warning(this,"error","NET_DVR_RealPlay_V40 error;error number "+QString::number(NET_DVR_GetLastError()));
        NET_DVR_Logout(userID);
        userID=-1;
        return -1;
    }

    return IRealPlayHandle;
}
long Widget::play1(HWND hWnd, NET_DVR_PREVIEWINFO struPlayInfo)
{
    struPlayInfo={0};
    struPlayInfo.hPlayWnd = hWnd;//需要 SDK 解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel = 1;//预览通道号
    struPlayInfo.dwStreamType = 0;//码流类型：0-主码流，1-子码流，2-三码流，3-虚拟码流，以此类推
    struPlayInfo.dwLinkMode = 0;//0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked = 1;//0- 非阻塞取流，1- 阻塞取流

    long IRealPlayHandle = NET_DVR_RealPlay_V40(userID1,&struPlayInfo,NULL,NULL);

    if(IRealPlayHandle <0)
    {
        qDebug() << "NET_DVR_RealPlay_V40 error;error number " <<NET_DVR_GetLastError();
        QMessageBox::warning(this,"error","NET_DVR_RealPlay_V40 error;error number "+QString::number(NET_DVR_GetLastError()));
        NET_DVR_Logout(userID1);
        userID1=-1;
        return -1;
    }

    return IRealPlayHandle;
}

bool Widget::initSDK()
{
    bool isok = NET_DVR_Init();
    if(isok == false)
    {
        qDebug()<< "NET_DVR_Init error;error number is " <<NET_DVR_GetLastError();
        QMessageBox::warning(this,"error","NET_DVR_Init error;error number is "+QString::number(NET_DVR_GetLastError()));
        return isok;
    }
    //设置连接时间与重连时间
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    return isok;
}

bool Widget::uninitSDK()
{
    bool isok = NET_DVR_Cleanup();
    if(isok == false)
    {
        qDebug()<< "NET_DVR_Cleanup error;error number is " <<NET_DVR_GetLastError();
        QMessageBox::warning(this,"error","NET_DVR_Cleanup error;error number "+QString::number(NET_DVR_GetLastError()));
        return isok;
    }
    return isok;
}

void Widget::OnClickedLogin()
{
//    NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
//    userID=NET_DVR_Login_V30("192.168.1.64",8000,"admin","xiaoyang1998922",&DeviceInfoTmp);
//    if(userID<0)
//    {
//        qDebug() << "NET_DVR_Login_V30 error;" << "error number is " << NET_DVR_GetLastError();
//        QMessageBox::warning(this,"error","NET_DVR_Login_V30 error;error number "+QString::number(NET_DVR_GetLastError()));
//        return;
//    }
//    qDebug()<<"Login Success,userID:" << userID<<endl;
    NET_DVR_DEVICEINFO_V30 DeviceInfoTmp1;
    userID1=NET_DVR_Login_V30("192.168.1.11",8000,"admin","",&DeviceInfoTmp1);
    if(userID1<0)
    {
        qDebug() << "NET_DVR_Login_V30 error;" << "error number is " << NET_DVR_GetLastError();
        QMessageBox::warning(this,"error","NET_DVR_Login_V30 error;error number "+QString::number(NET_DVR_GetLastError()));
        return;
    }
    qDebug()<<"Login Success,userID1:" << userID1<<endl;
}

void Widget::OnClickedLogout()
{
    if(userID!=-1)
    {
        if(NET_DVR_Logout(userID)==false)
        {
            qDebug() << "NET_DVR_Logout error;" << "error number is " << NET_DVR_GetLastError();
            QMessageBox::warning(this,"error","NET_DVR_Logout error;error number "+QString::number(NET_DVR_GetLastError()));
            return;
        }
        userID=-1;
        qDebug() <<"Logout Success!"<<endl;
    }
    if(userID1!=-1)
    {
        if(NET_DVR_Logout(userID1)==false)
        {
            qDebug() << "NET_DVR_Logout error;" << "error number is " << NET_DVR_GetLastError();
            QMessageBox::warning(this,"error","NET_DVR_Logout error;error number "+QString::number(NET_DVR_GetLastError()));
            return;
        }
        userID1=-1;
        qDebug() <<"ID1 Logout Success!"<<endl;
    }
}

void Widget::ClickedPreview()
{
    if(userID==-1)
    {
        qDebug() <<"Need Login!"<<endl;
        QMessageBox::warning(this,"error","Need Login!");
        return;
    }
    HWND hWnd = (HWND)ui->label_5->winId();
    NET_DVR_PREVIEWINFO struPlayInfo;
    previewID = play(hWnd,struPlayInfo);
    if(previewID==-1)
    {
        return;
    }
    qDebug()<<"Preview Success!" << userID<<endl;
}
   void Widget::ClickedPreview1()
 {
    if(userID1==-1)
    {
        qDebug() <<"Need Login!"<<endl;
        QMessageBox::warning(this,"error","Need Login!");
        return;
    }
    HWND hWnd = (HWND)ui->label->winId();
    NET_DVR_PREVIEWINFO struPlayInfo;
    previewID1 = play1(hWnd,struPlayInfo);
    if(previewID1==-1)
    {
        return;
    }
    qDebug()<<"ID1 -Preview Success!" << userID1<<endl;
}

void Widget::OnClickedStopPreview()
{
    if(previewID!=-1)
    {
        if(NET_DVR_StopRealPlay(previewID)==false)
        {
            qDebug() << "NET_DVR_StopRealPlay error;" << "error number is " << NET_DVR_GetLastError();
            QMessageBox::warning(this,"error","NET_DVR_StopRealPlay error;error number "+QString::number(NET_DVR_GetLastError()));
            return;
        }
        previewID=-1;
        qDebug() <<"Stop Preview Success!"<<endl;

    }
    if(previewID1!=-1)
    {
        if(NET_DVR_StopRealPlay(previewID1)==false)
        {
            qDebug() << "NET_DVR_StopRealPlay error;" << "error number is " << NET_DVR_GetLastError();
            QMessageBox::warning(this,"error","NET_DVR_StopRealPlay error;error number "+QString::number(NET_DVR_GetLastError()));
            return;
        }
        previewID1=-1;
        qDebug() <<"Stop Preview Success!"<<endl;

    }
}

void Widget::OnClickedCapture()
{

    if(userID!=-1)
    {
        NET_DVR_JPEGPARA IpJpegPara2;
        IpJpegPara2.wPicQuality=0;
        IpJpegPara2.wPicSize=0xff;

        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy-MM-dd_hh-mm-ss_zzz");
        current_date="C:\\Picture\\"+current_date+".jpg";
        char*  fileName;
        QByteArray ba = current_date.toLatin1(); // must
        fileName=ba.data();


        if(NET_DVR_CaptureJPEGPicture(userID,1,&IpJpegPara2,fileName)==false)
        {
            qDebug() << "NET_DVR_CaptureJPEGPicture error;" << "error number is " << NET_DVR_GetLastError();
            QMessageBox::warning(this,"error","NET_DVR_CaptureJPEGPicture error;error number "+QString::number(NET_DVR_GetLastError()));
            return;
        }
        qDebug() <<"Capture Success!"<<endl;
    }
}

void Widget::xboxcontorl(){
    double valuew=m_gamepad->axisLeftX();
    double valuex=m_gamepad->axisLeftY();
    if(m_gamepad->buttonL2()>0.01&&m_gamepad->buttonL2()<0.95){
         ui->radioButton_2->setChecked(true);
        vlevel=2;
    }
    if(m_gamepad->buttonL2()==1.00||m_gamepad->buttonR2()==1.00){
         ui->radioButton->setChecked(true);
         vlevel=1;
    }
    if(m_gamepad->buttonR2()>0.01&&m_gamepad->buttonR2()<0.95){
         ui->radioButton_3->setChecked(true);
         vlevel=3;
    }

    if(m_gamepad->buttonL1())
         view2->show();
    if(m_gamepad->buttonR1())
         view->show();
    if(valuex<0.0001&&valuex>-0.0001)
         valuex=0;
    if(valuew<0.0001&&valuew>-0.0001)
         valuew=0;
    QByteArray up1;
    up1.resize(14);
    float value1=-valuex*100*0.5*vlevel;
    float value=valuew*100*vlevel;
    ui->progressBar->setValue(value1/vlevel);
    ui->progressBar_2->setValue(value/vlevel);
    up1[0]=0x3e;
    up1[1]=0xa2;
    up1[2]=0x02;   //ID2
    up1[3]=0x04;
    up1[4]=0xe6;
    up1[8] =((int_fast32_t)value1 >> 24);
    up1[7] = ((int_fast32_t)value1>>16);
    up1[6] =  ((int_fast32_t)value1>> 8);
    up1[5] =  (int_fast32_t)value1;
    up1[12] = ((int_fast32_t)value >> 24);
    up1[11] =  ((int_fast32_t)value>>16);
    up1[10] = ((int_fast32_t)value>> 8);
    up1[9] =  (int_fast32_t)value;
    up1[13]=up1[5]+up1[6]+up1[7]+up1[8]+up1[9]+up1[10]+up1[11]+up1[12];
    int speedx=(((up1[8]&0xff))<<24 |((up1[7]&0xff)<<16) | ((up1[6]&0xff)<<8) | (up1[5]&0xff));
    int speedw=(((up1[12]&0xff)<<24) |((up1[11]&0xff)<<16) | ((up1[10]&0xff)<<8) | (up1[9]&0xff));
    qDebug()<<"speedx="<<speedx<<"speedw="<<speedw;
    my_serialPort->write(up1);
    ui->lcdNumber_7->display(speedx*0.01);
    ui->lcdNumber_8->display(speedw*0.01);
    ui->lcdNumber_7->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber_8->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    up1.clear();
    series2->append(countx++,speedx);
    seriesw2->append(countx++,speedw);
    axisX2 = new QValueAxis;
    axisX2->setRange(leftarea, rightarea);//设置坐标轴范围
    //axisX2->setTitleText("time(secs)");//标题
    axisX2->setLabelFormat("%.0f"); //标签格式：每个单位保留几位小数
    axisX2->setTickCount(11); //主分隔个数：0到10分成20个单位
    axisX2->setMinorTickCount(0); //每个单位之间绘制了多少虚网线
    axisY2 = new QValueAxis; //Y 轴
    axisY2->setRange(-100, 100);
    axisY2->setTitleText("(m/s)");
    axisY2->setLabelFormat("%.0f"); //标签格式
    axisY2->setTickCount(13);
    axisY2->setMinorTickCount(0);
    axisX2->setGridLineVisible(false);
    chart2->setAxisX(axisX2, series2);
    chart2->setAxisY(axisY2, series2);
    chart2->setAxisX(axisX2, seriesw2);
    chart2->setAxisY(axisY2, seriesw2);



    Sleep(15);
    double valuew2=m_gamepad->axisRightX();
    double valuex2=m_gamepad->axisRightY();
    if(valuex2<0.0001&&valuex2>-0.0001)
        valuex2=0;
    if(valuew2<0.0001&&valuew2>-0.0001)
        valuew2=0;
    float value3=-valuex2*100*0.5*vlevel;
    float value2=valuew2*100*vlevel;
    ui->progressBar_3->setValue(value2/vlevel);
    ui->progressBar_4->setValue(value3/vlevel);
    up1[0]=0x3e;
    up1[1]=0xa2;
    up1[2]=0x01;   //ID1
    up1[3]=0x04;
    up1[4]=0xe6;
    up1[8] = (int)value3 >> 24;
    up1[7] = (int)value3>>16;
    up1[6] = (int)value3>> 8;
    up1[5] = (int)value3;
    up1[12] = (int)value2 >> 24;
    up1[11] = (int)value2>>16;
    up1[10] = (int)value2>> 8;
    up1[9] = (int)value2;
    up1[13]=up1[5]+up1[6]+up1[7]+up1[8]+up1[9]+up1[10]+up1[11]+up1[12];
    int speedx2=(((up1[8]&0xff))<<24 |((up1[7]&0xff)<<16) | ((up1[6]&0xff)<<8) | (up1[5]&0xff));
    int speedw2=(((up1[12]&0xff)<<24) |((up1[11]&0xff)<<16) | ((up1[10]&0xff)<<8) | (up1[9]&0xff));
    qDebug()<<"speedx2="<<speedx2<<"speedw2="<<speedw2;
    my_serialPort->write(up1);
    up1.clear();
    Sleep(15);
    if(m_gamepad->axisLeftX()<-0.5)
        ui->toolButton_5->setArrowType(Qt::LeftArrow);
    if(m_gamepad->axisLeftX()>0.5)
        ui->toolButton_5->setArrowType(Qt::RightArrow);
    if(m_gamepad->axisRightX()<-0.5)
        ui->toolButton_3->setArrowType(Qt::LeftArrow);
    if(m_gamepad->axisRightX()>0.5)
        ui->toolButton_3->setArrowType(Qt::RightArrow);
    if(m_gamepad->axisLeftY()>0&&m_gamepad->axisLeftX()<0.5&&m_gamepad->axisLeftX()>-0.5)
        ui->toolButton_5->setArrowType(Qt::DownArrow);
    if(m_gamepad->axisLeftY()<0&&m_gamepad->axisLeftX()<0.5&&m_gamepad->axisLeftX()>-0.5)
        ui->toolButton_5->setArrowType(Qt::UpArrow);
    if(m_gamepad->axisLeftY()==0&&m_gamepad->axisLeftX()<0.5&&m_gamepad->axisLeftX()>-0.5)
        ui->toolButton_5->setArrowType(Qt::NoArrow);
    if(m_gamepad->axisRightY()>0&&m_gamepad->axisRightX()<0.5&&m_gamepad->axisRightX()>-0.5)
        ui->toolButton_3->setArrowType(Qt::DownArrow);
    if(m_gamepad->axisRightY()<0&&m_gamepad->axisRightX()<0.5&&m_gamepad->axisRightX()>-0.5)
        ui->toolButton_3->setArrowType(Qt::UpArrow);
    if(m_gamepad->axisRightY()==0&&m_gamepad->axisRightX()<0.5&&m_gamepad->axisRightX()>-0.5)
        ui->toolButton_3->setArrowType(Qt::NoArrow);
    ui->lcdNumber->display(speedx2*0.01);
    ui->lcdNumber_2->display(speedw2*0.01);
    ui->lcdNumber->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber_2->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    // 添加实验数据,可以用 append 方法或者 >> 操作符
    series->append(countx++,speedx2);
    seriesw->append(countx++,speedw2);
    if(countx==rightarea){
        leftarea=rightarea;
        rightarea+=500;
        axisX = new QValueAxis;
        axisX->setRange(leftarea, rightarea);//设置坐标轴范围
//      axisX->setTitleText("time(secs)");//标题
        axisX->setLabelFormat("%.0f"); //标签格式：每个单位保留几位小数
        axisX->setTickCount(11); //主分隔个数：0到10分成20个单位
        axisX->setMinorTickCount(0); //每个单位之间绘制了多少虚网线
        axisY = new QValueAxis; //Y 轴
        axisY->setRange(-100, 100);
        axisY->setTitleText("(m/s)");
        axisY->setLabelFormat("%.0f"); //标签格式
        axisY->setTickCount(13);
        axisY->setMinorTickCount(0);
        axisX->setGridLineVisible(false);
        chart->setAxisX(axisX, series);
        chart->setAxisY(axisY, series);
        chart->setAxisX(axisX, seriesw);
        chart->setAxisY(axisY, seriesw);
    }

}
void Widget::timerxbox(){       
             timer->start(10);
             timer2->stop();
             timer3->stop();
             timer4->stop();
             timer5->stop();
             timer6->stop();
             timer7->stop();
             ui->horizontalSlider->setValue(0);
             ui->horizontalSlider_2->setValue(0);
             ui->verticalSlider->setValue(0);
             ui->verticalSlider_2->setValue(0);
             qDebug()<<"接入遥控";
}
bool Widget::eventFilter(QObject *obj, QEvent *event)//QEvent::MouseButtonDblClick 表示鼠标双击事件，判断是否为双击鼠标
{
  if (event->type() == QEvent::MouseButtonDblClick) {
     ui-> label->showNormal();
     ui-> label_5->showNormal();
     return true;  //事件不需要往下级传递，则返回真，表示已接收事件
  }
  else {
      // standard event processing
      return QObject::eventFilter(obj, event);  //事件往下传递
  }
}
void Widget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPainter painter2(this);
    QPixmap pix;
    QPixmap pix2;
    pix.load(":slide_4.png");
    pix2.load(":slideleft.png");
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    painter.setRenderHint(QPainter::Antialiasing);
   // painter.drawPixmap(0,0,919,600,pix2);
}
void Widget::speedlevel(){
    if(ui->radioButton->isChecked())
       vlevel=1;
    if(ui->radioButton_2->isChecked())
       vlevel=2;
    if(ui->radioButton_3->isChecked())
       vlevel=3;
     qDebug()<<vlevel;
}

//百度语音

void Widget::AudioInit()
{
    const auto &&availableDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
        if(!availableDevices.isEmpty())
        {
            SpeechCurrentDevice = availableDevices.first();
            QAudioFormat format;
            format.setSampleRate(8000);
            format.setChannelCount(1);
            format.setSampleSize(16);
            format.setSampleType(QAudioFormat::SignedInt);
            format.setByteOrder(QAudioFormat::LittleEndian);
            format.setCodec("audio/pcm");
            audio_input = new QAudioInput(SpeechCurrentDevice, format, this);
        }
}

void Widget::TokenInit()
{
    if(JudgeTokenTime()) return;
    QUrl url=QUrl(p_BaiduVoice->Get_Token_URL+
                  "?grant_type=client_credentials"+
                  "&client_id="+p_BaiduVoice->API_KEY+
                  "&client_secret="+p_BaiduVoice->Secret_Key);
    QNetworkAccessManager *manager=new QNetworkAccessManager(this);
    QNetworkRequest request(url);
    connect(manager,SIGNAL(finished(QNetworkReply *)),this,SLOT(get_Token_slot(QNetworkReply *)));
    manager->get(request);
}

void Widget::get_Token_slot(QNetworkReply *reply)
{
    QByteArray JsonStr=reply->readAll();
    QJsonObject acceptedData(QJsonDocument::fromJson(JsonStr).object());
    if(acceptedData.contains("access_token"))
    {
        QJsonValue json_value = acceptedData.take("access_token");
        p_BaiduVoice->Access_Token=json_value.toString();
        qDebug()<<"success get token:"<<p_BaiduVoice->Access_Token;
    }
}




//声音转成文本
void Widget::Baidu_VoiceToText_replyFinish(QNetworkReply *reply)
{
    qDebug()<<"进到槽函数了哦~~";
        QByteArray JsonStr=reply->readAll();
        qDebug()<<"返回信息 "<<JsonStr;
        QJsonObject acceptedData(QJsonDocument::fromJson(JsonStr).object());

        if(acceptedData.contains("err_no"))
        {
            QJsonValue version_value = acceptedData.take("err_no");
            if(version_value.isDouble())
            {
                int version = version_value.toVariant().toInt();
                if(version == 3300){
                    //                textEdit -> append("错误代码:3300-输入参数不正确,请仔细核对文档及参照demo，核对输入参数");//重新写入文本
                }else if(version == 3301){
                    //                textEdit -> append("错误代码:3301-音频质量过差,请上传清晰的音频");//重新写入文本
                    QMessageBox::information(NULL,tr("识别失败"),tr("请等待一秒后开始录音，确认录音质量良好"));
                }else if(version == 3302){
                    //                textEdit -> append("错误代码:3302-鉴权失败,token字段校验失败。请用appkey 和 app secret生成");//重新写入文本
                }else if(version == 3303){
                    //                textEdit -> append("错误代码:3303-转pcm失败,如下2个原因 1. wav amr音频转码失败，即音频有问题。2. 服务端问题，请将api返回结果反馈至论坛或者QQ群");//重新写入文本
                }else if(version == 3304){
                    //                textEdit -> append("错误代码:3304-用户的请求QPS超限,请降低识别api请求频率 （qps以appId计算，移动端如果共用则累计）");//重新写入文本
                }else if(version == 3305){
                    //                textEdit -> append("错误代码:3305-用户的日pv（日请求量）超限,请“申请提高配额”，如果暂未通过，请降低日请求量");//重新写入文本
                }else if(version == 3307){
                    //                textEdit -> append("错误代码:3307-语音服务器后端识别出错问题,请将api返回结果反馈至论坛或者QQ群");//重新写入文本
                }else if(version == 3308){
                    //                textEdit -> append("错误代码:3308-音频过长,音频时长不超过60s，请将音频时长截取为60s以下");//重新写入文本
                }else if(version == 3309){
                    //                textEdit -> append("错误代码:3309-音频数据问题,服务端无法将音频转为pcm格式，可能是长度问题，音频格式问题等。 请将输入的音频时长截取为60s以下，并核对下音频的编码，是否是8K或者16K， 16bits，单声道。");//重新写入文本
                }else if(version == 3310){
                    //                textEdit -> append("错误代码:3310-输入的音频文件过大,语音文件共有3种输入方式： json 里的speech 参数（base64后）； 直接post 二进制数据，及callback参数里url。 分别对应三种情况：json超过10M；直接post的语音文件超过10M；callback里回调url的音频文件超过10M");//重新写入文本
                }else if(version == 3311){
                    //                textEdit -> append("错误代码:3311-采样率rate参数不在选项里,目前rate参数仅提供8000,16000两种，填写4000即会有此错误");//重新写入文本
                }else if(version == 3312){
                    //                textEdit -> append("错误代码:3312-音频格式format参数不在选项里,目前格式仅仅支持pcm，wav或amr，如填写mp3即会有此错误");//重新写入文本
                }
                //            return;
            }
        }
        if(!acceptedData["result"].isNull())
        {
            QString message=acceptedData["result"].toArray()[0].toString();
            ui->InputText->setText(message);
            changeBaiduAudioAns(message);
        }
}
void Widget::on_radioButton_4_clicked(){
    if(ui->radioButton_4->isChecked()){
        timer3->start(10);
        timer->stop();
        timer2->stop();
        timer4->stop();
        timer7->stop();
        timer5->stop();
        timer6->stop();
        qDebug()<<"语音控制启动";
        disconnect(this,&Widget::changeV2T,this,&Widget::voice_post_Tuling_slot);
        ui->OutputText->setText("");
    }
    else {
         timer3->stop();
         qDebug()<<"语音控制关闭";
         connect(this,&Widget::changeV2T,this,&Widget::voice_post_Tuling_slot);
    }

}
void Widget::Multimachinedeformation(){
    timer2->stop();
    static int multitrans=2;
    static int multitrans2=2;
    static int count=0;
    QByteArray up1;
    float value3=-voicespeedx*100*0.5*vlevel*0;
    float value2=voicespeedw*100*vlevel*0;
    ui->progressBar_3->setValue(value2/vlevel);
    ui->progressBar_4->setValue(value3/vlevel);
    up1[0]=0x3e;
    up1[1]=0xa2;
    up1[2]=0x01;   //ID1
    up1[3]=0x04;   //transform
    up1[4]=0xe6;
    up1[8] = (int)value3 >> 24;
    up1[7] = (int)value3>>16;
    up1[6] = (int)value3>> 8;
    up1[5] = (int)value3;
    up1[12] = (int)value2 >> 24;
    up1[11] = (int)value2>>16;
    up1[10] = (int)value2>> 8;
    up1[9] = (int)value2;
    up1[13]=up1[5]+up1[6]+up1[7]+up1[8]+up1[9]+up1[10]+up1[11]+up1[12];
    if(multitrans==1) {
        up1[3]=0x05;
         qDebug()<<"multitrans="<<multitrans;
    }
    if(multitrans==2) {
        up1[3]=0x06;
        qDebug()<<"multitrans="<<multitrans;

    }
    if(multitrans==0) {
         qDebug()<<"multitrans="<<multitrans;

    }
    my_serialPort->write(up1);
    ui->lcdNumber->display(voicespeedx);
    ui->lcdNumber_2->display(voicespeedw);
    ui->lcdNumber->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber_2->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    up1.clear();
    Sleep(15);
    up1[0]=0x3e;
    up1[1]=0xa2;
    up1[2]=0x02;   //ID2
    up1[3]=0x04;   //transform
    up1[4]=0xe6;
    up1[8] = (int)value3 >> 24;
    up1[7] = (int)value3>>16;
    up1[6] = (int)value3>> 8;
    up1[5] = (int)value3;
    up1[12] = (int)value2 >> 24;
    up1[11] = (int)value2>>16;
    up1[10] = (int)value2>> 8;
    up1[9] = (int)value2;
    up1[13]=up1[5]+up1[6]+up1[7]+up1[8]+up1[9]+up1[10]+up1[11]+up1[12];
    if(multitrans2==1) {
        up1[3]=0x05;
         qDebug()<<"multitrans2="<<multitrans2;
    }
    if(multitrans2==2) {
        up1[3]=0x06;
        qDebug()<<"multitrans2="<<multitrans2;

    }
    if(multitrans2==0) {
         qDebug()<<"multitrans2="<<multitrans2;

    }
    my_serialPort->write(up1);
    up1.clear();
    Sleep(15);
    count++;
    if(count<512){
    if(count>120)
        multitrans2=0;
    if(count>450)
        multitrans=0;
    if(count>510){
        timer6->stop();
        timer7->start(2);
        multitrans2=1;
        multitrans=1;
    }
    }
    if(count>=512){
        if(count-512>120)
            multitrans2=0;
        if(count-512>450)
            multitrans=0;
        if(count>1050){
            timer6->stop();
            timer7->start(2);
            count=0;
            multitrans=2;
            multitrans2=2;
        }
    }
    qDebug()<<"count=="<<count;

}
void Widget::changeBaiduAudioAns(QString str)
{
    int backforword=1;
    QString hash="零一二三四五六七八九";
    QString hash2="0123456789";
    this->voice_get_ans=str;
    qDebug()<<"识别声音结果 "<<this->voice_get_ans;
    for(QChar a:voice_get_ans){
        if(a=="前")
            backforword=1;
        if(a=="后")
            backforword=-1;
    }
    for(int i=0;i<voice_get_ans.length()-4;i++){
        if(voice_get_ans[i]=="速"&&voice_get_ans[i+1]=="度"){
        for(int t=0;t<hash.length();t++){
            if(voice_get_ans[i+2]==hash[t]||voice_get_ans[i+2]==hash2[t])
                 voicespeedx=t;
        }
        if(voice_get_ans[i+3]=="点"||voice_get_ans[i+3]=="."){
         for(int t=0;t<hash.length();t++){
             if(voice_get_ans[i+4]==hash[t]||voice_get_ans[i+4]==hash2[t])
                 voicespeedx=backforword*(voicespeedx+(t)*0.1);
         }

         }
    }
    }
    for(QChar a:voice_get_ans){
        if(a=="停"){
            voicespeedx=0;
            voicespeedw=0;
            trans=0;
        }
        if(a=="转"){
            voicespeedx=0;
            voicespeedw=1;
        }
        if(a=="还"){
            trans=1;
            timer5->start(21000);
        }
        if(a=="变"){
            trans=2;
            timer5->start(21000);
        }

    }
    emit changeV2T();
    }

void Widget::VoiceControl(){
    QByteArray up1;
    float value3=-voicespeedx*100*0.5*vlevel;
    float value2=voicespeedw*100*vlevel;
    ui->progressBar_3->setValue(value2/vlevel);
    ui->progressBar_4->setValue(value3/vlevel);
    up1[0]=0x3e;
    up1[1]=0xa2;
    up1[2]=0x01;   //ID1
    up1[3]=0x04;
    up1[4]=0xe6;
    up1[8] = (int)value3 >> 24;
    up1[7] = (int)value3>>16;
    up1[6] = (int)value3>> 8;
    up1[5] = (int)value3;
    up1[12] = (int)value2 >> 24;
    up1[11] = (int)value2>>16;
    up1[10] = (int)value2>> 8;
    up1[9] = (int)value2;
    up1[13]=up1[5]+up1[6]+up1[7]+up1[8]+up1[9]+up1[10]+up1[11]+up1[12];
    if(trans==1) {
        up1[3]=0x05;
        qDebug()<<trans;
    }
    if(trans==2) {
        up1[3]=0x06;
        qDebug()<<up1[3];
    }
    if(trans==0) {

        qDebug()<<trans;

    }
    my_serialPort->write(up1);
    ui->lcdNumber->display(voicespeedx);
    ui->lcdNumber_2->display(voicespeedw);
    ui->lcdNumber->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber_2->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    up1.clear();
    Sleep(15);
    qDebug()<<"voicespeedx=="<<voicespeedx;


}

bool Widget::JudgeTokenTime()
{
    QSettings *setting=new QSettings("GetTokenTime.ini",QSettings::IniFormat);
    //当前时间
    QDateTime current_date_time=QDateTime::currentDateTime();
    QDateTime file_date_time;
    //读取的文件时间
    QString file_date_str=setting->value("/TIME/last_time").toString();
    file_date_time=QDateTime::fromString(file_date_str,"yyyy-MM-dd");
    //    qDebug()<<"file_date_time"<<file_date_time;
    uint stime = file_date_time.toTime_t();
    uint etime = current_date_time.toTime_t();
    int ndaysec = 24*60*60;
    int Day= (etime - stime)/(ndaysec) + ((etime - stime)%(ndaysec)+(ndaysec-1))/(ndaysec) - 1;
    //Token 有效期内
    if(Day<30)    return true;
    else
    {
        //写时间
        setting->beginGroup("TIME");
        setting->setValue("last_time",current_date_time.toString("yyyy-MM-dd"));
        setting->endGroup();
        return false;
    }
}

//更改UI_ANS_String函数，用于发射信号
void Widget::setUIString(QString str)
{
    this->UI_ANS_String=str;
    this->ui->OutputText->setText(str);
    emit changeUIANS();
}

//获取本机MAC地址
void Widget::getMacAddress()
{

    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            p_BaiduVoice->MAC_cuid = nets[i].hardwareAddress();
            break;
        }
    }
    p_BaiduVoice->MAC_cuid.replace(":","-");
    qDebug()<<"自动获取的MAC地址 "<<p_BaiduVoice->MAC_cuid;
}

//获得图灵机器人的回答后再播放这个回答
void Widget::Baidu_TextToVoice_replyFinish()
{
    //最后一步 播放文本音频
    //设置url以及必要的请求格式
    QByteArray url="http://tsn.baidu.com/text2audio?";
    url.append(QString("&lan=zh"));
    url.append(QString("&cuid="+p_BaiduVoice->MAC_cuid));
    url.append("&ctp=1");
    url.append(QString("&tok="+p_BaiduVoice->Access_Token));
    url.append(QString("&pit=5"));
    url.append(QString("&per=4"));
    url.append(QString("&tex="));
    url.append(QUrl::toPercentEncoding(this->UI_ANS_String));
    qDebug()<<url;
    //播放url里面的音频
    media_player->setMedia(QUrl::fromLocalFile(url));
    media_player->play();
}

//当声音识别成文本后，发射信号到此函数，然后将文本发送给青云客
void Widget::voice_post_Tuling_slot()
{
    qDebug()<<"successes get voice_post_Tuling_slot";
    QString Url="http://api.qingyunke.com/api.php?key=free&appid=0&msg="+this->voice_get_ans;
    QUrl url;
    url.setUrl(Url);
    QNetworkRequest request(url);
    QNetworkAccessManager *manager=new QNetworkAccessManager(this);
    //处理获得文本，连接到voice_Tuling_replyFinish函数
    connect(manager,SIGNAL(finished(QNetworkReply *)),this,SLOT(voice_Tuling_replyFinish(QNetworkReply *)));
    manager->get(request);

}


//解析图灵API返回的json数据
void Widget::Tuling_replyFinish(QNetworkReply *reply)
{

    QString data=reply->readAll();//读取回话信息
    qDebug()<<"这是返回的结果 "+data;
    QJsonParseError json_error;
    //一定要toUtf8 否则会出错！
    QJsonDocument json=QJsonDocument::fromJson(data.toUtf8(),&json_error);
    //判断有没有错
    if(json_error.error==QJsonParseError::NoError)
    {
        if(json.isObject())
        {
            QJsonObject jsonObj=json.object();
            //取到text内容
            if(jsonObj.contains("content"))
            {
                qDebug()<<"成功获取text";
                QJsonValue text_value=jsonObj.take("content");
                if(text_value.isString())
                {
                    tuling_get_ans=text_value.toVariant().toString();
                    tuling_get_ans.replace("br","\n");
                    qDebug()<<"!!!返回的ans:"<<tuling_get_ans;
                }
            }
        }
    }
    reply->deleteLater();
    ui->OutputText->setText(tuling_get_ans);

}

//走语音识别的流程 这是将语音识别文本发送给图灵机器人，然后解析json
void Widget::voice_Tuling_replyFinish(QNetworkReply *reply)
{

    qDebug()<<"进入voice_Tuling_replyFinish...";
    QString data=reply->readAll();//读取回话信息
    qDebug()<<"这是返回的结果 "+data;
    QString tstr;
    QJsonParseError json_error;
    //一定要toUtf8 否则会出错！
    QJsonDocument json=QJsonDocument::fromJson(data.toUtf8(),&json_error);
    //判断有没有错
    if(json_error.error==QJsonParseError::NoError)
    {
        if(json.isObject())
        {
            QJsonObject jsonObj=json.object();
            //取到text内容
            if(jsonObj.contains("content"))
            {
                qDebug()<<"成功获取text";
                QJsonValue text_value=jsonObj.take("content");
                if(text_value.isString())
                {
                    tstr=text_value.toVariant().toString();
                    qDebug()<<"!!!返回的结果:"<<tuling_get_ans;
                }
            }
        }
    }
    reply->deleteLater();
    //    获得返回文本后
    this->setUIString(tstr);
    ui->axWidget->resize(500,300);
    ui->axWidget->setControl(QString::fromUtf8("{d27cdb6e-ae6d-11cf-96b8-444553540000}"));

    ui->axWidget->dynamicCall("LoadMovie(long,string)",0,"C:/Users/asus/Desktop/flash2544.swf"); //调用方法用于处理FLASH传来的字符串

    ui->axWidget->dynamicCall("FrameNum",200);//设置属性

    ui->axWidget->show();
}

void Widget::on_voiceBtn_pressed()
{

    movie->start();
    sirilabel->show();
    QIcon iconvoicecolor;
    iconvoicecolor.addFile(QString::fromUtf8(":/siri123.png"), QSize(), QIcon::Selected, QIcon::On);
    ui->voiceBtn->setIcon(iconvoicecolor);
    ui->axWidget->close();
    JsonBuffer=new QBuffer;
    JsonBuffer->open(QIODevice::ReadWrite);
    qDebug()<<"录音设备访问成功...";
    audio_input->start(JsonBuffer);
    qDebug()<<"录音设备运行成功...";

}

void Widget::on_voiceBtn_released()
{

    movie->stop();
    sirilabel->hide();
    qDebug()<<"结束录音哦~~~";
    audio_input->stop();
    const auto &sendData=JsonBuffer->data();
    JsonBuffer->deleteLater();
    QNetworkRequest request(QUrl(p_BaiduVoice->VOP_URL));
    request.setRawHeader("Content-Type", "application/json");
    qDebug()<<"jason_speech:request设置头成功 ...";
    QNetworkAccessManager *manager=new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply *)),this,SLOT(Baidu_VoiceToText_replyFinish(QNetworkReply *)));
    //这里应该放在百度类里面
    QJsonObject append;
    //设置json请求格式
    append["format"] = "pcm";
    append["rate"] = 8000;
    append["channel"] = 1;
    append["token"] = p_BaiduVoice->Access_Token;
    append["lan"] = "zh";
    //    append["cuid"] = "A0-8C-FD-1D-CF-0E";
    append["cuid"] = p_BaiduVoice->MAC_cuid;
    append["speech"] = QString(sendData.toBase64());
    append["len"] = sendData.size();
    //发送HTTP post请求
    manager->post(request,QJsonDocument(append).toJson());
}


baiduVoice::baiduVoice()
{
}
void Widget::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_Space:
            if(!event->isAutoRepeat()){
                movie->start();
                sirilabel->show();
                qDebug()<<"Key_Tab pressed";

            }
            break;

        /*default:
            this->ui.textEdit->append("KeyEvent");*/
    }
}

void Widget::keyReleaseEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_Space:
            {
                movie->stop();
                sirilabel->hide();
                qDebug()<<"Key_Tab Release";
            }
            break;
        /*default:
            this->ui.textEdit->append("KeyEvent");*/
    }
}
void Widget::on_toolButton_clicked(){
    timer4->start(50);
    timer->stop();
    timer2->stop();
    timer3->stop();
    timer5->stop();
    timer6->stop();
    timer7->stop();
}
void Widget::HandleControl(){
    QByteArray up1;
    up1.resize(14);
    float value1=handle->v*0.5*vlevel;
    float value=handle->w*vlevel;
    ui->progressBar->setValue(value1/vlevel);
    ui->progressBar_2->setValue(value/vlevel);
    up1[0]=0x3e;
    up1[1]=0xa2;
    up1[2]=0x02;   //ID2
    up1[3]=0x04;
    up1[4]=0xe6;
    up1[8] =((int_fast32_t)value1 >> 24);
    up1[7] = ((int_fast32_t)value1>>16);
    up1[6] =  ((int_fast32_t)value1>> 8);
    up1[5] =  (int_fast32_t)value1;
    up1[12] = ((int_fast32_t)value >> 24);
    up1[11] =  ((int_fast32_t)value>>16);
    up1[10] = ((int_fast32_t)value>> 8);
    up1[9] =  (int_fast32_t)value;
    up1[13]=up1[5]+up1[6]+up1[7]+up1[8]+up1[9]+up1[10]+up1[11]+up1[12];
    int speedx=(((up1[8]&0xff))<<24 |((up1[7]&0xff)<<16) | ((up1[6]&0xff)<<8) | (up1[5]&0xff));
    int speedw=(((up1[12]&0xff)<<24) |((up1[11]&0xff)<<16) | ((up1[10]&0xff)<<8) | (up1[9]&0xff));
    qDebug()<<"speedx="<<speedx<<"speedw="<<speedw;
    my_serialPort->write(up1);
    ui->lcdNumber_7->display(speedx*0.01);
    ui->lcdNumber_8->display(speedw*0.01);
    ui->lcdNumber_7->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber_8->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    up1.clear();
    Sleep(15);
    float value3=handle2->v*0.5*vlevel;
    float value2=handle2->w*vlevel;
    ui->progressBar_3->setValue(value2/vlevel);
    ui->progressBar_4->setValue(value3/vlevel);
    up1[0]=0x3e;
    up1[1]=0xa2;
    up1[2]=0x01;   //ID1
    up1[3]=0x04;
    up1[4]=0xe6;
    up1[8] = (int)value3 >> 24;
    up1[7] = (int)value3>>16;
    up1[6] = (int)value3>> 8;
    up1[5] = (int)value3;
    up1[12] = (int)value2 >> 24;
    up1[11] = (int)value2>>16;
    up1[10] = (int)value2>> 8;
    up1[9] = (int)value2;
    up1[13]=up1[5]+up1[6]+up1[7]+up1[8]+up1[9]+up1[10]+up1[11]+up1[12];
    int speedx2=(((up1[8]&0xff))<<24 |((up1[7]&0xff)<<16) | ((up1[6]&0xff)<<8) | (up1[5]&0xff));
    int speedw2=(((up1[12]&0xff)<<24) |((up1[11]&0xff)<<16) | ((up1[10]&0xff)<<8) | (up1[9]&0xff));
    qDebug()<<"speedx2="<<speedx2<<"speedw2="<<speedw2;
    ui->lcdNumber->display(speedx2*0.01);
    ui->lcdNumber_2->display(speedw2*0.01);
    ui->lcdNumber->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->lcdNumber_2->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    my_serialPort->write(up1);
    up1.clear();
    Sleep(15);
}
