#ifndef MUSICPAGE_H
#define MUSICPAGE_H

#include <QWidget>
#include <QtMultimedia>

namespace Ui {
class MusicPage;
}

class MusicPage : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPage(QWidget *parent = nullptr);
    ~MusicPage();

signals:
    void toHome();

private slots:
    void on_backToHome_clicked();

    void on_playBtn_clicked();

    void on_nextBtn_clicked();

    void on_musicBtn_clicked();

    void on_musicListBtn_clicked();

    void on_previousBtn_clicked();

    void on_addBtn_clicked();

    void on_removeBtn_clicked();

    void on_clearBtn_clicked();

private:
    Ui::MusicPage *ui;
    QMediaPlayer *player;
    bool isPlaying;

    QIcon *mediaPlaybackStart;
    QIcon *mediaPlaybackPause;
};

#endif // MUSICPAGE_H
