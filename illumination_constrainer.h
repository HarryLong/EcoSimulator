#ifndef ILLUMINATION_CONSTRAINER_H
#define ILLUMINATION_CONSTRAINER_H

struct IlluminationProperties {
    int illumination_requirements;

    IlluminationProperties(int p_illumination_requirements) : illumination_requirements(p_illumination_requirements) {}
};

class IlluminationConstrainer
{
public:
    IlluminationConstrainer(const IlluminationProperties * p_illumination_properties);
    ~IlluminationConstrainer();

    int getStrength(float p_ratio);

private:
    const IlluminationProperties * m_illumination_properties;
};

#endif // ILLUMINATION_CONSTRAINER_H
