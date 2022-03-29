#include "j_chess.h"
#include "ui_j_chess.h"
#include <fstream>
#include <QStandardItemModel>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QVector>
#include<QMouseEvent>
#include<QPainter>
#include<QTimer>
using namespace std;
char player = 'O';
char computer = 'X'; //若不行就char转QString
J_chess::J_chess(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::J_chess)
{

    ui->setupUi(this);
    this->setWindowTitle(tr("#字棋"));
    init_chess();//棋盘初始化
    connect(ui->X,&QCheckBox::clicked,[=](){
        ui->O->setChecked(false);
        player='X';computer='O';


    });
    connect(ui->O,&QCheckBox::clicked,[=](){
        player='O';computer='X';
        ui->X->setChecked(false);
    });

    connect(ui->comboBox,&QComboBox::currentTextChanged,[=](){
        if((ui->comboBox->currentText()=="玩家-AI")){
            //X先下
            srand((unsigned int)time(0));//初始化种子为随机值
            int  num_r=rand()%2; // 0 1
            qDebug()<<num_r;
            if(num_r==1){
                QMessageBox::information(NULL,"提示","玩家先下!",QObject::tr(("确认")) , QObject::tr(("取消")));
                connect(ui->tableView,&QTableView::clicked,[=](){
                    int row1= ui->tableView->currentIndex().row();
                    int col1=ui->tableView->currentIndex().column();
                    QAbstractItemModel *model = ui->tableView->model ();
                    QModelIndex index = model->index(row1,col1);
                    model->setData(index, QString("%1").arg(player));
                    //MyMethod();
                    react();
                    QEventLoop eventloop;
                    QTimer::singleShot(1000, &eventloop, SLOT(quit()));
                    eventloop.exec();
                    computerInput();
                    react();
                });
            }
            else if(num_r==0){
                QMessageBox::information(NULL,"提示","电脑先下!",QObject::tr(("确认")) , QObject::tr(("取消")));
                if(ui->O->isChecked())computer='X';
                else if(ui->X->isChecked())computer='O';
                QEventLoop eventloop;
                QTimer::singleShot(1000, &eventloop, SLOT(quit()));
                eventloop.exec();
                computerInput();
                react();
                connect(ui->tableView,&QTableView::clicked,[=](){
                    int row1= ui->tableView->currentIndex().row();
                    int col1=ui->tableView->currentIndex().column();
                    QAbstractItemModel *model = ui->tableView->model ();
                    QModelIndex index = model->index(row1,col1);
                    model->setData(index, QString("%1").arg(player));
                    //MyMethod();
                    react();
                    QEventLoop eventloop;
                    QTimer::singleShot(1000, &eventloop, SLOT(quit()));
                    eventloop.exec();
                    computerInput();
                    react();
                });
            }
        }
        if(ui->comboBox->currentText()=="AI-AI"){
            connect(ui->pushButton,&QPushButton::clicked,[=](){
                player = 'O';
                 computer = 'X'; //若不行就char转QString
               ben();
                react();
//                QEventLoop eventloop;
//                QTimer::singleShot(1000, &eventloop, SLOT(quit()));
                 computerInput();
                react();
            });

        }
    });
    //只要tablevie内容改变就响应
    //connect(ui->tableView->model(),SIGNAL(itemChanged(QStandardItem*)),this,SLOT(react()));

}
//很笨的下法
void J_chess::ben(){
    QAbstractItemModel *model = ui->tableView->model ();
    if(model->data(model->index(1,1)).toString()=="")
        model->setData(model->index(1,1), QString("%1").arg(player));
    else{
        while(1){
        srand((unsigned int)time(0));
        int sx=rand()%3;
        int sy=rand()%3;
        if(model->data(model->index(sx,sy)).toString()=="")
        {
        model->setData(model->index(sx,sy), "O");return;
        }
        else continue;
        }
    }
    }
    J_chess::~J_chess()
    {
        delete ui;
    }
    int J_chess::bestInput(QString state, QString nextState, int alpha, int beta){
        char ch;
        if (state=="computer")ch = computer;
        else ch = player;
        if (detectWin(QString("%1").arg(ch))) {
            if (state == "computer")return 1;
            else  return -1;
        }
        else if (isEmpty()) {
            return 0;
        }
        else {
            int score;
            QAbstractItemModel *mdl = ui->tableView->model();
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (mdl->data(mdl->index(i,j)).toString()  == "") {
                        mdl->setData(mdl->index(i,j),QString("%1").arg(ch));//电脑下
                        score = bestInput(nextState, state, alpha, beta);
                        mdl->setData(mdl->index(i,j),"");
                        if (state == "computer") {
                            if (score >= alpha)alpha = score;
                            if (alpha > beta)return beta;
                        }
                        else {
                            if (score < beta)beta = score;
                            if (beta <= alpha)return alpha;
                        }
                    }
                }
            }
            if (state == "computer")return alpha;
            else return beta;
        }
    }

    //电脑下
    void J_chess::computerInput(){
        QAbstractItemModel *mdl = ui->tableView->model();
        int row2=0, col2=0;
        int best = 0;
        int bestScore = -1000;
        int score;
        for (int i = 0; i <=2; i++) {
            for (int j = 0; j <= 2; j++) {
                if (mdl->data(mdl->index(i,j)).toString()  == "") {
                    mdl->setData(mdl->index(i,j),QString("%1").arg(computer));//电脑下
                    score = bestInput("player", "computer", -1000, 1000);//alpha-beta剪枝是一个根据上下界限剪枝的算法，初始的上下界限为无穷
                    if (score > bestScore) {//在同一层的节点里面需要不断试探性递归，用回溯法找到最合适的下棋点使自己胜算最大
                        bestScore = score;
                        best = (i) * 3 + j;
                    }
                    mdl->setData(mdl->index(i,j),"");
                }
            }
        }
        if (best == 6 || best == 7 || best == 8)row2 = 2;
        if (best == 3 || best == 4 || best == 5)row2 = 1;
        if (best == 0 || best == 1 || best == 2)row2 = 0;
        if (best == 0 || best == 3 || best == 6)col2 = 0;
        if (best == 1 || best == 4 || best == 7)col2 = 1;
        if (best == 2 || best == 5 || best == 8)col2 = 2;
        mdl->setData(mdl->index(row2,col2),QString("%1").arg(computer));
        return ;
    }

    //初始化棋盘
    void J_chess::init_chess(){
        ui->tableView->resize(40*3+5,40*3+5);
        QStandardItemModel* model = new QStandardItemModel;
        ui->tableView->setModel(model);
        //QStandardItem* item ;
        ui->tableView->verticalHeader()->setVisible(false); //隐藏列表头
        ui->tableView->horizontalHeader()->setVisible(false); //隐藏行表
        ui->tableView->verticalHeader()->setDefaultSectionSize(40);
        ui->tableView->horizontalHeader()->setDefaultSectionSize(40);
        for( int i=0;i<3;i++){
            for( int j=0;j<3;j++)
            {
                model->setItem(i,j,new QStandardItem(QString("")));
            }
        }
    }

    bool J_chess::isEmpty(){
        //检查落子点 是否有地方可以下棋
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++)
            {
                QAbstractItemModel *mdl = ui->tableView->model ();
                QModelIndex index = mdl->index(i,j);
                if(mdl->data(index).toString()=="") //有空位
                    return false;  //不空还有地方可以下
            }
        }
        return true;
    }
    //判断是否胜利
    bool J_chess::detectWin(QString py)
            //三个X或三个O X返回-1 O返回1 平局或未结束返回0
    {
        //三个X或三个O X返回-1 O返回1 平局或未结束返回0
        QAbstractItemModel *mdl = ui->tableView->model();
        for (int i = 0; i < 3; i++)
            if (mdl->data(mdl->index(i,0)).toString() == py && mdl->data(mdl->index(i,1)).toString() == py && mdl->data(mdl->index(i,2)).toString() == py) {
                return true;
            }
            else {
                if (mdl->data(mdl->index(0,i)).toString() == py && mdl->data(mdl->index(1,i)).toString() == py && mdl->data(mdl->index(2,i)).toString() == py)
                    return true;
            }
        if (mdl->data(mdl->index(0,0)).toString() == py && mdl->data(mdl->index(1,1)).toString() == py && mdl->data(mdl->index(2,2)).toString() == py)
            return true;
        if (mdl->data(mdl->index(0,2)).toString() == py && mdl->data(mdl->index(1,1)).toString() == py && mdl->data(mdl->index(2,0)).toString() == py)
            return true;
        return false;
    }
