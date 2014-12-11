#include "properties_editor_widgets.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>

/****************************
 * GROWTH PROPERTIES WIDGET *
 ****************************/
GrowthPropertiesWidget::GrowthPropertiesWidget(QWidget* parent, Qt::WindowFlags f):
    QWidget(parent, f)
{
    // Init UI elements
    m_max_vertical_growth_dsb = new MyCustomFloatingPointSizeSpinBox();
    m_canopy_width_multiplier_dsb = new MyCustomMultiplierSpinBox();
    m_max_roots_growth_dsb = new MyCustomFloatingPointSizeSpinBox();
    m_canopy_cb = new QCheckBox();
    m_canopy_cb->setChecked(true);

    init_layout();
    init_signals();
}

GrowthPropertiesWidget::~GrowthPropertiesWidget()
{
}

GrowthProperties* GrowthPropertiesWidget::getProperties()
{
    return new GrowthProperties (m_max_vertical_growth_dsb->value(),
                             m_max_roots_growth_dsb->value(),
                             m_canopy_width_multiplier_dsb->value());
}

void GrowthPropertiesWidget::setProperties(const GrowthProperties * p_properties)
{
    m_max_vertical_growth_dsb->setValue(p_properties->max_annual_vertical_growth);
    m_canopy_width_multiplier_dsb->setValue(p_properties->height_to_width_multiplier);
    m_max_roots_growth_dsb->setValue(p_properties->max_annual_root_growth);
}

void GrowthPropertiesWidget::clear()
{
    m_max_vertical_growth_dsb->clear();
    m_canopy_width_multiplier_dsb->clear();
    m_max_roots_growth_dsb->clear();
}

