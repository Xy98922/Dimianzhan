#include "Handle.h"
Handle::Handle(QWidget *parent)
    : QWidget(parent)
{
    //setAutoFillBackground(true);
    setPalette(QPalette(Qt::gray));
    resize(106,106);
    setMinimumSize(106,106);
    mouseX=width()/2;
    mouseY=height()/2;     //归中点
    tim=new QTimer(this);
    connect(tim,&QTimer::timeout,this,&Handle::getKeyNum);
    connect(this,&Handle::keyNumchanged,this,[=](int num){
        //qDebug()<<num<<endl;
    });
}

Handle::~Handle()
{

}
void Handle::paintEvent(QPaintEvent *){

    QPainter painter(this);

    int side = qMin(width(), height());

    padR=side/2; //底盘半径
    padX=padR;//底盘圆心
    padY=padR;//底盘圆心
    handleR=43/4;//摇杆圆半径（小灰圆点的半径）
    int handleMaxR=padR-handleR;  //小灰圆点运动的最大距离
    QColor handleColor(129,125,129);
    //加载底盘图像
//    painter.save();

//    painter.scale(side / 400.0, side / 400.0);//坐标会随窗口缩放
//    painter.drawPixmap(0, 0, QPixmap(":20191016111900901.png"));
//    painter.restore();

    //自绘底盘
//    painter.save();
//    QRadialGradient RadialGradient(padR,padR,padR*3,padR,padR);//圆心2，半径1，焦点2
//    RadialGradient.setColorAt(0,QColor(90,90,90,255));//渐变
//    RadialGradient.setColorAt(1,QColor(255,255,255,125));//渐变
//    painter.setBrush(RadialGradient);
//    painter.setPen(Qt::NoPen);
//    painter.drawEllipse(QPoint(padR,padR),side/2,side/2);//大圆盘
//    painter.restore();
//    painter.setRenderHint(QPainter::Antialiasing);

   // painter.drawText(20,20,tr("%1,%2,%3").arg(mouseX).arg(mouseY).arg(handPaddis));

    if(!mousePressed){//鼠标没按下则摇杆恢复到底盘中心
        mouseX=padX;
        mouseY=padY;
    }
    handPadDis=Pointdis(padR,padR,mouseX,mouseY);  //鼠标距离圆盘中心距离
    if(handPadDis<=handleMaxR){
        handleX=mouseX;
        handleY=mouseY;
    }
    else {
        handleX=(int)(handleMaxR*(mouseX-padX)/handPadDis+padX);
        handleY=(int)(handleMaxR*(mouseY-padY)/handPadDis+padY);
    } //设置摇杆运动的边界

   // painter.drawText(200,200,tr("%1,%2,%3").arg(handleX).arg(handleY).arg(handPaddis));
    painter.setPen(Qt::NoPen);
    painter.setBrush(handleColor);
    painter.drawEllipse(QPoint(handleX,handleY),handleR,handleR);//摇杆
}
void Handle::mouseMoveEvent(QMouseEvent* event){
    static bool r=false;
    mouseX=event->pos().x();
    mouseY=event->pos().y();
//    qDebug()<<"handleX"<<handleX<<endl;
//    qDebug()<<"handleY="<<handleY<<endl;
    if(r==true){
       update();
       r=false;
    }
    else{
        r=true;
    }
}
void Handle::mouseReleaseEvent(QMouseEvent* event){
    mouseX=width()/2;
    mouseY=height()/2;
    mousePressed=false;
    emit keyNumchanged(Handle::stop);
    update();
}
void Handle::mousePressEvent(QMouseEvent* event){
    mouseX=event->pos().x();
    mouseY=event->pos().y();
    tim->start(100);
    mousePressed=true;
    update();
}

double Handle::Pointdis(int a,int b,int x,int y){
    return sqrt((double)((x-a)*(x-a)+(y-b)*(y-b)));
}
void Handle::getKeyNum(){
    w=(handleX-padX)*100/42.25;
    v=-(handleY-padY)*100/42.25;
}
