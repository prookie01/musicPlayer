#ifndef WIDGET_H
#define WIDGET_H

#pragma once
#include <QWidget>
#include<QTimer>
#include<QLabel>
#include<QMouseEvent>
#include<QPoint>
#include<QPushButton>
#include<QFile>
#include<QRandomGenerator>
#include"wmusic.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    //成员变量
    Ui::Widget *ui;
    //图片
    QLabel* aniya;
    QLabel* label;
    int pictureFlag;
    //按钮
    QPushButton* butColse;
    bool buttonFlag;

    QPushButton* btuSyp;
    QPushButton* btuMusic;
    //事件
    QTimer* updateTime;

    wMusic* wmc;
    //成员函数
    void initWindows();
    void loadImagesTimer();
    bool eventFilter(QObject* watched,QEvent* ev)override;
    void initButton();
    void connectButton();
    void showEndImage();

};
#endif // WIDGET_H
