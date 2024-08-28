#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QNetworkAccessManager>
#include<QTimer>
#include<QFile>
#include <QXmlStreamReader>
#include <QMap>
#include <QList>
#include <QPoint>
#include "canvas.h"

namespace Ui {
class Map;
}

class Map : public QWidget
{
    Q_OBJECT

public:
    explicit Map(QWidget *parent = nullptr);
    ~Map();
    //获取当前主机ip

    //获取当前主机ip
    void getIp();

signals:
    void toHome();

private slots:
    void on_pushButton_clicked();
    //处理获取外网ip请求的 槽
    void onGetIp(QNetworkReply*);
    //根据ip获取经纬度  处理服务器响应内容
    void onGetCurrentLoc(QNetworkReply*);
    //处理服务器返回地图图片
    void onSendMapRequest();
    //搜索地址 发送请求
    void on_searchBtn_clicked();
    //搜索地址 处理响应内容
    void onSearchLoc(QNetworkReply*);

    void on_enlargeBtn_clicked();

    void on_reduceBtn_clicked();

private:
    Ui::Map *ui;
    QNetworkAccessManager *m_ipManager;
    QNetworkAccessManager *m_locManager;
    QNetworkAccessManager *m_searchManager;
    //网络管理对象  发送地图图片
    QNetworkAccessManager *m_mapManager;
    //响应对象 接收地图图片
    QNetworkReply *m_mapReply=NULL;

    Canvas *canvas;
    //初始化
    void init();
    //baidu App key
    QString ak="yYSnHDIgAbSRlyktehYXmvlzA0s3eEhX";
    //存储当前主机ip
    QString currentIp;
    //经度
    double m_lng;
    //纬度
    double m_lat;
    //控制好地图的精度  高清或低清[3-18]
    int m_zoom=15;
    //当前所在的城市
    QString m_city;
    //地图文件名
    QString m_mapFileName="map.png";
    QTimer m_timer;
    QFile mapFile;

    //是否按下鼠标按钮
    bool isPress=false;
    //是否释放鼠标按钮
    bool isRelease=false;
    //按下鼠标的坐标点
    QPoint startPoint;
    //释放鼠标的坐标点
    QPoint endPoint;
    //x轴距离差
    double mx;
    //y轴距离差
    double my;

    //根据ip获取经纬度  发送请求
    void getCurrentLoc();
    // 发送请求  获取地图图片
    void sendMapRequest();
    //过滤事件
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAP_H
