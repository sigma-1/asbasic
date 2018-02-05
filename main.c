/* #includes */ /*{{{C}}}*//*{{{*/
#undef  _POSIX_SOURCE
#define _POSIX_SOURCE   1
#undef  _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 2

#include "config.h"

#include <errno.h>
#include <fcntl.h>
#ifdef HAVE_GETTEXT
#include <libintl.h>
#define _(String) gettext(String)
#else
#define _(String) String
#endif
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#include "getopt.h"
#include "asbasic.h"
/*}}}*/

int main(int argc, char *argv[]) /*{{{*/
{
  /* variables */ /*{{{*/
  char *runFile=(char*)0;
  int usage=0,o;
  const char *lp="/dev/null";
  int backslash_colon=0;
  int uppercase=0;
  int restricted=0;
  int lpfd;

  system("clear ; tput cup 9999 0");
  
  static struct option lopts[]=
  {
    { "lp", required_argument, 0, 'l' },
    { "help", no_argument, 0, 'h' },
    { "restricted", no_argument, 0, 'r' },
    { "uppercase", no_argument, 0, 'u' },
    { "backslash-colon", no_argument, 0, 'b' },
    { "version", no_argument, 0, 'V' },
#if defined(__STDC__) && __STDC__
    { (const char*)0, 0, 0, '\0' }
#else
    { (char*)0, 0, 0, '\0' }
#endif
  };
  /*}}}*/

  setlocale(LC_MESSAGES,"");
  setlocale(LC_CTYPE,"");
#ifdef HAVE_GETTEXT
  bindtextdomain("asbasic",LOCALEDIR);
  textdomain("asbasic");
#endif
  /* parse arguments */ /*{{{*/
  while ((o=getopt_long(argc,argv,"+bl:ruVh?",lopts,(int*)0))!=EOF) switch (o)
  {
    case 'b': backslash_colon=1; break;
    case 'l': lp=optarg; break;
    case 'u': uppercase=1; break;
    case 'r': restricted=1; break;
    case 'V': printf("asbasic " VERSION "\n"); exit(0); break;
    case 'h': usage=2; break;
    default: usage=1; break;
  }
  if (optind<argc) runFile=argv[optind++];
  if (usage==1)
  {
    fputs(_("Usage: asbasic [-b] [-l file] [-r] [-u] [program [argument ...]]\n"),stderr);
    fputs(_("       asbasic [--backslash-colon] [--lp file] [--restricted] [--uppercase] [program [argument ...]]\n"),stderr);
    fputs(_("       asbasic -h|--help\n"),stderr);
    fputs(_("       asbasic --version\n"),stderr);
    fputs(  "\n",stderr);
    fputs(_("Try `asbasic -h' or `asbasic --help' for more information.\n"),stderr);
    exit(1);
  }
  if (usage==2)
  {
    fputs(_("Usage: asbasic [-b] [-l file] [-u] [program [argument ...]]\n"),stdout);
    fputs(_("       asbasic [--backslash-colon] [--lp file] [--restricted] [--uppercase] [program [argument ...]]\n"),stdout);
    fputs(_("       asbasic -h|--help\n"),stdout);
    fputs(_("       asbasic --version\n"),stdout);
    fputs("\n",stdout);
    fputs(_("BASIC interpreter.\n"),stdout);
    fputs("\n",stdout);
    fputs(_("-b, --backslash-colon  convert backslashs to colons\n"),stdout);
    fputs(_("-l, --lp               write LPRINT output to file\n"),stdout);
    fputs(_("-r, --restricted       forbid SHELL\n"),stdout);
    fputs(_("-u, --uppercase        output all tokens in uppercase\n"),stdout);
    fputs(_("-h, --help             display this help and exit\n"),stdout);
    fputs(_("    --version          output version information and exit\n"),stdout);
    fputs("\n",stdout);
    fputs(_("Report bugs to <michael@moria.de>.\n"),stdout);
    exit(0);
  }
  if ((lpfd=open(lp,O_WRONLY|O_CREAT|O_TRUNC,0666))==-1)
  {
    fprintf(stderr,_("asbasic: Opening `%s' for line printer output failed (%s).\n"),lp,strerror(errno));
    exit(2);
  }
  asbasic_argc=argc-optind;
  asbasic_argv=&argv[optind];
  asbasic_argv0=runFile;
  /*}}}*/
  asbasic_init(backslash_colon,restricted,uppercase,lpfd);
  if (runFile) asbasic_runFile(runFile);
  else asbasic_interpreter();
  asbasic_exit();
  return(0);
}
/*}}}*/
