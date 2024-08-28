#include "vedio.h"
#include "./ui_vedio.h"

Vedio::Vedio(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Vedio)
{
    ui->setupUi(this);
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;

    player->setVideoOutput(ui->videoWidget);

    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);

    ui->vSlider->installEventFilter(this);

    ui->volumeSlider->setSliderPosition(50);

    ui->speedComboBox->addItem("0.5x", QVariant(0.5));
    ui->speedComboBox->addItem("1x", QVariant(1.0));  // Default speed
    ui->speedComboBox->addItem("1.5x", QVariant(1.5));
    ui->speedComboBox->addItem("2x", QVariant(2.0));
    ui->speedComboBox->setCurrentIndex(1);  // Set default to 1x speed


    connect(player, &QMediaPlayer::positionChanged, this, &Vedio::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &Vedio::on_durationChanged);
    connect(ui->vSlider, &QSlider::sliderMoved, this, &Vedio::on_sliderMoved);
    // Connect the slider to control the volume
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &Vedio::on_volumeSlider_valueChanged);
    connect(ui->PlayPauseButton, &QPushButton::clicked, this, &Vedio::on_togglePlayPauseButton_clicked);
    connect(ui->speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Vedio::on_speedComboBox_currentIndexChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &Vedio::on_playbackStateChanged);
}

Vedio::~Vedio()
{
    delete player;
    delete audioOutput;
    delete ui;
}

void Vedio::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open Video"), "C:\\", tr("Video Files (*.mp4 *.avi *.mkv)"));
    if (!fileName.isEmpty()) {
        player->setSource(QUrl::fromLocalFile(fileName));
    } else {
        qWarning("未选择视频！");
        return;
    }

    player->setVideoOutput(ui->videoWidget);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(1.0);

    player->play();
    ui->PlayPauseButton->setText("Pause");
}

void Vedio::on_positionChanged(qint64 position)
{
    if (!ui->vSlider->isSliderDown()) {
        ui->vSlider->setValue(static_cast<int>(position));
    }
    int secs = position / 1000;
    int mins = secs / 60;
    secs %= 60;
    QString currentTime = QString::asprintf("%d:%02d", mins, secs);
    ui->nowTime->setText(currentTime);
}

void Vedio::on_sliderMoved(int position)
{
    player->setPosition(static_cast<qint64>(position));
}

void Vedio::on_durationChanged(qint64 duration)
{
    ui->vSlider->setRange(0, static_cast<int>(duration));
    int secs = duration / 1000;
    int mins = secs / 60;
    secs %= 60;
    QString totalTime = QString::asprintf("%d:%02d", mins, secs);
    ui->allTime->setText(totalTime);
}

void Vedio::on_volumeSlider_valueChanged(int value)
{
    qreal volume = static_cast<qreal>(value) / 100.0;
    audioOutput->setVolume(volume);

    ui->volumeLabel->setText(QString::asprintf("%d%%", value));
}

void Vedio::on_togglePlayPauseButton_clicked()
{
    if (player->playbackState() == QMediaPlayer::PlayingState) {
        player->pause();
        ui->PlayPauseButton->setText("Play");
    } else if(player->hasVideo() == true){
        player->play();
        ui->PlayPauseButton->setText("Pause");
    }
}

void Vedio::on_speedComboBox_currentIndexChanged(int index)
{
    qreal speed = ui->speedComboBox->currentData().toReal();
    player->setPlaybackRate(speed);
}

void Vedio::on_toHome_clicked()
{
    emit toHome();
    if (player->playbackState() == QMediaPlayer::PlayingState) {
        player->pause();
        ui->PlayPauseButton->setText("Play");
    }
}

void Vedio::on_playbackStateChanged()
{
    if(player->playbackState() == QMediaPlayer::StoppedState)
        ui->PlayPauseButton->setText("Play");
}
