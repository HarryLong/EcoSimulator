#include "input_widgets.h"
#include "math.h"
#include <iostream>
#include <QFileDialog>
#include <math.h>)

/*************************
 * ABSTRACT INPUT WIDGET *
 *************************/
InputWidget::InputWidget(PixelData * pixel_data, int width, int height, int min, int max, QString label, QString unit, QWidget *parent)
    : QWidget(parent), m_width(width), m_height(height), m_min(min), m_max(max), m_label(label),
      m_drawing(false), m_pixel_data(pixel_data), m_container_label(width, height),
      m_default_cursor(), m_custom_cursor(), painting(true), m_unit(unit)
{
    init_ui_elements();
    init_layout();
    init_signals();
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
    m_sensitivity_sb->setRange(m_min, m_max);
    m_sensitivity_sb->setValue(m_min + ((m_max-m_min)/2));

    // Reset all button
    m_reset_all_btn = new QPushButton("Reset");

    // Reset all button
    m_fill_all_btn = new QPushButton("Fill");

    // Cursor type combo box
    m_cursor_type_cb = new QComboBox();
    m_cursor_type_cb->addItem(cursor_type_painter);
    m_cursor_type_cb->addItem(cursor_type_erasor);

    // Save and Load buttons
    m_save_btn = new QPushButton("Save");
    m_load_btn = new QPushButton("Load");

    m_gradual_min_sb = new QSpinBox;
    m_gradual_min_sb->setRange(m_min, m_max);
    m_gradual_min_sb->setValue(m_min);

    m_gradual_max_sb = new QSpinBox;
    m_gradual_max_sb->setRange(m_min, m_max);
    m_gradual_max_sb->setValue(m_max);
    m_generate_gradual_btn = new QPushButton("Generate");

    m_variance_sb = new QSpinBox(this);
    m_variance_sb->setRange(m_min, m_max);
    m_variance_sb->setValue(0);
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
    connect(m_generate_gradual_btn, SIGNAL(clicked()), this, SLOT(generate_gradual_clicked()));

    connect(m_save_btn, SIGNAL(clicked()), this, SLOT(save_btn_clicked()));
    connect(m_load_btn, SIGNAL(clicked()), this, SLOT(load_btn_clicked()));
}

void InputWidget::init_layout()
{
    setFixedSize(m_width, m_height+200);

    QVBoxLayout * main_layout = new QVBoxLayout;

    // The main input widget
    main_layout->addWidget(&m_container_label);

    // controller buttons
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Cursor size: "), 0, Qt::AlignLeft);
        h_layout->addWidget(m_cursor_size_sb, 0, Qt::AlignLeft);

        h_layout->addWidget(new QLabel("Cursor type: "), 0, Qt::AlignLeft);
        h_layout->addWidget(m_cursor_type_cb, 0, Qt::AlignLeft);

        h_layout->addWidget(new QLabel(m_label), 0, Qt::AlignLeft);
        h_layout->addWidget(m_sensitivity_sb, 0, Qt::AlignLeft);
        h_layout->addWidget(new QLabel(m_unit), 0, Qt::AlignLeft);
        h_layout->addWidget(m_fill_all_btn, 0, Qt::AlignLeft);
        h_layout->addWidget(new QLabel(""), 1, Qt::AlignLeft);
        main_layout->addLayout(h_layout);
    }
    // controller buttons
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Gradual increase from "), 0, Qt::AlignLeft);
        h_layout->addWidget(m_gradual_min_sb, 0, Qt::AlignLeft);
        h_layout->addWidget(new QLabel(m_unit), 0, Qt::AlignLeft);
        h_layout->addWidget(new QLabel(" to "), 0, Qt::AlignLeft);
        h_layout->addWidget(m_gradual_max_sb, 0, Qt::AlignLeft);
        h_layout->addWidget(new QLabel(m_unit), 0, Qt::AlignLeft);
        h_layout->addWidget(m_generate_gradual_btn, 0, Qt::AlignLeft);
        h_layout->addWidget(new QLabel(""), 1, Qt::AlignLeft);
        main_layout->addLayout(h_layout);
    }

    // The save and load button
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(m_save_btn, 0, Qt::AlignCenter);
        h_layout->addWidget(m_reset_all_btn, 0, Qt::AlignCenter);
        h_layout->addWidget(m_load_btn, 0, Qt::AlignCenter);
        main_layout->addLayout(h_layout);
    }

    // The maximum button
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel(""), 1, Qt::AlignCenter);
        h_layout->addWidget(new QLabel("Maximum: + "), 0, Qt::AlignCenter);
        h_layout->addWidget(m_variance_sb, 0, Qt::AlignCenter);
        h_layout->addWidget(new QLabel(m_unit), 0, Qt::AlignCenter);
        h_layout->addWidget(new QLabel(""), 1, Qt::AlignCenter);
        main_layout->addLayout(h_layout);
    }
    setLayout(main_layout);
    refresh();
}

