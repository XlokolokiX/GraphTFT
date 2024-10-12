#pragma once
#include <TFT_eSPI.h>

#define CANVAS_WIDTH 128
#define CANVAS_HEIGHT 128

/* COLOURS DEFINITIONS IN RGB565 */
#define COLOR_BLACK 0x0000
#define COLOR_PAPER 0xD6D5
#define COLOR_WHITE 0xffff
#define CAKE_PINK 0xFE9F
#define CAKE_BLUE 0xD69F
#define BLUE_INK 0x527F

/* Default Parameters */
#define PADDING 10
#define ROUNDED 5
#define TEXT_SIZE 1

struct CANVA_STYLE
{
    uint16_t x;
    uint16_t y;
    uint16_t canvasWidth;
    uint16_t canvasHeight;
    uint16_t background;
    uint16_t draw;
    uint8_t fill;
    uint8_t padding;
    uint8_t rounded;
};

typedef enum{
    BARS,
    LINES,
    SMOOTH
}GRAPH_TYPE;

typedef enum{
    BLACK,
    PAPER,
    CAKE
}GRAPH_STYLE;

class Graph_TFT
{
private:
    TFT_eSPI *tft;
    CANVA_STYLE canva_style;
    uint8_t _type;
    uint16_t startX;
    uint16_t startY;
    uint16_t endX;
    uint16_t endY;
    uint16_t graphW;
    uint16_t graphH;
    char* title;

    void setCanva(GRAPH_STYLE style);
    void drawData(uint16_t* x_data, uint16_t* y_data, uint8_t n_data, uint16_t y_limit);
    void drawBackground(void);
    void drawTitle(void);
    uint16_t maxY(uint16_t *y_data, uint8_t n_data);
public:
    Graph_TFT(TFT_eSPI *display, uint16_t x, uint16_t y ,uint16_t width = CANVAS_WIDTH, uint16_t height = CANVAS_HEIGHT, uint8_t padding = 10, uint8_t rounded = 5, GRAPH_TYPE type = BARS, GRAPH_STYLE style = BLACK);
    Graph_TFT(TFT_eSPI *display, CANVA_STYLE canva_style ,GRAPH_TYPE type = BARS, GRAPH_STYLE style = BLACK);
    void setData(uint16_t* x_data, uint16_t* y_data, uint8_t n_data, uint16_t y_limit);
    void setData(uint16_t* x_data, uint16_t* y_data, uint8_t n_data);
    void setStyle(GRAPH_STYLE style);
    void setType(GRAPH_TYPE type);
    void setTitle(char* str);
    void setBackgroudColour(uint16_t RGB565);
    void setDrawingColour(uint16_t RGB565);
    uint16_t getWidth(void);
    uint16_t getHeight(void);
};

