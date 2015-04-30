#ifndef PROPERTIES_EDITOR_WIDGETS_H
#define PROPERTIES_EDITOR_WIDGETS_H

#include "plant_properties.h"
#include <QDoubleSpinBox>

class QHBoxLayout;
class QCheckBox;

/******************
 * HELPER CLASSES *
 ******************/
class MyCustomFloatingPointSizeSpinBox : public QDoubleSpinBox
{
public:
    MyCustomFloatingPointSizeSpinBox() : QDoubleSpinBox()
    {
        setRange(.0,10000.0);
        setDecimals(2);
    }

    virtual void clear()
    {
        setValue(.0);
        QDoubleSpinBox::clear();
    }
};
class MyCustomMultiplierSpinBox : public QDoubleSpinBox
{
public:
    MyCustomMultiplierSpinBox() : QDoubleSpinBox()
    {
        setRange(.0,1000.0);
        setDecimals(5);
    }

    virtual void clear()
    {
        setValue(.0);
        QDoubleSpinBox::clear();
    }
};

class MyPercentageSpinBox : public QSpinBox
{
public:
    MyPercentageSpinBox() : QSpinBox()
    {
        setRange(0,100);
    }

    virtual void clear()
    {
        setValue(0);
        QSpinBox::clear();
    }
};

class My24HourSpinBox : public QSpinBox
{
public:
    My24HourSpinBox() : QSpinBox()
    {
        setRange(0,24);
    }

    virtual void clear()
    {
        setValue(0);
        QSpinBox::clear();
    }
};

class MyAgeSpinBox : public QSpinBox
{
public:
    MyAgeSpinBox() : QSpinBox()
    {
        setRange(0,10000);
    }

    virtual void clear()
    {
        setValue(0);
        QSpinBox::clear();
    }
};

class MySensitivitySpinBox : public QSpinBox
{
public:
    MySensitivitySpinBox() : QSpinBox()
    {
        setRange(1,5);
    }

    virtual void clear()
    {
        setValue(1);
        QSpinBox::clear();
    }
};

class MyTemperatureSpinBox : public QSpinBox
{
public:
    MyTemperatureSpinBox() : QSpinBox()
    {
        setRange(-50,50);
    }

    virtual void clear()
    {
        setValue(15);
        QSpinBox::clear();
    }
};


/****************************
 * GROWTH PROPERTIES WIDGET *
 ****************************/
class GrowthPropertiesWidget : public QWidget
{
Q_OBJECT
public:
    GrowthPropertiesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~GrowthPropertiesWidget();

    GrowthProperties* getProperties();

    void setProperties(const GrowthProperties * p_properties);
    void clear();

private slots:
    void set_canopy_width_input_enabled(bool checked);

private:
    void init_signals();
    void init_layout();

    // UI Elements
    MyCustomFloatingPointSizeSpinBox* m_max_height_dsb;
    MyCustomFloatingPointSizeSpinBox* m_max_canopy_width_dsb;
    MyCustomFloatingPointSizeSpinBox* m_max_root_size_dsb;
    QCheckBox * m_canopy_cb;
};

/****************************
 * AGEING PROPERTIES WIDGET *
 ****************************/
class AgeingPropertiesWidget : public QWidget
{
public:
    AgeingPropertiesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~AgeingPropertiesWidget();

    AgeingProperties* getProperties();

    void setProperties(const AgeingProperties * p_properties);
    void clear();

private:
    void init_layout();

    // UI Elements
    MyAgeSpinBox * m_start_of_decline_sb;
    MyAgeSpinBox * m_max_age_sb;
};

/**********************************
 * ILLUMINATION PROPERTIES WIDGET *
 **********************************/
class IlluminationPropertiesWidget : public QWidget
{
public:
    IlluminationPropertiesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~IlluminationPropertiesWidget();

    IlluminationProperties* getProperties();

    void setProperties(const IlluminationProperties * p_properties);
    void clear();

private:
    void init_layout();

    // UI Elements
    My24HourSpinBox * m_prime_illumination_start_sb;
    My24HourSpinBox * m_prime_illumination_end_sb;

    My24HourSpinBox * m_min_illumination_sb;
    My24HourSpinBox * m_max_illumination_sb;
};

/***********************************
 * SOIL HUMIDITY PROPERTIES WIDGET *
 ***********************************/
class SoilHumidityPropertiesWidget : public QWidget
{
public:
    SoilHumidityPropertiesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~SoilHumidityPropertiesWidget();

    SoilHumidityProperties* getProperties();

    void setProperties(const SoilHumidityProperties * p_properties);
    void clear();

private:
    void init_layout();

    // UI Elements
    MyPercentageSpinBox* m_prime_humidity_start_sb;
    MyPercentageSpinBox* m_prime_humidity_end_sb;
    MyPercentageSpinBox * m_min_humidity_sb;
    MyPercentageSpinBox * m_max_humidity_sb;
};

/*********************************
 * TEMPERATURE PROPERTIES WIDGET *
 *********************************/
class TemperaturePropertiesWidget : public QWidget
{
public:
    TemperaturePropertiesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~TemperaturePropertiesWidget();

    TemperatureProperties* getProperties();

    void setProperties(const TemperatureProperties * p_properties);
    void clear();

private:
    void init_layout();

    // UI Elements
    MyTemperatureSpinBox* m_prime_temp_start_sb;
    MyTemperatureSpinBox* m_prime_temp_end_sb;
    MyTemperatureSpinBox * m_min_temp_sb;
    MyTemperatureSpinBox * m_max_temp_sb;
};

/*****************************
 * SEEDING PROPERTIES WIDGET *
 *****************************/
class SeedingPropertiesWidget : public QWidget
{
public:
    SeedingPropertiesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~SeedingPropertiesWidget();

    SeedingProperties* getProperties();

    void setProperties(const SeedingProperties * p_properties);
    void clear();

private:
    void init_layout();

    // UI Elements
    QSpinBox* m_max_seeding_distance_sb;
    QSpinBox* m_seed_count_sb;
};


#endif //PROPERTIES_EDITOR_WIDGETS_H
