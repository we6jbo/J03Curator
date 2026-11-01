// Retained from the original Qt Creator scaffold for compatibility/history.
// J03Curator intentionally uses no GUI and does not compile this file.
#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) { ui->setupUi(this); }
MainWindow::~MainWindow() { delete ui; }
