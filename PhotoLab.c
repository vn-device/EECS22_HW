/*********************************************************************/
/* PhotoLab.c: Assignment 2 for EECS 22, Winter 2026                 */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*** global definitions ***/
#define WIDTH  640		/* image width */
#define HEIGHT 360		/* image height */
#define SLEN    80		/* maximum length of file names */

/*** enumeration definitions ***/
enum USER_OPTIONS
{
    USER_OPTIONS_LOAD_PPM = 1,
    USER_OPTIONS_SAVE_IMG,
    USER_OPTIONS_CHANGE_TO_BNW,
    USER_OPTIONS_MAKE_NEG,
    USER_OPTIONS_COLOR_FILTER,
    USER_OPTIONS_SKETCH_EDGE,
    USER_OPTIONS_SHUFFLE,
    USER_OPTIONS_FLIP_HOR,
    USER_OPTIONS_MIRROR_VER,
    USER_OPTIONS_ADD_BORDER,
    USER_OPTIONS_TEST_ALL_FUNCS,
    USER_OPTIONS_EXIT,
};

/*** function declarations ***/

/* print a menu */
void PrintMenu(void);

/* parse a menu option */
void ParseMenuOption(const int* option,
        unsigned char R[WIDTH][HEIGHT],
	    unsigned char G[WIDTH][HEIGHT],
	    unsigned char B[WIDTH][HEIGHT]);

/* read image from a file */
int LoadImage(const char fname[SLEN],
	      unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* save a processed image */
int SaveImage(const char fname[SLEN],
	      unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* change a color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
		 unsigned char B[WIDTH][HEIGHT]);

/* reverse image color */
void Negative(unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* color filter */
void ColorFilter(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
                 unsigned char B[WIDTH][HEIGHT],
		 int target_r, int target_g, int target_b, int threshold,
		 int replace_r, int replace_g, int replace_b);

/* edge detection */
void Edge(unsigned char R[WIDTH][HEIGHT],
	  unsigned char G[WIDTH][HEIGHT],
          unsigned char B[WIDTH][HEIGHT]);

/* mirror image Vertically */
void VMirror(unsigned char R[WIDTH][HEIGHT],
	     unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);

/* shuffle the image */
void Shuffle(unsigned char R[WIDTH][HEIGHT],
	     unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);


/* add border */
void AddBorder(unsigned char R[WIDTH][HEIGHT],
	       unsigned char G[WIDTH][HEIGHT],
               unsigned char B[WIDTH][HEIGHT],
	       char color[SLEN], int border_width);

/* flip image horizontally */
void HFlip(unsigned char R[WIDTH][HEIGHT],
	   unsigned char G[WIDTH][HEIGHT],
           unsigned char B[WIDTH][HEIGHT]);

/* test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

int main(void)
{
    /* Two dimensional arrays to hold the current image data, */
    /* one array for each color component.                    */
    unsigned char   R[WIDTH][HEIGHT];
    unsigned char   G[WIDTH][HEIGHT];
    unsigned char   B[WIDTH][HEIGHT];

    /* Please replace the following code with proper menu with function calls for DIP operations */
    /* if (LoadImage("EngHall", R, G, B) != 0)
    {
      return 1;
    }
    */

    int userOption = 0;
    while (userOption != USER_OPTIONS_EXIT) {
        PrintMenu();
        
        scanf("%d", &userOption);
        ParseMenuOption(&userOption, R, G, B);
    }
    

    /* End of replacing */

    return 0;
}

int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char Type[SLEN];
    int  Width, Height, MaxValue;
    int  x, y;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "r");
    if (!File) {
        printf("\nCannot open file \"%s\" for loading!\n", fname);
        return 1;
    }
    fscanf(File, "%79s", Type);
    if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
        printf("\nUnsupported file format!\n");
        return 2;
    }
    fscanf(File, "%d", &Width);
    if (Width != WIDTH) {
        printf("\nUnsupported image width %d!\n", Width);
        return 3;
    }
    fscanf(File, "%d", &Height);
    if (Height != HEIGHT) {
        printf("\nUnsupported image height %d!\n", Height);
        return 4;
    }
    fscanf(File, "%d", &MaxValue);
    if (MaxValue != 255) {
        printf("\nUnsupported image maximum value %d!\n", MaxValue);
        return 5;
    }
    if ('\n' != fgetc(File)) {
        printf("\nCarriage return expected!\n");
        return 6;
    }
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            R[x][y] = fgetc(File);
            G[x][y] = fgetc(File);
            B[x][y] = fgetc(File);
        }
    }
    if (ferror(File)) {
        printf("\nFile error while reading from file!\n");
        return 7;
    }
    printf("%s was loaded successfully!\n", fname_ext);
    fclose(File);
    return 0;
}

