#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS 20
#define COLS 40
#define MAX_OBJECTS 50

typedef enum {
    RECTANGLE,
    CIRCLE,
    LINE,
    TRIANGLE
} ShapeType;

typedef struct {
    ShapeType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
} Shape;

static Shape objects[MAX_OBJECTS];
static int objectCount = 0;
static char picture[ROWS][COLS];

void clearPicture(void) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            picture[y][x] = '_';
        }
    }
}

void setPixel(int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        picture[y][x] = '*';
    }
}

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        setPixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2) {
    int left = x1 < x2 ? x1 : x2;
    int right = x1 < x2 ? x2 : x1;
    int top = y1 < y2 ? y1 : y2;
    int bottom = y1 < y2 ? y2 : y1;

    for (int x = left; x <= right; x++) {
        setPixel(x, top);
        setPixel(x, bottom);
    }
    for (int y = top; y <= bottom; y++) {
        setPixel(left, y);
        setPixel(right, y);
    }
}

void drawCircle(int cx, int cy, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    while (x <= y) {
        setPixel(cx + x, cy + y);
        setPixel(cx - x, cy + y);
        setPixel(cx + x, cy - y);
        setPixel(cx - x, cy - y);
        setPixel(cx + y, cy + x);
        setPixel(cx - y, cy + x);
        setPixel(cx + y, cy - x);
        setPixel(cx - y, cy - x);
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void redrawPicture(void) {
    clearPicture();
    for (int i = 0; i < objectCount; i++) {
        Shape *shape = &objects[i];
        switch (shape->type) {
            case RECTANGLE:
                drawRectangle(shape->x1, shape->y1, shape->x2, shape->y2);
                break;
            case CIRCLE:
                drawCircle(shape->x1, shape->y1, shape->radius);
                break;
            case LINE:
                drawLine(shape->x1, shape->y1, shape->x2, shape->y2);
                break;
            case TRIANGLE:
                drawTriangle(shape->x1, shape->y1, shape->x2, shape->y2, shape->x3, shape->y3);
                break;
        }
    }
}

void displayPicture(void) {
    printf("\n");
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            putchar(picture[y][x]);
        }
        putchar('\n');
    }
    printf("\n");
}

void printShape(int index) {
    Shape *shape = &objects[index];
    const char *name = "";
    switch (shape->type) {
        case RECTANGLE: name = "Rectangle"; break;
        case CIRCLE: name = "Circle"; break;
        case LINE: name = "Line"; break;
        case TRIANGLE: name = "Triangle"; break;
    }
    printf("%2d: %s ", index + 1, name);
    if (shape->type == RECTANGLE) {
        printf("[(%d,%d) to (%d,%d)]", shape->x1, shape->y1, shape->x2, shape->y2);
    } else if (shape->type == CIRCLE) {
        printf("[(%d,%d), r=%d]", shape->x1, shape->y1, shape->radius);
    } else if (shape->type == LINE) {
        printf("[(%d,%d) to (%d,%d)]", shape->x1, shape->y1, shape->x2, shape->y2);
    } else if (shape->type == TRIANGLE) {
        printf("[(%d,%d), (%d,%d), (%d,%d)]", shape->x1, shape->y1, shape->x2, shape->y2, shape->x3, shape->y3);
    }
    printf("\n");
}

void listObjects(void) {
    if (objectCount == 0) {
        puts("No objects in the picture.");
        return;
    }
    for (int i = 0; i < objectCount; i++) {
        printShape(i);
    }
}

