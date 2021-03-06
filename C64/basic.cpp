/*
 * (C) 2006 Dirk W. Hoffmann. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "basic.h"

struct timeval t;
long tv_base = ((void)gettimeofday(&t,NULL), t.tv_sec);

void
printReadable(const void *data, int length)
{
    int i;
    for(i = 0; i < length; i++) {
        char ch = ((char*)(data))[i];
        if (isascii(ch)) {
            fprintf(stderr, "%02x %c ", ch, ch);
        } else {
            fprintf(stderr, "%02x ? ", ch);
        }
        if (i > 0 && i % 16 == 0) {
            fprintf(stderr, "\n");
        }
    }
}

void translateToUnicode(char *petscii, uint16_t *unicode, uint16_t base, size_t max)
{
    unsigned i;
    
    assert(petscii != NULL);
    assert(unicode != NULL);
    
    for (i = 0; i < strlen(petscii) && i < max; i++) {
        unicode[i] = base + (unsigned char)petscii[i];
    }
    unicode[i] = 0;
}

uint8_t
petscii2printable(uint8_t c, uint8_t subst)
{
    if (c >= 0x20 /*' '*/ && c <= 0x7E /* ~ */) return c;
    return subst;
}

uint8_t
ascii2pet(uint8_t asciichar)
{
    if (asciichar == 0x00)
        return 0x00;
    
    asciichar = toupper(asciichar);
    
    if (asciichar >= 0x20 && asciichar <= 0x5D) {
        return asciichar;
    } else {
        return ' ';
    }
}

void
sprint8d(char *s, uint8_t value)
{
    for (int i = 2; i >= 0; i--) {
        uint8_t digit = value % 10;
        s[i] = '0' + digit;
        value /= 10;
    }
    s[3] = 0;
}

void
sprint8x(char *s, uint8_t value)
{
    for (int i = 1; i >= 0; i--) {
        uint8_t digit = value % 16;
        s[i] = (digit <= 9) ? ('0' + digit) : ('A' + digit - 10);
        value /= 16;
    }
    s[2] = 0;
}

void
sprint8b(char *s, uint8_t value)
{
    for (int i = 7; i >= 0; i--) {
        s[i] = (value & 0x01) ? '1' : '0';
        value >>= 1;
    }
    s[8] = 0;
}

void
sprint16d(char *s, uint16_t value)
{
    for (int i = 4; i >= 0; i--) {
        uint8_t digit = value % 10;
        s[i] = '0' + digit;
        value /= 10;
    }
    s[5] = 0;
}

void
sprint16x(char *s, uint16_t value)
{
    for (int i = 3; i >= 0; i--) {
        uint8_t digit = value % 16;
        s[i] = (digit <= 9) ? ('0' + digit) : ('A' + digit - 10);
        value /= 16;
    }
    s[4] = 0;
}

void
sprint16b(char *s, uint16_t value)
{
    for (int i = 15; i >= 0; i--) {
        s[i] = (value & 0x01) ? '1' : '0';
        value >>= 1;
    }
    s[16] = 0;
}

char *
ExtractFilename(const char *path)
{
    assert(path != NULL);
    
    const char *pos = strrchr(path, '/');
    return pos ? strdup(pos + 1) : strdup(path);
}

char *
ExtractSuffix(const char *path)
{
    assert(path != NULL);
    
    const char *pos = strrchr(path, '.');
    return pos ? strdup(pos + 1) : strdup("");
}

char *
ExtractFilenameWithoutSuffix(const char *path)
{
    assert(path != NULL);
    
    char *result;
    char *filename = ExtractFilename(path);
    char *suffix   = ExtractSuffix(filename);
    
    if (strlen(suffix) == 0)
        result = strdup(filename);
    else
        result = strndup(filename, strlen(filename) - strlen(suffix) - 1);
    
    free(filename);
    free(suffix);
    return result;
}

bool
checkFileSuffix(const char *filename, const char *suffix)
{
	assert(filename != NULL);
	assert(suffix != NULL);
	
	if (strlen(suffix) > strlen(filename))
		return false;
	
	filename += (strlen(filename) - strlen(suffix));
	if (strcmp(filename, suffix) == 0)
		return true;
	else
		return false;
}

long
getSizeOfFile(const char *filename)
{
    struct stat fileProperties;
    
    if (filename == NULL)
        return -1;
    
    if (stat(filename, &fileProperties) != 0)
        return -1;
    
    return fileProperties.st_size;
}

bool
checkFileSize(const char *filename, long min, long max)
{
    long filesize = getSizeOfFile(filename);
    
    if (filesize == -1)
        return false;
    
	if (min > 0 && filesize < min)
		return false;

	if (max > 0 && filesize > max)
		return false;

	return true;
}

bool 
checkFileHeader(const char *filename, const uint8_t *header)
{
	int i, c;
	bool result = true;
	FILE *file;

	assert(filename != NULL);
	assert(header != NULL);
	
	if ((file = fopen(filename, "r")) == NULL)
		return false; 

	for (i = 0; header[i] != 0; i++) {
		c = fgetc(file);
		if (c != (int)header[i]) {
			result = false;
			break;
		}
	}
	    
	fclose(file);
	return result;
}

//! Returns elepased time since application start in microseconds
uint64_t 
usec()
{
	struct timeval t;
	gettimeofday(&t,NULL);	
	return (uint64_t)1000000*(uint64_t)(t.tv_sec - tv_base) + (uint64_t)t.tv_usec;
}

uint8_t 
localTimeSec()
{
	time_t t = time(NULL);
	struct tm *loctime = localtime(&t);
	return (uint8_t)loctime->tm_sec;
}

uint8_t 
localTimeMinute()
{
	time_t t = time(NULL);
	struct tm *loctime = localtime(&t);
	return (uint8_t)loctime->tm_min;
}

uint8_t 
localTimeHour()
{
	time_t t = time(NULL);
	struct tm *loctime = localtime(&t);
	return (uint8_t)loctime->tm_hour;
}

	
void 
sleepMicrosec(unsigned usec)
{		
	if (usec > 0 && usec < 1000000) {
		usleep(usec);
	}
}

int64_t
sleepUntil(uint64_t kernelTargetTime, uint64_t kernelEarlyWakeup)
{
    uint64_t now = mach_absolute_time();
    int64_t jitter;
    
    if (now > kernelTargetTime)
        return 0;
    
    // Sleep
    // printf("Sleeping for %d\n", kernelTargetTime - now);
    mach_wait_until(kernelTargetTime - kernelEarlyWakeup);
    
    // Count some sheep to increase precision
    unsigned sheep = 0;
    do {
        jitter = mach_absolute_time() - kernelTargetTime;
        sheep++;
    } while (jitter < 0);
    
    return jitter;
}
