#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("菜单界面"));
    connect(ui->btn1,&QPushButton::clicked,[=](){
        //this->hide();
        q=new N_queen;
        q->show();
    });
    connect(ui->btn3,&QPushButton::clicked,[=](){
        //this->hide();
        j=new J_chess;
        j->show();
    });
    connect(ui->btn2,&QPushButton::clicked,[=](){
        m=new Monster;
        m->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
