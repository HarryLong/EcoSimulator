#include "monthly_edit_dlg.h"
#include <QBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QIntValidator>
#include <QResizeEvent>
#include <QLabel>
#include <QPushButton>

const QString MonthlySlider::_STYLESHEET =
        "QSlider::groove:vertical {\
            background: transparent;\
        }\
        QSlider::handle:vertical {\
            background: black;\
            border: 4px solid #000000;\
            width: 1px;\
            margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */\
            border-radius: 3px;\
        }";

MonthlySlider::MonthlySlider(int min, int max, int dft, QWidget * parent) : QSlider(Qt::Orientation::Vertical, parent), m_default(dft)
{
    setRange(min, max);
    setStyleSheet(MonthlySlider::_STYLESHEET);
    reset();
}

MonthlySlider::~MonthlySlider()
{

}

QPoint MonthlySlider::getHandlePosition()
{
    int range(maximum()-minimum());
    return mapToParent(QPoint(width()/2.0f, height() - ((((float)value()-minimum())/range) * height())));
}

void MonthlySlider::reset()
{
    setValue(m_default);
}

//-------------------------------------------------------------------------------------
MonthlyLineEdit::MonthlyLineEdit(int min, int max, int dft, QWidget * parent) :
    QLineEdit(parent),
    m_validator(new QIntValidator(min, max, parent)),
    m_default(dft)
{
    setValidator(m_validator);
    reset();
}

MonthlyLineEdit::~MonthlyLineEdit()
{
    delete m_validator;
}

void MonthlyLineEdit::focusOutEvent( QFocusEvent * event )
{
    QLineEdit::focusOutEvent(event);
    if(text().isEmpty())
        setText(QString::number(0));

    emit valueChanged(text().toInt());
}

void MonthlyLineEdit::setValue(int value)
{
    setText(QString::number(value));
}

void MonthlyLineEdit::reset()
{
    setText(QString::number(m_default));
}

//--------------------------------------------------------------------------------------
const QFont SingleMonthWidget::_LABEL_FONT("Times", 15, QFont::Weight::Bold);
SingleMonthWidget::SingleMonthWidget(QString label_txt, int min, int max, int dft, QWidget * parent) :
    QWidget(parent),
    m_slider(new MonthlySlider(min, max, dft, this)),
    m_le(new MonthlyLineEdit(min, max, dft, this)),
    m_label_txt(label_txt)
{
    connect(m_le, SIGNAL(valueChanged(int)), m_slider, SLOT(setValue(int)));
    connect(m_le, SIGNAL(valueChanged(int)), this, SLOT(emit_value_changed(int)));

    connect(m_slider, SIGNAL(valueChanged(int)), m_le, SLOT(setValue(int)));
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(emit_value_changed(int)));

    init_layout();
}

SingleMonthWidget::~SingleMonthWidget()
{

}

void SingleMonthWidget::reset()
{
    m_slider->reset();
    m_le->reset();
}

QPoint SingleMonthWidget::getHandlePosition()
{
    return mapToParent(m_slider->getHandlePosition());
}

void SingleMonthWidget::emit_value_changed(int value)
{
    emit valueChanged(value);
}

void SingleMonthWidget::init_layout()
{
    QBoxLayout * layout = new QVBoxLayout;

    layout->addWidget(m_slider, 8, Qt::AlignHCenter);
    layout->addWidget(m_le, 2, Qt::AlignHCenter);

    QLabel * lbl = new QLabel(m_label_txt);
    lbl->setFont(SingleMonthWidget::_LABEL_FONT);

    layout->addWidget(lbl, 2, Qt::AlignCenter);

    setLayout(layout);
}

int SingleMonthWidget::value()
{
    return m_slider->value();
}

//-------------------------------------------------------------------------------------
const QString LineEditQLabel::_STYLESHEET =
"QLineEdit {\
border: 0px solid gray;\
border-radius: 00px;\
padding: 0 0px;\
background: transparent;}";
const QFont LineEditQLabel::_LABEL_FONT("Times", 12, QFont::Weight::Bold);
LineEditQLabel::LineEditQLabel(QString text) :
    QLineEdit(text)
{
    setStyleSheet(LineEditQLabel::_STYLESHEET);
    setAlignment(Qt::AlignRight);
    setFont(LineEditQLabel::_LABEL_FONT);
}

