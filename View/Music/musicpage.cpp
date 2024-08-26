#include "musicpage.h"
#include "ui_musicpage.h"
#include<QFileDialog>

MusicPage::MusicPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicPage)
    , player(new QMediaPlayer)
    , mediaPlaybackStart(new QIcon(QIcon::fromTheme("media-playback-start")))
    , mediaPlaybackPause(new QIcon(QIcon::fromTheme("media-playback-pause")))
    , audioVolumeHigh(new QIcon(QIcon::fromTheme("audio-volume-high")))
    , audioVolumeMedium(new QIcon(QIcon::fromTheme("audio-volume-medium")))
    , audioVolumeLow(new QIcon(QIcon::fromTheme("audio-volume-low")))
    , audioVolumeMuted(new QIcon(QIcon::fromTheme("audio-volume-muted")))
    , playModeFlag(0)
    , isNextOrPrevious(false)
{
    ui->setupUi(this);
    player->setAudioOutput(new QAudioOutput(this));

    ui->musicListBtn->setIcon(QIcon(":/img/prefix1/Resource/img/Music/icons/icon-list.png"));
    ui->musicBtn->setIcon(QIcon(":/img/prefix1/Resource/img/Music/icons/icon-musical.png"));

    ui->musicSlider->installEventFilter(this);

    ui->volumeSlider->setSliderPosition(100);

    ui->playModeBtn->setIcon(QIcon(":/img/prefix1/Resource/img/Music/icons/icon-order.png"));

    connect(player, &QMediaPlayer::positionChanged, this, &MusicPage::do_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MusicPage::do_durationChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MusicPage::do_playbackStateChanged);

    init();    
}

MusicPage::~MusicPage()
{
    delete ui;
    delete player;
    delete mediaPlaybackStart;
    delete mediaPlaybackPause;
}

