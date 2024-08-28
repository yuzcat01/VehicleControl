#include "communicate.h"
#include "ui_communicate.h"
#include <QTimer>

Communicate::Communicate(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Communicate)
    ,webSocket(new QWebSocket)
{
    ui->setupUi(this);

    // 连接WebSocket服务器
    connect(webSocket, &QWebSocket::connected, this, &Communicate::onConnected);
    connect(webSocket, &QWebSocket::disconnected, this, &Communicate::onDisconnected);
    connect(webSocket, &QWebSocket::textMessageReceived, this, &Communicate::onTextMessageReceived);

    webSocket->open(QUrl("ws://localhost:8080/chat/1"));
}

Communicate::~Communicate()
{
    delete ui;
}

void Communicate::on_toHome_clicked()
{
     emit toHome();
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

        // 模拟回复
        QTimer::singleShot(1000, [this](){
            appendMessage("This is a reply.", "Chatbot");
        });
        // 发送消息到WebSocket服务器
        webSocket->sendTextMessage(message);
    }
}

void Communicate::appendMessage(const QString &message, const QString &sender)
{
    QString formattedMessage = QString("<b>%1:</b> %2").arg(sender, message);
    ui->chatDisplay->append(formattedMessage);
}

void Communicate::onConnected()
{
    appendMessage("Connected to the server.", "System");
}

void Communicate::onTextMessageReceived(QString message)
{
    appendMessage(message, "Server");
}

void Communicate::onDisconnected()
{
    appendMessage("Disconnected from the server.", "System");
}
