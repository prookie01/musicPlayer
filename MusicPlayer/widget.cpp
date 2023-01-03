#pragma once
#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
#include<ctime>
#include<Windows.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,aniya(new QLabel(this))
    ,label(new QLabel(this))
    ,pictureFlag(1)
    ,buttonFlag(false)
    ,wmc(NULL)
{
    ui->setupUi(this);
    this->initWindows();
    this->installEventFilter(this);//装载鼠标事件
    this->initButton();
    this->connectButton();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initWindows()
{
    this->setFixedSize(300,300);
    //设置去掉边框
    this->setWindowFlag(Qt::WindowType::FramelessWindowHint,true);
    //设置背景透明
    this->setAttribute(Qt::WA_TranslucentBackground);
    //图片轮播
    this->label->setVisible(false);
    aniya->setGeometry(0,0,200,200);
    this->updateTime = new QTimer(this);
    updateTime->callOnTimeout(this,&::Widget::loadImagesTimer);
    this->loadImagesTimer();
    updateTime->start(1000);
}

void Widget::loadImagesTimer()
{

    QRandomGenerator* rand = new QRandomGenerator();
    //aniya->setGeometry(0,0,150,150);
    rand->seed(time(NULL));
    QString header=":/picture/aniya/b";
    QString end = ".png";
    this->pictureFlag = rand->bounded(1,9);
    QString imgname = header+QString::number(this->pictureFlag)+end;
    //qDebug()<<imgname<<"\n";
    aniya->setPixmap(QPixmap::fromImage(QImage(imgname)).scaled(aniya->width(),aniya->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

}
//重写鼠标事件
bool Widget::eventFilter(QObject *watched, QEvent *ev)
{
    QMouseEvent* mouseev = static_cast<QMouseEvent*>(ev);
    //判断鼠标左键按下
    //判断鼠标移动
    static QPoint begpos;
    if(ev->type() == QEvent::MouseButtonPress)
    {
        begpos = mouseev->globalPos() - this->pos();

    }
    if(ev->type() == QEvent::MouseButtonDblClick && this->buttonFlag == false){

         this->butColse->setVisible(true);
         this->btuSyp->setVisible(true);
         this->btuMusic->setVisible(true);
         this->buttonFlag=true;
    }
    else if(ev->type() == QEvent::MouseButtonDblClick && this->buttonFlag == true){

         this->butColse->setVisible(false);
         this->btuSyp->setVisible(false);
         this->btuMusic->setVisible(false);
         this->buttonFlag=false;
    }

    if(ev->type()==QEvent::MouseMove &&
            mouseev->buttons()&Qt::MouseButton::LeftButton)
    {
        this->move(mouseev->globalPos()-begpos);
        //this->butColse->setVisible(true);
    }

    return false;
}

void Widget::initButton()
{
    //close按钮
    this->butColse = new QPushButton(this);
    //this->butColse->resize(35,35);
    this->butColse->setStyleSheet("border-radius:150px;");
    //设置大小及位置
    this->butColse->setGeometry(200,50,35,35);
    this->butColse->setIcon(QIcon(":/icon/icon/close.png"));
    this->butColse->setIconSize(QSize(35,35));
    this->butColse->setVisible(false);

    //spy按钮设计
    this->btuSyp = new QPushButton(this);
    this->btuSyp ->setStyleSheet("border-radius:150px;");
    this->btuSyp ->setGeometry(200,90,35,35);
    this->btuSyp ->setIcon(QIcon(":/icon/icon/syp.png"));
    this->btuSyp->setIconSize(QSize(35,35));
    this->btuSyp->setVisible(false);

    //music按钮
    this->btuMusic = new QPushButton(this);
    this->btuMusic ->setStyleSheet("border-radius:150px;");
    this->btuMusic ->setGeometry(200,130,35,35);
    this->btuMusic ->setIcon(QIcon(":/icon/icon/music.png"));
    this->btuMusic->setIconSize(QSize(35,35));
    this->btuMusic->setVisible(false);

}

void Widget::connectButton()
{
    connect(this->butColse,&QPushButton::clicked,[=](){
        showEndImage();
        Sleep(1000);
        this->close();

    });
    connect(this->btuSyp,&QPushButton::clicked,[=](){
        system("start /b https://www.bilibili.com/bangumi/media/md28237119/?spm_id_from=666.25.b_6d656469615f6d6f64756c65.2");
    });
    connect(this->btuMusic,&QPushButton::clicked,[=](){

        if(this->wmc!=NULL)
        {
            this->wmc->show();
        }
        else{
            this->wmc = new wMusic();
            this->wmc->show();
        }
    });
}

void Widget::showEndImage()
{
    qDebug()<<"结束"<<"\n";
    this->updateTime->stop();
    this->aniya->clear();
    this->label->clear();
    this->label->setVisible(true);
    aniya->setPixmap(QPixmap::fromImage(QImage(":/picture/aniya/b9.png")).scaled(aniya->width(),aniya->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    this->label->setPixmap(QPixmap(":/icon/icon/NO.png"));
    this->label->setGeometry(40,210,120,40);
    this->aniya->repaint();
    this->label->repaint();

}

