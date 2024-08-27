#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <QWidget>
#include <QWebSocket>


namespace Ui {
class Communicate;
}

class Communicate : public QWidget
{
    Q_OBJECT

public:
    explicit Communicate(QWidget *parent = nullptr);
    ~Communicate();

signals:
    void toHome();

private slots:
    void on_toHome_clicked();
    void on_sendButton_clicked();
    void onConnected();             // WebSocket连接建立成功时调用
    void onTextMessageReceived(QString message);  // 接收消息时调用
    void onDisconnected();          // WebSocket连接断开时调用


private:
    Ui::Communicate *ui;
    QWebSocket *webSocket;          // WebSocket对象
    void appendMessage(const QString &message, const QString &sender);
};

#endif // COMMUNICATE_H
