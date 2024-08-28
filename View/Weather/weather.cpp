#include "weather.h"
#include "ui_weather.h"

#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QUrl>

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonParseError>
#include<QJsonValue>
#include<QJsonArray>
#include<QByteArray>
#include<QMessageBox>
#include"weathertool.h"
#include<QPainter>

#define INCREMENT  3//温度每升高或降低一度，y轴坐标的增量
#define POINT_RADIUS 3 //曲线描点的大小
#define TEXT_OFFSET_X 12
#define TEXT_OFFSET_Y 12

Weather::Weather(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Weather)
    , cityCode("101010100")
    , isSearched(false)
{
    ui->setupUi(this);

    //将控件添加到控件数组(星期与日期)
    mWeekList << ui->lblWeek0_2 << ui->lblWeek1_2 << ui->lblWeek2_2 << ui->lblWeek3_2 << ui->lblWeek4_2 << ui->lblWeek5_2;
    mDateList << ui->lblDate0_2 << ui->lblDate1_2 << ui->lblDate2_2 << ui->lblDate3_2 << ui->lblDate4_2 << ui->lblDate5_2;

    //将控件添加到控件数组(天气与天气图标)
    mTypeList << ui->lblType0_2 << ui->lblType1_2 << ui->lblType2_2 << ui->lblType3_2 << ui->lblType4_2 << ui->lblType5_2;
    mTypeList << ui->lblTypeIcon0_2 << ui->lblTypeIcon1_2 << ui->lblTypeIcon2_2 << ui->lblTypeIcon3_2 << ui->lblTypeIcon4_2 << ui->lblTypeIcon5_2;

    //将控件添加到控件数组(污染指数)
    mAqiList << ui->lblQuality0_2 << ui->lblQuality1_2 << ui->lblQuality2_2 << ui->lblQuality3_2 << ui->lblQuality4_2 << ui->lblQuality5_2;

    mTypeMap.insert("晴", ":/img/prefix1/Resource/img/Weather/res/type/Qing.png");
    mTypeMap.insert("多云", ":/img/prefix1/Resource/img/Weather/res/type/DuoYun.png");
    mTypeMap.insert("阴", ":/img/prefix1/Resource/img/Weather/res/type/Yin.png");
    mTypeMap.insert("雨", ":/img/prefix1/Resource/img/Weather/res/type/Yu.png");
    mTypeMap.insert("雪", ":/img/prefix1/Resource/img/Weather/res/type/Xue.png");

    mTypeMap.insert("雷阵雨", ":/img/prefix1/Resource/img/Weather/res/type/LeiZhenYu.png");
    mTypeMap.insert("大雨", ":/img/prefix1/Resource/img/Weather/res/type/DaYu.png");
    mTypeMap.insert("小雨", ":/img/prefix1/Resource/img/Weather/res/type/XiaoYu.png");
    mTypeMap.insert("中雨", ":/img/prefix1/Resource/img/Weather/res/type/ZhongYu.png");
    mTypeMap.insert("阵雨", ":/img/prefix1/Resource/img/Weather/res/type/ZhenYu.png");
    mTypeMap.insert("暴雨", ":/img/prefix1/Resource/img/Weather/res/type/BaoYu.png");
    mTypeMap.insert("大暴雨", ":/img/prefix1/Resource/img/Weather/res/type/DaBaoYu.png");
    mTypeMap.insert("大到暴雨",":/img/prefix1/Resource/img/Weather/res/type/DaDaoBaoYu.png");
    mTypeMap.insert("暴雨到大暴雨",":/img/prefix1/Resource/img/Weather/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/img/prefix1/Resource/img/Weather/res/type/DaBaoYuDaoTeDaBaoYu.png");

    mTypeMap.insert("暴雪", ":/img/prefix1/Resource/img/Weather/res/type/BaoXue.png");
    mTypeMap.insert("大到暴雪",":/img/prefix1/Resource/img/Weather/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大雪", ":/img/prefix1/Resource/img/Weather/res/type/DaXue.png");
    mTypeMap.insert("小雪", ":/img/prefix1/Resource/img/Weather/res/type/XiaoXue.png");
    mTypeMap.insert("中雪", ":/img/prefix1/Resource/img/Weather/res/type/ZhongXue.png");

    mTypeMap.insert("雨夹雪", ":/img/prefix1/Resource/img/Weather/res/type/YuJiaXue.png");
    mTypeMap.insert("霾", ":/img/prefix1/Resource/img/Weather/res/type/Mai.png");
    mTypeMap.insert("扬沙", ":/img/prefix1/Resource/img/Weather/res/type/YangSha.png");
    mTypeMap.insert("沙尘暴", ":/img/prefix1/Resource/img/Weather/res/type/ShaChenBao.png");
    mTypeMap.insert("特大暴雨", ":/img/prefix1/Resource/img/Weather/res/type/TeDaBaoYu.png");
    mTypeMap.insert("乌", ":/img/prefix1/Resource/img/Weather/res/type/Wu.png");
    mTypeMap.insert("小到中雨", ":/img/prefix1/Resource/img/Weather/res/type/XiaoDaoZhongYu.png");
    mTypeMap.insert("小到中雪", ":/img/prefix1/Resource/img/Weather/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("雨夹雪", ":/img/prefix1/Resource/img/Weather/res/type/YuJiaXue.png");
    mTypeMap.insert("阵雪", ":/img/prefix1/Resource/img/Weather/res/type/ZhenXue.png");

    //将控件添加到控件数组(风向与风力)
    mFxList << ui->lblFx0_2 << ui->lblFx1_2 << ui->lblFx2_2 << ui->lblFx3_2 << ui->lblFx4_2 << ui->lblFx5_2;
    mFlList << ui->lblFl0_2 << ui->lblFl1_2 << ui->lblFl2_2 << ui->lblFl3_2 << ui->lblFl4_2 << ui->lblFl5_2;

    mNetAccessManager = new QNetworkAccessManager(this);
    connect(mNetAccessManager, &QNetworkAccessManager::finished, this,&Weather::onReplied);

    //给标签添加事件过滤器
    //参数指定为this，当前窗口对象
    ui->lblHighCurve_2->installEventFilter(this);
    ui->lblLowCurve_2->installEventFilter(this);
}

Weather::~Weather()
{
    delete ui;
}

void Weather::on_pushButton_clicked()
{
    emit toHome();
}

void Weather::getWeatherInfo(QString cityName)
{
    QString cityCode = WeatherTool::getCityCode(cityName);
    if(cityCode.isEmpty()) {
        QMessageBox::warning(this, "天气","请检查输入是否正确！",QMessageBox::Ok);
        return;
    }
    QUrl url("http://t.weather.itboy.net/api/weather/city/" + cityCode);
    mNetAccessManager->get(QNetworkRequest(url));
}

void Weather::parseJson(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error != QJsonParseError::NoError) {
        return;
    }

    QJsonObject rootobj = doc.object();
    qDebug() <<rootobj.value("message").toString();

    //1.解析日期和城市
    mToday.date = rootobj.value("date").toString();
    mToday.city = rootobj.value("cityInfo").toObject().value("city").toString();

    //2.解析yesterday的数据
    QJsonObject objData = rootobj.value("data").toObject();

    QJsonObject objYesterday = objData.value("yesterday").toObject();
    mDay[0].week = objYesterday.value("week").toString();
    mDay[0].date = objYesterday.value("ymd").toString();
    mDay[0].type = objYesterday.value("type").toString();

    //高低温
    QString s;
    s = objYesterday.value("high").toString().split(" ").at(1);
    QString s1 = s.left(s.length()-1);
    mDay[0].high = s1.toInt();

    s = objYesterday.value("low").toString().split(" ").at(1);
    s1 = s.left(s.length()-1);
    mDay[0].low = s1.toInt();

    //风向和风力
    mDay[0].fx = objYesterday.value("fx").toString();
    mDay[0].fl = objYesterday.value("fl").toString();

    //污染指数
    mDay[0].aqi = objYesterday.value("aqi").toDouble();

    //3.解析forcast中5天的数据(除了yesterday和今天)
    QJsonArray forecastArr = objData.value("forecast").toArray();
    for(int i=0; i < 5; i++) {
        QJsonObject objForecast = forecastArr[i].toObject();
        mDay[i+1].week = objForecast.value("week").toString();
        mDay[i+1].date = objForecast.value("ymd").toString();

        //天气类型
        mDay[i+1].type = objForecast.value("type").toString();

        //高温低温
        QString s;
        s = objForecast.value("high").toString().split(" ").at(1);
        s1 = s.left(s.length()-1);
        qDebug() << s1;
        mDay[i+1].high = s1.toInt();

        s = objForecast.value("low").toString().split(" ").at(1);
        s1 = s.left(s.length()-1);
        mDay[i+1].low = s1.toInt();

        //风向和风力
        mDay[i+1].fx = objForecast.value("fx").toString();
        mDay[i+1].fl = objForecast.value("fl").toString();

        //污染指数
        mDay[i+1].aqi = objForecast.value("aqi").toDouble();

        isSearched = true;
    }

    //4.解析今天的数据(还有一部分在forecast里面)
    mToday.ganmao = objData.value("ganmao").toString();
    QString s2 = objData.value("wendu").toString();
    mToday.wendu = s2.toDouble();
    mToday.shidu = objData.value("shidu").toString();
    mToday.pm25 = objData.value("pm25").toInt();
    mToday.quality = objData.value("quality").toString();

    //forecast中的今天数据
    mToday.type = mDay[1].type;
    mToday.fx = mDay[1].fx;
    mToday.fl = mDay[1].fl;
    mToday.high = mDay[1].high;
    mToday.low = mDay[1].low;

    //5.更新ui
    updateUI();

    //6.绘制温度曲线
    ui->lblHighCurve_2->update();
    ui->lblLowCurve_2->update();
}

