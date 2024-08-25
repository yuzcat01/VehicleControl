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
    player->setAudioOutput(new QAudioOutput(this));

    ui->musicSlider->installEventFilter(this);

    connect(player, &QMediaPlayer::positionChanged, this, &MusicPage::do_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MusicPage::do_durationChanged);
}

MusicPage::~MusicPage()
{
    delete ui;
    delete player;
    delete mediaPlaybackStart;
    delete mediaPlaybackPause;
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
}

void MusicPage::on_removeBtn_clicked()
{
    int index = ui->musicList->currentRow();
    if(index < 0)
        return;
    delete ui->musicList->takeItem(index);
    if(ui->musicList->currentRow() >= 0)
    {
        player->setSource(ui->musicList->currentItem()->data(Qt::UserRole).value<QUrl>());
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
            qDebug() << "触发了点击事件，位置为:" << clickedValue << Qt::endl;
            ui->musicSlider->setValue(clickedValue);
            player->setPosition(clickedValue); // 设置播放器位置
            return true;
        }
    } else if (event->type() == QEvent::MouseMove && watched == ui->musicSlider) {
        qDebug() << "触发了移动事件" << Qt::endl;
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
