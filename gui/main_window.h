#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QObject>

#include "widgets/central_widget.h"

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

protected:
    virtual void closeEvent ( QCloseEvent * event );

private:
    void init_layout();
    void init_menu();
    void init_actions();

    // Actions
    QAction * m_exit_act;

    // Menu elements
    QMenu * m_file_menu;

    CentralWidget * m_central_widget;
};

#endif //MAIN_WINDOW_H
