#ifndef FILTERSHIFT_H
#define FILTERSHIFT_H

#include "Filter.h"

enum ShiftDirection {
    SHIFT_LEFT,
    SHIFT_RIGHT
};

class FilterShift : public Filter {
public:
    FilterShift(ShiftDirection direction);
    virtual ~FilterShift() override;

    virtual void apply(Canvas2D *canvas) override;

private:
    ShiftDirection m_shiftDir;
    std::vector<float> m_kernel;
};

#endif
