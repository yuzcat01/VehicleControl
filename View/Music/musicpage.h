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

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

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

private:
    Ui::MusicPage *ui;

    QMediaPlayer *player;

    QIcon *mediaPlaybackStart;
    QIcon *mediaPlaybackPause;

    QString positionTime;
    QString durationTime;

    bool isDragging;
    QPoint lastMousePosition;
};

#endif // MUSICPAGE_H
