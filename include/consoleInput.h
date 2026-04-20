#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H

//Let the user input in valid datatype form
//The logic: all console input is in string datatype 
// => then validate 
// => then if true, assign value to target variable
// => then if false, throw prompt to let user re-enter


// ===== I. INPUT INT, FLOAT, YES/NO OPTION ================================

// 1.1. Input integer
void inputInteger (int *target, const char * prompt);

// 1.2. Input real number
void inputFloat (float var, const char * prompt);

// 1.3 Input yes no option
void inputYesNo (int option, const char * prompt);

// =========================================================================

#endif