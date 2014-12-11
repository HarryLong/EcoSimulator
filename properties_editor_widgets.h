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
        setRange(.0,1000.0);
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
    MyCustomFloatingPointSizeSpinBox* m_max_vertical_growth_dsb;
    MyCustomMultiplierSpinBox* m_canopy_width_multiplier_dsb;
    MyCustomFloatingPointSizeSpinBox* m_max_roots_growth_dsb;
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
    MyPercentageSpinBox * m_prob_of_death_at_birth;
    MyPercentageSpinBox * m_prob_of_death_at_upper;

    MyAgeSpinBox* m_prime_age_start_sb;
    MyAgeSpinBox* m_prime_age_end_sb;
    MyAgeSpinBox* m_upper_age_sb;
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
    MyPercentageSpinBox * m_shade_tolerance_sb;
    MySensitivitySpinBox * m_sensitivity_sb;
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
    MyPercentageSpinBox* m_prime_soil_humidity_percentage_start_sb;
    MyPercentageSpinBox* m_prime_soil_humidity_percentage_end_sb;
    MySensitivitySpinBox * m_sensitivity_sb;
};

#endif //PROPERTIES_EDITOR_WIDGETS_H
