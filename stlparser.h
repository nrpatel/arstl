#ifndef _STLPARSER_H_
#define _STLPARSER_H_

/* The array returned is single dimensional, with 12 floats per face in the 
 * STL. The first 3 represent the normal, and the remaining 9 are the vertices.
 * NULL is returned if parsing fails or memory cannot be allocated.
 */
 
// Just opens the file and returns an allocated buffer of its contents
char *load_file(const char *filename, int *len);
// Opens the file at filename and returns an allocated array of floats
float *load_stl(const char *filename, int *facets);
// Parses the data, determines if it is a binary or string, and returns floats
float *parse_stl(const char *data, int data_length, int *facets);
// Parses an ASCII STL string and returns the array of floats
float *parse_stl_string(const char *data, int data_length, int *facets);
// Parses an STL binary and returns the array of floats
float *parse_stl_binary(const char *data, int data_length, int *facets);

#endif /* _STLPARSER_H_ */
