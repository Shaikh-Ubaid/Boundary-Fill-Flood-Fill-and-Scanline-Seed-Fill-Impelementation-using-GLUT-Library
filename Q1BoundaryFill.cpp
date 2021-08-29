#include <GL/glut.h> /* for using glut library */
#include <cstdio>    /* for using printf and scanf */
#include <vector>    /* for using vector */

#define WINDOW_WIDTH 1294
#define WINDOW_HEIGHT 704

// #define WINDOW_WIDTH 600
// #define WINDOW_HEIGHT 400

/* defining a structure for representing a point of the polygon */
typedef struct Point
{
    int x, y;
} Point;

/* defining a structure for representing a colour of a point */
typedef struct Colour
{
    unsigned char red, green, blue;
} Colour;

int no_of_polygon_points;
std::vector<Point> polygon;
Point starting_point;
Colour cur_pixel, fill_colour, boundary_colour;

/* declaring the functions */
void init(void);
void drawPolygon(std::vector<Point>);
int isColourEqual(Colour *c1, Colour *c2);
int isValid(int x, int y);
void putPixel(int x, int y);
void getPixel(int x, int y);
void boundaryFill(int x, int y);
void display(void);

int main(int argc, char **argv)
{
    printf("\tWELCOME TO POLYGON FILLING USING BOUNDARY FILL ALGORITHM\n"); /* showing welcome message */
    /* prompting the user for input */
    printf("Enter the number of points in the polygon: ");
    scanf("%d", &no_of_polygon_points);
    printf("Enter %d points, each in a new line:\n", no_of_polygon_points);
    printf("CAUTION: please ensure that you enter the points in clockwise or anticlockwise order\n");
    polygon.resize(no_of_polygon_points);
    for (int i = 0; i < no_of_polygon_points; ++i)
    {
        printf("Enter coordinates of point %d (separated by a space): ", i + 1);
        scanf("%d %d", &polygon[i].x, &polygon[i].y);
    }
    printf("Enter the RGB values of colour for filling: ");
    scanf("%d %d %d", (int *)&fill_colour.red, (int *)&fill_colour.green, (int *)&fill_colour.blue);

    printf("Enter the coordinates for starting point of Boundary Fill: ");
    scanf("%d %d", &starting_point.x, &starting_point.y);

    glutInit(&argc, argv);                                             /* initializing glut */
    glutInitDisplayMode(GLUT_SINGLE);                                  /* use single color buffer and no depth buffer */
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);                   /* setting size of display area, in pixels. */
    glutInitWindowPosition(0, 0);                                      /* setting location of window in screen coordinates. */
    glutCreateWindow("POLYGON FILLING USING BOUNDARY FILL ALGORITHM"); /* giving name/title to window */
    init();                                                            /* setting our own OpenGL initialization */
    glutDisplayFunc(display);                                          /* register display() function as the callback handler for window-paint event */
    glutMainLoop();                                                    /* run the event loop! This function does not return */
    /* Program ends when user closes the window */
    return 0;
}

void init(void) /* initialize OpenGL Graphics */
{
    glClearColor(0.0, 0.0, 0.0, 1.0); /* set the "clearing" or background color as black and opaque*/
    glColor3ub(255, 255, 255);        /* set white colour as drawing colour */
    // glColor3i(255, 255, 255);
    glPointSize(1.0);                 /* each pixel is of size 1x1 */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT); /* setting window dimension in X and Y directions */
}

/* A function for drawing polygon. It expects given polygon points to be in clockwise or anticlockwise order */
void drawPolygon(std::vector<Point> polygon)
{
    glBegin(GL_LINE_LOOP); /* using line loop so that the polygon is not already filled */
    int N = polygon.size();
    for (int i = 0; i < N; ++i)
        glVertex2i(polygon[i].x, polygon[i].y);
    glEnd();
}

/* a function which returns true if given two colours are equal else it returns false */
int isColourEqual(Colour *c1, Colour *c2)
{
    return ((c1->red == c2->red) && (c1->green == c2->green) && (c1->blue == c2->blue));
}

/* a function which returns true if given point is inside the window */
int isValid(int x, int y)
{
    return !(x < 0 || y < 0 || x > WINDOW_WIDTH || y > WINDOW_HEIGHT);
}

/* a function to paint a pixel at given coordinates. It paints the pixel with colour value of fill_colour */
void putPixel(int x, int y)
{
    glRasterPos2i(x, y);
    glDrawPixels(1, 1, GL_RGB, GL_UNSIGNED_BYTE, &fill_colour);
    glFlush();
}

/* a function to get the colour values at the given coordinates. It stores the colour values in cur_pixel */
void getPixel(int x, int y)
{
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &cur_pixel);
}

/* my boundary fill function */
void boundaryFill(int x, int y)
{
    if(!isValid(x, y))
        return;

    getPixel(x, y);
    if (isColourEqual(&cur_pixel, &fill_colour) || isColourEqual(&cur_pixel, &boundary_colour))
        return;

    putPixel(x, y);
    boundaryFill(x - 1, y);
    boundaryFill(x + 1, y);
    boundaryFill(x, y - 1);
    boundaryFill(x, y + 1);
}

/* Our display function which runs when the window first appears and whenever there is a request to re-paint the window. */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT); /* clear the color buffer i.e. set background with the current "clearing" color */

    drawPolygon(polygon); /* draw the polygon */

    glReadPixels(polygon[0].x, polygon[0].y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &boundary_colour); /* get the colour of the boundary */

    /* printing boundary colour on the console for user verification */
    printf("Boundary Colour is %d %d %d\n", (int)boundary_colour.red, (int)boundary_colour.green, (int)boundary_colour.blue);

    boundaryFill(starting_point.x, starting_point.y); /* fill the polygon */

    glFlush();
}