#include "musicpage.h"
#include "ui_musicpage.h"

MusicPage::MusicPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicPage)
    , player(new QMediaPlayer)
    , mediaPlaybackStart(new QIcon(QIcon::fromTheme("media-playback-start")))
    , mediaPlaybackPause(new QIcon(QIcon::fromTheme("media-playback-pause")))
{
    ui->setupUi(this);
    isPlaying = false;
}

MusicPage::~MusicPage()
{
    delete ui;
    delete player;
}
void MusicPage::on_backToHome_clicked()
{
    emit toHome();
}


void MusicPage::on_playBtn_clicked()
{
    if(isPlaying)
    {
        player->pause();
        isPlaying = false;
        ui->playBtn->setIcon(*mediaPlaybackStart);
    }
    else
    {
        if(ui->musicList->count() <= 0)
            return;
        isPlaying = true;
        ui->playBtn->setIcon(*mediaPlaybackPause);
        if(player->playbackState() == QMediaPlayer::PausedState)
        {
            player->play();
            return;
        }
        player->setSource(ui->musicList->currentItem()->data(Qt::UserRole).value<QUrl>());
        player->play();
    }
}

void MusicPage::on_previousBtn_clicked()
{
    int curRow = ui->musicList->currentRow();
    --curRow;
    curRow = curRow < 0 ? ui->musicList->count() -1 : curRow;
    if(curRow >= 0)
    {
        ui->musicList->setCurrentRow(curRow);
        player->setSource(ui->musicList->currentItem()->data(Qt::UserRole).value<QUrl>());
        isPlaying = true;
        player->play();
    }
}


void MusicPage::on_nextBtn_clicked()
{
    int curRow = ui->musicList->currentRow();
    ++curRow;
    curRow = curRow > ui->musicList->count() -1 ? 0 : curRow;
    if(curRow >= 0)
    {
        ui->musicList->setCurrentRow(curRow);
        player->setSource(ui->musicList->currentItem()->data(Qt::UserRole).value<QUrl>());
        isPlaying = true;
        player->play();
    }
}

void MusicPage::on_musicBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MusicPage::on_musicListBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
