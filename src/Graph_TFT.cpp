#include "Graph_TFT.h"

Graph_TFT::Graph_TFT(TFT_eSPI *display, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t padding, uint8_t rounded, GRAPH_STYLE style)
{
    tft = display;
    canva_style.canvasWidth = width;
    canva_style.canvasHeight = height;
    canva_style.x = x;
    canva_style.y = y;
    canva_style.padding = padding;
    canva_style.rounded = rounded;
    setCanva(style);
}

Graph_TFT::Graph_TFT(TFT_eSPI *display, CANVA_STYLE canva_style, GRAPH_STYLE style)
{
    tft = display;
    this->canva_style = canva_style;
    setCanva(style);
}

void Graph_TFT::drawTitle()
{
    tft->drawString(title, canva_style.x + canva_style.padding * 2, canva_style.y + canva_style.padding / 2);
}

void Graph_TFT::drawBackground(void)
{
    tft->fillRoundRect(canva_style.x, canva_style.y, canva_style.canvasWidth, canva_style.canvasHeight, canva_style.rounded, canva_style.background);
}

void Graph_TFT::drawAxis(void)
{
    if (!canva_style.fill)
    {
        tft->drawRoundRect(canva_style.x, canva_style.y, canva_style.canvasWidth, canva_style.canvasHeight, canva_style.rounded, canva_style.draw1);
    }
    tft->drawLine(startX, startY, startX, endY, canva_style.draw1);
    tft->drawLine(startX, startY, endX, startY, canva_style.draw1);
}

void Graph_TFT::drawBARS(uint16_t *y_data, uint8_t n_data, uint16_t min_Y, uint16_t max_Y)
{
    drawBackground();
    drawAxis();
    drawTitle();

    float deltaX_px = (graphW - (n_data + 1)) / n_data;
    float deltaY_px = graphH / (max_Y - min_Y);

    uint16_t x = 2 + startX;
    uint16_t barHeight = 0;
    for (uint8_t i = 0; i < n_data; i++)
    {
        barHeight = (uint16_t)(deltaY_px * (y_data[i] - min_Y));

        if (canva_style.fill)
        {
            tft->fillRect(x, startY - barHeight, deltaX_px, barHeight, canva_style.draw2);
        }
        else
        {
            tft->drawRect(x, startY - barHeight, deltaX_px, barHeight, canva_style.draw2);
        }

        x += deltaX_px + 1; // span 1 px between bars
    }

    drawLabels(deltaX_px, deltaY_px, min_Y, max_Y, 1, n_data);
}

void Graph_TFT::drawLINES(uint16_t *x_data, uint16_t *y_data, uint8_t n_data, uint16_t min_Y, uint16_t max_Y)
{
    drawBackground();
    drawAxis();
    drawTitle();

    if (max_Y == min_Y)
        return;

    float deltaX_px = (graphW - (n_data + 1)) / n_data;
    float deltaY_px = graphH / (max_Y - min_Y);

    uint16_t *x_data_sorted = new uint16_t[n_data];
    uint16_t *y_data_sorted = new uint16_t[n_data];

    memcpy(x_data_sorted, x_data, n_data * sizeof(uint16_t));
    memcpy(y_data_sorted, y_data, n_data * sizeof(uint16_t));

    quickSortX(x_data_sorted, y_data_sorted, 0, n_data - 1);

    uint16_t min_X = x_data_sorted[0];
    uint16_t max_X = x_data_sorted[n_data - 1];

    uint16_t xs = startX + deltaX_px / 2 + 1;
    uint16_t ys = startY - deltaY_px * (y_data_sorted[0] - min_Y);
    uint16_t xe = 0, ye = 0;

    for (uint8_t i = 1; i <= n_data; i++)
    {
        if (canva_style.fill)
        {
            tft->fillCircle(xs, ys, 2, canva_style.draw2);
        }
        else
        {
            tft->drawCircle(xs, ys, 2, canva_style.draw2);
        }

        if (i != n_data)
        {
            xe = xs + deltaX_px + 1;
            ye = startY - deltaY_px * (y_data_sorted[i] - min_Y);

            tft->drawLine(xs, ys, xe, ye, canva_style.draw2);
            xs = xe;
            ys = ye;
        }
    }

    drawLabels(deltaX_px, deltaY_px, min_Y, max_Y, min_X, max_X);

    delete[] x_data_sorted;
    delete[] y_data_sorted;
}

void Graph_TFT::drawLabels(uint16_t deltaX_px, uint16_t deltaY_px, uint16_t min_Y, uint16_t max_Y, uint16_t min_X, uint16_t max_X)
{
    // Draw X-axis labels
    if (canva_style.x_axis)
    {
        uint16_t x = deltaX_px / 2;
        uint16_t x_increment = (deltaX_px + 1) * canva_style.axisDivX;

        for (uint16_t i = min_X; i <= max_X; i += canva_style.axisDivX)
        {
            tft->drawNumber(i, startX + x, startY + 2);
            x += x_increment;
        }
    }

    // Draw Y-axis labels
    if (canva_style.y_axis)
    {
        uint16_t y_position = 0;
        for (uint16_t i = min_Y; i <= max_Y; i += canva_style.axisDivY)
        {
            y_position = startY - (i - min_Y) * deltaY_px;
            tft->drawNumber(i, canva_style.x + 2, y_position - 2);
            tft->drawPixel(startX + 1, y_position, canva_style.draw1);
        }
    }
}

