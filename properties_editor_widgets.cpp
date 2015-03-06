#include "properties_editor_widgets.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>

#include <iostream>

/****************************
 * GROWTH PROPERTIES WIDGET *
 ****************************/
GrowthPropertiesWidget::GrowthPropertiesWidget(QWidget* parent, Qt::WindowFlags f):
    QWidget(parent, f)
{
    // Init UI elements
    m_max_height_dsb = new MyCustomFloatingPointSizeSpinBox();
    m_max_canopy_width_dsb = new MyCustomFloatingPointSizeSpinBox();
    m_max_root_size_dsb = new MyCustomFloatingPointSizeSpinBox();
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
    return new GrowthProperties (m_max_height_dsb->value(),
                             m_max_root_size_dsb->value(),
                             m_max_canopy_width_dsb->value());
}

void GrowthPropertiesWidget::setProperties(const GrowthProperties * p_properties)
{
    m_max_height_dsb->setValue(p_properties->max_height);
    m_max_canopy_width_dsb->setValue(p_properties->max_canopy_width);
    m_max_root_size_dsb->setValue(p_properties->max_root_size);
}

void GrowthPropertiesWidget::clear()
{
    m_max_height_dsb->clear();
    m_max_canopy_width_dsb->clear();
    m_max_root_size_dsb->clear();
}

void GrowthPropertiesWidget::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;

    int row(0);

    // Max monthly vertical growth
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Max height: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_height_dsb,0,Qt::AlignRight);
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
        h_layout->addWidget(new QLabel("Max canopy width: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_canopy_width_dsb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("Cm"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Max root size: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_root_size_dsb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("Cm"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }

    setLayout(main_layout);
}

void GrowthPropertiesWidget::set_canopy_width_input_enabled(bool checked)
{
    m_max_canopy_width_dsb->setEnabled(checked);

    if(!checked)
        m_max_canopy_width_dsb->setValue(.0);
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
    m_min_illumination_sb = new My24HourSpinBox;
    m_max_illumination_sb = new My24HourSpinBox;

    m_underexposure_sensitivity_sb = new MySensitivitySpinBox;
    m_overexposure_sensitivity_sb = new MySensitivitySpinBox;

    init_layout();
}

IlluminationPropertiesWidget::~IlluminationPropertiesWidget()
{
}

IlluminationProperties * IlluminationPropertiesWidget::getProperties()
{
    return new IlluminationProperties(m_min_illumination_sb->value(),
                                      m_max_illumination_sb->value(),
                                      m_underexposure_sensitivity_sb->value(),
                                      m_overexposure_sensitivity_sb->value());
}

void IlluminationPropertiesWidget::setProperties(const IlluminationProperties * p_properties)
{
    m_min_illumination_sb->setValue(p_properties->daily_illumination_hours_prime_start);
    m_max_illumination_sb->setValue(p_properties->daily_illumination_hours_prime_end);

    m_underexposure_sensitivity_sb->setValue(p_properties->underexposure_sensitivity);
    m_overexposure_sensitivity_sb->setValue(p_properties->overexposure_sensitivity);
}

void IlluminationPropertiesWidget::clear()
{
    m_min_illumination_sb->clear();
    m_max_illumination_sb->clear();
    m_underexposure_sensitivity_sb->clear();
    m_overexposure_sensitivity_sb->clear();
}

void IlluminationPropertiesWidget::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;

    int row(0);

    // Illumination configuration
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Start of prime illumination: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_min_illumination_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel(" hours"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("End of prime illumination: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_illumination_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel(" hours"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }

    // Sensitivity configuration
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Underexposure sensitivity: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_underexposure_sensitivity_sb,0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Overexposure sensitivity: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_overexposure_sensitivity_sb,0,Qt::AlignRight);
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
    m_drought_sensitivity = new MySensitivitySpinBox;
    m_flooding_sensitivity = new MySensitivitySpinBox;

    init_layout();
}

SoilHumidityPropertiesWidget::~SoilHumidityPropertiesWidget()
{

}

SoilHumidityProperties * SoilHumidityPropertiesWidget::getProperties()
{
    return new SoilHumidityProperties(m_prime_soil_humidity_percentage_start_sb->value(),
                                  m_prime_soil_humidity_percentage_end_sb->value(),
                                  m_drought_sensitivity->value(),
                                  m_flooding_sensitivity->value());
}

void SoilHumidityPropertiesWidget::setProperties(const SoilHumidityProperties * p_properties)
{
    m_prime_soil_humidity_percentage_start_sb->setValue(p_properties->soil_humidity_percentage_prime_start);
    m_prime_soil_humidity_percentage_end_sb->setValue(p_properties->soil_humidity_percentage_prime_end);
    m_drought_sensitivity->setValue(p_properties->drought_sensitivity);
    m_flooding_sensitivity->setValue(p_properties->flooding_sensitivity);
}

void SoilHumidityPropertiesWidget::clear()
{
    m_prime_soil_humidity_percentage_start_sb->clear();
    m_prime_soil_humidity_percentage_end_sb->clear();
    m_drought_sensitivity->clear();
    m_flooding_sensitivity->clear();
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
    // Drought Sensitivity
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Drought sensitivity: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_drought_sensitivity,0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    // Flood Sensitivity
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Flooding sensitivity: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_flooding_sensitivity,0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    setLayout(main_layout);
}

/*****************************
 * SEEDING PROPERTIES WIDGET *
 *****************************/
#define MIN_SEEDING_DISTANCE 1
#define MAX_SEEDING_DISTANCE 100

#define MIN_SEEDING_INTERVAL 6
#define MAX_SEEDING_INTERVAL 24

#define MIN_SEEDS 0
#define MAX_SEEDS 200

SeedingPropertiesWidget::SeedingPropertiesWidget(QWidget* parent, Qt::WindowFlags f )
{
    m_max_seeding_distance_sb = new QSpinBox();
    m_max_seeding_distance_sb->setRange(MIN_SEEDING_DISTANCE, MAX_SEEDING_DISTANCE);

    m_seeding_interval_sb = new QSpinBox();
    m_seeding_interval_sb->setRange(MIN_SEEDING_INTERVAL, MAX_SEEDING_INTERVAL);
    m_seeding_interval_sb->setSingleStep(6);

    m_max_seed_count_sb = new QSpinBox();
    m_max_seed_count_sb->setRange(MIN_SEEDS, MAX_SEEDS);

    init_layout();
}

SeedingPropertiesWidget::~SeedingPropertiesWidget()
{

}

SeedingProperties* SeedingPropertiesWidget::getProperties()
{
    return new SeedingProperties(m_max_seeding_distance_sb->value(),
                                  m_seeding_interval_sb->value(),
                                  m_max_seed_count_sb->value());
}

void SeedingPropertiesWidget::setProperties(const SeedingProperties * p_properties)
{
    m_max_seeding_distance_sb->setValue(p_properties->max_seed_distance);
    m_seeding_interval_sb->setValue(p_properties->seeding_interval);
    m_max_seed_count_sb->setValue(p_properties->max_seeds);
}

void SeedingPropertiesWidget::clear()
{

}

void SeedingPropertiesWidget::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;

    int row(0);

    // Start of prime
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Max Seeding Distance:: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_seeding_distance_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel(" Meters"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    // End of prime
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Seeding interval: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_seeding_interval_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel(" months"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    // Sensitivity
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Max seed count: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_seed_count_sb,0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    setLayout(main_layout);
}
