#ifndef MONTHLY_HUMIDITY_EDIT_DLG
#define MONTHLY_HUMIDITY_EDIT_DLG

#include "monthly_edit_dlg.h"
#include "../../resources/environment_soil_humidity.h"

class MonthlyHumidityEditDlg : public MonthlyEditDialog
{
public:
    static const int _MIN = EnvironmentSoilHumidity::_MIN;
    static const int _MAX = EnvironmentSoilHumidity::_MAX;
    static const int _DEFAULT = 35;//EnvironmentSoilHumidity::_MIN + ((EnvironmentSoilHumidity::_MAX - EnvironmentSoilHumidity::_MIN)/2.f);

    MonthlyHumidityEditDlg(QWidget * parent = 0) : MonthlyEditDialog(_MIN, _MAX, _DEFAULT, "Monthly humidity (mm)", parent) {}
    ~MonthlyHumidityEditDlg() {}
};

#endif