int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];
    char SysCmd[SLEN * 5];
    int  x, y;

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "w");
    if (!File) {
        printf("\nCannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    fprintf(File, "P6\n");
    fprintf(File, "%d %d\n", WIDTH, HEIGHT);
    fprintf(File, "255\n");

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            fputc(R[x][y], File);
            fputc(G[x][y], File);
            fputc(B[x][y], File);
        }
    }

    if (ferror(File)) {
        printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(File);
    printf("%s was saved successfully. \n", fname_ext);

    /*
     * rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
     * and make it world readable
     */
    sprintf(SysCmd, "~eecs22/bin/pnmtojpeg_hw2.tcsh %s", fname_ext);
    if (system(SysCmd) != 0) {
        printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
    }
    printf("%s.jpg was stored for viewing. \n", fname);

    return 0;
}

/*DO NOT EDIT AUTOTEST*/
/* Uncomment AutoTest() to run all DIP functions automatically. */

void AutoTest(unsigned char R[WIDTH][HEIGHT],
              unsigned char G[WIDTH][HEIGHT],
              unsigned char B[WIDTH][HEIGHT])
{
    LoadImage("EngHall", R, G, B);
    Negative(R, G, B);
    SaveImage("negative", R, G, B);
    printf("Negative tested!\n\n");

    LoadImage("EngHall", R, G, B);
    ColorFilter(R, G, B, 152, 135, 104, 20, 255, 0, 0);
    SaveImage("colorfilter", R, G, B);
    printf("Color Filter tested!\n\n");

    LoadImage("EngHall", R, G, B);
    Edge(R, G, B);
    SaveImage("edge", R, G, B);
    printf("Edge Detection tested!\n\n");

    LoadImage("EngHall", R, G, B);
    VMirror(R, G, B);
    SaveImage("vmirror", R, G, B);
    printf("VMirror tested!\n\n");

    LoadImage("EngHall", R, G, B);
    AddBorder(R, G, B, "orange", 24);
    SaveImage("border", R, G, B);
    printf("Border tested!\n\n");

    LoadImage("EngHall", R, G, B);
    BlackNWhite(R, G, B);
    SaveImage("bw", R, G, B);
    printf("Black & White tested!\n\n");

    LoadImage("EngHall", R, G, B);
    HFlip(R, G, B);
    SaveImage("hflip", R, G, B);
    printf("HFlip tested!\n\n");

    LoadImage("EngHall", R, G, B);
    Shuffle(R, G, B);
    SaveImage("shuffle", R, G, B);
    printf("Shuffle tested!\n\n");
}

/**************************************************************/
/* Please add your function definitions here...               */
/**************************************************************/

void PrintMenu(void)
{
    printf("--------------------------------\n");
    printf("1: Load a PPM image\n");
    printf("2: Save an image in PPM and JPEG format\n");
    printf("3: Change a color image to Black & White\n");
    printf("4: Make a negative of an image\n");
    printf("5: Color filter an image\n");
    printf("6: Sketch the edge of an image\n");
    printf("7: Shuffle an image\n");
    printf("8: Flip an image horizontally\n");
    printf("9: Mirror an image vertically\n");
    printf("10: Add Border to an image\n");
    printf("11: Test all functions\n");
    printf("12: Exit\n");
    printf("Please make your choice: ");
}

void ParseMenuOption(const int* option,
        unsigned char R[WIDTH][HEIGHT],
	    unsigned char G[WIDTH][HEIGHT],
	    unsigned char B[WIDTH][HEIGHT])
{
    if (*option == USER_OPTIONS_LOAD_PPM) {
        printf("Please input the file name to load: ");
        char fileName[SLEN] = {0};
        scanf("%s", fileName);
        
        LoadImage(fileName, R, G, B);
    }
    else if (*option == USER_OPTIONS_SAVE_IMG) {
        printf("Please input the file name to save: ");
        char fileName[SLEN] = {0};
        scanf("%s", fileName);

        SaveImage(fileName, R, G, B);
    }
    else if (*option == USER_OPTIONS_CHANGE_TO_BNW) {
        BlackNWhite(R, G, B);
    }
    else if (*option == USER_OPTIONS_MAKE_NEG) {
        Negative(R, G, B);
    }
    else if (*option == USER_OPTIONS_COLOR_FILTER) {
        int target_r, target_g, target_b, threshold;
        int replace_r, replace_g, replace_b;

        printf("Enter Red   component for the target color: ");
        scanf("%d", &target_r);

        printf("Enter Green component for the target color: ");
        scanf("%d", &target_g);

        printf("Enter Blue  component for the target color: ");
        scanf("%d", &target_b);

        printf("Enter threshold for the color difference: ");
        scanf("%d", &threshold);

        printf("Enter value for Red   component in the target color: ");
        scanf("%d", &replace_r);

        printf("Enter value for Green component in the target color: ");
        scanf("%d", &replace_g);

        printf("Enter value for Blue  component in the target color: ");
        scanf("%d", &replace_b);

        ColorFilter(R, G, B, target_r, target_g, target_b, threshold, replace_r, replace_g, replace_b);
    }
    else if (*option == USER_OPTIONS_SKETCH_EDGE) {
        Edge(R, G, B);
    }
    else if (*option == USER_OPTIONS_SHUFFLE) {
        Shuffle(R, G, B);
    }
    else if (*option == USER_OPTIONS_FLIP_HOR) {
        HFlip(R, G, B);
    }
    else if (*option == USER_OPTIONS_MIRROR_VER) {
        VMirror(R, G, B);
    }
    else if (*option == USER_OPTIONS_ADD_BORDER) {
        int borderWidth;
        printf("Enter border width: ");
        scanf("%d", &borderWidth);

        char optionsMsg[] = "Available border colors : black, white, red, green, blue, yellow, cyan, pink, orange";
        printf("%s\n", optionsMsg);

        char color[SLEN];
        printf("Select border color from the options: ");
        scanf("%s", color);

        AddBorder(R, G, B, color, borderWidth);
    }
    else if (*option == USER_OPTIONS_TEST_ALL_FUNCS) {
        AutoTest(R, G, B);
    }
    else if (*option == USER_OPTIONS_EXIT) {
        printf("Exiting program ...\n");
    }
    else {
        char warning[] = "is not a valid option. Please choose an option from 1 to 12.";
        printf("%d %s\n", *option, warning);
    }
}