void Graph_TFT::drawPIE(uint8_t *percentage, uint8_t n_data, const char *labels[])
{
    drawBackground();
    drawTitle();

    uint8_t acc = 0;
    for(uint8_t i = 0; i < n_data; i++)
    {
        acc+= percentage[i];
    }
    if(acc != 100){
        return;
    }
    delete &acc;

    const uint16_t colors[5] = {0x08aa, 0x1ad5, 0xa236, 0xf334, 0xfd36};
    const double conversion = 62.832e-3; // Equivalent to 2 * PI / 100 (to convert percentage to radians)
    uint8_t radius = (graphH < graphW) ? graphH / 2 : graphW / 2;
    uint16_t sx = startX + graphW / 2;
    uint16_t sy = startY - graphH / 2;
    uint8_t start = 0, end = 0;
    uint16_t ex, ey, lx, ly, midpoint;

    for (uint8_t i = 0; i < n_data; i++)
    {
        end = start + percentage[i];

        for (uint8_t p = start; p < end; p++)
        {
            double p_conv = p * conversion;
            double p1_conv = (p + 1) * conversion;

            ex = sx + radius * sin(p_conv);
            ey = sy - radius * cos(p_conv);

            // Draw each triangle for the pie slice
            tft->fillTriangle(sx, sy,
                              sx + radius * sin(p1_conv), sy - radius * cos(p1_conv),
                              ex, ey, colors[i % 5]);
        }

        // Compute midpoint for labels
        midpoint = (start + end) / 2;
        double midpoint_conv = midpoint * conversion;

        lx = sx + 1.1 * radius * sin(midpoint_conv);
        ly = sy - 1.1 * radius * cos(midpoint_conv);

        tft->drawNumber(percentage[i], lx, ly);

        // draw labels if provided
        if (labels != NULL)
        {
            tft->drawString(labels[i], lx - 8, ly - 8);
        }

        start = end;
    }
}

void Graph_TFT::drawLINES(uint16_t *x_data, uint16_t *y_data, uint8_t n_data, uint16_t y_limit)
{
    drawBackground();
    drawAxis();
    drawTitle();

    float deltaX_px = (graphW - (n_data + 1)) / n_data;
    float deltaY_px = graphH / y_limit;

    uint16_t x_data_sorted[n_data], y_data_sorted[n_data];
    memcpy(x_data_sorted, x_data, n_data * sizeof(uint16_t));
    memcpy(y_data_sorted, y_data, n_data * sizeof(uint16_t));
    quickSortX(x_data_sorted, y_data_sorted, 0, n_data - 1);

    uint16_t xs = startX + deltaX_px * x_data_sorted[0] + 2;
    uint16_t ys = startY - deltaY_px * y_data_sorted[0];
    uint16_t xe = 0;
    uint16_t ye = 0;
    for (uint8_t i = 1; i < n_data; i++)
    {
        if (canva_style.fill)
        {
            tft->fillCircle(xs, ys, 2, canva_style.draw2);
        }
        else
        {
            tft->drawCircle(xs, ys, 2, canva_style.draw2);
        }
        if (i != n_data)
        {
            xe = startX + deltaX_px * x_data_sorted[i] + 2;
            ye = startY - deltaY_px * y_data_sorted[i];
            tft->drawLine(xs, ys, xe, ye, canva_style.draw2);
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
            tft->drawPixel(startX + 1, startY - i * deltaY_px, canva_style.draw1);
        }
    }
}

void Graph_TFT::setDataPIE(uint8_t *percentage, uint8_t n_data)
{
    drawPIE(percentage, n_data, NULL);
}

void Graph_TFT::setDataPIE(uint8_t *percentage, uint8_t n_data, const char *labels[])
{
    drawPIE(percentage, n_data, labels);
}

