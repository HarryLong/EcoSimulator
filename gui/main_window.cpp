#include "main_window.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags)
{
    init_layout();
    init_actions();
    init_menu();
}

MainWindow::~MainWindow()
{
    delete m_exit_act;
    delete m_file_menu;
}

void MainWindow::init_layout()
{
    m_central_widget = new CentralWidget(this);
    setCentralWidget(m_central_widget);
}

void MainWindow::init_actions()
{
    m_exit_act = new QAction("Exit", this);
    m_exit_act->setShortcuts(QKeySequence::Close);
    connect(m_exit_act, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::init_menu()
{
    m_file_menu = menuBar()->addMenu("File");
    m_file_menu->addAction(m_exit_act);
}

void MainWindow::closeEvent ( QCloseEvent * event )
{
    qWarning() << "Stopping...";
    m_central_widget->stop_simulation();
}