void BlackNWhite(unsigned char R[WIDTH][HEIGHT],
		unsigned char G[WIDTH][HEIGHT],
		unsigned char B[WIDTH][HEIGHT])
{
    /* Body of BlackNWhite */
    for (int yPt = 0; yPt < HEIGHT; yPt++) {
        for (int xPt = 0; xPt < WIDTH; xPt++) {
            unsigned char newVal = (R[xPt][yPt] + G[xPt][yPt] + B[xPt][yPt]) / 3;
            R[xPt][yPt] = G[xPt][yPt] = B[xPt][yPt] = newVal;
        }
    }
    
    /* End of BlackNWhite */
    printf("\"Black & White\" operation is done!\n");
}

void Negative(unsigned char R[WIDTH][HEIGHT],
	    unsigned char G[WIDTH][HEIGHT],
	    unsigned char B[WIDTH][HEIGHT])
{
    /* Body of Negative */
    for (int yPt = 0; yPt < HEIGHT; yPt++) {
        for (int xPt = 0; xPt < WIDTH; xPt++) {
            R[xPt][yPt] -= 255;
            G[xPt][yPt] -= 255;
            B[xPt][yPt] -= 255;
        }
    }
    
    /* End of Negative */
    printf("\"Negative\" operation is done!\n");
}

void ColorFilter(unsigned char R[WIDTH][HEIGHT],
		unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT],
		int target_r, int target_g, int target_b, int threshold,
		int replace_r, int replace_g, int replace_b)
{ 
    /* Body of ColorFilter */
    for (int yPt = 0; yPt < HEIGHT; yPt++) {
        for (int xPt = 0; xPt < WIDTH; xPt++) {
            unsigned char currentR = R[xPt][yPt];
            unsigned char currentG = G[xPt][yPt];
            unsigned char currentB = B[xPt][yPt];

            if ( (currentR >= target_r - threshold && currentR <= target_r + threshold)
                && (currentG >= target_g - threshold && currentG <= target_g + threshold)
                && (currentB >= target_b - threshold && currentB <= target_b + threshold) )
            {
                R[xPt][yPt] = (unsigned char)replace_r;
                G[xPt][yPt] = (unsigned char)replace_g;
                B[xPt][yPt] = (unsigned char)replace_b;
            }
            else {
                /* Keep the current color */
            }
        }
    }
    
    /* End of ColorFilter */
    printf("\"Color Filter\" operation is done!\n");
}

void Edge(unsigned char R[WIDTH][HEIGHT],
	    unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT])
{
    /* Body of Edge */

    /* End of Edge */
    printf("\"Edge\" operation is done!\n");
}

void VMirror(unsigned char R[WIDTH][HEIGHT],
	    unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT])
{
    /* Body of VMirror */
    
    /* End of VMirror */
    printf("\"VMirror\" operation is done!\n");
}

void Shuffle(unsigned char R[WIDTH][HEIGHT],
	    unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT])
{
    /* Body of Shuffle */
    
    /* End of Shuffle */
    printf("\"Shuffle\" operation is done!\n");
}

void AddBorder(unsigned char R[WIDTH][HEIGHT],
	    unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT],
	    char color[SLEN], int border_width)
{
    /* Body of AddBorder */

    /* End of AddBorder */
    printf("\"Border\" operation is done!\n");
}

void HFlip(unsigned char R[WIDTH][HEIGHT],
	    unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT])
{
    /* Body of HFlip */

    /* End of HFlip */
    printf("\"HFlip\" operation is done!\n");
}

/* EOF */