void J_chess::init(){
        QAbstractItemModel *mdl = ui->tableView->model();
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                QModelIndex index = mdl->index(i,j);
                mdl->setData(index,"");
            }
        }
        this->close();
    }
    void J_chess::react(){
        if(detectWin(QString("%1").arg(computer))){
            int t=QMessageBox::information(NULL,"提示","电脑获胜!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
        }
        else if (detectWin(QString("%1").arg(player))){
            int t=QMessageBox::information(NULL,"提示","玩家获胜!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }

        }
        else {
            QAbstractItemModel *mdl = ui->tableView->model();int num=0;
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(mdl->data(mdl->index(i,j)).toString()  == "")//有一处为空 就没下满就不可能平局
                    {
                        return ;
                    }
                }
            }
            int t=QMessageBox::information(NULL,"提示","平局!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
        }
        return ;
    }

    void J_chess::MyMethod(){
        //三个X或三个O X返回-1 O返回1 平局或未结束返回0
        QString str;
        QString win_O="OOO";QString win_X="XXX";
        //处理行获胜
        //第一行
        QAbstractItemModel *mdl = ui->tableView->model();
        QModelIndex index0 = mdl->index(0,0);
        QModelIndex index1 = mdl->index(0,1);
        QModelIndex index2 = mdl->index(0,2);
        str.append(mdl->data(index0).toString());
        str.append(mdl->data(index1).toString());
        str.append(mdl->data(index2).toString());
        //    for(int i=0;i<str.length();i++){
        //        qDebug()<<str.at(i);
        //    }
        if(0==QString::compare(str,win_X)){
            int t=QMessageBox::information(NULL,"提示","你获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        if(0==QString::compare(str,win_O)){
            int t=QMessageBox::information(NULL,"提示","电脑获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        str.clear(); //清空内容
        //第二行
        QModelIndex index3 = mdl->index(1,0);
        QModelIndex index4 = mdl->index(1,1);
        QModelIndex index5 = mdl->index(1,2);
        str.append(mdl->data(index3).toString());
        str.append(mdl->data(index4).toString());
        str.append(mdl->data(index5).toString());
        if(0==QString::compare(str,win_X)){
            int t=QMessageBox::information(NULL,"提示","你获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        if(0==QString::compare(str,win_O)){
            int t=QMessageBox::information(NULL,"提示","电脑获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        str.clear(); //清空内容
        //第三行
        QModelIndex index6 = mdl->index(2,0);
        QModelIndex index7 = mdl->index(2,1);
        QModelIndex index8 = mdl->index(2,2);
        str.append(mdl->data(index6).toString());
        str.append(mdl->data(index7).toString());
        str.append(mdl->data(index8).toString());
        if(0==QString::compare(str,win_X)){
            int t=QMessageBox::information(NULL,"提示","你获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        if(0==QString::compare(str,win_O)){
            int t=QMessageBox::information(NULL,"提示","电脑获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        str.clear(); //清空内容
        //第一列
        QModelIndex l1=mdl->index(0,0);
        QModelIndex l2=mdl->index(1,0);
        QModelIndex l3=mdl->index(2,0);
        str.append(mdl->data(l1).toString());
        str.append(mdl->data(l2).toString());
        str.append(mdl->data(l3).toString());
        if(0==QString::compare(str,win_X)){
            int t=QMessageBox::information(NULL,"提示","你获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        if(0==QString::compare(str,win_O)){
            int t=QMessageBox::information(NULL,"提示","电脑获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        str.clear(); //清空内容
        //第二列
        QModelIndex l4=mdl->index(0,1);
        QModelIndex l5=mdl->index(1,1);
        QModelIndex l6=mdl->index(2,1);
        str.append(mdl->data(l4).toString());
        str.append(mdl->data(l5).toString());
        str.append(mdl->data(l6).toString());
        if(0==QString::compare(str,win_X)){
            int t=QMessageBox::information(NULL,"提示","你获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        if(0==QString::compare(str,win_O)){
            int t=QMessageBox::information(NULL,"提示","电脑获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        str.clear(); //清空内容
        //第三列
        QModelIndex l7=mdl->index(0,2);
        QModelIndex l8=mdl->index(1,2);
        QModelIndex l9=mdl->index(2,2);
        str.append(mdl->data(l7).toString());
        str.append(mdl->data(l8).toString());
        str.append(mdl->data(l9).toString());
        if(0==QString::compare(str,win_X)){
            int t=QMessageBox::information(NULL,"提示","你获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        if(0==QString::compare(str,win_O)){
            int t=QMessageBox::information(NULL,"提示","电脑获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        str.clear(); //清空内容
        //主对角线
        QModelIndex x1=mdl->index(0,0);
        QModelIndex x2=mdl->index(1,1);
        QModelIndex x3=mdl->index(2,2);
        str.append(mdl->data(x1).toString());
        str.append(mdl->data(x2).toString());
        str.append(mdl->data(x3).toString());
        if(0==QString::compare(str,win_X)){
            int t=QMessageBox::information(NULL,"提示","你获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }

            return ;
        }
        if(0==QString::compare(str,win_O)){
            int t=QMessageBox::information(NULL,"提示","电脑获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        str.clear(); //清空内容
        //斜对角线
        QModelIndex x4=mdl->index(0,2);
        QModelIndex x5=mdl->index(1,1);
        QModelIndex x6=mdl->index(2,0);
        str.append(mdl->data(x4).toString());
        str.append(mdl->data(x5).toString());
        str.append(mdl->data(x6).toString());
        if(0==QString::compare(str,win_X)){
            int t=QMessageBox::information(NULL,"提示","你获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        if(0==QString::compare(str,win_O)){
            int t=QMessageBox::information(NULL,"提示","电脑获胜了!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
        str.clear(); //清空内容
        if(isEmpty()){
            int t=QMessageBox::information(NULL,"提示","平局!",QObject::tr(("确认")) , QObject::tr(("取消")));
            if (t == QObject::tr(("确认")).toInt()) {
                init();
            }
            return ;
        }
    }