SingleMonthHeaderWidget::SingleMonthHeaderWidget(QString heading, QWidget * parent)
{
    init_layout(heading);
}

SingleMonthHeaderWidget::~SingleMonthHeaderWidget()
{

}

void SingleMonthHeaderWidget::init_layout( QString heading )
{
    QBoxLayout * layout = new QVBoxLayout;

    // Dummy space for Slider in main widgets
    layout->addWidget(new QLabel("", this), 8);

    // Rainfall Heading
    {
        QLineEdit * le = new LineEditQLabel(heading);
        le->setReadOnly(true);
        layout->addWidget(le, 2);
    }

    // Dummy space for the month label in main widgets
    layout->addWidget(new QLabel(" ", this), 2);

    setLayout(layout);
}

//--------------------------------------------------------------------------------------
const std::map<int,QString> MonthlyEditDialog::_NUM_TO_MONTH = MonthlyEditDialog::create_month_map();
MonthlyEditDialog::MonthlyEditDialog(int min, int max, int dft, QString heading, QWidget * parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    m_header_widget(new SingleMonthHeaderWidget(heading, this))
{
    m_graph_drawer_pen.setStyle(Qt::SolidLine);
    m_graph_drawer_pen.setWidth(3);
    m_graph_drawer_pen.setBrush(Qt::blue);
    m_graph_drawer_pen.setCapStyle(Qt::RoundCap);
    m_graph_drawer_pen.setJoinStyle(Qt::RoundJoin);

    for(int i = 0; i < 12; i++)
    {
        SingleMonthWidget * w = new SingleMonthWidget(_NUM_TO_MONTH.find(i+1)->second, min, max, dft, this);
        connect(w, SIGNAL(valueChanged(int)), this, SLOT(update()));
        m_monthly_widgets[i] = w;
    }

    init_layout();
}

MonthlyEditDialog::~MonthlyEditDialog()
{

}

void MonthlyEditDialog::reset()
{
    for(SingleMonthWidget * w : m_monthly_widgets)
        w->reset();

    update();
}

QSize MonthlyEditDialog::minimumSizeHint() const
{
    return QSize(1400,800);
}

QSize MonthlyEditDialog::sizeHint() const
{
    return minimumSizeHint();
}

int MonthlyEditDialog::value(int month)
{
    return m_monthly_widgets[month-1]->value();
}

std::vector<int> MonthlyEditDialog::values()
{
    std::vector<int> all_values;
    for(int i (0); i < 12; i++)
        all_values.push_back(m_monthly_widgets[i]->value());

    return all_values;
}

void MonthlyEditDialog::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.setPen(m_graph_drawer_pen);

    for(int i = 0; i < 11; i++)
    {
        QPoint p1(m_monthly_widgets[i]->getHandlePosition());
        QPoint p2(m_monthly_widgets[i+1]->getHandlePosition());

        painter.drawLine(p1, p2);
    }

    painter.end();

    QDialog::paintEvent(event);
}

void MonthlyEditDialog::init_layout()
{
    QVBoxLayout * main_layout = new QVBoxLayout;

    // Sliders
    {
        QHBoxLayout * layout = new QHBoxLayout;

        // Add header widget
        layout->addWidget(m_header_widget, 5);

        for(QWidget * s : m_monthly_widgets)
            layout->addWidget(s,2);

        main_layout->addLayout(layout,19);
    }

    setLayout(main_layout);
}

std::map<int, QString> MonthlyEditDialog::create_month_map()
{
    std::map<int, QString> ret;
    ret[1] = "Jan";
    ret[2] = "Feb";
    ret[3] = "Mar";
    ret[4] = "Apr";
    ret[5] = "May";
    ret[6] = "Jun";
    ret[7] = "Jul";
    ret[8] = "Aug";
    ret[9] = "Sep";
    ret[10] = "Oct";
    ret[11] = "Nov";
    ret[12] = "Dec";

    return ret;
}