void Weather::updateUI()
{
    //1.日期与城市
    ui->lblDate->setText(QDateTime::fromString(mToday.date, "yyyyMMdd").toString("yyyy/MM/dd") + " " + mDay[1].week);
    ui->lblCity_2->setText(mToday.city);

    //2.更新今天的数据
    ui->lblTypeIcon_2->setPixmap(mTypeMap[mToday.type]);
    ui->lblTemp_2->setText(QString::number(mToday.wendu) + "°C");
    ui->lblType_2->setText(mToday.type);
    ui->lblLowHigh_2->setText(QString::number(mToday.low) + "~" + QString::number(mToday.high) + "°C");
    ui->lblGanMao->setText("感冒指数：" + mToday.ganmao);
    ui->lblWindFx->setText(mToday.fx);
    ui->lblWindFl->setText(mToday.fl);
    ui->lblPM25->setText(QString::number(mToday.pm25));
    ui->lblShiDu->setText(mToday.shidu);
    ui->lblQuality->setText(mToday.quality);
    ui->lblTypeIcon0_2->setPixmap(mTypeMap[mDay[0].type]);
    ui->lblTypeIcon1_2->setPixmap(mTypeMap[mDay[1].type]);
    ui->lblTypeIcon2_2->setPixmap(mTypeMap[mDay[2].type]);
    ui->lblTypeIcon3_2->setPixmap(mTypeMap[mDay[3].type]);
    ui->lblTypeIcon4_2->setPixmap(mTypeMap[mDay[4].type]);
    ui->lblTypeIcon5_2->setPixmap(mTypeMap[mDay[5].type]);

    //3.更新六天的数据
    for(int i=0; i<6; i++) {
        //3.1更新日期
        mWeekList[i]->setText("周" + mDay[i].week.right(1));
        ui->lblWeek0_2->setText("昨天");
        ui->lblWeek1_2->setText("今天");
        ui->lblWeek2_2->setText("明天");

        QStringList ymdList = mDay[i].date.split("-");
        mDateList[i]->setText(ymdList[1] + "/" +ymdList[2]);

        //3.2更新天气类型
        mTypeList[i]->setText(mDay[i].type);

        //3.3更新空气质量（此时需要根据空气质量的指数进行一定的判断）
        if(mDay[i].aqi >= 0 && mDay[i].aqi <= 50) {
            mAqiList[i]->setText("优");
            mAqiList[i]->setStyleSheet("background-color: rgb(121,184,0);");
        } else if(mDay[i].aqi > 50 && mDay[i].aqi <= 100) {
            mAqiList[i]->setText("良");
            mAqiList[i]->setStyleSheet("background-color: rgb(255,187,23);");
        } else if(mDay[i].aqi > 100 && mDay[i].aqi <= 150) {
            mAqiList[i]->setText("轻度");
            mAqiList[i]->setStyleSheet("background-color: rgb(255,87,97);");
        } else if(mDay[i].aqi > 150 && mDay[i].aqi <= 200) {
            mAqiList[i]->setText("中度");
            mAqiList[i]->setStyleSheet("background-color: rgb(235,17,27);");
        } else if(mDay[i].aqi > 200 && mDay[i].aqi <= 250) {
            mAqiList[i]->setText("重度");
            mAqiList[i]->setStyleSheet("background-color: rgb(170,0,0);");
        } else {
            mAqiList[i]->setText("严重");
            mAqiList[i]->setStyleSheet("background-color: rgb(110,0,0);");
        }

        //3.4更新风力风向
        mFxList[i]->setText(mDay[i].fx);
        mFlList[i]->setText(mDay[i].fl);
    }
}

