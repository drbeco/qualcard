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
 * @copyright (c) 2016 GNU GPL v2
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
#include <string.h> /* Strings functions definitions */

/* #include <math.h> */ /* Mathematics functions */
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
#define EXTDB ".ex4" /**< Database extension: theme-question-answer.ex4 (example: english-word-definition.ex4) */
#define EXTCF ".cf4" /**< Configuration file extension: user-qualcard.cf4 */

/* Debug */
#ifndef DEBUG /* gcc -DDEBUG=1 */
#define DEBUG 0 /**< Activate/deactivate debug mode */
#endif

/** @brief Debug message if DEBUG on */
#define IFDEBUG(M) if(DEBUG) fprintf(stderr, "[DEBUG file:%s line:%d]: " M "\n", __FILE__, __LINE__); else {;}

/* limits */
#define TAMLINE 1500
#define STRSIZE 256 /**< String buffer size */
#define PEREXEC 10 /**< How many cards presented per round (execution) */
#define NEWEXEC 1 /**< Minimum number of new cards presented per round */

/* ---------------------------------------------------------------------- */
/* globals */

static int verb=0; /**< verbose level, global within the file */

typedef struct scfg /* configuration data struct */
{
    int QTDCARD; /* database size */
    char today[14]; /* yyyymmdd */
    char user[STRSIZE]; /* user name */
    char bdados[STRSIZE], config[STRSIZE]; /* filenames: database and configuration */
    FILE *fbd; /* database file pointer */
    FILE *fcf; /* config file pointer */
} tcfg; /* configuration data type */

/* ---------------------------------------------------------------------- */
/* prototypes */

void help(void); /* print some help */
void copyr(void); /* print version and copyright information */
void qualcard_init(tcfg *cfg); /* global initialization function */
void summary(tcfg c); /* how many cards to review */
int bdsize(FILE *fp); /* database size */
double rand_minmax(double min, double max); /* drawn a number from [min, max[ */
int newcard(tcfg c, char *card); /* drawn a new card */
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
    int SUMMA=0; /* summary only */
    time_t lt;
    struct tm *timeptr;
    /*int QTDCARD; / * database size * /
    char today[14]; / * yyyymmdd * /
    char bdados[STRSIZE], config[STRSIZE], user[STRSIZE]; */
    tcfg c={0};
    /* FILE *fbd=NULL, *fcf=NULL; */
    int l; /* line drawn */
    int i; /* index, auxiliary */
    char card[STRSIZE]; /* card drawn */

    IFDEBUG("Starting optarg loop...\n");

    /* getopt() configured options:
     *        -h  help
     *        -c  copyright & version
     *        -v  verbose
     *        -s  status of reviews
     */
    opterr = 0;
    while((opt = getopt(argc, argv, "vhcsu:")) != EOF)
        switch(opt)
        {
            case 'h': /* exit */
                help();
                break;
            case 'c': /* exit */
                copyr();
                break;
            case 'v':
                verb++;
                break;
            case 's':
                SUMMA=1;
                break;
            case 'u':
                strcpy(c.user, optarg);
                break;
            case '?':  /* exit */
            default:
                printf("Type\n\t$man %s\nor\n\t$%s -h\nfor help.\n\n", argv[0], argv[0]);
                return EXIT_FAILURE;
        }

    if(verb)
        printf("Verbose level set at: %d\n", verb);
    /* ...and we are done */

    qualcard_init(&c); /* initialization function */

    if(SUMMA)
    {
        summary(c); /* how many cards to review */
        exit(EXIT_SUCCESS);
    }


    c.fbd=fopen(c.bdados,"r");
    if(!c.fbd)
    {
        printf("Fail to open database %s.\n", c.bdados);
        exit(EXIT_FAILURE);
    }
    c.QTDCARD=bdsize(c.fbd);

    printf("Today: %s\n", c.today);
    printf("Number of cards: %d\n", c.QTDCARD);
    printf("User: %s\n", c.user);
    printf("DB: %s\n", c.bdados);
    printf("CF: %s\n", c.config);



    for(i=0; i<10; i++)
    {
        l=newcard(c, card);
        printf("Card %4d: %s", l, card);
    }

    if(c.fbd)
        fclose(c.fbd); /* database file */
    if(c.fcf)
        fclose(c.fcf); /* config file */

    return EXIT_SUCCESS;
}


/* database size */
int bdsize(FILE *fp)
{
    char line[TAMLINE];
    int qtdl=0;

    if(!fp)
        return 0;
    fseek(fp, 0, 0 ); /* linha 0 */
    do
    {
        fgets(line, TAMLINE-1, fp);
        qtdl++;
    }while(!feof(fp));
    return --qtdl;
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
void summary(tcfg c)
{
    printf("QualCard - Spaced Repetition - %s\n", c.today);
    printf("User: %s\n", c.user);
    printf("%s: you have %d cards to review today.\n", "English", 5);
    return;
}

/*
 * le um novo cartao aleatorio do arquivo
 * retorna o numero da linha lida (de 0 a FIM-1)
 * e a string lida
 */
int newcard(tcfg c, char *card)
{
    int l, i;
    char line[TAMLINE];

    l=(int)rand_minmax(0.0, c.QTDCARD); /* [= 0, QTDPAD-1 =] */
    fseek(c.fbd, 0, 0);
    for(i=0; i<=l; i++)
        fgets(line, TAMLINE-1, c.fbd);
    strcpy(card, line);
    return l; /* linha do arquivo, de 0 a QTDPAD-1 */
}

/**
 * \brief Drawn a float number in the given interval [min, max[
 * \param[in] min Lower number of the interval (closed, inclusive)
 * \param[in] max Maximum number of the interval (open, exclusive)
 * \param[out] sorteio The number drawn and returned
 * @author Ruben Carlo Benante
 * @version 20160409.000957
 * @date 2016-04-09
 */
double rand_minmax(double min, double max)
{
    double sorte;

    sorte = rand();
    sorte /= ((float)RAND_MAX);
    sorte *= (max-min);
    sorte += min;
    return sorte;
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
void qualcard_init(tcfg *cfg) //(char *td, char *bd, char *cf)
{
    IFDEBUG("qualcard_init()");
    /* initialization */

    time_t lt;
    struct tm *timeptr;
    /* const char *user=getenv("USER"); */
//     const char *bdcore="english-word-definition-test";
    const char *bdcore="english-word-definition";

    lt=time(NULL);
    timeptr=localtime(&lt);
    sprintf(cfg->today, "%04d%02d%02d", 1900 + timeptr->tm_year, 1 + timeptr->tm_mon, timeptr->tm_mday);

    srand((unsigned)time(&lt));

    if(cfg->user[0]=='\0')
        strcpy(cfg->user, getenv("USER"));

    sprintf(cfg->bdados, "%s%s", bdcore, EXTDB);
    // strcpy(bd, "english-word-definition.ex4");
    sprintf(cfg->config, "%s-%s%s", cfg->user, bdcore, EXTCF);
    // strcpy(cf, "beco-english-word-definition-test.cf4");

    return;
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
