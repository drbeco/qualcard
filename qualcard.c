/***************************************************************************
 *   qualcard.c                               Version 20160409.000957      *
 *                                                                         *
 *   Learn cards by Spaced Repetition Method.                              *
 *   This program helps you learn from a set of cards with questions       *
 *   and answers.                                                          *
 *   Copyright (C) 2016         by Ruben Carlo Benante                     *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *   To contact the author, please write to:                               *
 *   Ruben Carlo Benante                                                   *
 *   Email: rcb@beco.cc                                                    *
 *   Webpage: http://www.beco.cc                                           *
 *   Phone: +55 (81) 3184-7555                                             *
 ***************************************************************************/

/* ---------------------------------------------------------------------- */
/**
 * \name GroupUnique
 * @{ */

/* @ingroup GroupUnique */
/**
 * @file qualcard.c
 * @brief Learn cards by Spaced Repetition Method
 * @details This program helps you learn from a set of cards with questions and answers
 * @version 20160409.000957
 * @date 2016-04-09
 * @author Ruben Carlo Benante <<rcb@beco.cc>>
 * @par Webpage
 * <<a href="http://www.beco.cc">www.beco.cc</a>>
 * @copyright (c) 2016 GNU GPL v3
 * @note This program is free software: you can redistribute it
 * and/or modify it under the terms of the
 * GNU General Public License as published by
 * the Free Software Foundation version 2 of the License.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA. 02111-1307, USA.
 * Or read it online at <<http://www.gnu.org/licenses/>>.
 *
 *
 * @todo Now that you have the template, hands on! Programme!
 * @warning Be carefull not to lose your mind in small things.
 * @bug This file right now does nothing usefull
 *
 */

/* ---------------------------------------------------------------------- */
/* includes */

#include <stdio.h> /* Standard I/O functions */
#include <stdlib.h> /* Miscellaneous functions (rand, malloc, srand)*/
#include <getopt.h> /* get options from system argc/argv */
#include <time.h> /* Time and date functions */

/* #include <math.h> */ /* Mathematics functions */
/* #include <string.h> */ /* Strings functions definitions */
/* #include <assert.h> */ /* Verify assumptions with assert */
/* #include <dlfcn.h> */ /* Dynamic library */
/* #include <malloc.h> */ /* Dynamic memory allocation */
/* #include <unistd.h> */ /* UNIX standard function */
/* #include <limits.h> */ /* Various C limits */
/* #include <ctype.h> */ /* Character functions */
/* #include <errno.h> */ /* Error number codes errno */
/* #include <signal.h> */ /* Signal processing */
/* #include <stdarg.h> */ /* Functions with variable arguments */
/* #include <pthread.h> */ /* Parallel programming with threads */
/* #include <fcntl.h> */ /* File control definitions */
/* #include <termios.h> */ /* Terminal I/O definitions POSIX */
/* #include <sys/stat.h> */ /* File status and information */
/* #include <float.h> */ /* Float constants to help portability */
/* #include <setjmp.h> */ /* Bypass standard function calls and return */
/* #include <stddef.h> */ /* Various types and MACROS */
/* #include <SWI-Prolog.h> */ /* Prolog integration with C */
/* #include <ncurses.h> */ /* Screen handling and optimisation functions */
/* #include <allegro.h> */ /* A game library for graphics, sounds, etc. */
/* #include <libintl.h> */ /* Internationalization / translation */
/* #include <locale.h> */ /* MACROS LC_ for location specific settings */
/* #include "libeco.h" */ /* I/O, Math, Sound, Color, Portable Linux/Windows */
/* #include "qualcard.h" */ /* To be created for this template if needed */

/* ---------------------------------------------------------------------- */
/* definitions */

/* #define BUILD (20160409.000957) / * * < Build Version Number */
#define PEREXEC 10 /**< How many cards presented per round (execution) */
#define NEWEXEC 1 /**< Minimum number of new cards presented per round */
#define DBEXT ".ex4" /**< Database extension: theme-question-answer.ex4 (example: english-word-definition.ex4) */
#define CFEXT ".cf4" /**< Configuration file extension: user-qualcard.cf4 */

/* Debug */
#ifndef DEBUG /* gcc -DDEBUG=1 */
#define DEBUG 0 /**< Activate/deactivate debug mode */
#endif

/** @brief Debug message if DEBUG on */
#define IFDEBUG(M) if(DEBUG) fprintf(stderr, "[DEBUG file:%s line:%d]: " M "\n", __FILE__, __LINE__); else {;}

/* limits */
#define SBUFF 256 /**< String buffer */

/* ---------------------------------------------------------------------- */
/* globals */

static int verb=0; /**< verbose level, global within the file */

/* ---------------------------------------------------------------------- */
/* prototypes */

void help(void); /* print some help */
void copyr(void); /* print version and copyright information */
void qualcard_init(void); /* global initialization function */
void summary(void); /* how many cards to review */
int funcexample(int i, int *o, int *z); /* just an example with complete doxygen fields */

/* ---------------------------------------------------------------------- */
/* @ingroup GroupUnique */
/**
 * @brief This is the main event of the evening
 * @details Ladies and Gentleman... It's tiiiime!
 * Fightiiiiing at the blue corner,
 * he, who has compiled more C code than any other adversary in the history,
 * he, who has developed UNIX and Linux, and is an inspiration to maaany languages
 * and compilers, the GNU C Compiler, GCC!
 * Fightiiiiing at the red corner, the challenger, in his first fight, lacking of any
 * valid experience but angrily, blindly, and no doubtfully, will try to
 * compile this program without errors. He, the student, the apprentice,
 * the developer, beco!!
 *
 * @param[in] argc Argument counter
 * @param[in] argv Argument strings (argument values)
 *
 * @retval 0 If succeed (EXIT_SUCCESS).
 * @retval 1 Or another error code if failed.
 *
 * @par Example
 * @code
 *    $./qualcard -h
 * @endcode
 *
 * @warning   Be carefull with...
 * @bug There is a bug with...
 * @todo Need to do...
 * @note You can read more about it at <<a href="http://www.beco.cc">www.beco.cc</a>>
 * @author Ruben Carlo Benante
 * @version 20160409.000957
 * @date 2016-04-09
 *
 */
