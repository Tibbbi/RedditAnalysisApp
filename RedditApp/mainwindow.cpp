#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reddit_api.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QFormLayout>
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Stacked Widget pentru a comuta între pagini
    stackedWidget = new QStackedWidget(this);

    // --- FETCH POSTS WIDGET ---
    fetchPostsWidget = new QWidget(this);
    subredditLineEdit = new QLineEdit(this);
    fetchButton = new QPushButton("Fetch Posts", this);
    resultsTable = new QTableWidget(this);

    resultsTable->setColumnCount(6);
    resultsTable->setHorizontalHeaderLabels({"Title", "Author", "Score", "Comments", "Image", "Link"});
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable->horizontalHeader()->setStretchLastSection(true);

    filterGroupBox = new QGroupBox("Filters", this);
    minScoreSpinBox = new QSpinBox(this);
    minScoreSpinBox->setRange(0, 10000);
    minScoreSpinBox->setValue(100);
    minCommentsSpinBox = new QSpinBox(this);
    minCommentsSpinBox->setRange(0, 1000);
    minCommentsSpinBox->setValue(10);

    sortComboBox = new QComboBox(this);
    sortComboBox->addItems({"Sort by Score", "Sort by Date"});

    QFormLayout *filterLayout = new QFormLayout();
    filterLayout->addRow("Min Score:", minScoreSpinBox);
    filterLayout->addRow("Min Comments:", minCommentsSpinBox);
    filterLayout->addRow("Sort By:", sortComboBox);
    filterGroupBox->setLayout(filterLayout);

    QVBoxLayout *fetchLayout = new QVBoxLayout();
    fetchLayout->addWidget(subredditLineEdit);
    fetchLayout->addWidget(fetchButton);
    fetchLayout->addWidget(filterGroupBox);
    fetchLayout->addWidget(resultsTable);
    fetchPostsWidget->setLayout(fetchLayout);

    connect(fetchButton, &QPushButton::clicked, this, &MainWindow::onFetchPostsClicked);
    connect(resultsTable, &QTableWidget::cellClicked, this, [this](int row, int col) {
        if (col == 5) { // Link
            QTableWidgetItem *item = resultsTable->item(row, col);
            if (item) {
                QDesktopServices::openUrl(QUrl(item->text()));
            }
        }
    });

    // --- MESSAGES WIDGET ---
    messagesWidget = new QWidget(this);
    messagesLabel = new QLabel("Messages feature not implemented yet.", this);
    QVBoxLayout *messagesLayout = new QVBoxLayout();
    messagesLayout->addWidget(messagesLabel);
    messagesWidget->setLayout(messagesLayout);

    // Adaugă widget-urile în stackedWidget
    stackedWidget->addWidget(fetchPostsWidget);
    stackedWidget->addWidget(messagesWidget);
    setCentralWidget(stackedWidget);

    // --- MENU BAR ---
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menu = menuBar->addMenu("Menu");

    QAction *fetchPostsAction = new QAction("Fetch Posts", this);
    QAction *messagesAction = new QAction("Messages", this);

    connect(fetchPostsAction, &QAction::triggered, this, &MainWindow::showFetchPosts);
    connect(messagesAction, &QAction::triggered, this, &MainWindow::showMessages);

    menu->addAction(fetchPostsAction);
    menu->addAction(messagesAction);
    setMenuBar(menuBar);

    // Setează pagina inițială
    stackedWidget->setCurrentWidget(fetchPostsWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showFetchPosts()
{
    stackedWidget->setCurrentWidget(fetchPostsWidget);
}

void MainWindow::showMessages()
{
    stackedWidget->setCurrentWidget(messagesWidget);
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

    int minScore = minScoreSpinBox->value();
    int minComments = minCommentsSpinBox->value();
    QString sortCriteria = sortComboBox->currentText();

    QList<QStringList> filteredPosts;
    for (const QString &post : posts) {
        QStringList details = post.split("||");
        int score = details[2].toInt();
        int comments = details[3].toInt();

        if (score >= minScore && comments >= minComments) {
            filteredPosts.append(details);
        }
    }

    if (sortCriteria == "Sort by Score") {
        std::sort(filteredPosts.begin(), filteredPosts.end(), [](const QStringList &a, const QStringList &b) {
            return a[2].toInt() > b[2].toInt();
        });
    }

    resultsTable->setRowCount(0);
    for (const QStringList &details : filteredPosts) {
        int row = resultsTable->rowCount();
        resultsTable->insertRow(row);
        for (int col = 0; col < details.size(); ++col) {
            if (col == 4) {
                QLabel *imageLabel = new QLabel();
                QPixmap pixmap;
                if (!pixmap.load(details[4]) || details[4].isEmpty()) {
                    imageLabel->setText("No Image");
                    imageLabel->setAlignment(Qt::AlignCenter);
                } else {
                    imageLabel->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));
                }
                resultsTable->setCellWidget(row, col, imageLabel);
            } else if (col == 5) {
                QTableWidgetItem *linkItem = new QTableWidgetItem(details[5]);
                linkItem->setForeground(Qt::blue);
                resultsTable->setItem(row, col, linkItem);
            } else {
                resultsTable->setItem(row, col, new QTableWidgetItem(details[col]));
            }
        }
    }
}
