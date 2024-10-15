#pragma once
#include <TFT_eSPI.h>

#define CANVAS_WIDTH 128
#define CANVAS_HEIGHT 128

/* COLOURS DEFINITIONS IN RGB565 */
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xffff
#define COLOR_DARK_SEA 0x1148
#define COLOR_SAND 0xf6b8
#define COLOR_NEON_RED 0xf800
#define COLOR_PASTEL_PINK 0xfdbb
#define COLOR_PASTEL_PURPLE 0xa2d5
#define COLOR_BLUE_INK 0x527F
#define COLOR_PAPER 0xffdb

/* Default Parameters */
#define PADDING 10
#define ROUNDED 5
#define TEXT_SIZE 1
#define DEFAULT_AXIS_DIV 0
#define DEFAULT_PADDING 15
#define DEFAULT_ROUNDED 5

/**
 * @struct CANVA_STYLE
 * @brief Defines the canvas style for graph rendering.
 *
 * This structure contains settings for the canvas size, position, colors, padding, and axis visibility.
 */
struct CANVA_STYLE
{
    uint16_t x;            ///< X-coordinate of the canvas starting point from Top-Left.
    uint16_t y;            ///< Y-coordinate of the canvas starting point from Top-Left.
    uint16_t canvasWidth;  ///< Width of the canvas.
    uint16_t canvasHeight; ///< Height of the canvas.
    uint16_t background;   ///< Background color of the canvas (RGB565 format).
    uint16_t draw1;        ///< Primary Drawing color of graph elements (RGB565 format).
    uint16_t draw2;        ///< Secondary Drawing color of graph elements (RGB565 format).
    uint8_t padding;       ///< Padding around the canvas area.
    uint8_t rounded;       ///< Rounded corners for the canvas (0 for square, > 0 for rounded corners).
    uint8_t axisDivX = 1;  ///< Number of divisions for the Xaxis (default is 1).
    uint8_t axisDivY = 1;  ///< Number of divisions for the Yaxis (default is 1).
    bool x_axis = 1;       ///< Whether to draw the x-axis (1 to enable, 0 to disable).
    bool y_axis = 1;       ///< Whether to draw the y-axis (1 to enable, 0 to disable).
    bool fill;             ///< Whether to fill the graph bars or elements.
};

/**
 * @enum GRAPH_STYLE
 * @brief Defines the available DEFAULT styles for graph rendering.
 */
typedef enum
{
    BLACK, ///< Black style for the graph (dark theme).
    NEON,  ///< Black style and colored.
    OCEAN, ///< Blue style, themed by the sea
    PAPER, ///< Paper style for the graph (light theme).
    CAKE   ///< Cake style for the graph (colorful, possibly fun).
} GRAPH_STYLE;

/**
 * @class Graph_TFT
 * @brief A class for graphing data on TFT displays using the TFT_eSPI library.
 *
 * This class allows creating and rendering different types of graphs (like bar, charts, ...) on TFT displays.
 * It uses various customizable parameters for the canvas, style, and axis divisions.
 */
class Graph_TFT
{
private:
    TFT_eSPI *tft;           ///< Pointer to the TFT display object.
    CANVA_STYLE canva_style; ///< Structure that defines the canvas style.
    uint16_t startX;         ///< X-coordinate of the starting point of the graph.
    uint16_t startY;         ///< Y-coordinate of the starting point of the graph.
    uint16_t endX;           ///< X-coordinate of the ending point of the graph.
    uint16_t endY;           ///< Y-coordinate of the ending point of the graph.
    uint16_t graphW;         ///< Width of the graph.
    uint16_t graphH;         ///< Height of the graph.
    char *title;             ///< Title of the graph.

    /**
     * @brief Set the canvas style.
     * @param style Graph style to be applied to the canvas.
     */
    void setCanva(GRAPH_STYLE style);

    /**
     * @brief Draw the graph's title.
     */
    void drawTitle(void);

    /**
     * @brief Draw the background of the graph.
     */
    void drawBackground(void);

    /**
     * @brief Draw the axis of the graph.
     */
    void drawAxis(void);

    /**
     * @brief Draw a bar graph with the provided data.
     * @param y_data Array containing y-axis data for the bars.
     * @param n_data Number of data points.
     * @param min_Y Minimun value for y-axis scaling.
     * @param max_Y Maximum value for y-axis scaling.
     */
    void drawBARS(uint16_t *y_data, uint8_t n_data, uint16_t min_Y, uint16_t max_Y);

    /**
     * @brief Draw labels of the graph.
     * @param deltaX_px space of x axis in px.
     * @param deltaY_px space of y axis in px.
     * @param min_Y Minimun value for y-axis.
     * @param max_Y Maximum value for y-axis.
     * @param min_X Minimun value for x-axis.
     * @param max_X Maximum value for x-axis.
     */
    void drawLabels(uint16_t deltaX_px, uint16_t deltaY_px, uint16_t min_Y, uint16_t max_Y, uint16_t min_X, uint16_t max_X);

    /**
     * @brief Draw a bar graph with the provided data.
     * @param x_data Array containing y-axis data for the bars.
     * @param y_data Array containing y-axis data for the bars.
     * @param n_data Number of data points.
     * @param min_Y Minimun value for y-axis scaling.
     * @param max_Y Maximum value for y-axis scaling.
     */
    void drawLINES(uint16_t *x_data, uint16_t *y_data, uint8_t n_data, uint16_t min_Y, uint16_t max_Y);

    /**
     * @brief Draw a PIE graph with the percentage.
     * @param percentage Array containing percentage data.
     * @param n_data Number of data points.
     * @param labels array of labels.
     */
    void drawPIE(uint8_t *percentage, uint8_t n_data, const char *labels[]);