void InputWidget::fill_all()
{
    m_pixel_data->fillAll(m_sensitivity_sb->value());
    refresh();
}

void InputWidget::save_btn_clicked()
{
    QString filename (QFileDialog::getSaveFileName(this, "Save", ".", tr("PNG Images (*.png)")));
    if(!filename.isNull())
    {
        if(!filename.endsWith(".png"))
            filename.append(".png");
        toImage().save(filename, "png", 100);
    }
}

const QImage & InputWidget::toImage() const
{
    return m_pixel_data->toImage();
}

PixelData * InputWidget::getPixelData()
{
    return m_pixel_data;
}

void InputWidget::load_btn_clicked()
{
    QString filename (QFileDialog::getOpenFileName(this, "Open", ".", tr("PNG Images (*.png)")));
    if(!filename.isNull())
    {
        QImage image;
        image.load(filename);
        m_pixel_data->setData( image );
        refresh();
    }
}

void InputWidget::generate_gradual_clicked()
{
    int from (m_gradual_min_sb->value());
    int to (m_gradual_max_sb->value());
    int diff(to-from);

    bool incremental(diff > 0);

    int pixels_per_value(m_width);
    if(diff != 0)
        pixels_per_value = std::abs(m_width/diff);

    int pixel_value(from);
    for(int x = 0; x < m_width; x++)
    {
        if(x > 0 && (x % pixels_per_value == 0))
        {
            if(incremental)
                pixel_value++;
            else
                pixel_value--;
        }
        for(int y (0); y < m_height; y++)
            m_pixel_data->set(QPoint(x,y),pixel_value);
    }
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
    m_container_label.setPixmap(QPixmap::fromImage(m_pixel_data->toImage()));
}

int InputWidget::getVariance()
{
    return m_variance_sb->value();
}

/*******************
 * BASE PIXEL DATA *
 *******************/
PixelData::PixelData(const PixelDataTranslator * translator, int width, int height) : m_translator(translator), m_width(width), m_height(height), m_image(m_width, m_height, QImage::Format::Format_RGB32)
{
    resetAll();
}

PixelData::~PixelData()
{
    delete m_translator;
}

QRgb PixelData::toRGB(int p_value)
{
    return m_translator->toRGB(p_value);
}

int PixelData::toValue(QRgb p_pixel)
{
    return m_translator->toValue(p_pixel);
}

void PixelData::resetAll()
{
    m_image.fill(Qt::black);
}

const QImage & PixelData::toImage() const
{
    return m_image;
}

void PixelData::reset(QPoint p_point)
{
    m_image.setPixel(p_point, qRgb(0,0,0));
}

void PixelData::scale(int p_width, int p_height)
{
    setData(m_image.scaled(p_width, p_height, Qt::IgnoreAspectRatio));
    m_width = p_width;
    m_height = p_height;
}

int PixelData::getValue(QPoint p_point)
{
    return toValue(m_image.pixel(p_point));
}

void PixelData::fillAll(int p_value)
{
    m_image.fill(QColor(toRGB(p_value)));
}

void PixelData::setData(QImage p_image)
{
    // First scale the image if necessary
    if(p_image.width() != m_image.width() || p_image.height() != m_image.height())
        m_image = m_image.scaled(m_width, m_height, Qt::IgnoreAspectRatio);

    for(int x(0); x < p_image.width(); x++)
    {
        for(int y(0); y < p_image.height(); y++)
        {
            m_image.setPixel(x, y, toRGB(toValue(p_image.pixel(x,y))));
        }
    }
}

void PixelData::set(QPoint p_point,int p_value)
{
    m_image.setPixel(p_point, toRGB(p_value));
}

/******************
 * SIGNALED LABEL *
 ******************/
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
