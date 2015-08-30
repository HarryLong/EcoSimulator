#ifndef MONTHLY_TEMP_EDIT_DLG
#define MONTHLY_TEMP_EDIT_DLG

#include "monthly_edit_dlg.h"
#include "../../resources/environment_temp.h"

class MonthlyTempEditDlg : public MonthlyEditDialog
{
public:
    static const int _MIN = EnvironmentTemperature::_MIN;
    static const int _MAX = EnvironmentTemperature::_MAX;
    static const int _DEFAULT = 20;// EnvironmentTemperature::_MIN + ((EnvironmentTemperature::_MAX - EnvironmentTemperature::_MIN)/2.f);

    MonthlyTempEditDlg(QWidget * parent = 0) : MonthlyEditDialog(_MIN, _MAX, _DEFAULT, "Temperature (degrees)", parent) {}
    ~MonthlyTempEditDlg() {}
};

#endif
