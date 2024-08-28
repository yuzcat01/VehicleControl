#ifndef VEDIO_H
#define VEDIO_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QAudioOutput>
#include <QFile>
#include <QFileDialog>

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

private:
    Ui::Vedio *ui;

    QMediaPlayer * player;
    QVideoWidget * videoWidget;
    QAudioOutput *audioOutput;
    QString fileName;
};
#endif
