#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QObject>
#include "central_widget.h"
#include "plant_db_editor.h"

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

protected:
    virtual void closeEvent ( QCloseEvent * event );

private slots:
    void launch_plant_db_manager();

private:
    void init_layout();
    void init_menu();
    void init_actions();

    // Actions
    QAction * m_exit_act;
    QAction * m_launch_plant_db_action;

    // Menu elements
    QMenu * m_file_menu;

    PlantDBEditor m_plant_db_editor;

    CentralWidget * m_central_widget;
};

#endif //MAIN_WINDOW_H
