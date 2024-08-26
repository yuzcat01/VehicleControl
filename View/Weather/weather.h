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

    //��д�����eventfilter����
    bool eventFilter(QObject* watched, QEvent* event);

    //���Ƹߵ������ߵĺ���
    void paintHighCurve();
    void paintLowCurve();

private:
    Ui::Weather *ui;
    QString cityCode;
    QNetworkAccessManager* mNetAccessManager;

    Today mToday;
    Day mDay[6];

    //��ӿؼ�����(���ڣ�����)
    QList<QLabel*> mWeekList;
    QList<QLabel*> mDateList;

    //��ӿؼ�����(����������ͼ��)
    QList<QLabel*> mTypeList;
    QList<QLabel*> mTypeIconList;

    //��ӿؼ�����(��Ⱦָ��)
    QList<QLabel*> mAqiList;

    //��ӿؼ�����(��������
    QList<QLabel*> mFlList;
    QList<QLabel*> mFxList;

    QMap<QString, QString> mTypeMap;
};

#endif // WEATHER_H
