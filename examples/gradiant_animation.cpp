#include <Arduino.h>

typedef uint8_t ubyte;
typedef uint32_t uint;

struct vertex {
    uint x,y;
};

void initScreen(uint xRes, uint yRes);
void updateBuffer(ubyte* buffer);
void setPixel(uint x, uint y, ubyte r, ubyte g, ubyte b);
void drawLine(uint x1, uint y1, uint x2, uint y2, ubyte r, ubyte g, ubyte b);
void drawTriangle(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, ubyte r, ubyte g, ubyte b);
void drawRectangle(uint x, uint y, uint width, uint height, ubyte r, ubyte g, ubyte b);
void drawPolygon(ubyte numVertices, vertex* vertices, ubyte r, ubyte g, ubyte b);
void drawEllipse(uint x, uint y, uint width, uint height, ubyte r, ubyte g, ubyte b);

void sendUInt(uint i);

void setup() {
    Serial.begin(9600);
    Serial.print("VS START");
    initScreen(50, 50);
}

void loop() {
    static ubyte r = 0;
    static ubyte g = 0;
    static ubyte b = 255;
    static int x = 0;
    static int y = 0;
    setPixel(x, y, r, g, b);
    x ++;
    if(x == 50) {
        x = 0;
        y ++;
        if(y == 50) {
            y = 0;
        }
    }
    static ubyte stage = 0;
    switch(stage) {
    case 0:
        r++;
        if(r == 255) stage ++;
        break;
    case 1:
        b --;
        if(b == 0) stage ++;
        break;
    case 2:
        g ++;
        if(g == 255) stage ++;
        break;
    case 3:
        r --;
        if(r == 0) stage ++;
        break;
    case 4:
        b ++;
        if(b == 255) stage ++;
        break;
    case 5:
        g --;
        if(g == 0) stage = 0;
        break;
    }
}

void initScreen(uint xRes, uint yRes) {
    sendUInt(xRes);
    sendUInt(yRes);
}

void updateBuffer(ubyte* buffer) {
    Serial.write(0);
    Serial.write(buffer, xRes * yRes * 3);
}

void setPixel(uint x, uint y, ubyte r, ubyte g, ubyte b) {
    Serial.write(1);
    sendUInt(x);
    sendUInt(y);
    Serial.write(r);
    Serial.write(g);
    Serial.write(b);
}

void drawLine(uint x1, uint y1, uint x2, uint y2, ubyte r, ubyte g, ubyte b) {
    Serial.write(2);//draw line start
    sendUInt(x1);
    sendUInt(y1);
    sendUInt(x2);
    sendUInt(y2);
    Serial.write(r);
    Serial.write(g);
    Serial.write(b);
}

void drawTriangle(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, ubyte r, ubyte g, ubyte b) {
    Serial.write(3);
    sendUInt(x1);
    sendUInt(y1);
    sendUInt(x2);
    sendUInt(y2);
    sendUInt(x3);
    sendUInt(y3);
    Serial.write(r);
    Serial.write(g);
    Serial.write(b);
}

void drawRectangle(uint x, uint y, uint width, uint height, ubyte r, ubyte g, ubyte b) {
    Serial.write(4);
    sendUInt(x);
    sendUInt(y);
    sendUInt(width);
    sendUInt(height);
    Serial.write(r);
    Serial.write(g);
    Serial.write(b);
}

void drawPolygon(ubyte numVertices, vertex* vertices, ubyte r, ubyte g, ubyte b) {
    Serial.write(5);
    Serial.write(numVertices);
    for(ubyte i = 0; i < numVertices; i ++)
    {
        sendUInt(vertices[i].x);
        sendUInt(vertices[i].y);
    }
    Serial.write(r);
    Serial.write(g);
    Serial.write(b);
}

void drawEllipse(uint x, uint y, uint width, uint height, ubyte r, ubyte g, ubyte b) {
    Serial.write(6);
    sendUInt(x);
    sendUInt(y);
    sendUInt(width);
    sendUInt(height);
    Serial.write(r);
    Serial.write(g);
    Serial.write(b);
}

void sendUInt(uint i) {
    Serial.write((i >> 24) & 0xFF);
    Serial.write((i >> 16) & 0xFF);
    Serial.write((i >> 8) & 0xFF);
    Serial.write(i & 0xFF);
}