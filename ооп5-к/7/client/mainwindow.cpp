#include "ui_mainwindow.h"
#include "mainwindow.h"

QString xshow;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &MainWindow::deleteLater);
    socket->connectToHost("127.0.0.1", 2323);
    ui->type_show->setText("Текущий тип данных: complex");
    SendToServer("c");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        if (str != "") { ui->x_show->setText(xshow + str);}
        in >> str;
        ui->pol_show->setText("Полином: " + str);
    }
}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out << str;
    socket->write(Data);

}

void MainWindow::on_pushButton_2_clicked()
{
    //SendToServer(ui->lineEdit->text());
}

void MainWindow::on_lineEdit_returnPressed()
{
    //SendToServer(ui->lineEdit->text());
}



void MainWindow::on_add_el_clicked()
{
    if (type == 0){
    SendToServer("c");
    SendToServer("add_el");
    QString i_re = ui->add_el_re->text();
    QString i_lm = ui->add_el_lm->text();
    SendToServer(i_re);
    SendToServer(i_lm);}
    else
    {
        SendToServer("r");
        SendToServer("add_el");
        QString i_re = ui->add_el_re->text();
        SendToServer(i_re);
    }
}

void MainWindow::on_chng_coef_clicked()
{
    if (type == 0) {
        SendToServer("c");
    SendToServer("chng_coef");
    QString a_re = ui->chng_coef_re->text();
    QString a_lm = ui->chng_coef_lm->text();
    SendToServer(a_re);
    SendToServer(a_lm);
    }
    else
    {
        SendToServer("r");
        SendToServer("chng_coef");
        QString a_re = ui->chng_coef_re->text();
        SendToServer(a_re);
    }
}

void MainWindow::on_del_el_clicked()
{
    if (type == 0) {SendToServer("c");}
    else {SendToServer("r");}
    SendToServer("del_el");
    QString i = ui->del_el_n->text();
    SendToServer(i);
}

void MainWindow::on_find_val_clicked()
{
    if (type == 0){

        SendToServer("c");
    SendToServer("find_val");
    QString x_re = ui->find_val_re->text();
    QString x_lm = ui->find_val_lm->text();
    SendToServer(x_re);
    SendToServer(x_lm);
    xshow = "Значение в точке (" + x_re + " + " + x_lm + "i) : ";}
    else
    {
        SendToServer("r");
        SendToServer("find_val");
        QString x_re = ui->find_val_re->text();
        SendToServer(x_re);
        xshow = "Значение в точке " + x_re + " : ";
    }
}

void MainWindow::on_change_data_clicked()
{
    if (f == 1)
    {

        if (!ftype){
            ftype = 1;
            SendToServer("SI");
             ui->type_show->setText("Текущая функция SI");
        }
        else {
            ftype = 0;
             ui->type_show->setText("Текущая функция sin");
            SendToServer("sin");
        }
        return;
    }
    if (type == 1)
    {
        SendToServer("ch");
        type = 0;
        ui->type_show->setText("Текущий тип данных: complex");

        ui->find_val_lm->show();
        ui->add_el_lm->show();
        ui->chng_coef_lm->show();
        ui->label->show();
        ui->label_2->show();
        ui->label_3->show();
        ui->label_4->show();
        ui->label_6->show();
        ui->label_7->show();

        ui->find_val_lm->clear();
        ui->find_val_re->clear();
        ui->add_el_lm->clear();
        ui->add_el_re->clear();
        ui->chng_coef_lm->clear();
        ui->chng_coef_re->clear();
        ui->del_el_n->clear();

        ui->pol_show->setText("Полином: ");
        ui->x_show->setText("Значение в точке x: ");
    }
    else
    {
        SendToServer("rh");
        type = 1;
        ui->type_show->setText("Текущий тип данных: rational");
        ui->find_val_lm->clear();
        ui->find_val_re->clear();
        ui->add_el_lm->clear();
        ui->add_el_re->clear();
        ui->chng_coef_lm->clear();
        ui->chng_coef_re->clear();
        ui->del_el_n->clear();

        ui->find_val_lm->hide();
        ui->add_el_lm->hide();
        ui->chng_coef_lm->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_6->hide();
        ui->label_7->hide();

        ui->x_show->setText("Значение в точке x: ");
        ui->pol_show->setText("Полином: ");
    }
}

void MainWindow::on_pushButton_clicked()
{
    SendToServer("f");
    QString name;
    if (f == 0)
    {
        f = 1;
        ftype = 0;
        ui->pol_show->hide();

        ui->x_show->clear();
        ui->x_show->setText("Значение функции в точке: ");
        ui->type_show->setText("Выбранная функция: ");

        ui->type_show->setText("Текущая функция sin");

        ui->add_el->hide();
        ui->del_el->setText("Точность ");
        ui->chng_coef->hide();

        ui->change_data->setText("Изменить функцию");

        ui->chng_coef_lm->hide();
        ui->chng_coef_re->hide();
        ui->add_el_lm->hide();
        ui->add_el_re->hide();

        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();

    }
    else
    {
        f = 0;
        ui->pol_show->show();

        ui->x_show->clear();
        ui->x_show->setText("Значение в точке x: ");

        ui->type_show->setText("Текущий тип данных: complex");

        ui->find_val->setText("Рассчитать значение в точке");

        ui->add_el->show();
        ui->del_el_n->show();
        ui->chng_coef->show();

        ui->change_data->setText("Изменить тип данных");

        ui->del_el->setText("Удалить элемент по индексу ");
        ui->chng_coef_lm->show();
        ui->chng_coef_re->show();
        ui->add_el_lm->show();
        ui->add_el_re->show();

        ui->label->show();
        ui->label_2->show();
        ui->label_3->show();
        ui->label_4->show();
    }
}