bool Weather::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->lblHighCurve_2 && event->type() == QEvent::Paint) {
        paintHighCurve();
    }

    if(watched == ui->lblLowCurve_2 && event->type() == QEvent::Paint) {
        paintLowCurve();
    }

    return QWidget::eventFilter(watched, event);
}

void Weather::paintHighCurve()
{
    QPainter painter(ui->lblHighCurve_2);

    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

    //1.获取x坐标
    int pointX[6] = {0};
    for(int i=0; i<6; i++) {
        pointX[i] = mWeekList[i]->pos().x() + mWeekList[i]->width()/2;
    }

    //2.获取y坐标
    //求最高温度的平均值
    int tempSum = 0;
    int tempAverage = 0;
    for(int i=0; i<6; i++) {
        tempSum +=mDay[i].high;
    }
    tempAverage = tempSum / 6;

    //计算y轴坐标
    int pointY[6] = {0};
    int yCenter = ui->lblHighCurve_2->height()/2;
    for(int i=0; i<6; i++) {
        pointY[i] = yCenter - ((mDay[i].high-tempAverage)*INCREMENT);
    }

    //3.开始绘制
    //3.1。初始化画笔相关的内容
    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(255,0,0));

    painter.setPen(pen);
    //画刷，内部填充
    painter.setBrush(QColor(255,0,0));

    //3.2.画点画文本
    for(int i=0; i<6; i++) {
        //显示温度对应点
        painter.drawEllipse(QPoint(pointX[i], pointY[i]), POINT_RADIUS, POINT_RADIUS);

        //显示温度文本
        painter.drawText(pointX[i]-TEXT_OFFSET_X, pointY[i]-TEXT_OFFSET_Y, QString::number(mDay[i].high) + "°C");

        //画线
        for(int i=0; i < 5; i++) {
            if(i == 0) {
                pen.setStyle(Qt::DotLine);//虚线
                painter.setPen(pen);
            }else{
                pen.setStyle(Qt::SolidLine);//实线
                painter.setPen(pen);
            }
            painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
        }
    }
}