    /**
     * @brief Find the maximum value in the y-axis data.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     * @param max true to get the max value, false to get the min value
     * @return Maximum value in the data array.
     */
    uint16_t maxminValue(uint16_t *y_data, uint8_t n_data, bool max);

    /**
     * @brief Ascending bubble sort over a array, if arrY is not NULL, permorms permutations in both.
     * @param arrX Array of data X.
     * @param arrY Array of data Y.
     * @param n_data Number of data points.
     */
    void bubbleSortX(uint16_t *arrX, uint16_t *arrY, uint8_t n_data);

public:
    /**
     * @brief Constructor for the Graph_TFT class with individual parameters.
     * @param display Pointer to the TFT display object.
     * @param x X-coordinate of the canvas starting point from Top-Left.
     * @param y Y-coordinate of the canvas starting point from Top-Left.
     * @param width Width of the graph (default is CANVAS_WIDTH).
     * @param height Height of the graph (default is CANVAS_HEIGHT).
     * @param padding Padding around the graph (default is DEFAULT_PADDING).
     * @param rounded Rounded corners for the graph (default is DEFAULT_ROUNDED).
     * @param type Type of the graph (default is BARS).
     * @param style Style of the graph (default is BLACK).
     */
    Graph_TFT(TFT_eSPI *display, uint16_t x, uint16_t y, uint16_t width = CANVAS_WIDTH, uint16_t height = CANVAS_HEIGHT, uint8_t padding = DEFAULT_PADDING, uint8_t rounded = DEFAULT_ROUNDED, GRAPH_STYLE style = BLACK);

    /**
     * @brief Constructor for the Graph_TFT class using a CANVA_STYLE structure.
     * @param display Pointer to the TFT display object.
     * @param canva_style Structure containing canvas styling options.
     * @param type Type of the graph (default is BARS).
     * @param style Style of the graph (default is BLACK).
     */
    Graph_TFT(TFT_eSPI *display, CANVA_STYLE canva_style, GRAPH_STYLE style = BLACK);

    /**
     * @brief Set the data and limits for bar graphs.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     * @param min_Y Minimun value for y-axis scaling.
     * @param max_Y Maximum value for y-axis scaling.
     */
    void setDataBARS(uint16_t *y_data, uint8_t n_data, uint16_t min_Y, uint16_t max_Y);

    /**
     * @brief Set the data for bar graphs, y-axis scaling max with y-max.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     * @param min_Y Minimun value for y-axis scaling.
     */
    void setDataBARS(uint16_t *y_data, uint8_t n_data, uint16_t min_Y);

    /**
     * @brief Set the data for bar graphs, y-axis scaling with y-max - y-min.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     */
    void setDataBARS(uint16_t *y_data, uint8_t n_data);

    /**
     * @brief Set the data and limits for lines graph.
     * @param x_data Array of x-axis data points.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     * @param min_Y Minimun value for y-axis scaling.
     * @param max_Y Maximum value for y-axis scaling.
     */
    void setDataLINES(uint16_t *x_data, uint16_t *y_data, uint8_t n_data, uint16_t min_Y, uint16_t max_Y);

    /**
     * @brief Set the data and limits for lines graph, y-axis scaling max with y-max.
     * @param x_data Array of x-axis data points.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     * @param min_Y Maximum value for y-axis scaling.
     */
    void setDataLINES(uint16_t *x_data, uint16_t *y_data, uint8_t n_data, uint16_t min_Y);

    /**
     * @brief Set the data for lines graph, y-axis scaling with y-max y y-min.
     * @param x_data Array of x-axis data points.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     */
    void setDataLINES(uint16_t *x_data, uint16_t *y_data, uint8_t n_data);

    /**
     * @brief Set the data for PIE graph
     * @param percentage [0-100] value of every data
     * @param n_data Number of data.
     */
    void setDataPIE(uint8_t *percentage, uint8_t n_data);

    /**
     * @brief Set the data for PIE graph
     * @param percentage [0-100] value of every data
     * @param n_data Number of data.
     * @param labels Labels for each data.
     */
    void setDataPIE(uint8_t *percentage, uint8_t n_data, const char *labels[]);

    /**
     * @brief Set the number of axis divisions for labels.
     * @param divX Number of divisions for the X axis.
     * @param divY Number of divisions for the Y axis.
     */
    void setAxisDiv(uint8_t divX, uint8_t divY);

    /**
     * @brief Set the style of the graph.
     * @param style The desired graph style.
     */
    void setStyle(GRAPH_STYLE style);

    /**
     * @brief Set the title of the graph.
     * @param str string containing the title.
     */
    void setTitle(char *str);

    /**
     * @brief Set the background color of the graph.
     * @param RGB565 The color in RGB565 format.
     */
    void setBackgroudColour(uint16_t RGB565);

    /**
     * @brief Set the primary drawing color for the graph elements.
     * @param RGB565 The color in RGB565 format.
     */
    void setDrawingPrimaryColour(uint16_t RGB565);

    /**
     * @brief Set the secondary drawing color for the graph elements.
     * @param RGB565 The color in RGB565 format.
     */
    void setDrawingSecondaryColour(uint16_t RGB565);

    /**
     * @brief Enable or disable drawing of the x and y axes.
     * @param x_axis Boolean flag to enable/disable the x-axis.
     * @param y_axis Boolean flag to enable/disable the y-axis.
     */
    void setAxis(bool x_axis, bool y_axis);

    /**
     * @brief Get the width of the graph canvas.
     * @return The width of the canvas.
     */
    uint16_t getWidth(void);

    /**
     * @brief Get the height of the graph canvas.
     * @return The height of the canvas.
     */
    uint16_t getHeight(void);
};
