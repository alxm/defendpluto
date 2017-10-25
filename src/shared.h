#ifdef ARDUINO
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdint.h>
#else
    #include <a2x.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    S_BUTTON_UP,
    S_BUTTON_DOWN,
    S_BUTTON_LEFT,
    S_BUTTON_RIGHT,
    S_BUTTON_A,
    S_BUTTON_B,
    S_BUTTON_NUM
} SButtonType;

#ifdef ARDUINO
    typedef uint8_t SButton;
#else
    typedef AInputButton* SButton;
#endif

extern SButton s_buttons[S_BUTTON_NUM];

extern void s_setup(void);

extern bool s_button_pressed(SButton Button);

extern int s_screen_getWidth(void);
extern int s_screen_getHeight(void);

extern void s_draw_fill(bool White);
extern void s_draw_rectangle(int X, int Y, int W, int H, bool White);
extern void s_draw_pixel(int X, int Y, bool White);

#ifdef __cplusplus
}
#endif
