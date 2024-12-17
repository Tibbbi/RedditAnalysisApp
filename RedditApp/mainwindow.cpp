#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reddit_api.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenuBar>
#include <QListWidgetItem>
#include <QProcess>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create menu
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menu = menuBar->addMenu("Menu");

    QAction *fetchPostsAction = new QAction("Fetch Posts", this);
    QAction *fetchMessagesAction = new QAction("Fetch Messages", this);

    connect(fetchPostsAction, &QAction::triggered, this, &MainWindow::showFetchPostsPage);
    connect(fetchMessagesAction, &QAction::triggered, this, &MainWindow::showMessagesPage);

    menu->addAction(fetchPostsAction);
    menu->addAction(fetchMessagesAction);

    setMenuBar(menuBar);

    // Configure stacked widget
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Configure pages
    setupFetchPostsPage();
    setupMessagesPage();
}
void MainWindow::onSortCriteriaChanged(const QString &criteria) {
    // Implement the functionality for when the sort criteria changes
    // For example, you might want to refetch posts based on the new criteria
    fetchPosts();
}

void MainWindow::onSortCriteriaChanged(int index) {
    // This can be an overload if you need to handle index-based changes
    // Implement the functionality as needed
    fetchPosts();
}

void MainWindow::onTimeFrameChanged(int index) {
    // Implement the functionality for when the time frame changes
    // For example, refetch posts based on the new time frame
    fetchPosts();
}

void MainWindow::onAuthorFilterChanged(const QString &text) {
    // Implement the functionality for when the author filter text changes
    // For example, refilter the displayed posts based on the author
    fetchPosts();
}

void MainWindow::setupFetchPostsPage()
{
    postsPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(postsPage);

    subredditInput = new QLineEdit(this);
    subredditInput->setPlaceholderText("Enter subreddit name");
    fetchPostsButton = new QPushButton("Fetch Posts", this);
    postsTable = new QTableWidget(this);
    postsTable->setColumnCount(5);
    postsTable->setHorizontalHeaderLabels({"Title", "Author", "Score", "Comments", "Link"});

    layout->addWidget(subredditInput);
    layout->addWidget(fetchPostsButton);

    // ComboBox for sorting criteria
    sortComboBox = new QComboBox(this);
    sortComboBox->addItem("Hot", "hot");
    sortComboBox->addItem("New", "new");
    sortComboBox->addItem("Top", "top");
    sortComboBox->addItem("Controversial", "controversial");
    layout->addWidget(sortComboBox);
    connect(sortComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::fetchPosts);

    // ComboBox for time frame
    timeFrameComboBox = new QComboBox(this);
    timeFrameComboBox->addItem("All Time", "all");
    timeFrameComboBox->addItem("Past Hour", "hour");
    timeFrameComboBox->addItem("Past 24 Hours", "day");
    timeFrameComboBox->addItem("Past Week", "week");
    timeFrameComboBox->addItem("Past Month", "month");
    timeFrameComboBox->addItem("Past Year", "year");
    layout->addWidget(timeFrameComboBox);
    connect(timeFrameComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::fetchPosts);

    // LineEdit for author filter
    authorFilterInput = new QLineEdit(this);
    authorFilterInput->setPlaceholderText("Filter by author");
    layout->addWidget(authorFilterInput);
    connect(authorFilterInput, &QLineEdit::textChanged, this, &MainWindow::fetchPosts);

    layout->addWidget(postsTable);
    connect(fetchPostsButton, &QPushButton::clicked, this, &MainWindow::fetchPosts);

    stackedWidget->addWidget(postsPage);
}

void MainWindow::setupMessagesPage()
{
    messagesPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(messagesPage);

    messagesList = new QListWidget(this);
    messageInput = new QTextEdit(this);
    sendMessageButton = new QPushButton("Send Message", this);

    layout->addWidget(messagesList);
    layout->addWidget(messageInput);
    layout->addWidget(sendMessageButton);

    connect(sendMessageButton, &QPushButton::clicked, this, &MainWindow::fetchMessages);

    stackedWidget->addWidget(messagesPage);
}

void MainWindow::showFetchPostsPage()
{
    stackedWidget->setCurrentIndex(0); // First page: Fetch Posts
}

void MainWindow::showMessagesPage()
{
    stackedWidget->setCurrentIndex(1); // Second page: Messages
}

void MainWindow::fetchPosts()
{
    QString subreddit = subredditInput->text();
    if (subreddit.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a subreddit name.");
        return;
    }

    QString sortBy = sortComboBox->currentData().toString();
    QString timeFrame = timeFrameComboBox->currentData().toString();
    QString authorFilter = authorFilterInput->text();
    int limit = 10; // Number of posts to fetch

    QStringList posts = fetchHotPosts(subreddit, sortBy, limit, timeFrame);
    postsTable->setRowCount(0);

    for (const QString &post : posts) {
        QStringList postDetails = post.split("||");
        if (!authorFilter.isEmpty() && !postDetails[1].contains(authorFilter, Qt::CaseInsensitive)) {
            continue; // Skip posts that don't match the author filter
        }
        int row = postsTable->rowCount();
        postsTable->insertRow(row);

        for (int col = 0; col < postDetails.size(); ++col) {
            postsTable->setItem(row, col, new QTableWidgetItem(postDetails[col]));
        }
    }
}

void MainWindow::fetchMessages()
{
    QString token = QString::fromStdString(getBearerToken());
    if (token.isEmpty()) {
        QMessageBox::critical(this, "Error", "Failed to obtain Bearer Token.");
        return;
    }

    QStringList messages = fetchPrivateMessages(token);
    messagesList->clear();

    for (const QString &message : messages) {
        QListWidgetItem *item = new QListWidgetItem(message, messagesList);
        messagesList->addItem(item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
