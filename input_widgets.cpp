#include "input_widgets.h"
#include "math.h"
#include <iostream>

/*************************
 * ABSTRACT INPUT WIDGET *
 *************************/
InputWidget::InputWidget(PixelData * pixel_data, int width, int height, bool enable_sensitivity_percentage, QWidget *parent = 0)
    : QWidget(parent), m_width(width), m_height(height), m_drawing(false), m_pixel_data(pixel_data), m_container_label(width, height),
      m_default_cursor(), m_custom_cursor(), painting(true)
{
    init_ui_elements();
    init_layout();
    init_signals();

    m_sensitivity_sb->setEnabled(enable_sensitivity_percentage);
}

InputWidget::~InputWidget()
{

}

void InputWidget::init_ui_elements()
{
    // Cursor size spin box
    m_cursor_size_sb = new QSpinBox(this);
    m_cursor_size_sb->setMinimum(1);
    m_cursor_size_sb->setMaximum(50);

    // Sensitivity combo box
    m_sensitivity_sb = new QSpinBox(this);
    m_sensitivity_sb->setMinimum(1);
    m_sensitivity_sb->setMaximum(100);
    m_sensitivity_sb->setValue(20);

    // Reset all button
    m_reset_all_btn = new QPushButton("Reset");

    // Reset all button
    m_fill_all_btn = new QPushButton("Fill");

    // Cursor type combo box
    m_cursor_type_cb = new QComboBox();
    m_cursor_type_cb->addItem(cursor_type_painter);
    m_cursor_type_cb->addItem(cursor_type_erasor);
}

void InputWidget::init_signals()
{
    connect(m_cursor_size_sb, SIGNAL(valueChanged(int)), this, SLOT(set_cursor_size(int)));
    m_cursor_size_sb->setValue(10);
    set_cursor_size(10);

    connect(&m_container_label, SIGNAL(leaveEvent()), this, SLOT(set_default_cursor()));
    connect(&m_container_label, SIGNAL(enterEvent()), this, SLOT(set_custom_cursor()));
    connect(&m_container_label, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(mouse_pressed(QMouseEvent*)));
    connect(&m_container_label, SIGNAL(mouseMoved(QMouseEvent*)), this, SLOT(mouse_moved(QMouseEvent*)));
    connect(&m_container_label, SIGNAL(mouseReleased(QMouseEvent*)), this, SLOT(mouse_released(QMouseEvent*)));

    connect(m_cursor_type_cb, SIGNAL(currentTextChanged(QString)), this, SLOT(refresh_cursor_type(QString)));
    connect(m_reset_all_btn, SIGNAL(clicked()), this, SLOT(reset_all_clicked()));
    connect(m_fill_all_btn, SIGNAL(clicked()), this, SLOT(fill_all()));
}

void InputWidget::init_layout()
{
    setFixedSize(m_width, m_height+50);

    QHBoxLayout * controllers_h_layout = new QHBoxLayout();
    controllers_h_layout->addWidget(new QLabel("Cursor size: "), 0, Qt::AlignLeft);
    controllers_h_layout->addWidget(m_cursor_size_sb, 0, Qt::AlignLeft);

    controllers_h_layout->addWidget(new QLabel("Cursor type: "), 0, Qt::AlignLeft);
    controllers_h_layout->addWidget(m_cursor_type_cb, 0, Qt::AlignLeft);

    controllers_h_layout->addWidget(new QLabel("Sensitivity: "), 0, Qt::AlignLeft);
    controllers_h_layout->addWidget(m_sensitivity_sb, 0, Qt::AlignLeft);

    controllers_h_layout->addWidget(m_reset_all_btn, 0, Qt::AlignLeft);
    controllers_h_layout->addWidget(m_fill_all_btn, 0, Qt::AlignLeft);

    QGridLayout * main_layout = new QGridLayout;
    main_layout->addWidget(&m_container_label,0,0,1,1, Qt::AlignCenter);
    main_layout->addLayout(controllers_h_layout,1,0,1,1, Qt::AlignCenter);

    setLayout(main_layout);
    refresh();
}

void InputWidget::fill_all()
{
    m_pixel_data->fillAll(m_sensitivity_sb->value());
    refresh();
}

QSize InputWidget::minimumSizeHint() const
{
    return QSize(m_width, m_height);
}

QSize InputWidget::sizeHint() const
{
    return minimumSizeHint();
}

void InputWidget::set_default_cursor()
{
    setCursor(m_default_cursor);
}

void InputWidget::set_custom_cursor()
{
    setCursor(m_custom_cursor);
}

void InputWidget::refresh_cursor_type(QString type)
{
    painting = (type == cursor_type_painter);
}

void InputWidget::reset_all_clicked()
{
    m_pixel_data->resetAll();
    refresh();
}

void InputWidget::mouse_pressed(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        m_drawing = true;
        process_pixel_data(event->pos());
    }
}

void InputWidget::mouse_released(QMouseEvent * event)
{
    m_drawing = false;
}

void InputWidget::mouse_moved(QMouseEvent * event)
{
    if(m_drawing)
        process_pixel_data(event->pos());
}

