#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reddit_api.h"

#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    subredditLineEdit = new QLineEdit(this);
    fetchButton = new QPushButton("Fetch Posts", this);
    resultsTable = new QTableWidget(this);

    // Configurare tabelă
    resultsTable->setColumnCount(4);
    resultsTable->setHorizontalHeaderLabels({"Title", "Author", "Score", "Comments"});
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable->horizontalHeader()->setStretchLastSection(true);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(subredditLineEdit);
    layout->addWidget(fetchButton);
    layout->addWidget(resultsTable);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(fetchButton, &QPushButton::clicked, this, &MainWindow::onFetchPostsClicked);
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

    QStringList posts = fetchHotPosts(subreddit);
    if (posts.isEmpty()) {
        QMessageBox::information(this, "No Posts", "Could not fetch posts from the subreddit.");
        return;
    }

    resultsTable->setRowCount(0);  // Resetează tabelul
    for (const QString &post : posts) {
        int row = resultsTable->rowCount();
        resultsTable->insertRow(row);

        QStringList postDetails = post.split("||");
        for (int col = 0; col < postDetails.size() && col < 4; ++col) {
            resultsTable->setItem(row, col, new QTableWidgetItem(postDetails[col]));
        }
    }
}
