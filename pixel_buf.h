#ifndef PIXEL_BUF_H
#define PIXEL_BUF_H

#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
    #include <pins_arduino.h>
#endif

class PixelBuf
{
public:
    PixelBuf(uint16_t num_pixels);
    ~PixelBuf();

    /// Set the r, g and b LED values of the given pixel
    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

    /// Adds to the current values in the given pixel. Saturates at
    /// maximum brightness or currently set brightness value
    void addPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

    /// Subtracts from the current values in the given pixel.
    void remPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

    /// Sets maximum value any LED can reach
    void setMaxBrightness(uint8_t x);

    /// The quickest way in the west to turn off all pixels!
    void clear();

    /// Get a pointer to the internal pixel buffer. Needed
    /// by NeoPixel class to set LED hardware quickly.
    /// TODO: should this be private, with PixelBuf being a
    /// friend of NeoPixel?
    const uint8_t* getInternalBuf();


private:
    /// LED value buffer. Always stored r,g,b...
    uint8_t* pixels;

    /// Number of RGB LEDs in strip
    const uint16_t num_pixels;

    /// Max allowable value for all LEDs
    uint8_t max_brightness;
};

#endif // PIXEL_BUF_H


