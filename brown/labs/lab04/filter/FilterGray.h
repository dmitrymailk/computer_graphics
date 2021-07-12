#ifndef FilterGray_Header
#define FilterGray_Header

#include "Filter.h"

class FilterGray : public Filter {
public:
    virtual ~FilterGray() override;

    virtual void apply(Canvas2D *canvas) override;
};

#endif

