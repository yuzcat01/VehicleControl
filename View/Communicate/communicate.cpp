#include "communicate.h"
#include "ui_communicate.h"

#include <QTimer>

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

Communicate::Communicate(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Communicate)
    , webSocket (new QWebSocket)
    , networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
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
    loginData["password"] = password;

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

    QJsonObject registrationData;
    registrationData["username"] = username;
    registrationData["password"] = password;

    QNetworkRequest registerRequest(QUrl("http://localhost:8080/register"));
    registerRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* registerReply = networkManager->post(registerRequest, QJsonDocument(registrationData).toJson());
    QEventLoop eventLoop;
    connect(registerReply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    if (registerReply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Error", "Registration failed!");
    } else {
        QMessageBox::information(this, "Success", "Registration successful!");
    }
}

void Communicate::on_logoutButton_clicked()
{
    logout();
}
