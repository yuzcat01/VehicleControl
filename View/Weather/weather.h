#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>


#include "qlabel.h"
#include "weatherdata.h"

namespace Ui {
class Weather;
}

class Weather : public QWidget
{
    Q_OBJECT

public:
    explicit Weather(QWidget *parent = nullptr);
    ~Weather();
    void init();

signals:
    void toHome();

private slots:
    void onReplied(QNetworkReply* reply);

    void on_pushButton_clicked();

    void on_btnSearch_clicked();

protected:
    void getWeatherInfo(QString cityCode);
    void parseJson(QByteArray& byteArray);
    void updateUI();

    //重写父类的eventfilter方法
    bool eventFilter(QObject* watched, QEvent* event);

    //绘制高低温曲线的函数
    void paintHighCurve();
    void paintLowCurve();

private:
    Ui::Weather *ui;
    QString cityCode;
    QNetworkAccessManager* mNetAccessManager;

    Today mToday;
    Day mDay[6];

    //添加控件数组(星期，日期)
    QList<QLabel*> mWeekList;
    QList<QLabel*> mDateList;

    //添加控件数组(天气，天气图标)
    QList<QLabel*> mTypeList;
    QList<QLabel*> mTypeIconList;

    //添加控件数组(污染指数)
    QList<QLabel*> mAqiList;

    //添加控件数组(风力风向）
    QList<QLabel*> mFlList;
    QList<QLabel*> mFxList;

    QMap<QString, QString> mTypeMap;
};

#endif // WEATHER_H
