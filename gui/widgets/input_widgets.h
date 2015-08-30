#ifndef INPUT_WIDGETS_H
#define INPUT_WIDGETS_H

#include <QWidget>
#include <map>
#include <QLabel>
#include <QGridLayout>
#include <QMouseEvent>
#include <QSpinBox>
#include <QPainter>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>

class PixelData;
/******************
 * SIGNALED LABEL *
 ******************/
class MySignaledLabel : public QLabel
{
Q_OBJECT
public:
    MySignaledLabel(int width, int height);

signals:
    void leaveEvent();
    void enterEvent();
    void mousePressed(QMouseEvent * pos);
    void mouseMoved(QMouseEvent * pos);
    void mouseReleased(QMouseEvent * pos);

protected:
    void enterEvent ( QEvent * e );

    void leaveEvent(QEvent *e);

    void mousePressEvent(QMouseEvent *e);

    void mouseMoveEvent(QMouseEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);

private:
    bool m_pressed;
};

/****************
 * INPUT WIDGET *
 ****************/
const static QString cursor_type_painter = "Brush";
const static QString cursor_type_erasor = "Erasor";
class InputWidget : public QWidget
{
Q_OBJECT
public:
    InputWidget(PixelData * pixel_data, int min, int max, QString label, QString unit, QWidget *parent = 0);

    ~InputWidget();

    QSize minimumSizeHint() const;

    QSize sizeHint() const;

    const QImage & toImage() const;

    PixelData * getPixelData();

    int getVariance();

private slots:
    void set_cursor_size(int p_size);
    void set_default_cursor();
    void set_custom_cursor();
    void process_pixel_data(QPoint p_point);
    void mouse_pressed(QMouseEvent *  event);
    void mouse_released(QMouseEvent * event);
    void mouse_moved(QMouseEvent * event);
    void refresh_cursor_type(QString type);
    void reset_all_clicked();
    void fill_all();
    void load_btn_clicked();
    void save_btn_clicked();
    void generate_gradual_clicked();

private:
    void refresh();
    void init_layout();
    void init_signals();
    void init_ui_elements();

    int m_width, m_height;
    bool m_drawing;
    MySignaledLabel m_container_label;
    QSpinBox * m_cursor_size_sb;
    QSpinBox * m_sensitivity_sb;
    PixelData * m_pixel_data;
    QComboBox * m_cursor_type_cb;
    QPushButton * m_reset_all_btn;
    QPushButton * m_fill_all_btn;

    QPushButton * m_save_btn;
    QPushButton * m_load_btn;

    QSpinBox * m_gradual_min_sb;
    QSpinBox * m_gradual_max_sb;
    QPushButton * m_generate_gradual_btn;

    QSpinBox * m_variance_sb;

    QCursor m_custom_cursor;
    QCursor m_default_cursor;

    QString m_label;
    QString m_unit;

    int m_min;
    int m_max;

    bool painting;
};


/*****************
 * SOIL HUMIDITY *
 *****************/
class SoilHumidityInputWidget : public InputWidget
{
public:
    SoilHumidityInputWidget( QWidget * parent = 0 );
};

/****************
 * ILLUMINATION *
 ****************/
class IlluminationInputWidget : public InputWidget
{
public:
    IlluminationInputWidget( QWidget * parent = 0 );
};

/***************
 * TEMPERATURE *
 ***************/
class TemperatureInputWidget : public InputWidget
{
public:
    TemperatureInputWidget( QWidget * parent = 0 );
};

#endif // INPUT_WIDGETS_H
