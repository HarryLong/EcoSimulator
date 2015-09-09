#include "constrainers.h"
#include "math.h"


const int Constrainer::_MIN_STRENGTH = -100;
const int Constrainer::_MAX_STRENGTH = 100;

/***********
 * WRAPPER *
 ***********/
ConstrainersWrapper::ConstrainersWrapper( AgeConstrainer age_constrainer, IlluminationConstrainer illumination_constrainer,
                                          SoilHumidityConstrainer soil_humidity_constrainer, TemperatureConstrainer temp_constrainer):
                                          age_constrainer(age_constrainer), illumination_constrainer(illumination_constrainer),
                                          soil_humidity_constrainer(soil_humidity_constrainer), temp_constrainer(temp_constrainer)
{

}

ConstrainersWrapper::~ConstrainersWrapper()
{

}

//ConstrainersWrapper & ConstrainersWrapper::operator=(const ConstrainersWrapper& other)
//{
//    if (this != &other) // protect against invalid self-assignment
//    {
//        age_constrainer = other.age_constrainer;
//        illumination_constrainer = other.illumination_constrainer;
//        soil_humidity_constrainer = other.soil_humidity_constrainer;
//        temp_constrainer = other.temp_constrainer;
//    }
//    // by convention, always return *this
//    return *this;
//}

/*******************
 * AGE CONSTRAINER *
 *******************/
AgeConstrainer::AgeConstrainer(const AgeingProperties & p_ageing_properties) :
    m_age(0), m_properties(p_ageing_properties)
{
    // Build the pre-prime linear equation
    {
        m_ageing_equation.a =  (-Constrainer::_MAX_STRENGTH*2.0f) / (m_properties.max_age-m_properties.start_of_decline);
        m_ageing_equation.b = Constrainer::_MAX_STRENGTH - (m_ageing_equation.a * m_properties.start_of_decline);
    }
}

AgeConstrainer::~AgeConstrainer()
{
}

//AgeConstrainer & AgeConstrainer::operator=(const AgeConstrainer& other)
//{
//    if (this != &other) // protect against invalid self-assignment
//    {
//        m_age = other.m_age;
//        m_properties = other.m_properties;
//        m_ageing_equation = other.m_ageing_equation;
//    }
//    // by convention, always return *this
//    return *this;
//}

void AgeConstrainer::setAge(int p_age)
{
    m_age = p_age;
}

int AgeConstrainer::getStrength() const
{
    if(m_age > m_properties.start_of_decline)
    {
        return m_ageing_equation.calculateY(m_age);
    }
    return Constrainer::_MAX_STRENGTH;
}

/****************************
 * ILLUMINATION CONSTRAINER *
 ****************************/
IlluminationConstrainer::IlluminationConstrainer(const IlluminationProperties & p_illumination_properties) :
    m_daily_illumination(0), m_properties(p_illumination_properties)
{
    // Underexposure equation
    if(m_properties.prime_illumination.first > 0)
    {
        m_underexposure_equation.a = ((float)Constrainer::_MAX_STRENGTH*2.0) / (m_properties.prime_illumination.first - m_properties.min_illumination);
        m_underexposure_equation.b = Constrainer::_MAX_STRENGTH - (m_underexposure_equation.a * m_properties.prime_illumination.first);
    }
    else
    {
        m_underexposure_equation.a  = 0;
        m_underexposure_equation.b = Constrainer::_MAX_STRENGTH;
    }

    // Overexposure equation
    if(m_properties.prime_illumination.second < 24)
    {
        m_overexposure_equation.a = (-2.0f*Constrainer::_MAX_STRENGTH) / (m_properties.max_illumination - m_properties.prime_illumination.second);
        m_overexposure_equation.b = Constrainer::_MAX_STRENGTH - (m_overexposure_equation.a * m_properties.prime_illumination.second);
    }
    else
    {
        m_overexposure_equation.a  = 0;
        m_overexposure_equation.b = Constrainer::_MAX_STRENGTH;
    }
}

IlluminationConstrainer::~IlluminationConstrainer()
{
}

//IlluminationConstrainer & IlluminationConstrainer::operator=(const IlluminationConstrainer& other)
//{
//    if (this != &other) // protect against invalid self-assignment
//    {
//        m_properties = other.m_properties;
//        m_underexposure_equation = other.m_underexposure_equation;
//        m_overexposure_equation = other.m_overexposure_equation;
//        m_daily_illumination = other.m_daily_illumination;
//    }
//    // by convention, always return *this
//    return *this;
//}

void IlluminationConstrainer::setDailyIllumination(int p_daily_illumination)
{
    m_daily_illumination = p_daily_illumination;
}

bool IlluminationConstrainer::isUnderExposed()
{
    return m_daily_illumination < m_properties.prime_illumination.first;
}

/**
 * @brief IlluminationConstrainer::getStrength
 * @param p_ratio --> shadowed / all
 * @return
 */