void addObject(void) {
    if (objectCount >= MAX_OBJECTS) {
        puts("Cannot add more objects. The picture is full.");
        return;
    }

    printf("Select shape type:\n");
    printf("  1. Rectangle\n");
    printf("  2. Circle\n");
    printf("  3. Line\n");
    printf("  4. Triangle\n");
    printf("Choice: ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        puts("Invalid input.");
        return;
    }

    Shape shape;
    memset(&shape, 0, sizeof(shape));

    switch (choice) {
        case 1:
            shape.type = RECTANGLE;
            printf("Enter x1 y1 x2 y2: ");
            scanf("%d %d %d %d", &shape.x1, &shape.y1, &shape.x2, &shape.y2);
            break;
        case 2:
            shape.type = CIRCLE;
            printf("Enter center x y and radius: ");
            scanf("%d %d %d", &shape.x1, &shape.y1, &shape.radius);
            break;
        case 3:
            shape.type = LINE;
            printf("Enter x1 y1 x2 y2: ");
            scanf("%d %d %d %d", &shape.x1, &shape.y1, &shape.x2, &shape.y2);
            break;
        case 4:
            shape.type = TRIANGLE;
            printf("Enter x1 y1 x2 y2 x3 y3: ");
            scanf("%d %d %d %d %d %d", &shape.x1, &shape.y1, &shape.x2, &shape.y2, &shape.x3, &shape.y3);
            break;
        default:
            puts("Unknown shape type.");
            return;
    }

    objects[objectCount++] = shape;
    redrawPicture();
    puts("Shape added.");
}

void deleteObject(void) {
    if (objectCount == 0) {
        puts("No objects to delete.");
        return;
    }
    listObjects();
    printf("Enter object number to delete: ");
    int index;
    if (scanf("%d", &index) != 1 || index < 1 || index > objectCount) {
        puts("Invalid index.");
        return;
    }
    index--;
    for (int i = index; i < objectCount - 1; i++) {
        objects[i] = objects[i + 1];
    }
    objectCount--;
    redrawPicture();
    puts("Object deleted.");
}

void modifyObject(void) {
    if (objectCount == 0) {
        puts("No objects to modify.");
        return;
    }
    listObjects();
    printf("Enter object number to modify: ");
    int index;
    if (scanf("%d", &index) != 1 || index < 1 || index > objectCount) {
        puts("Invalid index.");
        return;
    }
    index--;

    Shape *shape = &objects[index];
    printf("Modifying object %d:\n", index + 1);
    if (shape->type == RECTANGLE) {
        printf("Enter new x1 y1 x2 y2: ");
        scanf("%d %d %d %d", &shape->x1, &shape->y1, &shape->x2, &shape->y2);
    } else if (shape->type == CIRCLE) {
        printf("Enter new center x y and radius: ");
        scanf("%d %d %d", &shape->x1, &shape->y1, &shape->radius);
    } else if (shape->type == LINE) {
        printf("Enter new x1 y1 x2 y2: ");
        scanf("%d %d %d %d", &shape->x1, &shape->y1, &shape->x2, &shape->y2);
    } else if (shape->type == TRIANGLE) {
        printf("Enter new x1 y1 x2 y2 x3 y3: ");
        scanf("%d %d %d %d %d %d", &shape->x1, &shape->y1, &shape->x2, &shape->y2, &shape->x3, &shape->y3);
    }
    redrawPicture();
    puts("Object modified.");
}

void displayMenu(void) {
    puts("2D Graphics Editor");
    puts("1. Display picture");
    puts("2. List objects");
    puts("3. Add object");
    puts("4. Delete object");
    puts("5. Modify object");
    puts("6. Clear picture");
    puts("7. Quit");
    printf("Choice: ");
}

int main(void) {
    clearPicture();
    redrawPicture();

    while (1) {
        displayMenu();
        int choice;
        if (scanf("%d", &choice) != 1) {
            puts("Invalid input.");
            return 1;
        }

        switch (choice) {
            case 1:
                displayPicture();
                break;
            case 2:
                listObjects();
                break;
            case 3:
                addObject();
                break;
            case 4:
                deleteObject();
                break;
            case 5:
                modifyObject();
                break;
            case 6:
                objectCount = 0;
                redrawPicture();
                puts("Picture cleared.");
                break;
            case 7:
                puts("Exiting.");
                return 0;
            default:
                puts("Unknown command.");
                break;
        }
    }
}