void Graph_TFT::setTitle(char *str)
{
    this->title = str;
    drawTitle();
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

void Graph_TFT::setDataBARS(uint16_t *y_data, uint8_t n_data, uint16_t min_Y, uint16_t max_Y)
{
    uint16_t max = maxminValue(y_data, n_data, true);
    uint16_t min = maxminValue(y_data, n_data, false);

    min_Y = (min < min_Y) ? min : min_Y;
    max_Y = (max > max_Y) ? max : max_Y;

    drawBARS(y_data, n_data, min_Y, max_Y);
}

void Graph_TFT::setDataBARS(uint16_t *y_data, uint8_t n_data, uint16_t min_Y)
{
    uint16_t max_Y = maxminValue(y_data, n_data, true);
    uint16_t min = maxminValue(y_data, n_data, false);

    min_Y = (min < min_Y) ? min : min_Y;

    drawBARS(y_data, n_data, min_Y, max_Y);
}

void Graph_TFT::setDataBARS(uint16_t *y_data, uint8_t n_data)
{
    drawBARS(y_data, n_data, maxminValue(y_data, n_data, false), maxminValue(y_data, n_data, true));
}

void Graph_TFT::setDataLINES(uint16_t *x_data, uint16_t *y_data, uint8_t n_data, uint16_t min_Y, uint16_t max_Y)
{
    uint16_t max = maxminValue(y_data, n_data, true);
    uint16_t min = maxminValue(y_data, n_data, false);

    min_Y = (min < min_Y) ? min : min_Y;
    max_Y = (max > max_Y) ? max : max_Y;

    drawLINES(x_data, y_data, n_data, min_Y, max_Y);
}

void Graph_TFT::setDataLINES(uint16_t *x_data, uint16_t *y_data, uint8_t n_data, uint16_t min_Y)
{
    uint16_t max_Y = maxminValue(y_data, n_data, true);
    uint16_t min = maxminValue(y_data, n_data, false);

    min_Y = (min < min_Y) ? min : min_Y;

    drawLINES(x_data, y_data, n_data, min_Y, max_Y);
}

void Graph_TFT::setDataLINES(uint16_t *x_data, uint16_t *y_data, uint8_t n_data)
{
    drawLINES(x_data, y_data, n_data, maxminValue(y_data, n_data, false), maxminValue(y_data, n_data, true));
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

void Graph_TFT::setDrawingPrimaryColour(uint16_t RGB565)
{
    this->canva_style.draw1 = RGB565;
}

void Graph_TFT::setDrawingSecondaryColour(uint16_t RGB565)
{
    this->canva_style.draw2 = RGB565;
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
        canva_style.draw1 = COLOR_WHITE;
        canva_style.draw2 = COLOR_WHITE;
        canva_style.fill = false;
        break;
    case NEON:
        canva_style.background = COLOR_BLACK;
        canva_style.draw1 = COLOR_WHITE;
        canva_style.draw2 = COLOR_NEON_RED;
        canva_style.fill = false;
        break;
    case PAPER:
        canva_style.background = COLOR_PAPER;
        canva_style.draw1 = COLOR_BLUE_INK;
        canva_style.draw2 = COLOR_NEON_RED;
        canva_style.fill = false;
        break;
    case OCEAN:
        canva_style.background = COLOR_DARK_SEA;
        canva_style.draw1 = COLOR_WHITE;
        canva_style.draw2 = COLOR_SAND;
        canva_style.fill = true;
        break;
    case CAKE:
        canva_style.background = COLOR_WHITE;
        canva_style.draw1 = COLOR_NEON_RED;
        canva_style.draw2 = COLOR_PASTEL_PURPLE;
        canva_style.fill = true;
        break;
    default:
        canva_style.background = COLOR_BLACK;
        canva_style.draw1 = COLOR_WHITE;
        canva_style.draw2 = COLOR_WHITE;
        canva_style.fill = false;
        break;
    }
    tft->setTextSize(TEXT_SIZE);
    tft->setTextColor(canva_style.draw1, canva_style.background, false);
}

void Graph_TFT::quickSortX(uint16_t *arrX, uint16_t *arrY, uint8_t left, uint8_t right)
{
    if (left < right)
    {
        // Obtiene el índice de partición
        uint8_t pivotIndex = partition(arrX, arrY, left, right);

        // Ordena las dos subpartes de forma recursiva
        quickSortX(arrX, arrY, left, pivotIndex - 1);
        quickSortX(arrX, arrY, pivotIndex + 1, right);
    }
}

uint8_t Graph_TFT::partition(uint16_t *arrX, uint16_t *arrY, uint8_t left, uint8_t right)
{
    uint16_t pivot = arrX[right];
    uint8_t i = left - 1;

    for (uint8_t j = left; j < right; j++)
    {
        if (arrX[j] < pivot)
        {
            i++;

            swap(&arrX[i], &arrX[j]);

            if (arrY != NULL)
            {
                swap(&arrY[i], &arrY[j]);
            }
        }
    }

    swap(&arrX[i + 1], &arrX[right]);

    if (arrY != NULL)
    {
        swap(&arrY[i + 1], &arrY[right]);
    }

    return i + 1;
}

void Graph_TFT::swap(uint16_t *a, uint16_t *b)
{
    uint16_t temp = *a;
    *a = *b;
    *b = temp;
}

uint16_t Graph_TFT::maxminValue(uint16_t *y_data, uint8_t n_data, bool max)
{
    if (n_data == 0)
        return 0;

    uint16_t val = y_data[0];
    for (int i = 1; i < n_data; i++)
    {
        if (max)
        {
            val = (val < y_data[i]) ? y_data[i] : val;
        }
        else
        {
            val = (val > y_data[i]) ? y_data[i] : val;
        }
    }
    return val;
}
