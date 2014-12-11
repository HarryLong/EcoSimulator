#include "main_window.h"
#include "plant_db_editor.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>

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
    delete m_launch_plant_db_action;
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

    m_launch_plant_db_action = new QAction("Plant DB", this);
    m_launch_plant_db_action->setStatusTip("Launch plant database editor");
    connect(m_launch_plant_db_action, SIGNAL(triggered()), this, SLOT(launch_plant_db_manager()));
}

void MainWindow::init_menu()
{
    m_file_menu = menuBar()->addMenu("File");
    m_file_menu->addAction(m_launch_plant_db_action);
    m_file_menu->addAction(m_exit_act);
}

void MainWindow::launch_plant_db_manager()
{
    PlantDBEditor plant_db_editor(this);
    plant_db_editor.exec();
}

int main(int argc, char *argv[])
{
    std::cout << "Starting..." << std::endl;
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(w.sizeHint());
    w.showMaximized();

    return app.exec();
}

void MainWindow::closeEvent ( QCloseEvent * event )
{
    std::cout << "Stopping..." << std::endl;
    m_central_widget->stop_simulation();
}