int IlluminationConstrainer::getStrength() const
{
    if(m_daily_illumination < m_properties.prime_illumination.first)
        return m_underexposure_equation.calculateY(m_daily_illumination);

    if(m_daily_illumination > m_properties.prime_illumination.second)
        return m_overexposure_equation.calculateY(m_daily_illumination);

    return Constrainer::_MAX_STRENGTH;
}

/*****************
 * SOIL HUMIDITY *
 *****************/
SoilHumidityConstrainer::SoilHumidityConstrainer(const SoilHumidityProperties & p_soil_humidity_properties) :
    m_properties(p_soil_humidity_properties)
{
    // Build the drought equation
    if(m_properties.prime_soil_humidity.first > 0)
    {
        m_drought_equation.a = ((float)Constrainer::_MAX_STRENGTH*2.0f) / (m_properties.prime_soil_humidity.first - m_properties.min_soil_humidity);
        m_drought_equation.b = Constrainer::_MAX_STRENGTH - (m_drought_equation.a * m_properties.prime_soil_humidity.first);
    }
    else
    {
        m_drought_equation.a  = 0;
        m_drought_equation.b = Constrainer::_MAX_STRENGTH;
    }

    // Build the flooding equation
    if(m_properties.prime_soil_humidity.second < 100)
    {
        m_flood_equation.a = (-2.0f*Constrainer::_MAX_STRENGTH) / (m_properties.max_soil_humidity - m_properties.prime_soil_humidity.second);
        m_flood_equation.b = Constrainer::_MAX_STRENGTH - ( m_flood_equation.a * m_properties.prime_soil_humidity.second );
    }
    else
    {
        m_flood_equation.a  = 0;
        m_flood_equation.b = Constrainer::_MAX_STRENGTH;
    }
}

SoilHumidityConstrainer::~SoilHumidityConstrainer()
{

}

//SoilHumidityConstrainer & SoilHumidityConstrainer::operator=(const SoilHumidityConstrainer& other)
//{
//    if (this != &other) // protect against invalid self-assignment
//    {
//        m_drought_equation = other.m_drought_equation;
//        m_flood_equation = other.m_flood_equation;
//        m_properties = other.m_properties;
//        m_soil_humidity = other.m_soil_humidity;
//    }
//    // by convention, always return *this
//    return *this;
//}

int SoilHumidityConstrainer::getStrength() const
{
    if(m_soil_humidity < m_properties.prime_soil_humidity.first)
        return m_drought_equation.calculateY(m_soil_humidity);

    if(m_soil_humidity > m_properties.prime_soil_humidity.second)
        return m_flood_equation.calculateY(m_soil_humidity);

    return Constrainer::_MAX_STRENGTH;
}

bool SoilHumidityConstrainer::isInDrought() const
{
    return m_soil_humidity < m_properties.prime_soil_humidity.first;
}

void SoilHumidityConstrainer::setSoilHumidity(int p_soil_humidity)
{
    m_soil_humidity = p_soil_humidity;
}

int SoilHumidityConstrainer::getMinimumPrimeSoilHumidity() const
{
    return m_properties.prime_soil_humidity.first;
}

/***************
 * TEMPERATURE *
 ***************/
TemperatureConstrainer::TemperatureConstrainer(const TemperatureProperties & p_temperature_properties) :
    m_properties(p_temperature_properties)
{
    // Build coldness equation
    m_chill_equation.a = ((float)Constrainer::_MAX_STRENGTH*2.0f) / (m_properties.prime_temp.first-m_properties.min_temp);
    m_chill_equation.b = Constrainer::_MAX_STRENGTH - (m_chill_equation.a * m_properties.prime_temp.first);

    // Build the heat equation
    m_warmth_equation.a = (-2.0f*Constrainer::_MAX_STRENGTH) / (m_properties.max_temp - m_properties.prime_temp.second);
    m_warmth_equation.b = Constrainer::_MAX_STRENGTH - (m_warmth_equation.a * m_properties.prime_temp.second);
}

TemperatureConstrainer::~TemperatureConstrainer()
{

}

//TemperatureConstrainer & TemperatureConstrainer::operator=(const TemperatureConstrainer& other)
//{
//    if (this != &other) // protect against invalid self-assignment
//    {
//        m_chill_equation = other.m_chill_equation;
//        m_warmth_equation = other.m_warmth_equation;
//        m_properties = other.m_properties;
//        m_temperature = other.m_temperature;
//    }
//    // by convention, always return *this
//    return *this;
//}

int TemperatureConstrainer::getStrength() const
{
    if(m_temperature < m_properties.prime_temp.first)
        return m_chill_equation.calculateY(m_temperature);

    if(m_temperature > m_properties.prime_temp.second)
        return m_warmth_equation.calculateY(m_temperature);

    return Constrainer::_MAX_STRENGTH;
}

void TemperatureConstrainer::setTemperature(int p_temp)
{
    m_temperature = p_temp;
}

bool TemperatureConstrainer::isTooCold() const
{
    return m_temperature < m_properties.prime_temp.first;
}
