#ifndef Filter_Header
#define Filter_Header

#include <assert.h>
#include "RGBA.h"
#include "Canvas2D.h"

class Canvas2D;

/**
 * @class Filter
 *
 * This is the superclass for all filters.
 *
 * It declares the abstract method apply, which is called by Canvas2D to apply the filter.
 */
class Filter {

public:
    virtual ~Filter();

    // Applies this filter on the given region.
    //
    // The apply method is called by FilterCanvas to apply this filter
    // to an entire image. This method is abstract, you will need to
    // redefine it each filter subclass.
    //
    virtual void apply(Canvas2D* canvas) = 0;
};

#endif

