#ifndef VEDIO_H
#define VEDIO_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QAudioOutput>
#include <QFile>
#include <QFileDialog>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonParseError>
#include<QJsonValue>

#include <QListWidgetItem>
#include <QUrl>

namespace Ui {
class Vedio;
}


class Vedio : public QWidget
{
    Q_OBJECT

public:
    Vedio(QWidget *parent = nullptr);
    ~Vedio();

signals:
    void toHome();

private slots:

    void on_pushButton_clicked();
    void on_positionChanged(qint64 position);
    void on_durationChanged(qint64 duration);
    void on_sliderMoved(int position);
    void on_volumeSlider_valueChanged(int value);
    void on_togglePlayPauseButton_clicked();
    void on_speedComboBox_currentIndexChanged(int index);
    void on_playbackStateChanged();

    void on_toHome_clicked();

    void onReplied(QNetworkReply *reply);
    void getVideoInfo(QString page);
    void parseJson(QByteArray& byteArray);

    void onItemDoubleClicked(QListWidgetItem *item);

private:
    Ui::Vedio *ui;

    QMediaPlayer * player;
    QVideoWidget * videoWidget;
    QAudioOutput *audioOutput;
    QString fileName;

    QNetworkAccessManager *mNetAccessManager;
};


class CustomListWidgetItem : public QListWidgetItem {
public:
    CustomListWidgetItem(const QString& text, const QUrl& url, QListWidget* parent = nullptr)
        : QListWidgetItem(text, parent), url(url) {}

    void setUrl(const QUrl& newUrl) { url = newUrl; }
    QUrl getUrl() const { return url; }

private:
    QUrl url;
};
#endif
