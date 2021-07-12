#ifndef FILTERIDENTITY_H
#define FILTERIDENTITY_H

#include "Filter.h"

class FilterIdentity : public Filter {
public:
    FilterIdentity();
    virtual ~FilterIdentity() override;

    virtual void apply(Canvas2D *canvas) override;

private:
    std::vector<float> m_kernel;
};

#endif

