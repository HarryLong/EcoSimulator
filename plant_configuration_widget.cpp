#include "plant_configuration_widget.h"
#include <QStringListModel>
#include <QGridLayout>
#include <QPushButton>
#include "constants.h"
#include <algorithm>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

PlantConfigurationWidget::PlantConfigurationWidget ( int width, int height, QWidget * parent) :
    QWidget(parent),
    m_width(width),
    m_height(height),
    m_plant_factory(),
    m_species_added()
{
    m_available_plants_list = new QListWidget(this);
    m_added_plants_table = new QTableWidget(this);

    m_added_plants_table->setItemDelegateForColumn(AddedPlantsTableColumns::Count, new NumberDelegate);

    init_layout();
    init_signals();
    init_available_plants_list();
    init_added_plants_table();
}

PlantConfigurationWidget::~PlantConfigurationWidget()
{

}

void PlantConfigurationWidget::reset()
{
    m_species_added.clear();
    m_added_plants_table->clearContents();
}

void PlantConfigurationWidget::init_layout()
{
    QFont title_font( "Arial", 16, QFont::Bold );

    QGridLayout * main_layout = new QGridLayout;

    // List of available plants
    QVBoxLayout * available_plants_vlayout = new QVBoxLayout();
    {
        QLabel * title = new QLabel("Available Plants");
        title->setFont(title_font);
        QHBoxLayout * available_plants_list_hlayout = new QHBoxLayout();
        available_plants_list_hlayout->addWidget(m_available_plants_list,1);

        available_plants_vlayout->addWidget(title, 0, Qt::AlignLeft);
        available_plants_vlayout->addLayout(available_plants_list_hlayout, 1);
    }

    // Added table
    QVBoxLayout * added_plants_layout = new QVBoxLayout();
    {
        QLabel * title = new QLabel("Added Plants");
        title->setFont(title_font);
        QHBoxLayout * added_plants_table_hlayout = new QHBoxLayout();
        added_plants_table_hlayout->addWidget(m_added_plants_table,1);

        available_plants_vlayout->addWidget(title, 0, Qt::AlignLeft);
        available_plants_vlayout->addLayout(added_plants_table_hlayout, 1);
    }

    main_layout->addLayout(available_plants_vlayout,0,0,1,1,Qt::AlignCenter);
    main_layout->addLayout(added_plants_layout,2,0,1,1,Qt::AlignCenter);

    setLayout(main_layout);
}

void PlantConfigurationWidget::init_signals()
{
    // Add button
    connect(m_available_plants_list, SIGNAL( itemDoubleClicked ( QListWidgetItem * ) ),
            this, SLOT( add_specie( QListWidgetItem * )) );

    connect(m_added_plants_table, SIGNAL( cellChanged(int,int)), this, SLOT(cell_changed(int,int)));
}

void PlantConfigurationWidget::add_specie(QListWidgetItem * item)
{
    QString specie ( item->text() );
    if(m_species_added.find(specie) == m_species_added.end()) // Not in table, add it
    {
        int row_id(m_added_plants_table->rowCount());
        m_added_plants_table->insertRow(row_id);
        m_added_plants_table->setItem(row_id, AddedPlantsTableColumns::Specie,  generate_read_only_cell(specie));

        m_added_plants_table->setItem(row_id, AddedPlantsTableColumns::Count, new QTableWidgetItem(QString::number(1)));
        m_species_added.insert(specie);
    }
}

std::vector<Plant*> PlantConfigurationWidget::getPlantsToCreate()
{
    std::vector<Plant*> ret;

    for(int row ( 0 ); row < m_added_plants_table->rowCount(); row++)
    {
        QString specie ( m_added_plants_table->item(row,AddedPlantsTableColumns::Specie)->text() );
        int count ( m_added_plants_table->item(row,AddedPlantsTableColumns::Count)->text().toInt() );

        for(int plant_count ( 0 ); plant_count < count; plant_count++)
            ret.push_back(m_plant_factory.generate(specie, generate_random_position()));
    }

    return ret;
}

void PlantConfigurationWidget::init_available_plants_list()
{
    std::vector<QString> sorted_plant_species ( m_plant_factory.getAllSpecieNames() );
    std::sort(sorted_plant_species.begin(), sorted_plant_species.end());

    for(QString specie_name : sorted_plant_species)
        m_available_plants_list->addItem(new QListWidgetItem(specie_name));
}

void PlantConfigurationWidget::init_added_plants_table()
{
    m_added_plants_table->setColumnCount(AddedPlantsTableColumns::NumberOfColumns);
    m_added_plants_table->setHorizontalHeaderItem(AddedPlantsTableColumns::Specie,new QTableWidgetItem("Specie"));
    m_added_plants_table->setHorizontalHeaderItem(AddedPlantsTableColumns::Count,new QTableWidgetItem("Count"));
}

QSize PlantConfigurationWidget::minimumSizeHint() const
{
    return QSize(m_width, m_height);
}

QSize PlantConfigurationWidget::sizeHint() const
{
    return QSize(m_width, m_height);
}

QPoint PlantConfigurationWidget::generate_random_position()
{
    return QPoint(rand()%AREA_WIDTH_HEIGHT, rand()%AREA_WIDTH_HEIGHT);
}

QTableWidgetItem * PlantConfigurationWidget::generate_read_only_cell(QString p_cell_content)
{
    QTableWidgetItem * cell = new QTableWidgetItem(p_cell_content);
    cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
}

void PlantConfigurationWidget::cell_changed(int p_row, int p_columm)
{
    if(p_columm == AddedPlantsTableColumns::Count && m_added_plants_table->item(p_row,p_columm)->text().toInt() == 0)
    {
        m_species_added.erase(m_added_plants_table->item(p_row, AddedPlantsTableColumns::Specie)->text());
        m_added_plants_table->removeRow(p_row);
    }
}

/*******************
 * NUMBER DELEGATE *
 *******************/
#include <QSpinBox>
NumberDelegate::NumberDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget * NumberDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(100000);

    return editor;
}

void NumberDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void NumberDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

void NumberDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                          const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
