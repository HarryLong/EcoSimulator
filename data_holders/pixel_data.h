#ifndef PIXEL_DATA_H
#define PIXEL_DATA_H

#include <QImage>

class ResourceConverter;
/*******************
 * BASE PIXEL DATA *
 *******************/
class PixelData {
public:
    PixelData(const ResourceConverter * translator, int width, int height);
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
    const ResourceConverter * m_translator;
};
#endif // PIXEL_DATA_H
