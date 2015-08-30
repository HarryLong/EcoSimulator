#ifndef MONTHLY_RAINFALL_EDIT_DLG
#define MONTHLY_RAINFALL_EDIT_DLG

#include <QDialog>
#include <vector>
#include <QSlider>
#include <QPen>
#include <QLineEdit>
#include <QFont>

class MonthlySlider : public QSlider
{
Q_OBJECT
public:
    MonthlySlider(int min, int max, int dft, QWidget * parent = 0);
    ~MonthlySlider();
    QPoint getHandlePosition();
    static const QString _STYLESHEET;

public slots:
    void reset();

private:
    int m_default;
};

//-------------------------------------------------------------------------------------

class QIntValidator;
class MonthlyLineEdit : public QLineEdit
{
Q_OBJECT
public:
    MonthlyLineEdit(int min, int max, int dft, QWidget * parent = 0);
    ~MonthlyLineEdit();

signals:
    void valueChanged(int);

public slots:
    void setValue(int);
    void reset();

protected:
    void focusOutEvent ( QFocusEvent * event ) override;
private:
    QIntValidator * m_validator;
    int m_default;
};

//-------------------------------------------------------------------------------------

class SingleMonthWidget : public QWidget
{
Q_OBJECT
public:
    SingleMonthWidget(QString lbl_txt, int min, int max, int dft, QWidget * parent = 0);
    ~SingleMonthWidget();
    QPoint getHandlePosition();

    int value();

public slots:
    void emit_value_changed(int);
    void reset();

signals:
    void valueChanged(int);

private:
    void init_layout();
    MonthlySlider * m_slider;
    MonthlyLineEdit * m_le;
    QString m_label_txt;

    static const QFont _LABEL_FONT;
};

//-------------------------------------------------------------------------------------

class LineEditQLabel : public QLineEdit
{
public:
    LineEditQLabel(QString text);

private:
    static const QString _STYLESHEET;
    static const QFont _LABEL_FONT;
};

class SingleMonthHeaderWidget : public QWidget
{
Q_OBJECT
public:
    SingleMonthHeaderWidget(QString heading, QWidget * parent = 0);
    ~SingleMonthHeaderWidget();

private:
    void init_layout(QString heading);
};

//-------------------------------------------------------------------------------------
class QPushButton;
class MonthlyEditDialog : public QDialog
{
Q_OBJECT
public:
    MonthlyEditDialog(int min, int max, int dft, QString heading, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~MonthlyEditDialog();
    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

    int value(int month);
    std::vector<int> values();

    static std::map<int, QString> create_month_map();

public slots:
    void reset();

protected:
    virtual void paintEvent(QPaintEvent * event) override;

private:
//    QPoint get_slider_position(QSlider * slider);
    void init_layout();
    SingleMonthWidget* m_monthly_widgets[12];
    QPen m_graph_drawer_pen;
    SingleMonthHeaderWidget * m_header_widget;

    static const std::map<int,QString> _NUM_TO_MONTH;
};

#endif
