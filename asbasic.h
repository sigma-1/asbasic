#ifndef ASBASIC_H
#define ASBASIC_H

#define STDCHANNEL 0
#define LPCHANNEL 32

extern int asbasic_argc;
extern char *asbasic_argv0;
extern char **asbasic_argv;
extern int asbasic_end;

extern void asbasic_init(int backslash_colon, int restricted, int uppercase, int lpfd);
extern void asbasic_runFile(const char *runFile);
extern void asbasic_runLine(const char *runLine);
extern void asbasic_interpreter(void);
extern void asbasic_exit(void);

#endif
