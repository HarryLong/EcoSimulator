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
#include "pixel_data_translator.h"

/*******************
 * BASE PIXEL DATA *
 *******************/
class PixelData {
public:
    PixelData(const PixelDataTranslator * translator, int width, int height);
    virtual ~PixelData();
    virtual void resetAll();
    virtual const QImage & toImage() const;
    virtual void reset(QPoint p_point);

    virtual void scale(int p_width, int p_height);
    virtual void setData(QImage p_image);
    virtual void set(QPoint p_point, int p_value);
    virtual void fillAll(int p_value);
    virtual int getValue(QPoint point);

    virtual QRgb toRGB(int p_value);
    virtual int toValue(QRgb p_pixel);

    int m_width, m_height;

protected:
    QImage m_image;
    const PixelDataTranslator * m_translator;
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
    InputWidget(PixelData * pixel_data, int width, int height, int min, int max, QString label, QWidget *parent = 0);

    ~InputWidget();

    QSize minimumSizeHint() const;

    QSize sizeHint() const;

    const QImage & toImage() const;

    PixelData * getPixelData();

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

    QCursor m_custom_cursor;
    QCursor m_default_cursor;

    QString m_label;

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
    SoilHumidityInputWidget( int width, int height, QWidget * parent = 0 ) : InputWidget(new PixelData(new SoilHumidityTranslator, width, height),
                                                                                         width, height,
                                                                                         0,100, "Humidity (%):", parent) {}
};

/****************
 * ILLUMINATION *
 ****************/
class IlluminationInputWidget : public InputWidget
{
public:
    IlluminationInputWidget( int width, int height, QWidget * parent = 0 ) : InputWidget(new PixelData(new IlluminationTranslator, width, height),
                                                                                         width, height,
                                                                                         0,24, "Illumination (hours):", parent) {}
};

/***************
 * TEMPERATURE *
 ***************/
class TemperatureInputWidget : public InputWidget
{
public:
    TemperatureInputWidget( int width, int height, QWidget * parent = 0 ) : InputWidget(new PixelData(new TemperatureTranslator, width, height),
                                                                                        width, height,
                                                                                         -50,50, "Temperature (celcius):", parent) {}
};
#endif // INPUT_WIDGETS_H
