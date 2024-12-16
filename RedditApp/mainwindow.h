#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlabel.h"
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QComboBox>
#include <QStackedWidget>
#include <QMenuBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showFetchPosts();
    void showMessages();
    void onFetchPostsClicked();

private:
    Ui::MainWindow *ui;

    // Widgets pentru meniuri
    QStackedWidget *stackedWidget;

    // Fetch Posts
    QWidget *fetchPostsWidget;
    QLineEdit *subredditLineEdit;
    QPushButton *fetchButton;
    QTableWidget *resultsTable;

    QGroupBox *filterGroupBox;
    QSpinBox *minScoreSpinBox;
    QSpinBox *minCommentsSpinBox;
    QComboBox *sortComboBox;

    // Messages
    QWidget *messagesWidget;
    QLabel *messagesLabel;
};

#endif
