#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_loadButtonClicked();
    void processText();
private:
    QTextEdit* displayText;
    QPushButton* loadButton;
    QPushButton* processButton;
};
#endif // MAINWINDOW_H
