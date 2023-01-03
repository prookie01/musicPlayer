#ifndef WMUSIC_H
#define WMUSIC_H

#pragma once
#include <QWidget>
#include<QMouseEvent>
#include<QPushButton>
#include<QListWidget>
#include<QTableWidget>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QFile>
#include<QLabel>
#include<QTimer>
#include<QProgressBar>
#include<QMediaContent>
#include<QMenu>
namespace Ui {
class wMusic;
}

class wMusic : public QWidget
{
    Q_OBJECT

public:
    explicit wMusic(QWidget *parent = nullptr);
    ~wMusic();

private:
    Ui::wMusic *ui;
    //成员变量
    QPushButton* btuClose;
    QPushButton* btuOpen;
    QPushButton* btuClearList;
    QPushButton* btuStart;
    QPushButton* btuCloseMusic;
    QPushButton* btuStopMusic;
    QPushButton* btuDown;
    QPushButton* btuUp;
    QPushButton* btuTheme;
    QMenu* menu;

    QListWidget* qvMusic;
    QMediaPlayer* music;
    QMediaPlaylist* musicPlayList;
    QMediaContent* musicLists;
    int musicListRow;
    int musicNowRow;
    int imageFlag;//图片标签
    QString themeFlag;//主题标签
    int volume;//音量
    QProgressBar* progressBar;
    //两个文件指针用于操作数据
    QFile* file;
    QFile* fileData;
    //动图
    QLabel* movie;
    QMovie* mv;
    //主界面轮播图
    QLabel* centerLabel;
    QTimer* updataTimer;
    //int qvMusicRow;
    //文件--保存路径前缀
    QString file_Names;
    //文件--保存数据
    QStringList dataList;

    //成员函数
    void initFile();
    void initFrame();
    void initCenterLabel();
    void loadimageTimer();
    bool eventFilter(QObject* watched,QEvent* ev)override;
    void initButton();
    void initBtuMenu();
    void connectButton();
    void setListWidget();
    void setMusic();
    void setMusicList(const QStringList &sList,int row);
    void updateFileterData();
    void updataFileterAddress();
};

#endif // WMUSIC_H
