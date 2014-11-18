#include "overview_widget.h"

OverViewWidget::OverViewWidget(int p_width, int p_height, QWidget *parent)
    : QTableWidget(parent), m_width(p_width), m_height(p_height)
{
}

OverViewWidget::~OverViewWidget()
{

}

QSize OverViewWidget::minimumSizeHint() const
{
    return QSize(m_width, m_height);
}

QSize OverViewWidget::sizeHint() const
{
    return QSize(m_width+100, m_height+100);
}
