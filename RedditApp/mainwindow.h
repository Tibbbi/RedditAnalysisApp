#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QStackedWidget>
#include <QComboBox>
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
    void fetchPosts();
    void fetchMessages();
    void showFetchPostsPage();
    void showMessagesPage();
    void onSortCriteriaChanged(const QString &criteria);
    void onSortCriteriaChanged(int index);
    void onTimeFrameChanged(int index);
    void onAuthorFilterChanged(const QString &text);


private:
    void setupFetchPostsPage();
    void setupMessagesPage();

    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    QWidget *postsPage;
    QWidget *messagesPage;
    QTableWidget *postsTable;
    QLineEdit *subredditInput;
    QPushButton *fetchPostsButton;
    QTextEdit *messageInput;
    QPushButton *sendMessageButton;
    QListWidget *messagesList;
    QComboBox *sortComboBox;
    QComboBox *timeFrameComboBox;
    QLineEdit *authorFilterInput;

};

#endif // MAINWINDOW_H
