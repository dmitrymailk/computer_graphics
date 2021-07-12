#include "FilterGray.h"

unsigned char RGBAToGray(const RGBA &pixel) {
    // TODO: Task 5

}

FilterGray::~FilterGray()
{
}

void FilterGray::apply(Canvas2D *canvas) {
    int width = canvas->width();

    RGBA* current_pixel = nullptr;

    /* Initialize the first pixel of the first row */
    RGBA* current_row = canvas->data();

    RGBA* data = canvas->data();
    size_t currentIndex = 0;

    for (int r = 0; r < canvas->height(); r++) {
        current_pixel = current_row;
        currentIndex = r * width;

        for (int c = 0; c < canvas->width(); c++) {
            // TODO: Task 4


            // TODO: Task 6


            /* Advance to the next pixel */
            current_pixel++;
            currentIndex++;
        }
        current_row += width;
    }
}

