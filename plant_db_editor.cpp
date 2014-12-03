#include "plant_db_editor.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

PlantDBEditor::PlantDBEditor(QWidget *parent, Qt::WindowFlags f) :
  m_plant_db(),
  m_available_plants_list ( new QListWidget() ),
  m_plant_data(m_plant_db.getAllPlantData()),
  m_specie_name_te(new QTextEdit),
  m_property_widgets(
      new GrowthPropertiesWidget(this),
      new AgeingPropertiesWidget(this),
      new IlluminationPropertiesWidget(this),
      new SoilHumidityPropertiesWidget(this)),
  m_edit_confirm_btn( new QPushButton("Edit")),
  m_cancel_btn( new QPushButton("Cancel")),
  m_new_btn( new QPushButton("New") ),
  m_edit_mode_on( false )
{
    init_layout();
    init_content();
    init_signals();
    set_widgets_enabled(false);
}

PlantDBEditor::~PlantDBEditor()
{
}

void PlantDBEditor::init_signals()
{
    connect(m_available_plants_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(update_widgets(QListWidgetItem*)));
    connect(m_edit_confirm_btn, SIGNAL(clicked()), this, SLOT(edit_btn_clicked()));
    connect(m_cancel_btn, SIGNAL(clicked()), this, SLOT(cancel_btn_clicked()));
    connect(m_new_btn, SIGNAL(clicked()), this, SLOT(new_btn_clicked()));
}

void PlantDBEditor::edit_btn_clicked()
{
    if(m_edit_mode_on)
    {
        m_edit_confirm_btn->setText("Edit");
    }
    else
    {
        m_edit_confirm_btn->setText("Confirm");

    }
}

void PlantDBEditor::cancel_btn_clicked()
{

}

void PlantDBEditor::new_btn_clicked()
{

}

void PlantDBEditor::update_widgets(QListWidgetItem* p_selected_list_item)
{

}

QSize PlantDBEditor::minimumSizeHint() const
{
    return QSize(PLANT_DB_EDITOR_DIALOG_WIDTH, PLANT_DB_EDITOR_DIALOG_HEIGHT);
}

QSize PlantDBEditor::sizeHint() const
{
    return minimumSizeHint();
}

void PlantDBEditor::init_content()
{
    // The list of available species
    for(auto it (m_plant_data.begin()); it != m_plant_data.end(); it++)
    {
        m_available_plants_list->addItem(new QListWidgetItem(QString(it->first.c_str())));
    }
}

void PlantDBEditor::init_layout()
{
    QFont title_font( "Arial", 16, QFont::Bold );

    // The plant selector
    QVBoxLayout * plants_v_layout = new QVBoxLayout();
    {
        QLabel * plants_title_lbl = new QLabel("Species");
        plants_title_lbl->setFont(title_font);
        plants_v_layout->addWidget(plants_title_lbl);
        plants_v_layout->addWidget(m_available_plants_list);
    }

    // The property widgets
    QVBoxLayout * widgets_v_layout = new QVBoxLayout();
    {
        // The specie name TextEdit
        QHBoxLayout * specie_name_layout = new QHBoxLayout;
        {
            QFont custom_title_font( "Arial", 12, QFont::Bold );
            QLabel * specie_name_title = new QLabel("Specie Name: ");
            specie_name_title->setFont(custom_title_font);
            specie_name_layout->addWidget(specie_name_title, 1, Qt::AlignLeft);
            specie_name_layout->addWidget(m_specie_name_te, 0, Qt::AlignRight);
        }
        widgets_v_layout->addLayout(specie_name_layout);

        // Growth properties
        QLabel * growth_title_lbl = new QLabel("Growth Properties");
        growth_title_lbl->setFont(title_font);
        widgets_v_layout->addWidget(growth_title_lbl);
        widgets_v_layout->addWidget(m_property_widgets.growth_property_widget);

        // Ageing properties
        QLabel * ageing_title_lbl = new QLabel("Ageing Properties");
        ageing_title_lbl->setFont(title_font);
        widgets_v_layout->addWidget(ageing_title_lbl);
        widgets_v_layout->addWidget(m_property_widgets.ageing_properties_widget);

        // Illumination properties
        QLabel * illumination_title_lbl = new QLabel("Illumination Properties");
        illumination_title_lbl->setFont(title_font);
        widgets_v_layout->addWidget(illumination_title_lbl);
        widgets_v_layout->addWidget(m_property_widgets.illumination_properties_widget);

        // Soil humidity properties
        QLabel * soil_humidity_title_lbl = new QLabel("Soil Humidity Properties");
        soil_humidity_title_lbl->setFont(title_font);
        widgets_v_layout->addWidget(soil_humidity_title_lbl);
        widgets_v_layout->addWidget(m_property_widgets.soil_humidity_properties_widget);
    }

    QGridLayout * main_layout = new QGridLayout;
    main_layout->addLayout(plants_v_layout,0,0,1,1,Qt::AlignTop);
    main_layout->addLayout(widgets_v_layout,0,1,1,5,Qt::AlignTop);

    setLayout(main_layout);
}

