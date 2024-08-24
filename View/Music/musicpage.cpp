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
        isPlaying = false;
        ui->playBtn->setIcon(*mediaPlaybackStart);
    }
    else
    {
        isPlaying = true;
        ui->playBtn->setIcon(*mediaPlaybackPause);
    }
}


void MusicPage::on_nextBtn_clicked()
{

}


void MusicPage::on_musicBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MusicPage::on_musicListBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
