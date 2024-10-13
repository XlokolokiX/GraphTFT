#include "Graph_TFT.h"

Graph_TFT::Graph_TFT(TFT_eSPI *display, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t padding, uint8_t rounded, GRAPH_TYPE type, GRAPH_STYLE style)
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

Graph_TFT::Graph_TFT(TFT_eSPI *display, CANVA_STYLE canva_style, GRAPH_TYPE type, GRAPH_STYLE style)
{
    tft = display;
    _type = type;
    this->canva_style = canva_style;
    setCanva(style);
}

void Graph_TFT::setCanva(GRAPH_STYLE style)
{
    setStyle(style);

    endX = canva_style.x + canva_style.canvasWidth - canva_style.padding;
    endY = canva_style.y + canva_style.padding;
    startX = canva_style.x + canva_style.padding;
    startY = canva_style.y + canva_style.canvasHeight - canva_style.padding;
    graphW = endX - startX;
    graphH = startY - endY;

    drawBackground();
}

void Graph_TFT::drawBackground(void)
{
    tft->fillRoundRect(canva_style.x, canva_style.y, canva_style.canvasWidth, canva_style.canvasHeight, canva_style.rounded, canva_style.background);
    if (!canva_style.fill)
    {
        tft->drawRoundRect(canva_style.x, canva_style.y, canva_style.canvasWidth, canva_style.canvasHeight, canva_style.rounded, canva_style.draw);
    }
    tft->drawLine(startX, startY, startX, endY, canva_style.draw);
    tft->drawLine(startX, startY, endX, startY, canva_style.draw);
}

void Graph_TFT::setDataBARS(uint16_t *y_data, uint8_t n_data, uint16_t y_limit)
{
    drawBARS(y_data, n_data, y_limit);
}

void Graph_TFT::setDataBARS(uint16_t *y_data, uint8_t n_data)
{
    uint16_t y_limit = maxValue(y_data, n_data);
    drawBARS(y_data, n_data, y_limit);
}

void Graph_TFT::setDataLINES(uint16_t* x_data, uint16_t* y_data, uint8_t n_data, uint16_t y_limit)
{
    drawLINES(x_data, y_data, n_data, y_limit);
}

void Graph_TFT::setDataLINES(uint16_t* x_data, uint16_t* y_data, uint8_t n_data)
{
    uint16_t y_limit = maxValue(y_data, n_data);
    drawLINES(x_data, y_data, n_data, y_limit);
}

void Graph_TFT::drawBARS(uint16_t *y_data, uint8_t n_data, uint16_t y_limit)
{
    drawBackground();
    drawTitle();

    float deltaX_px = (graphW - (n_data + 1)) / n_data;
    float deltaY_px = graphH / y_limit;

    uint8_t x = 0;
    for (uint8_t i = 0; i < n_data; i++)
    {
        uint16_t barHeight = (uint16_t)(deltaY_px * y_data[i]);

        if (canva_style.fill)
        {
            tft->fillRect(2 + startX + x, startY - barHeight, deltaX_px, barHeight, canva_style.draw);
        }
        else
        {
            tft->drawRect(2 + startX + x, startY - barHeight, deltaX_px, barHeight, canva_style.draw);
        }

        x += deltaX_px + 1; // span 1 px
    }

    if (canva_style.x_axis)
    {
        x = 0;
        for (uint8_t i = 0; i < n_data / canva_style.axisDivX; i++)
        {
            tft->drawNumber(canva_style.axisDivX * i + 1, 2 + startX + x, startY + 2);
            x += (deltaX_px + 1) * canva_style.axisDivX;
        }
    }

    if (canva_style.y_axis)
    {
        for (uint8_t i = 0; i <= y_limit; i += canva_style.axisDivY)
        {
            tft->drawNumber(i, canva_style.x + 2, startY - i * deltaY_px - 2);
            tft->drawPixel(startX + 1, startY - i * deltaY_px, canva_style.draw);
        }
    }
}

void Graph_TFT::drawLINES(uint16_t* x_data, uint16_t* y_data, uint8_t n_data, uint16_t y_limit)
{
    drawBackground();
    drawTitle();

    float deltaX_px = (graphW - (n_data + 1)) / n_data;
    float deltaY_px = graphH / y_limit;

    uint16_t x_data_sorted[n_data], y_data_sorted[n_data];
    memcpy(x_data_sorted, x_data, n_data * sizeof(uint16_t));
    memcpy(y_data_sorted, y_data, n_data * sizeof(uint16_t));
    bubbleSortX(x_data_sorted, y_data_sorted, n_data);

    uint16_t xs = startX + deltaX_px*x_data_sorted[0] + 2;
    uint16_t ys = startY - deltaY_px*y_data_sorted[0];
    uint16_t xe = 0;
    uint16_t ye = 0;
    for(uint8_t i = 1; i < n_data; i++)
    {   
        if(canva_style.fill)
        {
            tft->fillCircle(xs, ys, 2, canva_style.draw);
        }else
        {
            tft->drawCircle(xs, ys, 2, canva_style.draw);
        }
        if(i != n_data)
        {   
            xe = startX + deltaX_px*x_data_sorted[i] + 2;
            ye = startY - deltaY_px*y_data_sorted[i];
            tft->drawLine(xs, ys, xe, ye, canva_style.draw);
        }
        xs = xe;
        ys = ye;
    }

    if (canva_style.x_axis)
    {
        uint8_t x = 0;
        for (uint8_t i = 0; i < n_data / canva_style.axisDivX; i++)
        {
            tft->drawNumber(canva_style.axisDivX * i + 1, 2 + startX + x, startY + 2);
            x += (deltaX_px + 1) * canva_style.axisDivX;
        }
    }

    if (canva_style.y_axis)
    {
        for (uint8_t i = 0; i <= y_limit; i += canva_style.axisDivY)
        {
            tft->drawNumber(i, canva_style.x + 2, startY - i * deltaY_px - 2);
            tft->drawPixel(startX + 1, startY - i * deltaY_px, canva_style.draw);
        }
    }

}

void Graph_TFT::setTitle(char *str)
{
    this->title = str;
    drawTitle();
}

void Graph_TFT::drawTitle()
{
    tft->drawString(title, canva_style.x + canva_style.padding * 2, canva_style.y + canva_style.padding / 2);
}

uint16_t Graph_TFT::maxValue(uint16_t *y_data, uint8_t n_data)
{
    uint16_t max = 0;
    for (int i = 0; i < n_data; i++)
    {
        max = (max < y_data[i]) ? y_data[i] : max;
    }
    return max;
}

void Graph_TFT::setAxisDiv(uint8_t divX, uint8_t divY)
{
    this->canva_style.axisDivX = divX;
    this->canva_style.axisDivY = divY;
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

void Graph_TFT::setAxis(bool x_axis, bool y_axis)
{
    this->canva_style.x_axis = x_axis;
    this->canva_style.y_axis = y_axis;
}

void Graph_TFT::setStyle(GRAPH_STYLE style)
{
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
}

void Graph_TFT::bubbleSortX(uint16_t *arrX, uint16_t *arrY, uint8_t n_data)
{  
    for(uint8_t i = 0; i < n_data; i++)
    {
        for(uint8_t j = i + 1; j < n_data; j++)
        {
            if(arrX[i] > arrX[j])
            {
                uint16_t tempX = arrX[i], tempY = arrY[i];
                arrX[i] = arrX[j];
                arrX[j] = tempX;
                arrY[i] = arrY[j];
                arrY[j] = tempY;
            }
        }
    }
}