void InputWidget::process_pixel_data(QPoint p_point)
{
    if(!m_drawing)
        return;

    int square_radius(m_cursor_size_sb->value());
    int x_from(std::max(0, p_point.x()-square_radius));
    int x_to(std::min(m_width-1, p_point.x()+square_radius));

    int y_from(std::max(0, p_point.y()-square_radius));
    int y_to(std::min(m_height-1, p_point.y()+square_radius));

    int sensitivity (m_sensitivity_sb->value());

    for(int x(x_from); x <= x_to; x++)
    {
        for(int y(y_from); y <= y_to; y++)
        {
            if(painting)
                m_pixel_data->set(QPoint(x,y), sensitivity);
            else
                m_pixel_data->reset(QPoint(x,y));
        }
    }

    refresh();
}

void InputWidget::set_cursor_size(int p_size)
{
    QPixmap pixmap(p_size*2+1,p_size*2+1);
    pixmap.fill(Qt::transparent); // Otherwise you get a black background :(
    QPainter painter(&pixmap);
    painter.setPen(Qt::white);
    painter.drawRect(0,0,p_size*2,p_size*2);

    m_custom_cursor = QCursor(pixmap);
    setCursor(m_custom_cursor);
}

void InputWidget::refresh()
{
    m_container_label.setPixmap(QPixmap::fromImage(*(m_pixel_data->toImage())));
}

const QImage * InputWidget::getData()
{
    return m_pixel_data->toImage();
}

/****************************
 * SOIL HUMIDITY PIXEL DATA *
 ****************************/
SoilHumidityPixelData::SoilHumidityPixelData(int width, int height) :
    PixelData(width, height)
{
    m_image = new QImage(m_width, m_height, QImage::Format::Format_RGB32);
    m_image->fill(Qt::black);
}

SoilHumidityPixelData::~SoilHumidityPixelData()
{

}

void SoilHumidityPixelData::set(QPoint p_point, int p_percentage_of_max)
{
    int value ((p_percentage_of_max/100.0f) * 255);
    m_image->setPixel(p_point, qRgb(0,0,value));
}

void SoilHumidityPixelData::reset(QPoint p_point)
{
    m_image->setPixel(p_point, qRgb(0,0,0));
}

int SoilHumidityPixelData::getValue(QPoint p_point)
{
    return (int) ((get_blue(p_point) / 255.f) * MAX_HUMIDITY);
}

int SoilHumidityPixelData::get_blue(QPoint p_point)
{
    return qBlue(m_image->pixel(p_point.x(), p_point.y()));
}

QImage * SoilHumidityPixelData::toImage()
{
    return m_image;
}

void SoilHumidityPixelData::resetAll()
{
    m_image->fill(Qt::black);
}

void SoilHumidityPixelData::fillAll(int p_percentage_of_max)
{
    m_image->fill(QColor(0,0,(p_percentage_of_max/100.f) * 255));
}

/***************************
 * ILLUMINATION PIXEL DATA *
 ***************************/
IlluminationPixelData::IlluminationPixelData(int width, int height) :
    PixelData(width, height)
{
    m_image = new QImage(m_width, m_height, QImage::Format::Format_RGB32);
    m_image->fill(Qt::black);
}

IlluminationPixelData::~IlluminationPixelData()
{

}

void IlluminationPixelData::set(QPoint p_point, int p_percentage_of_max)
{
    m_image->setPixel(p_point, QColor(Qt::yellow).rgb());
}

void IlluminationPixelData::reset(QPoint p_point)
{
    m_image->setPixel(p_point, qRgb(0,0,0));
}

int IlluminationPixelData::getValue(QPoint p_point)
{
    if(qRed(m_image->pixel(p_point.x(), p_point.y())) > 0)
        return 100;
    else
        return 0;
}

QImage * IlluminationPixelData::toImage()
{
    return m_image;
}

void IlluminationPixelData::resetAll()
{
    m_image->fill(Qt::black);
}

void IlluminationPixelData::fillAll(int p_percentage_of_max)
{
    m_image->fill(QColor(255,255,0));
}

/*******************
 * SIAGNALED LABEL *
 *******************/
MySignaledLabel::MySignaledLabel(int width, int height)
{
    setFixedSize(width, height);
}

void MySignaledLabel::enterEvent ( QEvent * e ) {
    QLabel::enterEvent(e);
    emit enterEvent();
}

void MySignaledLabel::leaveEvent(QEvent *e){
    QLabel::leaveEvent(e);
    emit leaveEvent();
}

void MySignaledLabel::mousePressEvent(QMouseEvent *e)
{
    QLabel::mousePressEvent(e);
    std::cout << "Mouse position: " << "[" << e->pos().x() << ", " << e->pos().y() << "]" << std::endl;
    emit mousePressed(e);
}

void MySignaledLabel::mouseMoveEvent(QMouseEvent *e)
{
    QLabel::mouseMoveEvent(e);
    emit mouseMoved(e);
}

void MySignaledLabel::mouseReleaseEvent(QMouseEvent *e)
{
    QLabel::mouseReleaseEvent(e);
    emit mouseReleased(e);
}
