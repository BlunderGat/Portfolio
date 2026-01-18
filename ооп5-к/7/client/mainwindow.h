#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDataStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool type = 0;
    bool f = 0;
    bool ftype = 0;


public slots:
    void slotReadyRead();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_returnPressed();


private slots:
    void on_add_el_clicked();

    void on_chng_coef_clicked();

    void on_del_el_clicked();

    void on_find_val_clicked();

    void on_change_data_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    void SendToServer(QString str);
};
#endif // MAINWINDOW_H
