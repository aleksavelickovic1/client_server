#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include "UiClient.hpp"

class ChatWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit ChatWindow(const std::string& host, int port, QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onLogin();
    void onSend();
    void onMessageReceived(const QString& text);
    void onDisconnected();

private:
    void buildLoginPage();
    void buildChatPage();

    QStackedWidget* stack_;

    QLineEdit*  usernameEdit_;
    QPushButton* loginButton_;
    QLabel*     statusLabel_;

    QTextEdit*  chatLog_;
    QLineEdit*  messageEdit_;
    QPushButton* sendButton_;

    UiClient* client_;
};