#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "n_queen.h"
#include "j_chess.h"
#include "monster.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    N_queen *q;
    J_chess *j;
    Monster *m;
};

#endif // MAINWINDOW_H
