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
    // Creates main chat window and initializes UI
    explicit ChatWindow(const std::string& host, int port, QWidget* parent = nullptr);

protected:
    // Handles window close and disconnects client
    void closeEvent(QCloseEvent* event) override;

private slots:
    // Handles login button click or enter
    void onLogin();

    // Sends message from input field
    void onSend();

    // Displays received message in chat log
    void onMessageReceived(const QString& text);

    // Handles server disconnection
    void onDisconnected();

private:
    // Builds login UI page
    void buildLoginPage();

    // Builds chat UI page
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