#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reddit_api.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configurare meniu
    QMenu *menu = menuBar()->addMenu("Menu");
    QAction *fetchPostsAction = new QAction("Fetch Posts", this);
    QAction *messagesAction = new QAction("Messages", this);
    menu->addAction(fetchPostsAction);
    menu->addAction(messagesAction);

    connect(fetchPostsAction, &QAction::triggered, this, &MainWindow::showFetchPostsPage);
    connect(messagesAction, &QAction::triggered, this, &MainWindow::showMessagesPage);

    // Configurare pagini
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Pagina Fetch Posts
    fetchPostsPage = new QWidget(this);
    subredditLineEdit = new QLineEdit(fetchPostsPage);
    fetchButton = new QPushButton("Fetch Posts", fetchPostsPage);
    resultsTable = new QTableWidget(fetchPostsPage);

    sortCriteriaComboBox = new QComboBox(this);
    sortCriteriaComboBox->addItem("Newest");
    sortCriteriaComboBox->addItem("Oldest");
    sortCriteriaComboBox->addItem("Most Popular");
    sortCriteriaComboBox->addItem("Most Commented");
    sortCriteriaComboBox->addItem("Highest Score");

    resultsTable->setColumnCount(5);
    resultsTable->setHorizontalHeaderLabels({"Title", "Author", "Score", "Comments", "Link"});
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable->horizontalHeader()->setStretchLastSection(true);

    // Inițializare ComboBox pentru filtrarea după timp
    timeFilterComboBox = new QComboBox(this);
    timeFilterComboBox->addItems({"All Time", "Last 24 Hours", "Last Week", "Last Month"});

    // Inițializare CheckBox-uri pentru sortare
    sortByScoreCheckBox = new QCheckBox("Sort by Score", this);
    sortByCommentsCheckBox = new QCheckBox("Sort by Comments", this);


    QVBoxLayout *fetchLayout = new QVBoxLayout(fetchPostsPage);
    fetchLayout->addWidget(subredditLineEdit);
    fetchLayout->addWidget(fetchButton);
    fetchLayout->addWidget(resultsTable);
    fetchLayout->addWidget(sortCriteriaComboBox);
    fetchLayout->addWidget(timeFilterComboBox);
    fetchLayout->addWidget(sortByScoreCheckBox);
    fetchLayout->addWidget(sortByCommentsCheckBox);


    stackedWidget->addWidget(fetchPostsPage);

    connect(fetchButton, &QPushButton::clicked, this, &MainWindow::onFetchPostsClicked);
    connect(resultsTable, &QTableWidget::cellClicked, this, &MainWindow::onCellClicked);

    // Pagina Messages
    messagesPage = new QWidget(this);
    QVBoxLayout *messagesLayout = new QVBoxLayout(messagesPage);

    messageInput = new QTextEdit(messagesPage);
    sendMessageButton = new QPushButton("Send Message", messagesPage);

    messagesLayout->addWidget(messageInput);
    messagesLayout->addWidget(sendMessageButton);

    stackedWidget->addWidget(messagesPage);

    connect(sendMessageButton, &QPushButton::clicked, this, &MainWindow::onSendMessageClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFetchPostsClicked()
{
    QString subreddit = subredditLineEdit->text();
    if (subreddit.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a subreddit name.");
        return;
    }

    // Obținerea filtrului de timp selectat
    QString timeFilter;
    QString selectedTime = timeFilterComboBox->currentText();
    if (selectedTime == "Last 24 Hours") {
        timeFilter = "day";
    } else if (selectedTime == "Last Week") {
        timeFilter = "week";
    } else if (selectedTime == "Last Month") {
        timeFilter = "month";
    } else {
        timeFilter = "all";
    }

    // Obținerea selecțiilor de sortare
    bool sortByScore = sortByScoreCheckBox->isChecked();
    bool sortByComments = sortByCommentsCheckBox->isChecked();

    auto posts = fetchHotPosts(subreddit, timeFilter, sortByScore, sortByComments);
    if (posts.isEmpty()) {
        QMessageBox::information(this, "No Posts", "Could not fetch posts from the subreddit.");
        return;
    }

    resultsTable->setRowCount(0);  // Resetează tabelul
    for (const auto &post : posts) {
        int row = resultsTable->rowCount();
        resultsTable->insertRow(row);

        QStringList postDetails = post.split("||");
        for (int col = 0; col < postDetails.size() && col < 5; ++col) {
            auto *item = new QTableWidgetItem(postDetails[col]);
            resultsTable->setItem(row, col, item);
        }
    }
}



void MainWindow::onCellClicked(int row, int column)
{
    if (column == 4) { // Coloana "Link"
        QString url = resultsTable->item(row, column)->text();
        QDesktopServices::openUrl(QUrl(url));
    }
}


void MainWindow::showFetchPostsPage()
{
    stackedWidget->setCurrentWidget(fetchPostsPage);
}

void MainWindow::showMessagesPage()
{
    stackedWidget->setCurrentWidget(messagesPage);
}

void MainWindow::onSendMessageClicked()
{
    QString message = messageInput->toPlainText();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a message to send.");
        return;
    }

    // Aici poți adăuga codul pentru a trimite mesajul către un server sau altă destinație
    // De exemplu, poți folosi QTcpSocket pentru a trimite mesajul către un server

    // Pentru demonstrație, vom afișa mesajul într-o fereastră de dialog
    QMessageBox::information(this, "Message Sent", "Your message has been sent:\n" + message);

    // Golește câmpul de introducere a mesajului după trimitere
    messageInput->clear();
}
