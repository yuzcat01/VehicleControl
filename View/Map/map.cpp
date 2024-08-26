#include "map.h"
#include "ui_map.h"

#include <QHostInfo>
#include <QDebug>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTimer>
#include <QPoint>
#include <QPainter>                // 绘图
#include <QPen>


Map::Map(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Map),
    canvas(new Canvas()),
    m_lng(116.185),
    m_lat(39.7385)
{
    ui->setupUi(this);


    QString qss;
    QFile qssFile(":/qss/prefix2/Resource/qss/myQss.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        ui->widget->setStyleSheet(qss);
        qssFile.close();
    }
    //注册事件过滤
    installEventFilter(this);

    //初始化
    init();
    ui->stackedWidget->addWidget(canvas);
    ui->stackedWidget->setCurrentIndex(0);

}



void Map::on_pushButton_clicked()
{
    emit toHome();
}

void Map::init(){
    //网络管理对象
    m_ipManager = new QNetworkAccessManager(this);
    //请求响应事件
    connect(m_ipManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onGetIp(QNetworkReply*)));
    //网络管理对象
    m_locManager =new QNetworkAccessManager(this);
    //请求响应事件
    connect(m_locManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onGetCurrentLoc(QNetworkReply*)));
    //网络管理对象
    m_searchManager=new QNetworkAccessManager(this);
    connect(m_searchManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onSearchLoc(QNetworkReply*)));
    //请求响应事件
    m_mapManager =new QNetworkAccessManager(this);
}

//获取ip地址
void Map::getIp(){
    QUrl url("http://httpbin.org/ip");
    QNetworkRequest request(url);
    m_ipManager->get(request);
}

//处理服务器的响应内容
void Map::onGetIp(QNetworkReply *reply){
    //将相应内容转换成jsondocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
    //转换成json对象
    QJsonObject jsonObj = jsonDoc.object();
    //从内容中取出ip地址
    currentIp=jsonObj.value("origin").toString();
    qDebug() << "Current IP:" << currentIp<<Qt::endl;

    //调用获取当前经纬度的函数
    getCurrentLoc();
}

//根据ip获取经纬度
void Map::getCurrentLoc(){
    QString host= "http://api.map.baidu.com/location/ip";
    QString query_str=QString("ip=%1&coor=bd09ll&ak=%2").arg(currentIp).arg(ak);
    QUrl url(host+"?"+query_str);
    QNetworkRequest request(url);
    m_locManager->get(request);
}
//处理服务器响应内容
void Map::onGetCurrentLoc(QNetworkReply *reply){
    //从point中获取经纬度，从content中获取城市
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject jsonContent=jsonObj.value("content").toObject();
    QJsonObject jsonPonit=jsonContent.value("point").toObject();

    m_city=jsonContent.value("address_detail").toObject().value("city").toString();
    qDebug() << jsonPonit.value("x").toString() <<Qt::endl;
    qDebug() << jsonPonit.value("y").toString() <<Qt::endl;
    sendMapRequest();
}
//发送请求，获取地图照片
void  Map::sendMapRequest(){
    //断调前一次请求
    if(m_mapReply!=NULL){
        m_mapReply->disconnect();
        //断掉事件连接
        disconnect(m_mapReply,&QIODevice::readyRead,this,&Map::onSendMapRequest);
    }
    //获得url
    QString host= "http://api.map.baidu.com/staticimage/v2";
    QString query_str=QString("ak=%1&width=1000&height=469&center=%2,%3&zoom=%4").arg(ak).arg(m_lng).arg(m_lat).arg(m_zoom);
    QUrl url(host+"?"+query_str);
    qDebug()<<host+"?"+query_str<<Qt::endl;
    QNetworkRequest request(url);
    m_mapReply= m_mapManager->get(request);
    connect(m_mapReply,&QIODevice::readyRead,this,&Map::onSendMapRequest);
}
//处理服务器返回地图图片
void  Map::onSendMapRequest(){
    //删除原有的地图图片
    system("del map.png");
    mapFile.setFileName(m_mapFileName);
    //通过只写模式打开文件并删除文件中的所有内容
    mapFile.open(QIODevice::WriteOnly| QIODevice::Truncate);
    m_timer.start(2);
    connect(&m_timer,&QTimer::timeout,[=](){
        m_timer.stop();
        mapFile.write(m_mapReply->readAll());
        mapFile.close();

        //更新ui设置新的图片
        QPixmap pixmap;
        if(pixmap.load(m_mapFileName)){
            ui->mapWidget->setStyleSheet("QWidget{border-image:url(./map.png);}");
        }

    });
}


Map::~Map()
{
    delete ui;
}

//搜索地址  发送请求
void Map::on_searchBtn_clicked(){
    //1、取文本框的值

}
//搜索地址  处理响应内容
void Map::onSearchLoc(QNetworkReply *reply){
    //    qDebug()<<reply->readAll()<<Qt::endl;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();
    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray addrArray= jsonObj.value("results").toArray();
    QJsonObject addrJson=addrArray.at(0).toObject();
    QJsonObject xyJson=addrJson.value("location").toObject();
    //m_lng=xyJson.value("lng").toDouble();
    //m_lat=xyJson.value("lat").toDouble();
    m_city=addrJson.value("city").toString();

    qDebug()<<m_lng<<Qt::endl;
    qDebug()<<m_lat<<Qt::endl;

    m_zoom=17;
    canvas->m_zoom = 17;
    //调取地图图片的函数
    sendMapRequest();
    canvas->loadOSMData("map.osm");
}

//放大按钮，点击后改变m_zoom [3-18]
void Map::on_enlargeBtn_clicked(){
    //判断m_zoom是否小于18，
    if(m_zoom<19){
        m_zoom+=1;
        canvas->m_zoom += 1;
        //调用函数重新获取地图图片
        sendMapRequest();
        canvas->loadOSMData("map.osm");
    }
}

//缩小按钮，点击后改变m_zoom [3-18]
void Map::on_reduceBtn_clicked(){
    if(m_zoom>3){
        m_zoom-=1;
        canvas->m_zoom -= 1;
        sendMapRequest();
        canvas->loadOSMData("map.osm");
    }
}

//过滤事件
bool  Map::eventFilter(QObject *watched, QEvent *event){
    //    qDebug()<<event->type()<<Qt::endl;
    if(event->type()==QEvent::MouseButtonPress){
        qDebug()<<event->type()<< cursor().pos().x()<<":"<<cursor().pos().y()<<Qt::endl;
        isPress=true;
        startPoint.setX(cursor().pos().x());
        startPoint.setY(cursor().pos().y());
    }

    if(event->type()==QEvent::MouseButtonRelease){
        qDebug()<<event->type()<< cursor().pos().x()<<":"<<cursor().pos().y()<<Qt::endl;
        isRelease=true;
        endPoint.setX(cursor().pos().x());
        endPoint.setY(cursor().pos().y());
    }

    if(isPress&&isRelease){
        isPress=false;
        isRelease=false;
        //计算距离差
        mx=startPoint.x()-endPoint.x();
        my=startPoint.y()-endPoint.y();

        if(qAbs(mx)>5||qAbs(my)>5){
            m_lng+=mx*0.000002*(19-m_zoom)*(19-m_zoom);
            m_lat-=my*0.000002*(19-m_zoom)*(19-m_zoom);
            canvas->m_x+=mx;
            canvas->m_y+=my;
            sendMapRequest();
            canvas->loadOSMData("map.osm");
        }
    }

    return QWidget::eventFilter(watched,event);
}

