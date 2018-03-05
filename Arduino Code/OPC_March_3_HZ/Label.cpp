#include "Label.h"

Label::Label(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Elegoo_TFTLCD *tft) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->tft = tft;
}

void Label::setColor(uint16_t foreground, uint16_t background) {
  this->foregroundColor = foreground;
  this->backgroundColor = background;
}

void Label::setCentered(bool centered) {
  this->centered = centered;
}

void Label::setSize(uint8_t size) {
  this->size = size;
}

void Label::update(const char* text) {
  // Used to center the string inside the label area.
  uint16_t strLen = strlen(text);
  // Draw the bacground color of the lable.
  tft->fillRect(x, y, width, height, backgroundColor);
  if (centered) {
    tft->setCursor(x + width / 2 - strLen * 6 * size / 2, y + height / 2 - size * 8 / 2);
  } else {
    // Set the location of the text to draw.
    tft->setCursor(x, y);
  }
  // Update the size of the text.
  tft->setTextSize(size);
  // Update the color of the text.
  tft->setTextColor(foregroundColor);
  // Print the actual text to the tft screen.
  tft->print(text);
}


void Label::update(uint16_t data) {
  // Allocate memory on the stack to contain the data in string format.
  char text[6];
  // Convert the integer to a string.
  itoa(data, text, 10);
  // Draw the text on the screen.
  update(text);
}

void Label::clear() {
  tft->fillRect(x, y, width, height, backgroundColor);
}

bool Label::contains(uint16_t x, uint16_t y) {
  return x > this->x && x < (this->x + this->width) &&
         y > this->y && y < (this->y + this->height);
}

