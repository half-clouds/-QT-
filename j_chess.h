#ifndef J_CHESS_H
#define J_CHESS_H

#include <QMainWindow>
using namespace std;
namespace Ui {
class J_chess;
}

class J_chess : public QMainWindow
{
    Q_OBJECT

public:
    explicit J_chess(QWidget *parent = 0);
    ~J_chess();
    void init_chess();
    void startGame();
    bool isEmpty();
    bool detectWin(QString py);
    void xiaqi(char p);
    void init();
    void playerInput();
    void computerInput();
    int bestInput(QString state, QString nextState, int alpha, int beta);
    void ben();
private:
    Ui::J_chess *ui;
public slots:
    void MyMethod();
    void react();
};

#endif // J_CHESS_H
