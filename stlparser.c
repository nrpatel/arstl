/* To the extent possible under law, Nirav Patel has waived all copyright
 * and related or neighboring rights to stlparser. 
 */

#include "stlparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

char *load_file(const char *filename, int *len)
{
    char *buf = NULL;
    FILE *fp;
    
    if (!(fp = fopen(filename, "r"))) {
        return NULL;
    }
    
    fseek(fp, 0, SEEK_END);
    *len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buf = (char *)malloc(*len);
    
    *len = fread(buf, 1, *len, fp);
    fclose(fp);
    
    return buf;
}

// returns 1 if the data looks like a binary stl
static int stl_binary_check(const char *data, int data_length)
{
    // STL ASCII files should start with "solid"
    if (data_length >= 5 && (strncasecmp(data, "solid", 5) == 0)) {
        return 0;
    } else if (data_length >= 84) {
        uint32_t facets = *(uint32_t *)(data+80);
        // A check suggested from thingiview.js, since ASCII STLs can be
        // malformed.  Checks if the binary STL's triangle count matches
        // the file length plus a little fuzz in case there is end padding
        if (abs(data_length - (facets*50 + 84)) < 10) {
            return 1;
        }
    }

    return 0;
}

// Opens the file at filename and returns an allocated array of floats
float *load_stl(const char *filename, int *facets)
{
    int data_length = 0;
    char *data = load_file(filename, &data_length);
    if (!data) {
        fprintf(stderr, "Could not load file %s\n", filename);
        return NULL;
    }
    
    return parse_stl(data, data_length, facets);
}

// Parses the data, determines if it is a binary or string, and returns floats
float *parse_stl(const char *data, int data_length, int *facets)
{
    if (stl_binary_check(data, data_length)) {
        return parse_stl_binary(data, data_length, facets);
    } else {
        return parse_stl_string((char *)data, data_length, facets);
    }
}

// Parses an ASCII STL string and returns the array of floats
float *parse_stl_string(const char *data, int data_length, int *facets)
{
    int faces = 0;
    float *parsed = NULL;
    char *loc = (char *)data;
    // Find the number of faces first
    if ((loc = strstr(loc, "endfacet"))) {
        faces = 1;
        while((loc = strstr(loc+sizeof("endfacet"), "endfacet"))) faces++;
    } else {
        return NULL;
    }
    
    parsed = (float *)malloc(faces*12*4);
    if (!parsed) return NULL;
    
    *facets = faces;
    // Go past the name line
    if (!(loc = strchr((char *)data, '\n'))) {
        free(parsed);
        return NULL;
    }
    float x, y, z;
    float *ptr = parsed;
    while (1) {
        // Parse each face
        if (!(loc = strstr(loc+sizeof("vertex"),"normal"))) break;
        if (sscanf(loc, "normal %f %f %f", &x, &y, &z) != 3) break;
        *ptr++ = x;
        *ptr++ = y;
        *ptr++ = z;
        if (!(loc = strstr(loc+sizeof("normal"),"vertex"))) break;
        if (sscanf(loc, "vertex %f %f %f", &x, &y, &z) != 3) break;
        *ptr++ = x;
        *ptr++ = y;
        *ptr++ = z;
        if (!(loc = strstr(loc+sizeof("normal"),"vertex"))) break;
        if (sscanf(loc, "vertex %f %f %f", &x, &y, &z) != 3) break;
        *ptr++ = x;
        *ptr++ = y;
        *ptr++ = z;
        if (!(loc = strstr(loc+sizeof("normal"),"vertex"))) break;
        if (sscanf(loc ,"vertex %f %f %f", &x, &y, &z) != 3) break;
        *ptr++ = x;
        *ptr++ = y;
        *ptr++ = z;
    }
    
    return parsed;
}

// Parses an STL binary and returns the array of floats
float *parse_stl_binary(const char *data, int data_length, int *facets)
{
    int faces = 0;
    float *parsed = NULL;

    if (!facets || data_length <= 84) return NULL;
    
    data += 80;
    faces = *(uint32_t *)(data);
    data += 4;
    
    // Make sure the data provided can actually contain the number of faces
    if (faces*50+84 < data_length) return NULL;
    
    parsed = (float *)malloc(faces*12*4);
    if (!parsed) return NULL;
    
    *facets = faces;
    float *ptr = parsed;
    while (faces > 0) {
        memcpy(ptr, data, 12*4);
        data += 50;
        ptr += 12;
        faces--;
    }
    
    return parsed;
}
