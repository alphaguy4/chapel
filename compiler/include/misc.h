#ifndef _misc_H_
#define _misc_H_

#include "driver.h"

class d_loc_t;
class BaseAST;
class IFAAST;
class Var;

void clean_exit(int status);
#define exit(x) dont_use_exit_use_clean_exit_instead

void gdbShouldBreakHere(void);  // must be exposed to avoid dead-code alim.

int show_error(char *str, d_loc_t &loc, ...);
int show_error(char *str, IFAAST *a, ...);
int show_error(char *str, Var *v, ...);
int buf_read(char *pathname, char **buf, int *len);
char *loc_string(d_loc_t &l);
char *get_file_line(char *filename, int lineno);
char *get_file_line(char *filename, int lineno);


// INT_FATAL(ast, format, ...)
//   where ast         == IFAAST* or NULL
//         format, ... == normal printf stuff
// results in something like:
// INTERNAL ERROR in compilerSrc.c (lineno): your text here (usrSrc:usrLineno)

#define INT_ASSERT(x) \
  do { if (!(x)) INT_FATAL("Assertion Error"); } while (0)

#define INT_FATAL \
  setupError(__FILE__, __LINE__, true, false, false, false, false), printProblem

#define INT_WARN \
  setupError(__FILE__, __LINE__, false, false, true, false, false), printProblem

#define USR_FATAL \
  setupError(__FILE__, __LINE__, true, true, false, false, false), printProblem

#define USR_FATAL_CONT \
  setupError(__FILE__, __LINE__, true, true, true, false, false), printProblem

#define USR_WARN \
  setupError(__FILE__, __LINE__, false, true, true, false, ignore_warnings), printProblem

#define USR_PRINT \
  setupError(__FILE__, __LINE__, false, true, true, true, false), printProblem

#define USR_STOP \
  check_fatal_errors_encountered

void setupError(const char* filename, int lineno,
                bool fatal, bool user, bool cont, bool print, bool ignore);
void printProblem(const char* fmt, ...);
void printProblem(IFAAST* ast, const char* fmt, ...);
void printProblem(BaseAST* ast, const char* fmt, ...);
void check_fatal_errors_encountered(void);

void startCatchingSignals(void);
void stopCatchingSignals(void);

#endif
