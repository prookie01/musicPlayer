#pragma once
#include "wmusic.h"
#include "ui_wmusic.h"
#include<QFileDialog>
#include<QDebug>
#include<QTextList>
#include<string.h>
#include<QMovie>
#include<QRandomGenerator>
wMusic::wMusic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wMusic),
    music(new QMediaPlayer(this)),
    musicPlayList(new QMediaPlaylist(this)),
    musicListRow(0),
    themeFlag("theme1"),
    volume(40)
{
    ui->setupUi(this);
    this->initFrame();
    this->initCenterLabel();

    this->initFile();
    this->installEventFilter(this);
    this->initButton();
    this->initBtuMenu();
    this->connectButton();
}

wMusic::~wMusic()
{
    delete ui;
}

void wMusic::initFile()
{
    this->file = new QFile("./file_Address.txt");
    if(!this->file->open(QIODevice::ReadOnly))
    {
        qDebug()<<"文件file_Address读取失败！\n";
        return;
    }
    else if(this->file->size() == 0)
    {
        return ;
    }
    else{
        while(!file->atEnd())
        {
            this->file_Names = file->readLine();
        }
        this->file->close();
    }


    this->fileData = new QFile("./dataList.txt");
    if(!this->fileData->open(QIODevice::ReadWrite))
    {
        qDebug()<<"文件dataList读取失败！\n";
        return;
    }
    else if(this->fileData->size() == 0)
    {
        return ;
    }
    else{

        QTextStream in(fileData);
        in.setCodec("GBK");
        QString line;
        line=in.readLine();
        while(!line.isNull())
        {
            this->dataList.append(line);
            line=in.readLine();
        }
        this->fileData->close();

        //把数据写入列表组件
        if(this->dataList.size()!=0)
        {
            for(int i=0;i<dataList.size();i++)
            {
                this->qvMusic->addItem(this->dataList[i]);
            }
        }
        else{
            return;
        }
    }
}

void wMusic::initFrame()
{
    this->setFixedSize(800,600);
    this->setWindowFlag(Qt::WindowType::FramelessWindowHint,true);
    this->setStyleSheet("background-color:rgb(125,210,240);"

                        );
    this->setListWidget();
    this->movie = new QLabel(this);
    this-> mv = new QMovie(":/icon/icon/640.gif");
    this->mv->setScaledSize(QSize(150,150));
    this->movie->setMovie(this->mv);
    this->movie->setGeometry(50,360,150,150);
    this->mv->start();
    this->mv->setPaused(true);
    //进度条
    this->progressBar = new QProgressBar(this);
    this->progressBar->setMaximum(100);
    this->progressBar->setMinimum(0);
    this->progressBar->setGeometry(50,560,200,15);
    this->progressBar->setTextVisible(false);
    this->progressBar->setValue(this->volume);
    this->progressBar->setStyleSheet("QProgressBar{"
                                     "border: 2px solid grey;"
                                     "border-radius: 5px;"
                                     "background-color: #FFFFFF;}"
                                     "QProgressBar::chunk {"
                                     "background-color:rgb(181,229,246);"
                                     "}");
}

void wMusic::initCenterLabel()
{
    this->centerLabel = new QLabel(this);
    this->centerLabel->setGeometry(350,100,400,400);
    this->centerLabel->setStyleSheet("background-color:rgb(181,229,246);"
                                     "border-style:solid;"
                                     "border-width:10px;"
                                     "border-color:black;"
                                     "border-radius:20px;");

    this->updataTimer = new QTimer(this);
    this->updataTimer->callOnTimeout(this,&::wMusic::loadimageTimer);
    this->loadimageTimer();
    this->updataTimer->stop();
    //updataTimer->start(5000);
}

