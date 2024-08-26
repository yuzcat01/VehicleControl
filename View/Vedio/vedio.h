#ifndef VEDIO_H
#define VEDIO_H

#include <QWidget>

#include <QMainWindow>
#include <QPushButton>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QAudioOutput>
#include <QThread>
#include <QKeyEvent>
#include <QEvent>
#include <stdlib.h>

namespace Ui {
class Vedio;
}

class Vedio : public QWidget
{
    Q_OBJECT

public:
    explicit Vedio(QWidget *parent = nullptr);
    ~Vedio();
    QMessageBox*    msg;
    QMediaPlayer*   player;
    QAudioOutput*   audioOut;
    QString         selectVideo;

    QString format_time(int ms);
    QString get_process_text(QString inTotalDuration, QString inCurrent = "0:0:0");
    int     get_process_percent(qint64 inValue, qint64 inTotal);
    void setPlayerStatus(bool playStatus);
    void init_player();

signals:
    void toHome();

private slots:
    void on_toHome_clicked();

private:
    Ui::Vedio *ui;
    bool eventFilter(QObject *obj, QEvent *e) override;
};

class ProcessBarThread : public QThread
{
    Q_OBJECT

public:

protected:
    void run() override;
signals:
    void current_play_duration();
public slots:

};

#endif // VEDIO_H
