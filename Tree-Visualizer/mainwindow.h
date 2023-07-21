#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPushButton>

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
    void displayTree();
    void style();

private:
//    Ui::MainWindow *ui;
    QGraphicsView *graphicsView;
    QLineEdit *traversalInput;
    QPushButton *displayButton;
};
#endif // MAINWINDOW_H