void Weather::paintLowCurve()
{
    QPainter painter(ui->lblLowCurve_2);

    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

    //1.获取x坐标
    int pointX[6] = {0};
    for(int i=0; i<6; i++) {
        pointX[i] = mWeekList[i]->pos().x() + mWeekList[i]->width()/2;
    }

    //2.获取y坐标
    //求最高温度的平均值
    int tempSum = 0;
    int tempAverage = 0;
    for(int i=0; i<6; i++) {
        tempSum +=mDay[i].low;
    }
    tempAverage = tempSum / 6;

    //计算y轴坐标
    int pointY[6] = {0};
    int yCenter = ui->lblLowCurve_2->height()/2;
    for(int i=0; i<6; i++) {
        pointY[i] = yCenter - ((mDay[i].low-tempAverage)*INCREMENT);
    }

    //3.开始绘制
    //3.1。初始化画笔相关的内容
    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(0,255,255));

    painter.setPen(pen);
    //画刷，内部填充
    painter.setBrush(QColor(0,255,255));

    //3.2.画点画文本
    for(int i=0; i<6; i++) {
        //显示温度对应点
        painter.drawEllipse(QPoint(pointX[i], pointY[i]), POINT_RADIUS, POINT_RADIUS);

        //显示温度文本
        painter.drawText(pointX[i]-TEXT_OFFSET_X, pointY[i]-TEXT_OFFSET_Y, QString::number(mDay[i].low) + "°C");

        //画线
        for(int i=0; i < 5; i++) {
            if(i == 0) {
                pen.setStyle(Qt::DotLine);//虚线
                painter.setPen(pen);
            }else{
                pen.setStyle(Qt::SolidLine);//实线
                painter.setPen(pen);
            }
            painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
        }

    }
}

void Weather::onReplied(QNetworkReply *reply)
{
    qDebug() <<"onReplied success";

    //获取响应码
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error() != QNetworkReply::NoError || status_code != 200) {
        QMessageBox::warning(this, "天气", "请求数据失败", QMessageBox::Ok);
    }else{
        //获取响应信息
        QByteArray byteArray = reply->readAll();
        qDebug() << "" << byteArray.data();
        parseJson(byteArray);
    }

    reply->deleteLater();
}

void Weather::on_btnSearch_clicked()
{
    QString cityName = ui->leCity->text();
    getWeatherInfo(cityName);
}
