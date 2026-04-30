#include "ChatWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QString>

ChatWindow::ChatWindow(const std::string& host, int port, QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Chat");
    resize(480, 600);

    client_ = new UiClient(host, port, this);
    connect(client_, &UiClient::messageReceived, this, &ChatWindow::onMessageReceived);
    connect(client_, &UiClient::disconnected,    this, &ChatWindow::onDisconnected);

    stack_ = new QStackedWidget(this);
    setCentralWidget(stack_);

    buildLoginPage();
    buildChatPage();

    stack_->setCurrentIndex(0);
}


void ChatWindow::buildLoginPage() {
    auto* page   = new QWidget;
    auto* layout = new QVBoxLayout(page);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(12);

    auto* title = new QLabel("Join Chat");
    QFont f = title->font();
    f.setPointSize(18);
    f.setBold(true);
    title->setFont(f);
    title->setAlignment(Qt::AlignCenter);

    usernameEdit_ = new QLineEdit;
    usernameEdit_->setPlaceholderText("Username");
    usernameEdit_->setMaximumWidth(260);

    loginButton_ = new QPushButton("Connect");
    loginButton_->setMaximumWidth(260);

    statusLabel_ = new QLabel;
    statusLabel_->setAlignment(Qt::AlignCenter);
    statusLabel_->setStyleSheet("color: red;");

    layout->addWidget(title);
    layout->addSpacing(16);
    layout->addWidget(usernameEdit_, 0, Qt::AlignCenter);
    layout->addWidget(loginButton_,  0, Qt::AlignCenter);
    layout->addWidget(statusLabel_,  0, Qt::AlignCenter);

    connect(usernameEdit_, &QLineEdit::returnPressed, this, &ChatWindow::onLogin);
    connect(loginButton_,  &QPushButton::clicked,     this, &ChatWindow::onLogin);

    stack_->addWidget(page);
}


void ChatWindow::buildChatPage() {
    auto* page   = new QWidget;
    auto* layout = new QVBoxLayout(page);
    layout->setSpacing(6);

    chatLog_ = new QTextEdit;
    chatLog_->setReadOnly(true);

    auto* inputRow = new QHBoxLayout;
    messageEdit_ = new QLineEdit;
    messageEdit_->setPlaceholderText("Type a message...");
    sendButton_  = new QPushButton("Send");

    inputRow->addWidget(messageEdit_);
    inputRow->addWidget(sendButton_);

    layout->addWidget(chatLog_);
    layout->addLayout(inputRow);

    connect(messageEdit_, &QLineEdit::returnPressed, this, &ChatWindow::onSend);
    connect(sendButton_,  &QPushButton::clicked,     this, &ChatWindow::onSend);

    stack_->addWidget(page);
}


void ChatWindow::onLogin() {
    QString username = usernameEdit_->text().trimmed();
    if (username.isEmpty()) {
        statusLabel_->setText("Username cannot be empty.");
        return;
    }

    loginButton_->setEnabled(false);
    statusLabel_->setText("Connecting...");

    try {
        client_->connectToServer(username.toStdString());
    } catch (const std::exception& ex) {
        statusLabel_->setText(QString("Error: %1").arg(ex.what()));
        loginButton_->setEnabled(true);
        return;
    }

    setWindowTitle("Chat — " + username);
    stack_->setCurrentIndex(1);
    messageEdit_->setFocus();
}

void ChatWindow::onSend() {
    QString text = messageEdit_->text().trimmed();
    if (text.isEmpty()) return;

    if (!client_->sendMessage(text.toStdString())) {
        chatLog_->append("--- Failed to send message --");
        return;
    }

    messageEdit_->clear();
}

void ChatWindow::onMessageReceived(const QString& text) {
    chatLog_->append(text);
}

void ChatWindow::onDisconnected() {
    chatLog_->append("--- Disconnected from server ---");
    sendButton_->setEnabled(false);
    messageEdit_->setEnabled(false);
}

void ChatWindow::closeEvent(QCloseEvent* event) {
    client_->disconnect();
    event->accept();
}