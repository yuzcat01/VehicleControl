#include "musicpage.h"
#include "ui_musicpage.h"
#include<QFileDialog>

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
    if(ui->musicList->count() > 0)
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
    if(ui->musicList->count() > 0)
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

void MusicPage::on_addBtn_clicked()
{
    QString curPath = QDir::homePath();
    QString dlgTitle = "选择音频文件";
    QString filter = "音频文件(*.mp3 *.wav *.wma)";

    QStringList fileList = QFileDialog::getOpenFileNames(this, dlgTitle, curPath, filter);
    if(fileList.isEmpty())
        return;

    foreach (const auto& item, fileList) {
        QFileInfo fileInfo(item);
        QListWidgetItem *aItem = new QListWidgetItem(fileInfo.fileName());
        /* To do
        aItem->setIcon(QIcon(":/images/images/musicFile.png"));
        */
        aItem->setData(Qt::UserRole, QUrl::fromLocalFile(item));
        ui->musicList->addItem(aItem);
    }

    //如果现在没有正在播放，就开始播放第一个文件
    if(player->playbackState() != QMediaPlayer::PlayingState){
        ui->musicList->setCurrentRow(0);
        QUrl source = ui->musicList->currentItem()->data(Qt::UserRole).value<QUrl>();
        player->setSource(source);
        player->play();
    }
}

void MusicPage::on_removeBtn_clicked()
{
    int index = ui->musicList->currentRow();
    if(index < 0)
        return;
    delete ui->musicList->takeItem(index);
}

void MusicPage::on_clearBtn_clicked()
{
    ui->musicList->clear();
    player->stop();
}
