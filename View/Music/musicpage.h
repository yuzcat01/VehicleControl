#ifndef MUSICPAGE_H
#define MUSICPAGE_H

#include <QWidget>
#include <QtMultimedia>
#include <QListWidgetItem>

namespace Ui {
class MusicPage;
}

class MusicPage : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPage(QWidget *parent = nullptr);
    ~MusicPage();

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void toHome();

private slots:
    void on_backToHome_clicked();

    void on_playBtn_clicked();
    void on_nextBtn_clicked();
    void on_previousBtn_clicked();

    void on_musicBtn_clicked();
    void on_musicListBtn_clicked();

    void on_addBtn_clicked();
    void on_removeBtn_clicked();
    void on_clearBtn_clicked();

    void do_positionChanged(qint64 position);
    void do_durationChanged(qint64 duration);
    void do_playbackStateChanged(QMediaPlayer::PlaybackState newState);

    void on_volumeBtn_clicked();

    void on_volumeSlider_valueChanged(int value);

    void on_playModeBtn_clicked();

    void on_musicList_itemPressed(QListWidgetItem *item);

private:

    void init();
    void setPlaySource(QListWidgetItem *item);
    Ui::MusicPage *ui;

    QMediaPlayer *player;

    QIcon *mediaPlaybackStart;
    QIcon *mediaPlaybackPause;

    QIcon *audioVolumeHigh;
    QIcon *audioVolumeMedium;
    QIcon *audioVolumeLow;
    QIcon *audioVolumeMuted;

    QString positionTime;
    QString durationTime;

    bool isDragging;
    QPoint lastMousePosition;

    int playModeFlag;
    int isAutoless;
};

#endif // MUSICPAGE_H
