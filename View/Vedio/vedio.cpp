#include "vedio.h"
#include "./ui_vedio.h"
#include <QMessageBox>
#include <QByteArray>

Vedio::Vedio(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Vedio)
{
    ui->setupUi(this);
    player = new QMediaPlayer;
    //player->setSource(QUrl("qrc:/new/prefix1/SnapAny.mp4"));
    audioOutput = new QAudioOutput;
    //player->setSource(QUrl("http://baobab.kaiyanapp.com/api/v1/playUrl?vid=325222&resourceType=video&editionType=default&source=aliyun&playUrlType=url_oss&udid="));

    player->setVideoOutput(ui->videoWidget);

    // QWidget *centralWidget = new QWidget(this); // 'this' is your QMainWindow or QDialog
    //QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    // layout->addWidget(videoWidget);

    // Set the central widget if using QMainWindow
    // setCentralWidget(centralWidget);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(1.0);

    ui->vSlider->installEventFilter(this);

    ui->volumeSlider->setSliderPosition(100);

    ui->speedComboBox->addItem("0.5x", QVariant(0.5));
    ui->speedComboBox->addItem("1x", QVariant(1.0));  // Default speed
    ui->speedComboBox->addItem("1.5x", QVariant(1.5));
    ui->speedComboBox->addItem("2x", QVariant(2.0));
    ui->speedComboBox->setCurrentIndex(1);  // Set default to 1x speed

    //连接每日推荐视频api
    mNetAccessManager = new QNetworkAccessManager(this);
    connect(mNetAccessManager,&QNetworkAccessManager::finished,this,&Vedio::onReplied);

    getVideoInfo("1");

    // 连接双击信号到槽函数
    connect(ui->listWidget, &QListWidget::itemDoubleClicked,this,&Vedio:: onItemDoubleClicked);

    ui->listWidget->show();


    connect(player, &QMediaPlayer::positionChanged, this, &Vedio::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &Vedio::on_durationChanged);
    connect(ui->vSlider, &QSlider::sliderMoved, this, &Vedio::on_sliderMoved);
    // Connect the slider to control the volume
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &Vedio::on_volumeSlider_valueChanged);
    connect(ui->PlayPauseButton, &QPushButton::clicked, this, &Vedio::on_togglePlayPauseButton_clicked);
    connect(ui->speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Vedio::on_speedComboBox_currentIndexChanged);
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
        // Handle the case where no file was selected
        qWarning("No video file selected!");
    }

    player->setVideoOutput(ui->videoWidget);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(1.0);

    player->play();
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
    // Convert the slider value (0-100) to a volume percentage (0.0 to 1.0)
    qreal volume = static_cast<qreal>(value) / 100.0;
    audioOutput->setVolume(volume);

    // Update the QLabel to show the volume percentage
    ui->volumeLabel->setText(QString::asprintf("%d%%", value));
}

void Vedio::on_togglePlayPauseButton_clicked()
{
    if (player->playbackState() == QMediaPlayer::PlayingState) {
        player->pause();
        ui->PlayPauseButton->setText("Play");
    } else {
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

void Vedio::onReplied(QNetworkReply *reply)
{
    qDebug() << "onReplied success";

    if(reply->error() != QNetworkReply::NoError){
        qDebug() << reply->errorString().toLatin1().data();
        QMessageBox::warning(this,"推荐视频","请求数据失败",QMessageBox::Ok);
    }else{
        QByteArray byteArray = reply->readAll();
        //qDebug() << "" <<byteArray.data();
        parseJson(byteArray);
    }
}

void Vedio::getVideoInfo(QString page)
{
    QUrl url("http://baobab.kaiyanapp.com/api/v4/discovery/hot?start=1&num=10");
    mNetAccessManager->get(QNetworkRequest(url));
}

void Vedio::parseJson(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray,&err);
    if(err.error != QJsonParseError::NoError){
        return;
    }
    QJsonObject rootObj = doc.object();
    QJsonArray videoListObj = rootObj.value("itemList").toArray();
    for(int i=0;i<10;i++){
        QJsonObject videoObj = videoListObj[i].toObject();
        QJsonObject videoDataObj = videoObj.value("data").toObject();
        QString title = videoDataObj.value("title").toString();
        QString url_video = videoDataObj.value("playUrl").toString();
        //qDebug() << title;
        //qDebug() << url_video;
        CustomListWidgetItem* item = new CustomListWidgetItem(title, QUrl(url_video));
        ui->listWidget->addItem(item);
    }
}

void Vedio::onItemDoubleClicked(QListWidgetItem *item)
{
    CustomListWidgetItem* customItem = dynamic_cast<CustomListWidgetItem*>(item);
    if (customItem) {
        QUrl url = customItem->getUrl();
        // 使用 URL播放视频
        //qDebug() << "Double-clicked item URL:" << url.toString();
        player->setSource(url);
        player->setVideoOutput(ui->videoWidget);
        player->setAudioOutput(audioOutput);
        audioOutput->setVolume(1.0);

        player->play();
    }
}
