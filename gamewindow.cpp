#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_actionNew_triggered()
{

}

void GameWindow::on_actionOpen_triggered()
{

}

void GameWindow::on_actionSave_triggered()
{

}

void GameWindow::on_actionSave_as_triggered()
{

}

void GameWindow::on_actionExit_triggered()
{

}
