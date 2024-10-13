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
    uint16_t x;               ///< X-coordinate of the canvas starting point from Top-Left.
    uint16_t y;               ///< Y-coordinate of the canvas starting point from Top-Left.
    uint16_t canvasWidth;     ///< Width of the canvas.
    uint16_t canvasHeight;    ///< Height of the canvas.
    uint16_t background;      ///< Background color of the canvas (RGB565 format).
    uint16_t draw;            ///< Drawing color of graph elements (RGB565 format).
    uint8_t padding;          ///< Padding around the canvas area.
    uint8_t rounded;          ///< Rounded corners for the canvas (0 for square, > 0 for rounded corners).
    uint8_t axisDivX = 1;     ///< Number of divisions for the Xaxis (default is 1).
    uint8_t axisDivY = 1;     ///< Number of divisions for the Yaxis (default is 1).
    bool x_axis = 1;          ///< Whether to draw the x-axis (1 to enable, 0 to disable).
    bool y_axis = 1;          ///< Whether to draw the y-axis (1 to enable, 0 to disable).
    bool fill;                ///< Whether to fill the graph bars or elements.
};

/**
 * @enum GRAPH_TYPE
 * @brief Defines the available types of graphs.
 */
typedef enum {
    BARS,    ///< Bar graph type.
    LINES,   ///< Line graph type.
    SMOOTH   ///< Smooth graph type (smoothed lines or curves).
} GRAPH_TYPE;

/**
 * @enum GRAPH_STYLE
 * @brief Defines the available DEFAULT styles for graph rendering.
 */
typedef enum {
    BLACK,  ///< Black style for the graph (dark theme).
    PAPER,  ///< Paper style for the graph (light theme).
    CAKE    ///< Cake style for the graph (colorful, possibly fun).
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
    TFT_eSPI *tft;                     ///< Pointer to the TFT display object.
    CANVA_STYLE canva_style;           ///< Structure that defines the canvas style.
    uint8_t _type;                     ///< Type of the graph (BARS, LINES, etc.).
    uint16_t startX;                   ///< X-coordinate of the starting point of the graph.
    uint16_t startY;                   ///< Y-coordinate of the starting point of the graph.
    uint16_t endX;                     ///< X-coordinate of the ending point of the graph.
    uint16_t endY;                     ///< Y-coordinate of the ending point of the graph.
    uint16_t graphW;                   ///< Width of the graph.
    uint16_t graphH;                   ///< Height of the graph.
    char* title;                       ///< Title of the graph.

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
     * @brief Draw a bar graph with the provided data.
     * @param y_data Array containing y-axis data for the bars.
     * @param n_data Number of data points.
     * @param y_limit Maximum value for y-axis scaling.
     */
    void drawBARS(uint16_t* y_data, uint8_t n_data, uint16_t y_limit);

    /**
     * @brief Draw a bar graph with the provided data.
     * @param x_data Array containing y-axis data for the bars.
     * @param y_data Array containing y-axis data for the bars.
     * @param n_data Number of data points.
     * @param y_limit Maximum value for y-axis scaling.
     */
    void drawLINES(uint16_t* x_data, uint16_t* y_data, uint8_t n_data, uint16_t y_limit);

    /**
     * @brief Find the maximum value in the y-axis data.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     * @return Maximum value in the data array.
     */
    uint16_t maxValue(uint16_t *y_data, uint8_t n_data);

    /**
     * @brief Performs a ascending bubble sort over a array;
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
    Graph_TFT(TFT_eSPI *display, uint16_t x, uint16_t y, uint16_t width = CANVAS_WIDTH, uint16_t height = CANVAS_HEIGHT, uint8_t padding = DEFAULT_PADDING, uint8_t rounded = DEFAULT_ROUNDED, GRAPH_TYPE type = BARS, GRAPH_STYLE style = BLACK);

    /**
     * @brief Constructor for the Graph_TFT class using a CANVA_STYLE structure.
     * @param display Pointer to the TFT display object.
     * @param canva_style Structure containing canvas styling options.
     * @param type Type of the graph (default is BARS).
     * @param style Style of the graph (default is BLACK).
     */
    Graph_TFT(TFT_eSPI *display, CANVA_STYLE canva_style, GRAPH_TYPE type = BARS, GRAPH_STYLE style = BLACK);

    /**
     * @brief Set the data and limits for bar graphs.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     * @param y_limit Maximum value for y-axis scaling.
     */
    void setDataBARS(uint16_t* y_data, uint8_t n_data, uint16_t y_limit);

    /**
     * @brief Set the data for bar graphs, y-axis scaling with y-max.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     */
    void setDataBARS(uint16_t* y_data, uint8_t n_data);

    /**
     * @brief Set the data and limits for lines graph.
     * @param x_data Array of x-axis data points.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     * @param y_limit Maximum value for y-axis scaling.
     */
    void setDataLINES(uint16_t* x_data, uint16_t* y_data, uint8_t n_data, uint16_t y_limit);

    /**
     * @brief Set the data for lines graph, y-axis scaling with y-max.
     * @param x_data Array of x-axis data points.
     * @param y_data Array of y-axis data points.
     * @param n_data Number of data points.
     */
    void setDataLINES(uint16_t* x_data, uint16_t* y_data, uint8_t n_data);

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
     * @brief Set the type of the graph.
     * @param type The desired graph type (e.g., BARS, LINES).
     */
    void setType(GRAPH_TYPE type);

    /**
     * @brief Set the title of the graph.
     * @param str string containing the title.
     */
    void setTitle(char* str);

    /**
     * @brief Set the background color of the graph.
     * @param RGB565 The color in RGB565 format.
     */
    void setBackgroudColour(uint16_t RGB565);

    /**
     * @brief Set the drawing color for the graph elements.
     * @param RGB565 The color in RGB565 format.
     */
    void setDrawingColour(uint16_t RGB565);

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


