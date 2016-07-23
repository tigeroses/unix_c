/*
 * bounce.h
 * some settings for the game
 */
#define BLANK   ' '
#define X_BUND '_'
#define XX_BUND '-'
#define Y_BUND '|'
#define DFL_SYMBOL 'o'
#define TOP_ROW 5
#define BOT_ROW 25
#define ERROR 20
#define LEFT_EDGE 10
#define RIGHT_EDGE 70
#define X_INIT 10
#define Y_INIT 10
#define TICKS_PER_SEC 50

#define X_TTM 5
#define Y_TTM 8

struct ppball
{
    int y_pos, x_pos,
        y_ttm, x_ttm,
        y_ttg, x_ttg,
        y_dir, x_dir;
    char symbol;
};

struct board
{
    int left, row, length, how;
};