void MusicPage::init()
{
    QString musicFolderPath = QCoreApplication::applicationDirPath() + "/../../Resource/music";;
    QDir musicDir(musicFolderPath);

    QStringList nameFilters;
    nameFilters << "*.mp3" << "*.wav" << "*.wma";
    QFileInfoList fileList = musicDir.entryInfoList(nameFilters, QDir::Files);

    foreach (const QFileInfo &fileInfo, fileList) {
        QListWidgetItem *aItem = new QListWidgetItem(fileInfo.fileName());
        aItem->setData(Qt::UserRole, QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
        ui->musicList->addItem(aItem);
    }

    ui->stackedWidget->setCurrentIndex(0);
    ui->musicCover_1->setAlignment(Qt::AlignCenter);
    ui->musicCover_2->setAlignment(Qt::AlignCenter);
}

void MusicPage::setPlaySource(QListWidgetItem *item)
{
    player->setSource(item->data(Qt::UserRole).value<QUrl>());
    QString name = item->text().split(".")[0];
    QString coverPath = QCoreApplication::applicationDirPath() + "/../../Resource/img/Music/covers/" + name;
    QString jpgPath = coverPath + ".jpg";
    QString pngPath = coverPath + ".png";

    // 检查封面图片是否存在
    QFileInfo checkJpg(jpgPath);
    QFileInfo checkPng(pngPath);
    if (checkJpg.exists() && checkJpg.isFile())
    {
        QPixmap coverPixmap(jpgPath);
        ui->musicCover_1->setPixmap(coverPixmap.scaled(ui->musicCover_1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->musicCover_2->setPixmap(coverPixmap.scaled(ui->musicCover_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (checkPng.exists() && checkPng.isFile())
    {
        QPixmap coverPixmap(pngPath);
        ui->musicCover_1->setPixmap(coverPixmap.scaled(ui->musicCover_1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->musicCover_2->setPixmap(coverPixmap.scaled(ui->musicCover_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
    {
        ui->musicCover_1->clear();
        ui->musicCover_2->clear();
    }
}

void MusicPage::on_backToHome_clicked()
{
    emit toHome();
}

void MusicPage::on_playBtn_clicked()
{
    if(player->playbackState() == QMediaPlayer::PlayingState)
    {
        player->pause();
        ui->playBtn->setIcon(*mediaPlaybackStart);
    }
    else
    {
        if(ui->musicList->count() <= 0)
            return;
        if(ui->musicList->currentRow() < 0)
            ui->musicList->setCurrentRow(0);
        ui->playBtn->setIcon(*mediaPlaybackPause);
        if(player->playbackState() == QMediaPlayer::PausedState)
        {
            player->play();
            return;
        }
        setPlaySource(ui->musicList->currentItem());
        player->play();
    }
}

void MusicPage::on_previousBtn_clicked()
{
    isNextOrPrevious = true;
    int curRow = ui->musicList->currentRow();
    --curRow;
    curRow = curRow < 0 ? ui->musicList->count() -1 : curRow;
    if(ui->musicList->count() > 0)
    {
        ui->musicList->setCurrentRow(curRow);
        setPlaySource(ui->musicList->currentItem());
        player->play();
    }
}

void MusicPage::on_nextBtn_clicked()
{
    isNextOrPrevious = true;
    int curRow = ui->musicList->currentRow();
    ++curRow;
    curRow = curRow > ui->musicList->count() -1 ? 0 : curRow;
    if(ui->musicList->count() > 0)
    {
        ui->musicList->setCurrentRow(curRow);
        setPlaySource(ui->musicList->currentItem());
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
        aItem->setData(Qt::UserRole, QUrl::fromLocalFile(item));
        ui->musicList->addItem(aItem);
    }
}

void MusicPage::on_removeBtn_clicked()
{
    int index = ui->musicList->currentRow();
    if(index < 0)
        return;
    delete ui->musicList->takeItem(index);
    if(ui->musicList->currentRow() >= 0)
    {
        setPlaySource(ui->musicList->currentItem());
        player->play();
        ui->playBtn->setIcon(*mediaPlaybackPause);
    }
    else
    {
        player->stop();
        ui->playBtn->setIcon(*mediaPlaybackStart);
    }
}

void MusicPage::on_clearBtn_clicked()
{
    ui->musicList->clear();
    player->stop();
    ui->playBtn->setIcon(*mediaPlaybackStart);
}

void MusicPage::do_positionChanged(qint64 position)
{
    if(ui->musicSlider->isSliderDown())
        return;
    ui->musicSlider->setSliderPosition(position);
    int secs = position/1000;
    int mins = secs/60;
    secs %= 60;
    positionTime = QString::asprintf("%d:%02d", mins, secs);
    ui->nowTime->setText(positionTime);
    ui->allTime->setText(durationTime);
}

void MusicPage::do_durationChanged(qint64 duration)
{
    ui->musicSlider->setMaximum(duration);
    int secs = duration / 1000;
    int mins = secs / 60;
    secs %= 60;
    durationTime = QString::asprintf("%d:%02d", mins, secs);
    ui->nowTime->setText(positionTime);
    ui->allTime->setText(durationTime);
}

bool MusicPage::eventFilter(QObject *watched, QEvent *event)
{
    // 处理滑块点击事件
    if (event->type() == QEvent::MouseButtonPress && watched == ui->musicSlider) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        // 计算鼠标点击位置对应的值
        if (mouseEvent->button() == Qt::LeftButton)	//判断左键
        {
            int clickedValue = ui->musicSlider->minimum() + ((ui->musicSlider->maximum() - ui->musicSlider->minimum()) * mouseEvent->pos().x() / ui->musicSlider->width());
            ui->musicSlider->setValue(clickedValue);
            player->setPosition(clickedValue); // 设置播放器位置
            return true;
        }
    } else if (event->type() == QEvent::MouseMove && watched == ui->musicSlider) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->buttons() & Qt::LeftButton) {
            int draggedValue = ui->musicSlider->minimum() + ((ui->musicSlider->maximum() - ui->musicSlider->minimum()) * mouseEvent->pos().x() / ui->musicSlider->width());
            ui->musicSlider->setValue(draggedValue); // 更新滑块的值
            player->setPosition(draggedValue); // 设置播放器位置
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void MusicPage::on_volumeBtn_clicked()
{
    bool mute = player->audioOutput()->isMuted();
    player->audioOutput()->setMuted(!mute);
    if(mute)
    {
        float volume = player->audioOutput()->volume();
        if (volume > 0.66)
            ui->volumeBtn->setIcon(*audioVolumeHigh);
        else if (volume > 0.33)
            ui->volumeBtn->setIcon(*audioVolumeMedium);
        else if (volume > 0)
            ui->volumeBtn->setIcon(*audioVolumeLow);
        else
            ui->volumeBtn->setIcon(*audioVolumeMuted);
    }
    else
        ui->volumeBtn->setIcon(*audioVolumeMuted);
}

void MusicPage::on_volumeSlider_valueChanged(int value)
{
    player->audioOutput()->setVolume(value/100.0);

    float volume = player->audioOutput()->volume();
    if (volume > 0.66)
        ui->volumeBtn->setIcon(*audioVolumeHigh);
    else if (volume > 0.33)
        ui->volumeBtn->setIcon(*audioVolumeMedium);
    else if (volume > 0)
        ui->volumeBtn->setIcon(*audioVolumeLow);
    else
        ui->volumeBtn->setIcon(*audioVolumeMuted);
}

void MusicPage::on_playModeBtn_clicked()
{
    ++playModeFlag;
    if(playModeFlag > 2)
        playModeFlag = 0;
    if (playModeFlag == 0)
        ui->playModeBtn->setIcon(QIcon(":/img/prefix1/Resource/img/Music/icons/icon-order.png"));
    else if(playModeFlag == 1)
        ui->playModeBtn->setIcon(QIcon(":/img/prefix1/Resource/img/Music/icons/icon-repeat.png"));
    else
        ui->playModeBtn->setIcon(QIcon(":/img/prefix1/Resource/img/Music/icons/icon-random.png"));
}

void MusicPage::do_playbackStateChanged(QMediaPlayer::PlaybackState newState)
{
    if(isNextOrPrevious)
    {
        isNextOrPrevious = false;
        return;
    }
    if((newState == QMediaPlayer::StoppedState) && (ui->musicList->count() > 0))
    {
        if(playModeFlag == 0)
        {
            int count = ui->musicList->count();
            int curRow = ui->musicList->currentRow();
            ++curRow;
            curRow = curRow >= count ? 0 : curRow;
            ui->musicList->setCurrentRow(curRow);
            setPlaySource(ui->musicList->currentItem());
            player->play();
        }
        else if(playModeFlag == 1)
        {
            setPlaySource(ui->musicList->currentItem());
            player->play();
        }
        else
        {
            qint32 random = QRandomGenerator::global()->bounded(0, ui->musicList->count());
            ui->musicList->setCurrentRow(random);
            setPlaySource(ui->musicList->currentItem());
            player->play();
        }
    }
}
