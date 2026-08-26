


// whether or not to use a shift register for input, otherwise single input on the data pin
// #define USE_SHIFT_REG 1
#ifndef USE_SHIFT_REG
    #define USE_SHIFT_REG 0
#endif

const unsigned long DEBOUNCE_DELAY = 50; // debounce time in ms

// how many encoder ticks per scroll
#define TICKS_PER_SCROLL 4

   
// These should be implemented for more flexibility
// #define PROFILES_PER_PAGE 4
// #define COMMANDS_PER_PAGE 8



// i just realized i never called register setup