#ifndef UTILS_H
#define UTILS_H

static const char CHARS[] =
	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

unsigned long getRandomSeed();
void getRandomString(char value[], int length);

#endif
