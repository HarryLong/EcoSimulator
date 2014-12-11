#include "plant_db_editor.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QLineEdit>

const static char * EDIT_BTN_TEXT = "Edit";
const static char * SAVE_EDITS_BTN_TEXT = "Save";
const static char * CANCEL_BTN_TEXT = "Cancel";
const static char * NEW_BTN_TEXT = "New";
const static char * CONFIRM_BTN_TEXT = "Commit";
const static char * REMOVE_BTN_TEXT = "Delete";

/****************************
 * PROPERTY WIDGETS WRAPPER *
 ****************************/
PropertyWidgetsWrapper::PropertyWidgetsWrapper() :
    m_specie_name_widget(new QLineEdit),
    growth_property_widget(new GrowthPropertiesWidget),
    ageing_properties_widget(new AgeingPropertiesWidget),
    illumination_properties_widget(new IlluminationPropertiesWidget),
    soil_humidity_properties_widget(new SoilHumidityPropertiesWidget)
{
    init_layout();
}

void PropertyWidgetsWrapper::setProperties(const SpecieProperties * p_plant_data)
{
    m_specie_name_widget->setText(p_plant_data->specie_name);
    ageing_properties_widget->setProperties(p_plant_data->ageing_properties);
    growth_property_widget->setProperties(p_plant_data->growth_properties);
    illumination_properties_widget->setProperties(p_plant_data->illumination_properties);
    soil_humidity_properties_widget->setProperties(p_plant_data->soil_humidiry_properties);
}

void PropertyWidgetsWrapper::setEnabled(bool p_enabled)
{
    m_specie_name_widget->setEnabled(p_enabled);
    ageing_properties_widget->setEnabled(p_enabled);
    growth_property_widget->setEnabled(p_enabled);
    illumination_properties_widget->setEnabled(p_enabled);
    soil_humidity_properties_widget->setEnabled(p_enabled);
}

void PropertyWidgetsWrapper::clear()
{
    m_specie_name_widget->clear();
    ageing_properties_widget->clear();
    growth_property_widget->clear();
    illumination_properties_widget->clear();
    soil_humidity_properties_widget->clear();
}

void PropertyWidgetsWrapper::init_layout()
{
    QFont title_font( "Arial", 16, QFont::Bold );

    QVBoxLayout * main_layout = new QVBoxLayout;

    QLabel * specie_name_title = new QLabel("Specie Name: ");
    specie_name_title->setFont(title_font);
    main_layout->addWidget(specie_name_title);
    main_layout->addWidget(m_specie_name_widget);

    // Growth properties
    QLabel * growth_title_lbl = new QLabel("Growth Properties");
    growth_title_lbl->setFont(title_font);
    main_layout->addWidget(growth_title_lbl);
    main_layout->addWidget(growth_property_widget);

    // Ageing properties
    QLabel * ageing_title_lbl = new QLabel("Ageing Properties");
    ageing_title_lbl->setFont(title_font);
    main_layout->addWidget(ageing_title_lbl);
    main_layout->addWidget(ageing_properties_widget);

    // Illumination properties
    QLabel * illumination_title_lbl = new QLabel("Illumination Properties");
    illumination_title_lbl->setFont(title_font);
    main_layout->addWidget(illumination_title_lbl);
    main_layout->addWidget(illumination_properties_widget);

    // Soil humidity properties
    QLabel * soil_humidity_title_lbl = new QLabel("Soil Humidity Properties");
    soil_humidity_title_lbl->setFont(title_font);
    main_layout->addWidget(soil_humidity_title_lbl);
    main_layout->addWidget(soil_humidity_properties_widget);

    setLayout(main_layout);
}


SpecieProperties * PropertyWidgetsWrapper::toProperties()
{
    return new SpecieProperties(m_specie_name_widget->text(),
                           ageing_properties_widget->getProperties(),
                           growth_property_widget->getProperties(),
                           illumination_properties_widget->getProperties(),
                           soil_humidity_properties_widget->getProperties());
}


/*******************
 * PLANT DB EDITOR *
 *******************/
PlantDBEditor::PlantDBEditor(QWidget *parent, Qt::WindowFlags f) :
  m_plant_db(),
  m_available_plants_list ( new QListWidget() ),
  m_plant_data(m_plant_db.getAllPlantData()),
  m_property_widgets_wrapper(new PropertyWidgetsWrapper),
  m_edit_save_edits_btn( new QPushButton()),
  m_cancel_btn( new QPushButton(CANCEL_BTN_TEXT)),
  m_new_confirm_btn( new QPushButton()),
  m_remove_btn(new QPushButton(REMOVE_BTN_TEXT))
{
    m_available_plants_list->setSelectionMode(QAbstractItemView::SingleSelection);
    setWindowTitle("Plant DB");
    init_layout();
    init_content();
    init_signals();

    // Initial conditions
    set_mode(READ_ONLY);
}

PlantDBEditor::~PlantDBEditor()
{
    for(auto it (m_plant_data.begin()); it != m_plant_data.end(); it++)
        delete it->second;
}

void PlantDBEditor::init_signals()
{
    connect(m_available_plants_list, SIGNAL(itemSelectionChanged()), this, SLOT(refresh_property_widgets()));
    connect(m_edit_save_edits_btn, SIGNAL(clicked()), this, SLOT(edit_btn_clicked()));
    connect(m_cancel_btn, SIGNAL(clicked()), this, SLOT(cancel_btn_clicked()));
    connect(m_new_confirm_btn, SIGNAL(clicked()), this, SLOT(new_btn_clicked()));
    connect(m_remove_btn, SIGNAL(clicked()), this, SLOT(remove_btn_clicked()));
}

