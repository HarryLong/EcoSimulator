#include "pixel_data.h"
#include "pixel_data_translator.h"

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
