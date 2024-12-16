#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

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

private:
    Ui::MainWindow *ui;

    QLineEdit *subredditLineEdit;
    QPushButton *fetchButton;
    QTableWidget *resultsTable;
};

#endif // MAINWINDOW_H
