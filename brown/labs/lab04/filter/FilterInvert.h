#ifndef FilterInvert_Header
#define FilterInvert_Header

#include "Filter.h"

class FilterInvert : public Filter {
public:
    virtual ~FilterInvert() override;

    virtual void apply(Canvas2D *canvas) override;
    void applyOptional(Canvas2D *canvas);
};

#endif