int main(int argc, char *argv[])
{
  int opt; /* return from getopt() */

  qualcard_init(); /* initialization function */

  IFDEBUG("Starting optarg loop...\n");

  /* getopt() configured options:
   *        -h  help
   *        -c  copyright & version
   *        -v  verbose
   *        -s  status of reviews
   */
  opterr = 0;
  while((opt = getopt(argc, argv, "vhcs")) != EOF)
    switch(opt)
    {
      case 'h':
        help();
        break;
      case 'c':
        copyr();
        break;
      case 'v':
        verb++;
        break;
      case 's':
          summary(); /* how many cards to review */
          break;
      case '?':
      default:
        printf("Type\n\t$man %s\nor\n\t$%s -h\nfor help.\n\n", argv[0], argv[0]);
        return EXIT_FAILURE;
    }

  if(verb)
    printf("Verbose level set at: %d\n", verb);
  /* ...and we are done */
  return EXIT_SUCCESS;
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief Prints the summary and exit
 * @details Prints how many cards to review in each database
 * @return Void
 * @author Ruben Carlo Benante
 * @version 20160409.000957
 * @date 2016-04-09
 *
 */
void summary(void)
{
    printf("QualCard - Spaced Repetition\n");
    printf("%s: you have %d cards to review today.\n", "English", 5);
    exit(EXIT_SUCCESS);
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief Prints help information and exit
 * @details Prints help information (usually called by opt -h)
 * @return Void
 * @author Ruben Carlo Benante
 * @version 20160409.000957
 * @date 2016-04-09
 *
 */
void help(void)
{
  IFDEBUG("help()");
  printf("%s - %s\n", "qualcard", "Learn cards by Spaced Repetition Method");
  printf("\nUsage: %s [-h|-v]\n", "qualcard");
  printf("\nOptions:\n");
  printf("\t-h,  --help\n\t\tShow this help.\n");
  printf("\t-c,  --version\n\t\tShow version and copyright information.\n");
  printf("\t-v,  --verbose\n\t\tSet verbose level (cumulative).\n");
  printf("\t-s,  --status\n\t\tShow how many cards needs review in each database.\n");
  printf("\t-u username,  --user username\n\t\tUse the username's profile.\n");
  /* add more options here */
  printf("\nExit status:\n\t0 if ok.\n\t1 some error occurred.\n");
  printf("\nTodo:\n\tLong options not implemented yet.\n\t-u username not implemented yet.\n");
  printf("\nAuthor:\n\tWritten by %s <%s>\n\n", "Ruben Carlo Benante", "rcb@beco.cc");
  exit(EXIT_FAILURE);
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief Prints version and copyright information and exit
 * @details Prints version and copyright information (usually called by opt -V)
 * @return Void
 * @author Ruben Carlo Benante
 * @version 20160409.000957
 * @date 2016-04-09
 *
 */
void copyr(void)
{
  IFDEBUG("copyr()");
  printf("%s - Version %s\n", "qualcard", VERSION);
  printf("\nCopyright (C) %d %s <%s>, GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>. This  is  free  software:  you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law. USE IT AS IT IS. The author takes no responsability to any damage this software may inflige in your data.\n\n", 2016, "Ruben Carlo Benante", "rcb@beco.cc");
  if(verb>3) printf("copyr(): Verbose: %d\n", verb); /* -vvvv */
  exit(EXIT_FAILURE);
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief This function initializes some operations before start
 * @details Details to be written
 * @return Void
 * @todo Need to implement it. Its empty now.
 * @author Ruben Carlo Benante
 * @version 20160409.000957
 * @date 2016-04-09
 *
 */
void qualcard_init(void)
{
  IFDEBUG("qualcard_init()");
  /* initialization */
  return;
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief This function does bla bla bla
 * @details It works by doing first a bla, followed by two bla's, giving
 * bla bla bla
 *
 * @pre You need to call foo() before calling this function
 *
 * @param[in] i Input parameter that does bla
 * @param[out] o Parameter that outputs ble
 * @param[in,out] z The @a z variable is used as input and output
 *
 * @return The returned value @a r means rhit
 * @retval 0 Returned when bli
 * @retval 1 Error code: function returned blo
 *
 * @par Example
 * @code
 *    if(x==funcexample(i, o, z))
 *       printf("And that is it\n");
 * @endcode
 *
 * @warning   Be carefull with blu
 * @deprecated This function will be deactivated in version +33
 * @see help()
 * @see copyr()
 * @bug There is a bug with x greater than y
 * @todo Need to change its output format to conform POSIX
 * @note You can read more about it at <<a href="http://www.beco.cc">www.beco.cc</a>>
 * @author Ruben Carlo Benante
 * @version 20160409.000957
 * @date 2016-04-09
 * @copyright Only if not the same as the whole file
 *
 */
int funcexample(int i, int *o, int *z)
{
  IFDEBUG("funcexample()");
  i += *z;
  *o = (*z)++;
  return i-4;
}

/**  @} */
/* End of GroupUnique */
/* ---------------------------------------------------------------------- */
/* vi: set ai et ts=2 sw=2 tw=0 wm=0 fo=croql : C config for Vim modeline */
/* Template by Dr. Beco <rcb at beco dot cc> Version 20150619.231433      */
