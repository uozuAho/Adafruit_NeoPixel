#include "pixel_buf.h"


//-------------------------------------------------------------------
// file-private functions

// Saturating addition to LED value
static inline uint8_t ledAdd(uint8_t a, uint8_t b, uint8_t max_brightness)
{
    uint16_t new_val = a + b;
    if (new_val > 255)  new_val = 255;
    if (max_brightness)     new_val = min(new_val, max_brightness);
    return new_val;
}


//-------------------------------------------------------------------
// class methods

PixelBuf::PixelBuf(uint16_t num_pixels) :
    num_pixels(num_pixels),
    max_brightness(255)
{
    if((pixels = (uint8_t *)malloc(num_pixels * 3)))
    {
        memset(pixels, 0, num_pixels * 3);
    }
}

PixelBuf::PixelBuf(uint8_t* pixelmem, uint16_t num_pixels) :
    num_pixels(num_pixels),
    max_brightness(255)
{
    pixels = pixelmem;
}

PixelBuf::~PixelBuf()
{
    if (pixels) free(pixels);
}

void PixelBuf::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    if (n < num_pixels)
    {
        uint8_t *p = &pixels[n * 3];
        if (max_brightness < 255)
        {
            r = min(r, max_brightness);
            g = min(g, max_brightness);
            b = min(b, max_brightness);
        }
        *p++ = g;
        *p++ = r;
        *p = b;
    }
}

void PixelBuf::addPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    if (n < num_pixels)
    {
        uint8_t *p = &pixels[n * 3];
        *p = ledAdd(*p, g, max_brightness); p++;
        *p = ledAdd(*p, r, max_brightness); p++;
        *p = ledAdd(*p, b, max_brightness); p++;
    }
}

static inline uint8_t ledSubtract(uint8_t a, uint8_t b) {
    int16_t new_val = (int16_t)a - b;
    if (new_val < 0) new_val = 0;
    return new_val;
}

void PixelBuf::remPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    if (n < num_pixels)
    {
        uint8_t *p = &pixels[n * 3];
        *p = ledSubtract(*p, g); p++;
        *p = ledSubtract(*p, r); p++;
        *p = ledSubtract(*p, b); p++;
    }
}

void PixelBuf::setMaxBrightness(uint8_t x)
{
    max_brightness = x;
}

void PixelBuf::clear()
{
    memset(pixels, 0, num_pixels * 3);
}

void PixelBuf::clearRange(uint16_t start, uint16_t len)
{
    if (start < num_pixels)
    {
        if ((start + len) > num_pixels)
            len = num_pixels - start;
        memset(&pixels[start * 3], 0, len * 3);
    }
}

void PixelBuf::addBuf(PixelBuf& buf)
{
    if (buf.num_pixels == num_pixels)
    {
        const uint8_t* buf_ptr = buf.getInternalBuf();
        int num_bytes = num_pixels * 3;
        int i = 0;
        for (; i < num_bytes; i++)
            pixels[i] += buf_ptr[i];
    }
}

void PixelBuf::remBuf(PixelBuf& buf)
{
    if (buf.num_pixels == num_pixels)
    {
        const uint8_t* buf_ptr = buf.getInternalBuf();
        int num_bytes = num_pixels * 3;
        int i = 0;
        for (; i < num_bytes; i++)
            pixels[i] -= buf_ptr[i];
    }
}

const uint8_t* PixelBuf::getInternalBuf() const
{
    return pixels;
}
