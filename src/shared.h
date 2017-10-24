#ifdef ARDUINO
    #include <stdbool.h>
    #include <stdint.h>
#else
    #include <a2x.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SHARED_UP,
    SHARED_DOWN,
    SHARED_LEFT,
    SHARED_RIGHT,
    SHARED_NUM
} SharedButtonType;

#ifdef ARDUINO
    typedef uint8_t SharedButton;
#else
    typedef AInputButton* SharedButton;
#endif

extern SharedButton g_buttons[SHARED_NUM];

extern void shared_setup(void);

extern bool shared_pressed(SharedButton Button);

extern int shared_getWidth(void);
extern int shared_getHeight(void);
extern void shared_fill(bool White);
extern void shared_rect(int X, int Y, int W, int H, bool White);

#ifdef __cplusplus
}
#endif