void GrowthPropertiesWidget::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;

    int row(0);

    // Max monthly vertical growth
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Max annual vertical growth: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_vertical_growth_dsb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("Cm"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    // Canopy width checkbox
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Does plant have a canopy (shade projection): "),0,Qt::AlignLeft);
        h_layout->addWidget(m_canopy_cb,0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    // Max monthly canopy growth
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Height to canopy width multiplier: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_canopy_width_multiplier_dsb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("x"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Max annual roots growth (radius): "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_roots_growth_dsb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("Cm"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }

    setLayout(main_layout);
}

void GrowthPropertiesWidget::set_canopy_width_input_enabled(bool checked)
{
    m_canopy_width_multiplier_dsb->setEnabled(checked);

    if(!checked)
        m_canopy_width_multiplier_dsb->setValue(.0);
}

void GrowthPropertiesWidget::init_signals()
{
    connect(m_canopy_cb, SIGNAL(clicked(bool)), this, SLOT(set_canopy_width_input_enabled(bool)));
}

/****************************
 * AGEING PROPERTIES WIDGET *
 ****************************/
AgeingPropertiesWidget::AgeingPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
{
    // Init UI elements
    m_prob_of_death_at_birth = new MyPercentageSpinBox();
    m_prob_of_death_at_upper = new MyPercentageSpinBox();

    m_prime_age_start_sb = new MyAgeSpinBox();
    m_prime_age_end_sb = new MyAgeSpinBox();
    m_upper_age_sb = new MyAgeSpinBox();

    init_layout();
}

AgeingPropertiesWidget::~AgeingPropertiesWidget()
{

}

AgeingProperties* AgeingPropertiesWidget::getProperties()
{
    return new AgeingProperties(m_prob_of_death_at_birth->value(),
                            m_prob_of_death_at_upper->value(),
                            m_prime_age_start_sb->value(),
                            m_prime_age_end_sb->value(),
                            m_upper_age_sb->value());
}

void AgeingPropertiesWidget::setProperties(const AgeingProperties * p_properties)
{
    m_prob_of_death_at_birth->setValue(p_properties->probability_of_death_at_birth);
    m_prob_of_death_at_upper->setValue(p_properties->probability_of_death_at_upper);
    m_prime_age_start_sb->setValue(p_properties->prime_age_start);
    m_prime_age_end_sb->setValue(p_properties->prime_age_end);
    m_upper_age_sb->setValue(p_properties->upper_age);
}

void AgeingPropertiesWidget::clear()
{
    m_prob_of_death_at_birth->clear();
    m_prob_of_death_at_upper->clear();

    m_prime_age_start_sb->clear();
    m_prime_age_end_sb->clear();
    m_upper_age_sb->clear();
}

void AgeingPropertiesWidget::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;

     int row(0);

     // Prob death at birth
     {
         QHBoxLayout * h_layout = new QHBoxLayout();
         h_layout->addWidget(new QLabel("Probability of death at birth: "),0,Qt::AlignLeft);
         h_layout->addWidget(m_prob_of_death_at_birth,0,Qt::AlignRight);
         h_layout->addWidget(new QLabel("%"),0,Qt::AlignRight);
         main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
     }
      // Prob death at upper
     {
         QHBoxLayout * h_layout = new QHBoxLayout();
         h_layout->addWidget(new QLabel("Probability of death at upper: "),0,Qt::AlignLeft);
         h_layout->addWidget(m_prob_of_death_at_upper,0,Qt::AlignRight);
         h_layout->addWidget(new QLabel("%"),0,Qt::AlignRight);
         main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
     }
     // Prime age start
     {
         QHBoxLayout * h_layout = new QHBoxLayout();
         h_layout->addWidget(new QLabel("Start of prime age: "),0,Qt::AlignLeft);
         h_layout->addWidget(m_prime_age_start_sb,0,Qt::AlignRight);
         h_layout->addWidget(new QLabel("months"),0,Qt::AlignRight);
         main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
     }
      // Prime age end
     {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("End of prime age: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_prime_age_end_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("months"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
     }
     // Upper age
     {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Upper age: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_upper_age_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("months"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
     }
     setLayout(main_layout);
}

/**********************************
 * ILLUMINATION PROPERTIES WIDGET *
 **********************************/
IlluminationPropertiesWidget::IlluminationPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
{
    m_shade_tolerance_sb = new MyPercentageSpinBox();
    m_sensitivity_sb = new MySensitivitySpinBox;

    init_layout();
}

IlluminationPropertiesWidget::~IlluminationPropertiesWidget()
{
}

IlluminationProperties * IlluminationPropertiesWidget::getProperties()
{
    return new IlluminationProperties(m_shade_tolerance_sb->value(),
                                  m_sensitivity_sb->value());
}

void IlluminationPropertiesWidget::setProperties(const IlluminationProperties * p_properties)
{
    m_shade_tolerance_sb->setValue(p_properties->shade_tolerance);
    m_sensitivity_sb->setValue(p_properties->sensitivity);
}

void IlluminationPropertiesWidget::clear()
{
    m_shade_tolerance_sb->clear();
    m_sensitivity_sb->clear();
}

void IlluminationPropertiesWidget::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;

    int row(0);

    // Start of negative impact
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Shade tolerance: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_shade_tolerance_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("%"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    // Sensitivity
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Sensitivity: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_sensitivity_sb,0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }

    setLayout(main_layout);
}

/***********************************
 * SOIL HUMIDITY PROPERTIES WIDGET *
 ***********************************/
SoilHumidityPropertiesWidget::SoilHumidityPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
{
    m_prime_soil_humidity_percentage_start_sb = new MyPercentageSpinBox();
    m_prime_soil_humidity_percentage_end_sb = new MyPercentageSpinBox();
    m_sensitivity_sb = new MySensitivitySpinBox;

    init_layout();
}

SoilHumidityPropertiesWidget::~SoilHumidityPropertiesWidget()
{

}

SoilHumidityProperties * SoilHumidityPropertiesWidget::getProperties()
{
    return new SoilHumidityProperties(m_prime_soil_humidity_percentage_start_sb->value(),
                                  m_prime_soil_humidity_percentage_end_sb->value(),
                                  m_sensitivity_sb->value());
}

void SoilHumidityPropertiesWidget::setProperties(const SoilHumidityProperties * p_properties)
{
    m_prime_soil_humidity_percentage_start_sb->setValue(p_properties->soil_humidity_percentage_prime_start);
    m_prime_soil_humidity_percentage_end_sb->setValue(p_properties->soil_humidity_percentage_prime_end);
    m_sensitivity_sb->setValue(p_properties->sensitivity);
}

void SoilHumidityPropertiesWidget::clear()
{
    m_prime_soil_humidity_percentage_start_sb->clear();
    m_prime_soil_humidity_percentage_end_sb->clear();
    m_sensitivity_sb->clear();
}

void SoilHumidityPropertiesWidget::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;

    int row(0);

    // Start of prime
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Start of prime humidity: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_prime_soil_humidity_percentage_start_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("%"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    // End of prime
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("End of prime humidity: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_prime_soil_humidity_percentage_end_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("%"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    // Sensitivity
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Sensitivity: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_sensitivity_sb,0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    setLayout(main_layout);
}
