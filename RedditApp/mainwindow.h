#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QStackedWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
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
    void onFetchPostsClicked();
    void onCellClicked(int row, int column);
    void showFetchPostsPage();
    void showMessagesPage();
    void onSendMessageClicked();

private:
    Ui::MainWindow *ui;
    QComboBox *timeFilterComboBox;
    QCheckBox *sortByScoreCheckBox;
    QCheckBox *sortByCommentsCheckBox;
    QComboBox *sortCriteriaComboBox;
    QLineEdit *subredditLineEdit;
    QPushButton *fetchButton;
    QTableWidget *resultsTable;
    QStackedWidget *stackedWidget;
    QWidget *fetchPostsPage;
    QWidget *messagesPage;
    QTextEdit *messageInput;
    QPushButton *sendMessageButton;
};

#endif // MAINWINDOW_H
