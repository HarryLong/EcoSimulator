#ifndef MONTHLY_ILLUMINATION_EDIT_DLG
#define MONTHLY_ILLUMINATION_EDIT_DLG

#include "monthly_edit_dlg.h"
#include "../../resources/environment_illumination.h"

class MonthlyIlluminationEditDlg : public MonthlyEditDialog
{
public:
    static const int _MIN = EnvironmentIllumination::_MIN;
    static const int _MAX = EnvironmentIllumination::_MAX;
    static const int _DEFAULT = 10;//EnvironmentIllumination::_MIN + ((EnvironmentIllumination::_MAX - EnvironmentIllumination::_MIN)/2.f);

    MonthlyIlluminationEditDlg(QWidget * parent = 0) : MonthlyEditDialog(_MIN, _MAX, _DEFAULT, "Daily Illumination (h)", parent) {}
    ~MonthlyIlluminationEditDlg() {}
};

#endif
