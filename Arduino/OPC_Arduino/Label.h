#ifndef LABEL_H
#define LABEL_H

#include <inttypes.h>
#include <Elegoo_TFTLCD.h>

/**
   @brief The Label class allows for easy access to display simple text with a foreground and background color.
   The Label contains the screen coordines, foreground and background color, and the text to be displayed.
   The content may be set at any time, but will only be drawn to the screen when the update() function
   is called.
*/
class Label {
  private:
    /// Contains the x position of the label.
    uint16_t x;
    /// Contains the y position of the label.
    uint16_t y;
    /// Contains the width of the label.
    uint16_t width;
    /// Contains the height of the label.
    uint16_t height;
    /// Contains the bacground color of the label.
    uint16_t backgroundColor = 0x0000;
    /// Contains the foreground color of the label.
    uint16_t foregroundColor = 0xFFFF;
    /// TODO(jrh) document.
    bool centered = false;
    /// TODO(jrh) document.
    uint8_t size = 2;
    /// The TFT reference to draw to the lcd display.
    Elegoo_TFTLCD *tft;
    
  public:

    /**
       @brief default constructor doesn't exist because the label must contain screen coordinates.
    */
    Label() = delete;

    /**
        @brief Initialize the label with screen cordinates and a tft reference to render the content.
    */
    Label(uint16_t x, uint16_t, uint16_t width, uint16_t height, Elegoo_TFTLCD *tft);

    /**
       @brief Sets the foreground and background color of the Label.
       @param foreground The foreground color code for the Label.
       @param background The background color code for the Label.
    */
    void setColor(uint16_t foreground, uint16_t background);

    void setCentered(bool centered);

    void setSize(uint8_t size);

    /**
       @brief Updates the contents of the Label.
    */
    void update(const char* text);

    void update(uint16_t data);

    void clear();

    bool contains(uint16_t x, uint16_t y);

};
#endif  // LABEL_H