void PlantDBEditor::set_widgets_enabled(bool enabled)
{
    m_specie_name_te->setEnabled(enabled);
    m_property_widgets.growth_property_widget->setEnabled(enabled);
    m_property_widgets.ageing_properties_widget->setEnabled(enabled);
    m_property_widgets.illumination_properties_widget->setEnabled(enabled);
    m_property_widgets.soil_humidity_properties_widget->setEnabled(enabled);
}

/****************************
 * GROWTH PROPERTIES WIDGET *
 ****************************/
GrowthPropertiesWidget::GrowthPropertiesWidget(QWidget* parent, Qt::WindowFlags f):
    QWidget(parent, f)
{
    // Init UI elements
    m_max_vertical_growth_dsb = new MyCustomDoubleSpinBox();
    m_max_canopy_growth_dsb = new MyCustomDoubleSpinBox();
    m_max_roots_growth_dsb = new MyCustomDoubleSpinBox();;

    init_layout();
}

GrowthPropertiesWidget::~GrowthPropertiesWidget()
{
}

GrowthProperties GrowthPropertiesWidget::getProperties()
{
}

void GrowthPropertiesWidget::setProperties(GrowthProperties p_properties)
{

}

void GrowthPropertiesWidget::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;

    int row(0);

    // Max monthly vertical growth
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Max monthly vertical growth: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_vertical_growth_dsb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("Cm"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    // Max monthly canopy growth
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Max monthly canopy growth: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_canopy_growth_dsb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("Cm"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Max monthly roots growth: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_max_roots_growth_dsb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("Cm"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }

    setLayout(main_layout);
}

/****************************
 * AGEING PROPERTIES WIDGET *
 ****************************/
AgeingPropertiesWidget::AgeingPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
{
    // Init UI elements
    m_prob_of_death_at_birth = new MyPercentageSpinBox();
    m_prob_of_death_at_upper = new MyPercentageSpinBox();

    m_prime_age_start_sb = new QSpinBox();
    m_prime_age_end_sb = new QSpinBox();
    m_upper_age_sb = new QSpinBox();

    init_layout();
}

AgeingPropertiesWidget::~AgeingPropertiesWidget()
{

}

AgeingProperties AgeingPropertiesWidget::getProperties()
{

}

void AgeingPropertiesWidget::setProperties(AgeingProperties p_properties)
{

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
    m_shadowed_percentage_start_of_negative_impact_sb = new MyPercentageSpinBox();
    m_prob_of_death_in_complete_shade_sb = new MyPercentageSpinBox();

    init_layout();
}

IlluminationPropertiesWidget::~IlluminationPropertiesWidget()
{
}

IlluminationProperties IlluminationPropertiesWidget::getProperties()
{
}

void IlluminationPropertiesWidget::setProperties(IlluminationProperties p_properties)
{
}

void IlluminationPropertiesWidget::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;

    int row(0);

    // Start of negative impact
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Start of negative impact shading: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_shadowed_percentage_start_of_negative_impact_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("%"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }
    // Prob death when completely shaded
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("Probability of death when completely shaded: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_prob_of_death_in_complete_shade_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("%"),0,Qt::AlignRight);
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

    init_layout();
}

SoilHumidityPropertiesWidget::~SoilHumidityPropertiesWidget()
{

}

SoilHumidityProperties SoilHumidityPropertiesWidget::getProperties()
{

}

void SoilHumidityPropertiesWidget::setProperties(SoilHumidityProperties p_properties)
{

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
    // Prob death when completely shaded
    {
        QHBoxLayout * h_layout = new QHBoxLayout();
        h_layout->addWidget(new QLabel("End of prime humidity: "),0,Qt::AlignLeft);
        h_layout->addWidget(m_prime_soil_humidity_percentage_end_sb,0,Qt::AlignRight);
        h_layout->addWidget(new QLabel("%"),0,Qt::AlignRight);
        main_layout->addLayout(h_layout,row++,0,1,1,Qt::AlignLeft);
    }

    setLayout(main_layout);
}