void PlantDBEditor::edit_btn_clicked()
{
    if(m_current_mode == EDITING)
    {
        commit(true);
        set_mode(READ_ONLY);
    }
    else // In read only
    {
        set_mode(EDITING);
    }
}

void PlantDBEditor::cancel_btn_clicked()
{
    set_mode(READ_ONLY);
}

void PlantDBEditor::new_btn_clicked()
{
    if(m_current_mode == ADDING)
    {
        commit(false);
        set_mode(READ_ONLY);
    }
    else // READ ONLY
    {
        QListWidgetItem * selected_list_item(get_current_selected_specie());
        if(selected_list_item != NULL) // unselect current item
            m_available_plants_list->setItemSelected(selected_list_item, false);

        set_mode(ADDING);
    }
}

void PlantDBEditor::remove_btn_clicked()
{
    auto it (m_plant_data.find(get_current_selected_specie()->text()));

    m_plant_db.removePlant(it->second->specie_id); // remove from the db
    m_plant_data.erase(it); // Remove from current data
    delete m_available_plants_list->takeItem(m_available_plants_list->currentRow()); // Remove current item from list
}

void PlantDBEditor::set_mode(Mode p_mode)
{
    refresh_property_widgets();

    switch (p_mode)
    {
    case READ_ONLY:
        m_current_mode = READ_ONLY;
        set_property_widgets_enabled(false);
        // Edit button
        m_edit_save_edits_btn->setText(EDIT_BTN_TEXT);
        m_edit_save_edits_btn->setEnabled(m_available_plants_list->selectedItems().size() > 0); // Enabled only if an item is currently selected
        // Cancel button
        m_cancel_btn->setEnabled(false);
        // New/Confirm button
        m_new_confirm_btn->setText(NEW_BTN_TEXT);
        m_new_confirm_btn->setEnabled(true);
        break;
    case EDITING:
        m_current_mode = EDITING;
        set_property_widgets_enabled(true);
        // Edit button
        m_edit_save_edits_btn->setText(SAVE_EDITS_BTN_TEXT);
        // Cancel button
        m_cancel_btn->setEnabled(true);
        // New/Confirm button
        m_new_confirm_btn->setText(NEW_BTN_TEXT);
        m_new_confirm_btn->setEnabled(false);
        break;
    case ADDING:
        m_current_mode = ADDING;
        m_property_widgets_wrapper->clear();
        set_property_widgets_enabled(true);
        // Edit button
        m_edit_save_edits_btn->setText(EDIT_BTN_TEXT);
        m_edit_save_edits_btn->setEnabled(false);
        // Cancel button
        m_cancel_btn->setEnabled(true);
        // New/Confirm button
        m_new_confirm_btn->setText(CONFIRM_BTN_TEXT);
        m_new_confirm_btn->setEnabled(true);
        break;
    }
}

void PlantDBEditor::commit(bool update)
{
    // TODO: Update the internal data map
    SpecieProperties * properties(m_property_widgets_wrapper->toProperties());

    if(update)
    {
        // Retrieve the id
        auto it (m_plant_data.find(get_current_selected_specie()->text()));
        properties->specie_id = it->second->specie_id;
        m_plant_data.erase(it); // Remove from current data (it will be re-inserted later)
        delete m_available_plants_list->takeItem(m_available_plants_list->currentRow()); // Remove current item from list

        m_plant_db.updatePlantData(properties);
    }
    else
    {
        m_plant_db.insertNewPlantData(properties);
    }

    m_plant_data.insert(std::pair<QString, const SpecieProperties *>(properties->specie_name, const_cast<const SpecieProperties*>(properties)));
    m_available_plants_list->addItem(new QListWidgetItem(properties->specie_name));
}

void PlantDBEditor::refresh_property_widgets()
{
    QListWidgetItem* selected_list_item(get_current_selected_specie());

    if(selected_list_item == NULL)
    {
        m_edit_save_edits_btn->setEnabled(false);
        m_remove_btn->setEnabled(false);
        m_property_widgets_wrapper->clear();
    }
    else
    {
        const SpecieProperties* plant_data(m_plant_data.find(get_current_selected_specie()->text())->second);
        m_property_widgets_wrapper->setProperties(plant_data);
        m_edit_save_edits_btn->setEnabled(true);
        m_remove_btn->setEnabled(true);
    }
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
        m_available_plants_list->addItem(new QListWidgetItem(it->first));
    }
}

QListWidgetItem* PlantDBEditor::get_current_selected_specie()
{
    if(m_available_plants_list->selectedItems().size() > 0)
        return m_available_plants_list->currentItem();

    return NULL;
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

    QScrollArea* property_widgets_scroll_area = new QScrollArea;
    property_widgets_scroll_area->setWidget(m_property_widgets_wrapper);

    // main layout
    QHBoxLayout * main_h_layout = new QHBoxLayout;
    {
        main_h_layout->addLayout(plants_v_layout, 0);
        main_h_layout->addWidget(property_widgets_scroll_area, 1);
    }

    // The buttons
    QHBoxLayout * buttons_h_layout = new QHBoxLayout;
    {
        buttons_h_layout->addWidget(m_cancel_btn, 0, Qt::AlignCenter);
        buttons_h_layout->addWidget(m_edit_save_edits_btn, 0, Qt::AlignCenter);
        buttons_h_layout->addWidget(m_new_confirm_btn, 0, Qt::AlignCenter);
        buttons_h_layout->addWidget(m_remove_btn, 0, Qt::AlignCenter);
    }

    QVBoxLayout * main_layout = new QVBoxLayout;

    main_layout->addLayout(main_h_layout, 1);
    main_layout->addLayout(buttons_h_layout, 0);

    setLayout(main_layout);
}

void PlantDBEditor::set_property_widgets_enabled(bool enabled)
{
    m_property_widgets_wrapper->setEnabled(enabled);
}
