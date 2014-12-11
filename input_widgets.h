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

/*******************
 * BASE PIXEL DATA *
 *******************/
class PixelData {
public:
    PixelData(int width, int height);
    virtual ~PixelData() {}
    virtual void resetAll();
    virtual const QImage & toImage() const;
    virtual void setData(QImage image);
    virtual void reset(QPoint p_point);
    virtual void set(QPoint p_point, int p_percentage_of_max) = 0;
    virtual void fillAll(int p_percentage_of_max) = 0;
    virtual int getValue(QPoint point) = 0;

protected:
    int m_width, m_height;
    QImage m_image;
};

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
    InputWidget(PixelData * pixel_data, int width, int height, bool enable_sensitivity_percentage, QWidget *parent);

    ~InputWidget();

    QSize minimumSizeHint() const;

    QSize sizeHint() const;

    const QImage & getData() const;

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

    QCursor m_custom_cursor;
    QCursor m_default_cursor;

    bool painting;
};


/*****************
 * SOIL HUMIDITY *
 *****************/
#define MIN_HUMIDITY 0
#define MAX_HUMIDITY 100
#define INCREMENT 10
class SoilHumidityPixelData : public PixelData
{
public:
    SoilHumidityPixelData(int width, int height);
    ~SoilHumidityPixelData();
    virtual void set(QPoint p_point,int p_percentage_of_max);
    virtual void fillAll(int p_percentage_of_max);
    int getValue(QPoint point);

private:
    int get_blue(QPoint p_point);
};
class SoilHumidityInputWidget : public InputWidget
{
public:
    SoilHumidityInputWidget( int width, int height, QWidget * parent = 0 ) : InputWidget(new SoilHumidityPixelData(width, height), width, height, true, parent) {}
};

/****************
 * ILLUMINATION *
 ****************/
class IlluminationPixelData : public PixelData
{
public:
    IlluminationPixelData(int width, int height);
    ~IlluminationPixelData();
    virtual void set(QPoint p_point,int p_percentage_of_max);
    virtual void fillAll(int p_percentage_of_max);
    int getValue(QPoint point);        
};
class IlluminationInputWidget : public InputWidget
{
public:
    IlluminationInputWidget( int width, int height, QWidget * parent = 0 ) : InputWidget(new IlluminationPixelData(width, height), width, height, true, parent) {}
};
#endif // INPUT_WIDGETS_H
