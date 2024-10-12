#include "Graph_TFT.h"

Graph_TFT::Graph_TFT(TFT_eSPI *display, uint16_t x, uint16_t y ,uint16_t width, uint16_t height, uint8_t padding, uint8_t rounded, GRAPH_TYPE type, GRAPH_STYLE style)
{   
    tft = display;
    _type = type;
    canva_style.canvasWidth = width;
    canva_style.canvasHeight = height;
    canva_style.x = x;
    canva_style.y = y;
    canva_style.padding = padding;
    canva_style.rounded = rounded;
    setCanva(style);
}

Graph_TFT::Graph_TFT(TFT_eSPI *display, CANVA_STYLE canva_style ,GRAPH_TYPE type, GRAPH_STYLE style)
{   
    tft = display;
    _type = type;
    this->canva_style = canva_style;
    setCanva(style);
}

void Graph_TFT::setCanva(GRAPH_STYLE style){
    switch (style)
    {
    case BLACK:
        canva_style.background = COLOR_BLACK;
        canva_style.draw = COLOR_WHITE;
        canva_style.fill = false;
        break;
    case PAPER:
        canva_style.background = COLOR_PAPER;
        canva_style.draw = BLUE_INK;
        canva_style.fill = false;
        break;
    case CAKE:
        canva_style.background = CAKE_PINK;
        canva_style.draw = BLUE_INK;
        canva_style.fill = true;
        break;
    default:
        canva_style.background = COLOR_BLACK;
        canva_style.draw = COLOR_WHITE;
        canva_style.fill = false;
        break;
    }
    tft->setTextSize(TEXT_SIZE);
    tft->setTextColor(canva_style.draw, canva_style.background, false);

    endX = canva_style.x + canva_style.canvasWidth - canva_style.padding;
    endY = canva_style.y + canva_style.padding;
    startX = canva_style.x + canva_style.padding;
    startY = canva_style.y + canva_style.canvasHeight - canva_style.padding;
    graphW =  endX - startX;
    graphH =  startY - endY;

    drawBackground();
}   

void Graph_TFT::drawBackground(void){
    tft->fillRoundRect(canva_style.x, canva_style.y, canva_style.canvasWidth, canva_style.canvasHeight, canva_style.rounded, canva_style.background);
    if(!canva_style.fill){
        tft->drawRoundRect(canva_style.x, canva_style.y, canva_style.canvasWidth, canva_style.canvasHeight, canva_style.rounded, canva_style.draw);
    }
    tft->drawLine( startX, startY, startX, endY, canva_style.draw);
    tft->drawLine( startX, startY, endX, startY, canva_style.draw);
}

void Graph_TFT::setData(uint16_t* x_data, uint16_t* y_data, uint8_t n_data, uint16_t y_limit)
{ 
    drawData(x_data, y_data, n_data, y_limit);
}

void Graph_TFT::setData(uint16_t* x_data, uint16_t* y_data, uint8_t n_data)
{ 
    uint16_t y_limit = maxY(y_data, n_data);
    drawData(x_data, y_data, n_data, y_limit);

}

void Graph_TFT::drawData(uint16_t* x_data, uint16_t* y_data, uint8_t n_data, uint16_t y_limit){
    drawBackground();
    drawTitle();

    uint8_t span = (graphW - n_data) / n_data;
    float delta_px = (float)graphH / y_limit;

    uint8_t x = 0;
    for (int i = 0; i < n_data; i++) {
        uint16_t barHeight = (uint16_t)(delta_px * y_data[i]);
        if(canva_style.fill){
            tft->fillRect(1 + startX + x, startY - barHeight, span, barHeight, canva_style.draw);
        }else{
            tft->drawRect(1 + startX + x, startY - barHeight, span, barHeight, canva_style.draw);
        }
        x += span + 1;  //span 1 px
    }
}


void Graph_TFT::setTitle(char* str)
{
    this->title = str;
    drawTitle();
}

void Graph_TFT::drawTitle()
{
    tft->drawString(title, canva_style.x + canva_style.padding*2, canva_style.y + canva_style.padding);
}

uint16_t Graph_TFT::maxY(uint16_t *y_data, uint8_t n_data)
{
    uint16_t max = 0;
    for(int i = 0; i<n_data; i++){
        max = (max<y_data[i])? y_data[i]:max;
    }
    return max;
}

void Graph_TFT::setBackgroudColour(uint16_t RGB565)
{
    this->canva_style.background = RGB565;
}

void Graph_TFT::setDrawingColour(uint16_t RGB565)
{
    this->canva_style.draw = RGB565;
}

uint16_t Graph_TFT::getWidth(void)
{
    return canva_style.canvasWidth;
}

uint16_t Graph_TFT::getHeight(void)
{
    return canva_style.canvasHeight;
}