void wMusic::loadimageTimer()
{
    QRandomGenerator* rand = new QRandomGenerator();
    rand->seed(time(NULL));
    QString header=":/picture/";
    QString end = ".png";
    this->imageFlag = rand->bounded(1,9);
    QString imgname = header+this->themeFlag+"/"+QString::number(this->imageFlag)+end;
    this->centerLabel->setPixmap(QPixmap::fromImage(QImage(imgname)).scaled(this->centerLabel->width(),this->centerLabel->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    if(this->music->mediaStatus() == 7)
    {
        this->mv->setPaused(true);
        this->updataTimer->stop();
    }
}
//窗体移动事件
bool wMusic::eventFilter(QObject *watched, QEvent *ev)
{
    QMouseEvent* mouseev = static_cast<QMouseEvent*>(ev);
    //判断鼠标左键按下
    //判断鼠标移动
    static QPoint begpos;
    if(ev->type() == QEvent::MouseButtonPress)
    {
        begpos = mouseev->globalPos() - this->pos();

    }
    else if(ev->type()==QEvent::MouseMove &&
            mouseev->buttons()&Qt::MouseButton::LeftButton)
    {
        this->move(mouseev->globalPos()-begpos);
    }
    return false;
}

//按钮加载
void wMusic::initButton()
{
    //标题按钮-方形
    //关闭按钮
    this->btuClose = new QPushButton(this);
    this->btuClose->setStyleSheet("border-radius:150px;");
    this->btuClose->setGeometry(750,10,40,40);
    this->btuClose->setIcon(QIcon(":/icon/icon/close.png"));
    this->btuClose->setIconSize(QSize(40,40));
    //导入文件按钮
    this->btuOpen = new QPushButton(this);
    this->btuOpen->setGeometry(20,20,100,35);
    this->btuOpen->setIcon(QIcon(":/icon/icon/addFile.png"));
    this->btuOpen->setStyleSheet("border-radius:5px;");
    this->btuOpen->setIconSize(QSize(100,35));
    //清空播放列表
    this->btuClearList = new QPushButton(this);
    this->btuClearList->setGeometry(140,20,100,35);
    this->btuClearList->setStyleSheet("border-radius:5px;");
    this->btuClearList->setIcon(QIcon(":/icon/icon/clearList.png"));
    this->btuClearList->setIconSize(QSize(100,35));



    //互动按钮
    //开始播放按钮
    this->btuStart = new QPushButton(this);
    this->btuStart->setStyleSheet("border-radius:150px;");
    this->btuStart->setGeometry(25,500,40,40);
    this->btuStart->setIcon(QIcon(":/icon/icon/start.png"));
    this->btuStart->setIconSize(QSize(40,40));
    //关闭音乐按钮
    this->btuCloseMusic = new QPushButton(this);
    this->btuCloseMusic->setStyleSheet("border-radius:150px;");
    this->btuCloseMusic->setGeometry(125,500,40,40);
    this->btuCloseMusic->setIcon(QIcon(":/icon/icon/end.png"));
    this->btuCloseMusic->setIconSize(QSize(40,40));
    //暂停音乐按钮
    this->btuStopMusic = new QPushButton(this);
    this->btuStopMusic->setStyleSheet("border-radius:150px;");
    this->btuStopMusic->setGeometry(75,500,40,40);
    this->btuStopMusic->setIcon(QIcon(":/icon/icon/stop.png"));
    this->btuStopMusic->setIconSize(QSize(40,40));
    //音量+
    this->btuUp = new QPushButton(this);
    this->btuUp->setStyleSheet("border-radius:15px;");
    this->btuUp->setGeometry(195,503,35,35);
    this->btuUp->setIcon(QIcon(":/icon/icon/Up.png"));
    this->btuUp->setIconSize(QSize(35,35));
    //音量-
    this->btuDown = new QPushButton(this);
    this->btuDown->setStyleSheet("border-radius:15px;");
    this->btuDown->setGeometry(240,503,35,35);
    this->btuDown->setIcon(QIcon(":/icon/icon/Down.png"));
    this->btuDown->setIconSize(QSize(35,35));


}

void wMusic::initBtuMenu()
{
    //下拉按钮
    this->btuTheme = new QPushButton(this);
    this->btuTheme->setGeometry(260,20,100,35);
    this->btuTheme->setStyleSheet("border-radius:5px;");
    this->btuTheme->setIcon(QIcon(":/icon/icon/change.png"));
    this->btuTheme->setIconSize(QSize(100,35));
    //下拉按钮
    this->menu = new QMenu(this);
    /*this->menu->setStyleSheet("QMenu{"
                              "background-color:rgb(207,254,238);"
                              "width: 100px;"
                              "background-image:url(:/icon/icon/btuBk.png);"
                              "}");*/
    QAction* act1 = new QAction("Onmyoji",this);
    QAction* act2 = new QAction("Genshin",this);
    QAction* act3 = new QAction("None",this);
    QAction* act4 = new QAction("None",this);
    menu->addAction(act1);
    this->menu->addSeparator();
    menu->addAction(act2);
    this->menu->addSeparator();
    menu->addAction(act3);
    this->menu->addSeparator();
    menu->addAction(act4);
    this->btuTheme->setMenu(menu);
    this->menu->setVisible(false);
    //act1->setIcon(QIcon(":/icon/icon/addFile.png"));
    connect(this->btuTheme,&QPushButton::clicked,[=](){
        this->menu->setVisible(true);
    });
    connect(act1,&QAction::triggered,[=](){
        qDebug()<<"YYS"<<"\n";
        if(this->themeFlag.compare("theme1")==0)
        {
            return ;
        }
        else{
            this->themeFlag = "theme1";
        }
    });
    connect(act2,&QAction::triggered,[=](){
        qDebug()<<"YS"<<"\n";
        if(this->themeFlag.compare("theme2")==0)
        {
            return ;
        }
        else{
            this->themeFlag = "theme2";
        }
    });
    connect(act3,&QAction::triggered,[=](){
        qDebug()<<"YYS"<<"\n";
    });
    connect(act4,&QAction::triggered,[=](){
        qDebug()<<"YYS"<<"\n";
    });
}

//设置信号
void wMusic::connectButton()
{
    //关闭文件
    connect(this->btuClose,&QPushButton::clicked,[=](){
        this->hide();
    });
    //打开文件
    connect(this->btuOpen,&QPushButton::clicked,[=](){
        this->dataList.clear();
        QFileDialog* qfd = new QFileDialog(this);
        QStringList fileName;
        fileName = qfd->getOpenFileNames(this,"音乐文件","./MUSIC","*.mp3;;.*wav");
        if(fileName.size()==0)
        {
            return ;
        }
        this->file_Names = fileName[0].split("/MUSIC/")[0]+"/MUSIC/";


        //更新数据并保存
        this->updataFileterAddress();


        if(this->qvMusic == NULL)
        {
            //this->qvMusic = new QListWidget(this);
            this->setListWidget();
        }


        for(int i=0;i<fileName.size();i++)
        {
            this->qvMusic->addItem(fileName[i].split("/MUSIC/")[1]);
            QString tmpStr = fileName[i].split("/MUSIC/")[1];
            this->dataList.append(tmpStr);
            this->musicLists = new QMediaContent(QUrl::fromLocalFile(tmpStr));
        }
        this->updateFileterData();

    });

    //清空列表
    connect(this->btuClearList,&QPushButton::clicked,[=](){
        this->fileData = new QFile("./dataList.txt");

        if(!this->fileData->open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate))
        {
            return ;
        }
        else{
            if(this->fileData->size()==0)
            {
                qDebug()<<"YES\n";
            }
            this->fileData->close();
        }
        this->dataList.clear();
        this->qvMusic->clear();

    });

    //双击列表
    connect(this->qvMusic,&QListWidget::itemDoubleClicked,[=](){
        this->setMusicList(this->dataList,this->musicListRow);
        this->setMusic();
        this->music->play();
        this->mv->setPaused(false);
        this->updataTimer->start(5000);
    });
    //单击列表--更新行号
    connect(this->qvMusic,&QListWidget::itemClicked,[=](){

        this->musicListRow=this->qvMusic->row(this->qvMusic->currentItem());

    });
    //开始按钮信号
    connect(this->btuStart,&QPushButton::clicked,[=](){
        if(this->dataList.size()==0){
            return;
        }
        QString fileAdress = this->file_Names+this->qvMusic->currentItem()->text();
        switch( this->music->mediaStatus() )
        {
        case 0:{
            break;
        }
        case 1:{
            this->setMusicList(this->dataList,this->musicListRow);
            this->setMusic();
            this->music->play();
            break;
        }
        case 2:
        case 3:
        case 4:
        case 5:{
            this->setMusicList(this->dataList,this->musicListRow);
            this->setMusic();
            this->music->play();
            break;
        }
        case 6:{
            if(this->musicListRow!=this->musicNowRow)
            {
                this->setMusicList(this->dataList,this->musicListRow);
                this->setMusic();
                this->music->play();
            }

            this->music->play();
            break;
        }
        case 7:
        case 8:{
            this->setMusicList(this->dataList,this->musicListRow);
            this->setMusic();
            this->music->play();
            break;
        }
        }
         this->mv->setPaused(false);
         this->updataTimer->start(5000);
    });
    //关闭
    connect(this->btuCloseMusic,&QPushButton::clicked,[=](){
        if(this->music->mediaStatus() == 6)
        {
            this->music->stop();
        }
        this->mv->setPaused(true);
        this->updataTimer->stop();
    });
    //暂停
    connect(this->btuStopMusic,&QPushButton::clicked,[=](){
        if(this->music->mediaStatus() == 6)
        {
           this->music->pause();
        }
        this->mv->setPaused(true);
    });
    //音量加减
    connect(this->btuUp,&QPushButton::clicked,[=](){
        if(this->volume >= 100)
        {
            return ;
        }
        this->volume+= 5;
        this->music->setVolume(this->volume);
        //this->volume+= 5;
        this->progressBar->setValue(this->volume);
        this->progressBar->repaint();
    });

    connect(this->btuDown,&QPushButton::clicked,[=](){
        if(this->volume <= 0)
        {
            return ;
        }
        this->volume -= 5;
        this->music->setVolume(this->volume);
        //this->volume -= 5;
        this->progressBar->setValue(this->volume);
        this->progressBar->repaint();
    });



}
//设置音乐列表
void wMusic::setListWidget()
{
    QLabel* listTitle = new QLabel(this);
    listTitle->setGeometry(50,70,200,30);
    listTitle->setPixmap(QPixmap(":/icon/icon/listTitle.png"));
    //this->qvMusic->clear();
    this->qvMusic = new QListWidget(this);
    this->qvMusic->setGeometry(30,110,230,240);
    //设置列表为只读
    this->qvMusic->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->qvMusic->setStyleSheet("QListWidget{"
                                 "background-color:rgb(181,229,246);"
                                 "color:black;font-size:20px;"
                                 "border-style:solid;"
                                 "border-width:10px;"
                                 "border-color:white;"
                                 "font-family:'Courier New', Courier, monospace,'等线';"
                                 "border-radius:15px;"
                                 "}"
                                 "QListWidget::item{"
                                 "background-color:rgb(125,240,213);"
                                 "border-radius:10px;"
                                 "}"
                                 "QListWidget::item:pressed{"
                                 "color:green;"
                                 "}");
    this->qvMusic->repaint();

}

//设置音乐媒体
void wMusic::setMusic()
{

    //this->music->setMedia(QUrl::fromLocalFile(str));
    this->music->setVolume(30);
    this->musicPlayList->setPlaybackMode(QMediaPlaylist::Loop);
    this->music->setPlaylist(this->musicPlayList);
}

//设置待播放音乐列表
void wMusic::setMusicList(const QStringList &sList, int row)
{
    this->musicPlayList->clear();
    for(int i=row;i<sList.size();i++)
    {
        QString musicAddress = this->file_Names+sList.at(i);
        QMediaContent MyMusicList( QUrl::fromLocalFile(musicAddress));
        bool flag = this->musicPlayList->addMedia(MyMusicList);
        if(!flag)
        {
            qDebug()<<"添加失败"<<"\n";
            return ;
        }
    }
    this->musicNowRow = row;
}

void wMusic::updataFileterAddress()
{
    this->file = new QFile("./file_Address.txt");

    //重写更新文件内容
    if(!this->file->open(QIODevice::Truncate|QIODevice::WriteOnly))
    {
        qDebug()<<"文件file_Address.txt重写失败"<<"\n";
    }
    else{
        QString str1;// = this->file_Names;
        str1 = QStringLiteral().append(this->file_Names);
        this->file->write(str1.toLocal8Bit());
        this->file->close();
    }
}

void wMusic::updateFileterData()
{

    //写入歌单信息
    this->fileData = new QFile("./dataList.txt");
    if(!this->fileData->open(QIODevice::Truncate|QIODevice::WriteOnly))
    {
        qDebug()<<"文件dataList.txt重写失败"<<"\n";
    }
    else{

        for(int i=0;i<this->dataList.size();i++)
        {
            QString data =(this->dataList.at(i)+"\n");
            QString data_new;
            data_new = QStringLiteral("").append(data);
            this->fileData->write(data_new.toLocal8Bit());
        }
        this->fileData->close();
    }
}


