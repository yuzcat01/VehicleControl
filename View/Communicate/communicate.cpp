#include "communicate.h"
#include "ui_communicate.h"

#include <QTimer>

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QLineEdit>
#include <QCryptographicHash>

Communicate::Communicate(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Communicate)
    , webSocket (new QWebSocket)
    , networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->passwordInput->setEchoMode(QLineEdit::Password);
    ui->passwordInput_2->setEchoMode(QLineEdit::Password);
}

Communicate::~Communicate()
{
    delete ui;
    delete webSocket;
    delete networkManager;
}

void Communicate::on_toHome_clicked()
{
    emit toHome();
}

QString Communicate::getUsername() const
{
    return ui->usernameInput->text();
}

QString Communicate::getPassword() const
{
    return ui->passwordInput->text();
}

QString Communicate::getPassword_2() const
{
    return ui->passwordInput_2->text();
}

void Communicate::connectWebSocket()
{
    ui->stackedWidget->setCurrentIndex(1);
    disconnect(webSocket, &QWebSocket::textMessageReceived, this, &Communicate::onTextMessageReceived);
    connect(webSocket, &QWebSocket::textMessageReceived, this, &Communicate::onTextMessageReceived);

    webSocket->open(QUrl("ws://localhost:8080/chat/" + token));
}

void Communicate::logout()
{
    webSocket->close();  // Close WebSocket connection
    ui->stackedWidget->setCurrentIndex(0);
}

void Communicate::on_sendButton_clicked()
{
    // 获取输入框中的消息
    QString message = ui->messageInput->text();
    if(!message.isEmpty()) {
        // 显示消息
        appendMessage(message, "Me");
        // 清空输入框
        ui->messageInput->clear();
        webSocket->sendTextMessage(message);
    }
}

void Communicate::appendMessage(const QString &message, const QString &sender)
{
    QString formattedMessage = QString("<b>%1:</b> %2").arg(sender, message);
    ui->chatDisplay->append(formattedMessage);
}

void Communicate::onTextMessageReceived(QString message)
{
    QList<QString> messageSplits = message.split("：");
    message = messageSplits[1];
    for(int i = 2; i < messageSplits.size(); ++i)
        message += "：" + messageSplits[i];
    appendMessage(message, messageSplits[0]);
}

void Communicate::on_loginButton_clicked()
{
    QString username = getUsername();
    QString password = getPassword();

    QJsonObject loginData;
    loginData["username"] = username;
    loginData["password"] = SHA256Util(password);
    QNetworkRequest loginRequest(QUrl("http://localhost:8080/login"));
    loginRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* loginReply = networkManager->post(loginRequest, QJsonDocument(loginData).toJson());
    QEventLoop eventLoop;
    connect(loginReply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    if (loginReply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Error", "Login failed!");
        return;
    }

    QJsonObject loginResponse = QJsonDocument::fromJson(loginReply->readAll()).object();
    int loginCode = loginResponse["code"].toInt();
    if (loginCode != 1) {
        QMessageBox::critical(this, "Error", loginResponse["message"].toString());
        return;
    }

    token = loginResponse["data"].toString();
    connectWebSocket();  // Connect to WebSocket after successful login
    return;
}

void Communicate::on_registerButton_clicked()
{
    QString username = getUsername();
    QString password = getPassword();
    QString password_2 = getPassword_2();
    if(!(password == password_2)){
        QMessageBox::critical(this, "Error", "前后密码不一致!");
        return;
    }
    QJsonObject registrationData;
    registrationData["username"] = username;
    registrationData["password"] = SHA256Util(password);

    QNetworkRequest registerRequest(QUrl("http://localhost:8080/register"));
    registerRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* registerReply = networkManager->post(registerRequest, QJsonDocument(registrationData).toJson());
    QEventLoop eventLoop;
    connect(registerReply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    if (registerReply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Error", "Registration failed!");
        return;
    }

    QJsonObject registerResponse = QJsonDocument::fromJson(registerReply->readAll()).object();
    int loginCode = registerResponse["code"].toInt();
    if (loginCode != 1) {
        QMessageBox::critical(this, "Error", registerResponse["message"].toString());
        return;
    } else {
        QMessageBox::critical(this, "Success", "注册成功");
        ui->passwordInput_2->clear();
        ui->stackedWidget_2->setCurrentIndex(0);
    }
    return;
}

void Communicate::on_logoutButton_clicked()
{
    logout();
}

QString Communicate::SHA256Util(QString rawPassword){
    rawPassword += "vehiclecontrol";
    QByteArray password = QCryptographicHash::hash(rawPassword.toUtf8(),QCryptographicHash::Sha256).toHex();
    qDebug() << static_cast<QString>(password);
    return static_cast<QString>(password);
}

void Communicate::on_toRegister_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}


void Communicate::on_toLogin_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}



void Communicate::on_hideandshow_clicked()
{
    if(ui->passwordInput->echoMode()==QLineEdit::Password){
        ui->passwordInput->setEchoMode(QLineEdit::Normal);
        ui->hideandshow->setText("隐藏密码");
    }else{
        ui->passwordInput->setEchoMode(QLineEdit::Password);
        ui->hideandshow->setText("显示密码");
    }
}


void Communicate::on_hideandshow_2_clicked()
{
    if(ui->passwordInput_2->echoMode()==QLineEdit::Password){
        ui->passwordInput_2->setEchoMode(QLineEdit::Normal);
        ui->hideandshow_2->setText("隐藏密码");
    }else{
        ui->passwordInput_2->setEchoMode(QLineEdit::Password);
        ui->hideandshow_2->setText("显示密码");
    }
}

