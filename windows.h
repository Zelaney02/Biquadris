#ifndef A5_WINDOWS_H
#define A5_WINDOWS_H
#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
    Display *d;
    Window w;
    int s, width, height;
    GC gc;
    unsigned long colours[5];

public:
    Xwindow(int width=500, int height=500);  // Constructor; displays the window.
    ~Xwindow();                              // Destructor; destroys the window.

    enum {White=0, Black, Red, Green, Blue, Yellow, Cyan, Magenta}; // Available colours.

    int getWidth() const;
    int getHeight() const;

    // Draws a rectangle
    void fillRectangle(int x, int y, int width, int height, int colour=Black);

    // Draws a string
    void drawString(int x, int y, std::string msg);

};
#endif //A5_WINDOWS_H
