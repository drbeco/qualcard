/***************************************************************************
 *   qualcard.c                                                Version 1.7 *
 *                                                                         *
 *   Learn cards by Spaced Repetition Method.                              *
 *   This program helps you learn from a set of cards with questions       *
 *   and answers.                                                          *
 *   Copyright (C) 2016-2022+   by Ruben Carlo Benante                     *
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
#include <dirent.h> /* Defines directory entries */
#include <assert.h> /* Verify assumptions with assert */
#include <unistd.h> /* UNIX standard function */
#include <sys/stat.h> /* File status and information */
#include <errno.h> /* Error number codes errno */
#include <math.h> /* Math functions */

/* ---------------------------------------------------------------------- */
/* definitions */

/* #define BUILD (20160409.000957) / * * < First build Version Number */
#define EXTDB ".ex4" /**< Database extension: theme-question-answer.ex4 (example: english-word-definition.ex4) */
#define EXTCF ".cf4" /**< History file, extension: user-qualcard.cf4 */
#define CFGINI "qualcard.ini" /*<< Configuration file */
#define SCOREA 4.92
#define SCOREB 3.90
#define SCOREC 3.05
#define SCORED 2.20
#define SCOREE 1.35
#define SCOREF 0.60
#define TMEM 0 /**< tencards memory index */
#define TFIL 1 /**< tencards file index (card number) */
#define TEND -2 /**< tencards end of list */
#define TVAL -1 /* not in memory, but valid new card from file */

/* randnorep() input: mode */
#define DRAWBASKET   -1 /**< MODE set randnorep() to draw a item from the basket */
#define LISTBASKET   -2 /**< MODE set randnorep() to list itens in the basket */
#define REMOVEBASKET -3 /**< MODE set randnorep() to remove a specific item from basket */
#define FILLBASKET   -4 /**< MODE set randnorep() to fill basket with new numbers */
/* randnorep() output: error codes returned */
#define BASKETOK      0 /**< randnorep() return code for no error */
#define BASKETEMPTY  -1 /**< randnorep() return code for empty list */
#define BASKETLISTED -2 /**< randnorep() return code for all listed */
#define BASKETERROR  -3 /**< randnorep() return code for other error */
#define BASKETFULL   -4 /**< randnorep() return code for full basket */

/* VERSION */
#ifndef VERSION /* gcc -DDEBUG=1 */
#define VERSION "1.9.20250410.111416" /**< VERSION comes from makefile*/
#endif

/* Debug */
#ifndef DEBUG /* gcc -DDEBUG=1 */
#define DEBUG 0 /**< Activate/deactivate debug mode */
#endif

/** @brief Debug message if DEBUG on */
#define IFDEBUG(M) if(DEBUG) fprintf(stderr, "[DEBUG file:%s line:%d]: " M "\n", __FILE__, __LINE__); else {;}

/* limits */
#define STRSIZE 1500 /**< String buffer size */
#define STROPT 100 /**< String size for options INI */
#define SOPT 4 /* opt string '99\n\0' */
#define PATHSIZE 1600 /**< Maximum $PATH size */
#define DTSIZE 9 /**< String with yyyymmdd */
#define PEREXEC 10 /**< How many cards presented per round (execution) */
#define NEWEXEC 1 /**< Minimum number of new cards presented per round */

/* completed() function for score */
#define COMPLSTRT 0  /* completed() start */
#define COMPLCONT 1  /* completed() continue */
#define COMPLSTOP 2  /* completed() stop */

/* ---------------------------------------------------------------------- */
/* globals */

static int verb = 0; /**< verbose level, global within the file */
static int SUMMA = 0; /**< print summary only and exit */
static int DBNUM = 0; /**< pick a database (ex4) by number from command line */

typedef struct scfg /* configuration data struct */
{
    int QTDCARD; /* database size */
    int today; /* yyyymmdd */
    time_t tstart; /* start time (s) of this session */
    double session; /* accumulated time of all session(s) */
    char dbpath[PATHSIZE]; /* path to commom database directory */
    char cfgrealpath[PATHSIZE]; /* path to own config directory with config (ini), history (cf4) and databases (ex4), read and write */
    char cfguserpath[PATHSIZE]; /* path to some other user config directory (just for read) */
    char pathuser[STRSIZE]; /* pathuser/.config another user name account for read only */
    char fileuser[STRSIZE]; /* fileuser-theme-question-answer.cf4 history file for username currently practicing */
    char realuser[STRSIZE]; /* user name of the account */
    char dbasef[STRSIZE], histf[STRSIZE]; /* current filenames: database (ex4) and history (cf4) */
    int *cfcard, *cfdate; /* card num, last date */
    double *cfave; /* card average */
    int cfsize; /* size of history file, number of cards */
    char **dbfiles; /* char dbfiles[number of ex4 files][string lenght of the biggest];*/
    int dbfsize; /* number of database ex4 files */
    int invert; /* if true, print first the back, then the front of the card */
    int option[2]; /* [0]:order=[0:sort, 1:random], [1]:score=[0:after, 1:before] */
} tcfg; /* configuration data type */

/* ---------------------------------------------------------------------- */
/* prototypes */

void help(void); /* print some help */
void copyr(void); /* print version and copyright information */
void qualcard_init(tcfg *cfg); /* global initialization function */
void summary(tcfg c); /* how many cards to review */
double randmm(double min, double max); /* drawn a number from [min, max[ */
int ave2day(double ave); /* given an average, return how many days for next review */
int newdate(int oldd, int days); /* add days to a date */
char *prettydate(int somedate); /* return date in a pretty format */
void readcfg(tcfg *c); /* read history file */
void *reallocordie(void *ptr, size_t sz); /* resize the memory block of a pointer or die */
void readdbfiles(tcfg *c); /* read files from directory and create a dynamic vector of strings */
char *theme(char *file); /* take the theme from a database file name */
int newcard(tcfg c, int tencards[10][2]); /* drawn a new card */
void select10cards(tcfg *c, int tencards[10][2]); /* select 10 cards (old or new) to be presented */
void sortmemo(tcfg *c); /* prioritary (old) comes first (selection sort) */
void getcard(char *dbfile, int cardnum, char *cardfr, char *cardbk); /* given a card number, get it from file */
void cardfaces(char *card, char *fr, char *bk); /* get card faces front/back */
void save2memo(tcfg *c, int i, int card, double scor); /* save new or update old card in memory */
void save2file(tcfg c); /* save updated cards in memory to history file */
int dbsize(char *dbname); /* database ex4 size */
void cfanalyses(char *histfile, int today, int qtd, int *view, int *learn, double *pct, double *addscore, int *ncardl); /* analyses a history file */
void createcfgdir(tcfg *c); /* creates /home/user/.config/qualcard/ */
char *filenopath(char *filepath); /* get filename with no path */
int randnorep(int mode, int *n); /* drawn numbers from a list with no repetition */
void changebarnet(char *s); /* change \n and \t to the real thing */
void changecolon(char *nt); /* change char 254 back to a colon sign */
int diffdays(int newd, int oldd); /* Difference of two dates in days. Positive if new>old, 0 if equals, negative c.c. */
void menudb(tcfg *c); /* read menu */
char *dbcore(char *s); /* grab the core name of the file */
void sessiontime(tcfg *c); /* calculates duration of this session and accumulated time */
double getactime(FILE *fp); /* read time if exists. points to first card stat */
void sstrip(char *s); /* remove \n \t and double spaces from string */
void setoption(int optini[2], char *optarg); /* read option from command line and update vector of options in memory */
void readini(tcfg *c); /* read INI file options */
void cmpoption(tcfg *c, int mopt[2]); /* compare command line options with INI file and save if necessary */
double score(double ave, int late); /* return the new score when the revision is late */
double completed(int start, double ave, int late, int qtd); /* return a percentage 0..100 till the deck is finished */

/* ---------------------------------------------------------------------- */
/* @ingroup GroupUnique */
/**
 * @brief Main function, loops the cards, save and exit
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
    char sopt[SOPT]; /* string opt */
    char *p; /* strchr remove \n from sopt */
    int opt; /* return from getopt() and user options */
    tcfg c = {0}; /* struct to configuration variables */
    int i; /* index, auxiliary */
    int newd; /* new date after adding up day's equivalent score */
    int futd; /* future revision date after presenting today */
    int tencards[10][2]; /* ten cards, index in memory (-1 if new), line in file */
    int again = 1; /* while some card score presented is still zero */
    int repet[10] = {0}; /* which card repeated how many times */
    double sco, oldsco; /* the score to a card */
    char cardfr[STRSIZE], cardbk[STRSIZE]; /* card front and back */
    int memopt[2] = {-1, -1}; /* options in memory to compare to INI file */

    IFDEBUG("Starting optarg loop...\n");

    /* getopt() configured options:
     *        -h                Help.
     *        -c                Copyright & version.
     *        -v                Increase verbose.
     *        -q                Quiet, decrease verbose.
     *        -s                Status of all database reviews. Together with -n N shows only the
     *                              status of a single chosen database.
     *        -u username       Set the player name (default: whoami).
     *        -p username       Set the username for config path (together with -s only).
     *        -d database       Set the database to use (default: ask).
     *        -n N              Pick a database number N from command line. With -s, print status of
     *                              that specific database.
     *        -i                Invert presentation order (first the back, then the front of the card).
     *
     *        -e OPTION=VALUE   Set OPTION. Options are: order=[sort|random], score=[before|after]
     *                          Example: qualcard -e order=sort -e score=after
     *                          Options are saved into ~/.config/qualcard/qualcard.ini
     */
    opterr = 0;
    while((opt = getopt(argc, argv, "hcvqsp:u:d:in:e:")) != EOF)
        switch(opt)
        {
            case 'h': /* help and exit */
                help();
                break;
            case 'c': /* copyright and exit */
                copyr();
                break;
            case 'v': /* verbose */
                verb++;
                break;
            case 'q': /* quiet */
                verb--;
                break;
            case 's': /* summary */
                SUMMA = 1;
                break;
            case 'u': /* username */
                strncpy(c.fileuser, optarg, STRSIZE);
                break;
            case 'p': /* username for other account (path) */
                strncpy(c.pathuser, optarg, STRSIZE);
                break;
            case 'i': /* invert */
                c.invert = 1;
                break;
            case 'd': /* database */
                strncpy(c.dbasef, optarg, STRSIZE);
                break;
            case 'n': /* database number */
                DBNUM = strtol(optarg, NULL, 10);
                if(DBNUM == 0)
                {
                    help();
                    printf("\nError: wrong database number on arg -n N\n");
                    /* exit(EXIT_FAILURE); */
                }
                break;
            case 'e': /* options -e order=sort/random -e score=after/before */
                setoption(memopt, optarg); /* read option from command line and update vector of options in memory */
                break;
            case '?':  /* wrong option, exit */
            default:
                printf("Type\n\t$man %s\nor\n\t$%s -h\nfor help.\n\n", argv[0], argv[0]);
                return EXIT_FAILURE;
        }

    if(verb >= 0)
    {
        printf("QualCard v.%s - Spaced Repetition", VERSION);
        if(verb > 0)
            printf(", by Dr. Beco");
        printf("\n");
    }

    if(verb > 1)
        printf("Verbose level set at: %d\n", verb);

    if(c.pathuser[0] != '\0' && SUMMA != 1)
    {
        if(verb > 1)
            printf("Option -p must be used with -s. Setting -s forcefully. Use -h for help.\n");
        SUMMA = 1;
    }

    qualcard_init(&c); /* initialization function */
    cmpoption(&c, memopt); /* compare command line options with INI file and save if necessary */

    if(DBNUM != 0)
    {
        if(verb > 1)
            printf("Used option: -n %d\n", DBNUM);
        if(DBNUM < 1 || DBNUM > c.dbfsize)
        {
            printf("Database %d not found!\n", DBNUM);
            exit(EXIT_FAILURE);
        }
    }

    summary(c); /* how many cards to review */

    if(SUMMA)
        exit(EXIT_SUCCESS);

    /* TODO: read dbasef from system databases with:
     *    -d english
     */
    menudb(&c);

    if(verb > 2)
    {
        printf("\nDataBase file: %s (%d cards)\n", filenopath(c.dbasef), c.QTDCARD);
        printf("History  file: %s\n", filenopath(c.histf));
    }

    readcfg(&c); /* read card stats from chosen dbfile */
    select10cards(&c, tencards);

    printf("\n");
    while(again)
    {
        again = 0;
        for(i = 0; i < 10; i++)
        {
            if(tencards[i][TMEM] == TEND) /* already presented and ok */
                continue;
            getcard(c.dbasef, tencards[i][TFIL], cardfr, cardbk);

            printf("-----------------------------------------------------------\n");
            if(TVAL == tencards[i][TMEM]) /* new card? */
            {
                printf("Card %d (new card for today)\n\n", tencards[i][TFIL] + 1);
                oldsco = 0.0;
            }
            else
            {
                oldsco = c.cfave[tencards[i][TMEM]];
                newd = newdate(c.cfdate[tencards[i][TMEM]], ave2day(oldsco));
                printf("Card %d (revision date %s)\n\n", tencards[i][TFIL] + 1, prettydate(newd));
            }

            if(c.invert)
                printf("%s\n", cardbk); /* fgets already get one \n */
            else
                printf("%s\n\n", cardfr);


            printf("---------------Press <ENTER> to turn the card--------------\n");
            do opt = getchar();
            while(opt != '\n');

            if(c.invert)
                printf("%s\n\n", cardfr);
            else
                printf("%s\n", cardbk);  /* fgets already get one \n */

            do
            {
                if(c.option[1]==0) /* INI score=after */
                    printf("Your self-evaluation (from 0 to 5, default 0) is: ");
                else /* INI score=before */
                    printf("Current score: %.1f  Self-evaluation ([0] ... 5): ", oldsco);
                /* scanf("%d%*c", &opt); /1* discard the '\n'. Better use fgets() *1/ */
                fgets(sopt, SOPT, stdin);
                if((p = strchr(sopt, '\n'))) * p = '\0';
                if(sopt[0] == '\0') /* pressing <enter> means evaluating to 0 */
                {
                    opt = 0;
                    break;
                }
                opt = strtol(sopt, NULL, 10);
            }
            while(opt < 0 || opt > 5);
            if(!opt)
            {
                repet[i]++;
                again = 1;
            }
            else
            {
                if(repet[i])
                    sco = (double)opt / ((double)repet[i] + 1.0);
                else
                    sco = (double)opt;
                save2memo(&c, tencards[i][TMEM], tencards[i][TFIL], sco); /* update card in memory */
                if(tencards[i][TMEM] == TVAL)
                    tencards[i][TMEM] = c.cfsize - 1;
                futd = newdate(c.today, ave2day(c.cfave[tencards[i][TMEM]]));
                if(c.option[1]==0) /* INI score=after */
                    printf("Old score: %.1f, new score: %.1f, revision set to %s\n", oldsco, c.cfave[tencards[i][TMEM]], prettydate(futd));
                else /* INI score=before */
                    printf("New score: %.1f  Revision set to %s\n", c.cfave[tencards[i][TMEM]], prettydate(futd));
                tencards[i][TMEM] = TEND; /* presented and ok */
            }
        } /* for i < 10 cards */
    } /* while(again) */
    printf("-----------------------------------------------------------\n");

    sessiontime(&c); /* calculates duration of this session and accumulated time */
    save2file(c);

    printf("\nLive as if you were to die tomorrow.\nLearn as if you were to live forever.  (Mahatma Gandhi)\n");
    printf("Thank you for using QualCard version %s.\n\n", VERSION);
    return EXIT_SUCCESS;
}

/* save new or update old card */
void save2memo(tcfg *c, int i, int card, double scor)
{
    if(-1 == i) /* no index implies new memory block */
    {
        c->cfsize++;
        c->cfcard = (int *)reallocordie(c->cfcard, sizeof(int) * c->cfsize);
        c->cfdate = (int *)reallocordie(c->cfdate, sizeof(int) * c->cfsize);
        c->cfave = (double *)reallocordie(c->cfave, sizeof(double) * c->cfsize);
        c->cfcard[c->cfsize - 1] = card;
        c->cfdate[c->cfsize - 1] = c->today;
        c->cfave[c->cfsize - 1] = scor / 1.3; /* first score -23.07% */
        return;
    }

    assert(c->cfcard[i] == card); /* if c->cfcard[i] != card; then error; */
    c->cfdate[i] = c->today;
    c->cfave[i] = (c->cfave[i] + scor) / 2.0;
    return;
}

/* save updated cards in memory to history file */
/* file format cf4:
 *          session time (double)
 *          card number (int) card last date (int) card grade average (double)
 *          ...
 */
void save2file(tcfg c)
{
    int i;
    FILE *fp;

    if((fp = fopen(c.histf, "w")) != NULL) /* create from scratch */
    {
        fprintf(fp, "%lf\n", c.session); /* accumulated time */
        for(i = 0; i < c.cfsize; i++)
            fprintf(fp, "%5d %8d %6.4f\n", c.cfcard[i], c.cfdate[i], c.cfave[i]);

        fclose(fp);
    }
    else  /* /home/carol/.config/qualcard/carol-ls-key-description.cf4 */
        fprintf(stderr, "save2file(): can't open history file %s for writing.\n", c.histf);
    return;
}

/* read option from string and update vector of options */
/* options -e order=sort/random -e score=after/before */
void setoption(int vopt[2], char *sopt)
{
    char *p = NULL;
    char option[STROPT];
    char value[STROPT];
    int o = -1, v = -1; /* option and value indexes */

    strncpy(option, sopt, STROPT);
    /* printf("setoption option='%s'\n", option); */
    if((p = strchr(option, '\n')) != NULL)
        * p = '\0';
    if((p = strchr(option, '=')) == NULL)
        return;
    *p = '\0';

    if(!strncmp(option, "order", STROPT))
        o = 0;
    if(!strncmp(option, "score", STROPT))
        o = 1;
    if(o == -1)
        return;

    strncpy(value, p + 1, STROPT);
    /* printf("setoption value='%s'\n", value); */
    if(o == 0)
    {
        if(!strncmp(value, "sort", STROPT))
            v = 0;
        if(!strncmp(value, "random", STROPT))
            v = 1;
    }
    if(o == 1)
    {
        if(!strncmp(value, "after", STROPT))
            v = 0;
        if(!strncmp(value, "before", STROPT))
            v = 1;
    }
    if(v == -1)
        return;

    vopt[o] = v;
    return;
}

/* compare command line options with INI file and save if necessary */
void cmpoption(tcfg *c, int mopt[2])
{
    int o;
    int fs = 0; /* flag save is necessary */
    char filename[STRSIZE];
    FILE *fp;

    for(o = 0; o < 2; o++)
    {
        if(mopt[o] == -1)
            continue;
        if(c->option[o] == mopt[o])
            continue;
        fs = 1;
        c->option[o] = mopt[o];
    }

    if(verb > 1)
    {
        printf("INI option order=%s\n", c->option[0] ? "random" : "sort");
        printf("INI option score=%s\n", c->option[1] ? "before" : "after");
    }

    if(fs) /* saving ini file */
    {
        snprintf(filename, STRSIZE, "%s/%s", c->cfgrealpath, CFGINI);
        if((fp = fopen(filename, "w")) != NULL) /* lets save a ini file! */
        {
            fprintf(fp, "# Qualcard Initialization File\n");
            fprintf(fp, "# 2021-11-15 (C) by Ruben Carlo Benante\n");
            fprintf(fp, "#\n");
            fprintf(fp, "# Usage:\n");
            fprintf(fp, "# option=value\n");
            fprintf(fp, "#\n");
            fprintf(fp, "# Options available:\n");
            fprintf(fp, "#\n");
            fprintf(fp, "# order=[sort|random]\n");
            fprintf(fp, "#       sort   : select cards on due date (default)\n");
            fprintf(fp, "#       random : select random cards to practice\n");
            fprintf(fp, "#\n");
            fprintf(fp, "# score=[after|before]\n");
            fprintf(fp, "#       after  : show old score only after asking your self-evaluation (default)\n");
            fprintf(fp, "#       before : show score before asking your self-evaluation\n");
            fprintf(fp, "\n");
            fprintf(fp, "order=%s\n", c->option[0] ? "random" : "sort");
            fprintf(fp, "score=%s\n", c->option[1] ? "before" : "after");
            fprintf(fp, "\n");

            fclose(fp);
        }
        else
            fprintf(stderr, "Can't save INI file.\n");
    }
}

/* get card faces */
void cardfaces(char *card, char *fr, char *bk)
{
    char *colon;

    strncpy(fr, card, STRSIZE);
    changebarnet(fr);
    strncpy(bk, fr, STRSIZE); /* default in case of problem: back == front */
    colon = fr;
    while(1)
    {
        colon = strchr(colon, ':');
        if(!colon)
        {
            fprintf(stderr, "\n------------------------------------------\n");
            fprintf(stderr, "Wrong card without '::' separator.\n%s\n", card);
            changecolon(fr); /* change char 254 back to a colon sign */
            changecolon(bk); /* change char 254 back to a colon sign */
            return; /* not found, return all card to front and back */
        }
        colon++;
        if(*colon == ':') /* found :: */
            break;
    }
    *(colon - 1) = '\0'; /* front */
    strncpy(bk, colon + 1, STRSIZE); /* back */
    changecolon(fr); /* change char 254 back to a colon sign */
    changecolon(bk); /* change char 254 back to a colon sign */
    return;
}

/* change char 254 back to a colon sign */
void changecolon(char *nt)
{
    do
        if((nt = strchr(nt, 254)))
            * nt = ':';
    while(nt != NULL);
}

/* change '\n', '\t' and '\\' to the real thing */
void changebarnet(char *nt)
{
    do
        if((nt = strchr(nt, '\\')))
            switch(*++nt)
            {
                case 'n':        /* \n becomes space+\n */
                    *nt = '\n';
                    goto space;
                case 't':        /* \t becomes space+\t */
                    *nt = '\t';
                    goto space;
                case ':':        /* : becomes space + þ 254 */
                    *nt = 254;
                    goto space;
space:
                case '\\':       /* \\ becomes space+\ */
                    *(nt - 1) = ' ';
                    nt++;
            }
    while(nt != NULL);
}

/* get filename with no path */
char *filenopath(char *filepath)
{
    static char filename[STRSIZE];
    char *bar;

    strncpy(filename, filepath, STRSIZE);
    if((bar = strrchr(filepath, '/'))) /* find the last / */
    {
        bar++; /* next char starts the filename */
        strncpy(filename, bar, STRSIZE);
    }
    /* else /1* no bar? that's odd... *1/ */
    /* { */
    /* fprintf(stderr, "Filename %s must be an absolute path.\n", filepath); */
    /* exit(EXIT_FAILURE); */
    /* } */
    return filename;
}

/* prioritary (older) comes first (selection sort) */
void sortmemo(tcfg *c)
{
    int i, j, iux;
    double ki, kj, fux;

    if(c->cfsize < 2)
        return;

    for(i = 0; i < c->cfsize - 1; i++)
        for(j = i + 1; j < c->cfsize; j++)
        {
            ki = newdate(c->cfdate[i], ave2day(c->cfave[i])) + c->cfave[i] / 10.0;
            kj = newdate(c->cfdate[j], ave2day(c->cfave[j])) + c->cfave[j] / 10.0;
            if(ki > kj) /* ki is after, invert */
            {
                /* if(ki == kj && c->cfave[i] <= c->cfave[j]) /1* 1st score is lower, do not swap *1/ */
                /* continue; */
                /* swap cards number */
                iux = c->cfcard[i];
                c->cfcard[i] = c->cfcard[j];
                c->cfcard[j] = iux;
                /* swap cards presented date */
                iux = c->cfdate[i];
                c->cfdate[i] = c->cfdate[j];
                c->cfdate[j] = iux;
                /* swap cards average */
                fux = c->cfave[i];
                c->cfave[i] = c->cfave[j];
                c->cfave[j] = fux;
            }
        }
}

/* select 10 cards (old or new) to be presented */
void select10cards(tcfg *c, int tencards[10][2])
{
    int i, j;

    for(i = 0; i < 10; i++) /* zeroing */
        tencards[i][TMEM] = tencards[i][TFIL] = TEND; /* end of cards */

    sortmemo(c); /* sort cfcard, cfdate and cfave, by date+days(ave) */
    for(i = 0; i < 9 && i < c->cfsize; i++) /* nine olds if possible */
    {
        if(newdate(c->cfdate[i], ave2day(c->cfave[i])) > c->today) /* just until today */
            break;
        tencards[i][TMEM] = i; /* take the first nine or less */
        tencards[i][TFIL] = c->cfcard[i]; /* file line */
    }
    for(j = i; j < 10; j++)
    {
        /* new not in memory nor in tencards already */
        if((tencards[j][TFIL] = newcard(*c, tencards)) == TEND)
            break; /* there is not enough */
        tencards[j][TMEM] = TVAL; /* not in memory, but valid new card from file */
    }
    if(j < 10) /* still need, lets see cards for tomorrow */
    {
        while(i < c->cfsize && j < 10)
        {
            tencards[j][TMEM] = i; /* take the next waiting in history */
            tencards[j][TFIL] = c->cfcard[i]; /* file line */
            i++;
            j++;
        }
    }
}

/* database ex4 size, number of lines = QTDCARD */
int dbsize(char *dbname)
{
    char line[STRSIZE];
    int qtdl = 0;
    FILE *fp;

    if(!(fp = fopen(dbname, "r")))
    {
        fprintf(stderr, "Fail to open database %s.\n", dbname);
        exit(EXIT_FAILURE);
    }

    do
    {
        fgets(line, STRSIZE, fp);
        qtdl++;
    }
    while(!feof(fp));
    fclose(fp);
    return --qtdl;
}

/* get accumulated time if exists. points to first card stat */
double getactime(FILE *fp)
{
    int card, date; /* card_number, last_card_revision_date */
    double ave; /* card_score_average */
    double ac = 0.0; /* accumulated time session */
    errno = 0;

    fseek(fp, 0, 0);
    if(3 == fscanf(fp, "%d %d %lf\n", &card, &date, &ave))
    {
        fseek(fp, 0, 0);
        errno = ENODATA;
        return 0.0; /* old file format, no accumulated time */
    }

    fseek(fp, 0, 0);
    if(1 == fscanf(fp, "%lf\n", &ac)) /* read first line of accumulated time */
        return ac; /* let it point to the second line and return */

    errno = EPROTO;
    perror("some strange error\n");
    fseek(fp, 0, 0);
    return 0.0; /* no accumulated time */
}

/* history analises */
void cfanalyses(char *histfile, int today, int qtd, int *view, int *learn, double *pct, double *addscore, int *ncardl)
{
    int card, date, revd; /* card number, card date last seen, card review date */
    double ave; /* average of a single card written in disk */
    FILE *fp; /* file pointer to history file */
    int alla = 1; /* all cards are A */
    int late=0; /* days late */

    *view = 0; /* number of cards viewed from the total */
    *learn = 0; /* number of cards with score greater than SCOREA */
    *pct = 0.0; /* percentage of completeness decays with time */
    *addscore = 0.0; /* just the scores of cards seen */
    *ncardl = 0; /* number of cards late that need review */

    if(!(fp = fopen(histfile, "r"))) /* temporary histfile in a loop */
        return;

    (void) getactime(fp); /* ignore accumulated time if it exists */

    completed(COMPLSTRT, ave, late, qtd); /* initialize weight 0.0 */

    /* History file formad: ~/.config/qualcard/histfile.cf4 :  11 20211211 3.8462 (first line, accumulated time) */
    while(3 == fscanf(fp, "%d %d %lf\n", &card, &date, &ave))
    {
        (*view)++;
        *addscore += ave;

        revd = newdate(date, ave2day(ave));
        late = 0;
        if(today - revd >= 0) /* late or due today */
        {
            late = diffdays(today, revd);
            (*ncardl)++;
        }

        if(ave >= SCOREB)
            (*learn)++;

        if(ave < SCOREA)
            alla = 0; /* not all cards are A */

        completed(COMPLCONT, ave, late, qtd); /* keep adding */
    }
    fclose(fp);

    *pct = completed(COMPLSTOP, ave, late, qtd); /* return result */

    if(*view > 0)
        *addscore /= ((double) * view); /* average of scores you've got so far */
    if(*addscore > SCOREA && alla) /* it is not impossible to achieve 5.0 */
        *addscore = 5.0;

    return;
}

/* return percentage of completed database */
/* flag start: 0=start the sum, 1=keep adding, 2=return result */
/* Verification, tau=80.7:
 * 5.0 -> 0.1 : ~316 days (~10 months)
 * 5.0 -> 4.0 : ~21 days (~3 weeks)
 */
double completed(int start, double ave, int late, int qtd)
{
    static double weight=0.0;

    if(start == COMPLSTRT)
        weight = 0.0;
    if(start == COMPLSTOP)
        return weight / qtd * 100.0;

    if(ave >= SCOREB)
        weight += score(ave, late) / 5.0; /* full credit, with time penalty */
    else
        weight += score(ave, late) / 6.18; /* partial credit, with time penalty */
    return weight; // ignored when start == COMPLCONT
}

/* return the new score 0..5 when the revision is late */
double score(double ave, int late)
{
    double scr = ave;
    double penalty;

    penalty = exp(-late / 80.7); /* Exponencial decay, halves every ~8 weeks */
    scr = ave * penalty;
    return scr;
}

/* calculates duration of this session and accumulated time */
void sessiontime(tcfg *c)
{
    double fsec; /* seconds */
    time_t ttnow;
    int h0, m0, s0, h1, m1, s1;

    ttnow = time(NULL);

    fsec = difftime(ttnow, c->tstart); /* this session's duration in seconds */

    c->session += fsec; /* all sessions in seconds */

    h0 = (int)fsec / 3600;
    m0 = (int)(((fsec / 3600.0) - ((int)fsec / 3600)) * 60.0);
    s0 = (int)(((fsec / 60.0) - ((int)(h0 * 60 + m0))) * 60.0);

    h1 = (int)c->session / 3600;
    m1 = (int)(((c->session / 3600.0) - ((int)c->session / 3600)) * 60.0);
    s1 = (int)(((c->session / 60.0) - ((int)(h1 * 60 + m1))) * 60.0);

    /* printf("end=%.2lf, start=%.2lf, diff=%.2lf, sessionn=%.2lf\n", (double)ttnow, (double)c->tstart, fsec, c->session); */

    printf("This session: %02d:%02d:%02d. Accumulated time: %02d:%02d:%02d\n", h0, m0, s0, h1, m1, s1);
    return;
}

/* Difference of two dates in days. Positive if new>old, 0 if equals, negative c.c. */
int diffdays(int newd, int oldd)
{
    int ano, mes, dia;//, aux;
    double fsec; /* seconds */
    time_t ttnew, ttold; /* epochs */
    struct tm tmnew = {0}, tmold = {0}; /* date fields */

    ano = oldd / 10000;
    oldd -= ano * 10000;
    mes = oldd / 100;
    oldd -= mes * 100;
    dia = oldd;
    tmold.tm_year = ano - 1900;
    tmold.tm_mon = mes - 1;
    tmold.tm_mday = dia;
    ttold = mktime(&tmold);

    ano = newd / 10000;
    newd -= ano * 10000;
    mes = newd / 100;
    newd -= mes * 100;
    dia = newd;
    tmnew.tm_year = ano - 1900;
    tmnew.tm_mon = mes - 1;
    tmnew.tm_mday = dia;
    ttnew = mktime(&tmnew);

    fsec = difftime(ttnew, ttold); /* difference in seconds */
    dia = fsec / 86400.0; /* difference in days */

    return dia;
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
 * @todo This function is not ready
 *
 */
void summary(tcfg c)
{
    int i, view = 0, learn = 0, qtd = 0;
    char *dbc;
    double pview, plearn; /* percentages */
    double ave; /* average 0...5 of cards you've seen in a database */
    double pct; /* percentage of your achievements, decay with time */
    int clate; /* number of cards late */
    int maxlen = 14, len;
    char histfile[PATHSIZE]; /* history file name */
    char cardfr[STRSIZE], cardbk[STRSIZE]; /* card front and back */

    for(i = 0; i < c.dbfsize; i++) /* database file list */
        if((len = strlen(theme(filenopath(c.dbfiles[i])))) > maxlen)
            maxlen = len;
    maxlen++; /* 15 or more */

    /* |  N | Database                   | Comp.% | Total|    Viewed (%) |   Learned (%) | Review | Score | */
    printf("|  N | %-*s | %6s |%6s | %13s | %13s | %5s | %5s |\n", maxlen, "Database", "Comp.%", "Total", "Viewed (%)", "Learned (%)", "Review", "Score");

    for(i = 0; i < c.dbfsize; i++) /* database file list */
    {
        if(DBNUM && DBNUM - 1 != i)
            continue;
        qtd = dbsize(c.dbfiles[i]);

        dbc = dbcore(c.dbfiles[i]);
        if(strlen(c.cfguserpath) + strlen(c.fileuser) + strlen(dbc) + strlen(EXTCF) >= PATHSIZE)
        {
            fprintf(stderr, "Summary filename overflow\n");
            exit(EXIT_FAILURE);
        }
        snprintf(histfile, PATHSIZE, "%s/%s-%s%s", c.cfguserpath, c.fileuser, dbc, EXTCF);

        cfanalyses(histfile, c.today, qtd, &view, &learn, &pct, &ave, &clate);
        pview = ((double)view / (double)qtd) * 100.0;
        plearn = ((double)learn / (double)qtd) * 100.0;

        printf("| %2d | %-*s | %5.1f%% | %5d | %4d (%5.1f%%) | %4d (%5.1f%%) | %6d | %5.1f |\n", i + 1, maxlen, theme(filenopath(c.dbfiles[i])), pct, qtd, view, pview, learn, plearn, clate, ave);
    }
    if(SUMMA && DBNUM)
    {
        printf("List of cards, revision and scores:\n");
        menudb(&c);
        readcfg(&c);
        for(i = 0; i < c.cfsize; i++) /* entries in history file cf4 */
        {
            getcard(c.dbasef, c.cfcard[i], cardfr, cardbk);
            sstrip(cardfr);
            sstrip(cardbk);
            /* printf("debug revisao %d score: %lf --", c.cfdate[i], c.cfave[i]); */
            printf("Card: %4d   Revision: %s   Score: %.4lf   Brief: %.9s :: %.9s\n", c.cfcard[i] + 1, prettydate(newdate(c.cfdate[i], ave2day(c.cfave[i]))), c.cfave[i], cardfr, cardbk);
        }
    }
    return;
}

/* remove \n \t and double spaces from string */
void sstrip(char *s)
{
    char *p;
    int fr = 1; /* flag remove */

    if(strlen(s) < 2)
        return;
    while((p = strchr(s, '\n')))
        * p = ' ';
    while((p = strchr(s, '\t')))
        * p = ' ';
    while(fr)
    {
        fr = 0;
        p = s;
        while(*p)
        {
            if(*p == ' ' && *(p + 1) == ' ')
            {
                fr = 1;
                break;
            }
            p++;
        }
        while(*p)
        {
            *p = *(p + 1);
            p++;
        }
    }
}

/* take the theme out of a file name */
char *theme(char *file)
{
    char *dash;
    static char theme[STRSIZE];

    strncpy(theme, file, STRSIZE);
    if((dash = strchr(theme, '-')))
        * dash = '\0';

    return theme;
}

/* Sorteia um cartao novo que esta no arquivo history e nunca foi visto */
int newcard(tcfg c, int tencards[10][2])
{
    int l, i;

    randnorep(FILLBASKET, &c.QTDCARD); /* fill */

    for(i = 0; i < 10; i++) /* remove from basket all in memory tencards */
        if(tencards[i][TFIL] != TEND)
            randnorep(REMOVEBASKET, &tencards[i][TFIL]);

    for(i = 0; i < c.cfsize; i++) /* remove from basket all in file history cf4 */
        randnorep(REMOVEBASKET, &c.cfcard[i]);

    if(randnorep(DRAWBASKET, &l) != BASKETOK)
        l = TEND;

    return l;
}

/* given a card number, get it from file */
void getcard(char *dbfile, int cardnum, char *cardfr, char *cardbk)
{
    char card[STRSIZE];
    FILE *fp;
    int i;

    if(!(fp = fopen(dbfile, "r")))
    {
        printf("Fail to open database %s.\n", dbfile);
        exit(EXIT_FAILURE);
    }

    for(i = 0; i <= cardnum; i++)
        fgets(card, STRSIZE, fp);
    fclose(fp);

    cardfaces(card, cardfr, cardbk);

    return;
}

/**
 * @brief Drawn a double number in the given interval [min, max[
 * @param[in] min Lower number of the interval (closed, inclusive)
 * @param[in] max Maximum number of the interval (open, exclusive)
 * @param[out] sorteio The number drawn and returned
 * @author Ruben Carlo Benante
 * @version 20160409.000957
 * @date 2016-04-09
 */
double randmm(double min, double max)
{
    double s;

    s = rand();
    s /= ((double)RAND_MAX);
    s *= (max - min);
    s += min;
    return s;
}

/* create /home/realuser/.config/qualcard/ */
void createcfgdir(tcfg *c)
{
    char path1[PATHSIZE];

    snprintf(path1, PATHSIZE, "/home/%s/.config", c->realuser);
    if(0 != access(path1, F_OK))
    {
        if(ENOENT == errno) /* does not exist */
        {
            if(mkdir(path1, S_IRWXU | S_IRGRP | S_IXGRP) != 0) /* cannot create */
            {
                printf("cannot create directory %s. I can't save your progress.\n", path1);
                exit(EXIT_FAILURE);
            }
        }
        else
            if(ENOTDIR == errno) /* not a directory */
            {
                printf("%s is not a directory. I can't save your progress.\n", path1);
                exit(EXIT_FAILURE);
            }
    }

    if(0 != access(c->cfgrealpath, F_OK))
    {
        if(ENOENT == errno) /* does not exist */
        {
            if(mkdir(c->cfgrealpath, S_IRWXU | S_IRGRP | S_IXGRP) == 0) /* successifully created */
                return;
            printf("cannot create directory %s. I can't save your progress.\n", c->cfgrealpath);
            exit(EXIT_FAILURE);
        }
        else
            if(ENOTDIR == errno) /* not a directory */
            {
                printf("%s is not a directory. I can't save your progress.\n", c->cfgrealpath);
                exit(EXIT_FAILURE);
            }
    }

    return;
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
void qualcard_init(tcfg *cfg)
{
    IFDEBUG("qualcard_init()");

    struct tm *timeptr;
    char stoday[DTSIZE];
    char binpath[PATHSIZE] = {0};

    cfg->tstart = time(NULL);
    timeptr = localtime(&cfg->tstart);
    snprintf(stoday, DTSIZE, "%04d%02d%02d", 1900 + timeptr->tm_year, 1 + timeptr->tm_mon, timeptr->tm_mday);
    cfg->today = (int)strtol(stoday, NULL, 10);

    srand((unsigned)time(NULL)); /* new unknow seed */

    strncpy(cfg->realuser, getenv("USER"), STRSIZE);
    if(cfg->pathuser[0] == '\0')
        strncpy(cfg->pathuser, cfg->realuser, STRSIZE);

    if(cfg->fileuser[0] == '\0')
    {
        if(strcmp(cfg->realuser, cfg->pathuser) && verb > 1)
            printf("Assuming -u %s. See -h for help.\n", cfg->pathuser);
        strncpy(cfg->fileuser, cfg->pathuser, STRSIZE);
    }

    if(verb > 0)
    {
        printf("Hello %s, ", cfg->fileuser);
        printf("today is %s. ", prettydate(cfg->today));
        if(verb > 1)
            printf("Welcome back!");
        printf("\n");
    }

    if(readlink("/proc/self/exe", binpath, PATHSIZE) == -1) /* /usr/local/bin/qualcard */
    {
        perror("readlink");
        printf("I can't find the binary path\n");
        exit(EXIT_FAILURE);
    }
    if(strlen(binpath) >= PATHSIZE - 2)
    {
        fprintf(stderr, "Binary path filename overflow\n");
        exit(EXIT_FAILURE);
    }
    snprintf(cfg->dbpath, PATHSIZE - 2, "%sdb", binpath); /* /usr/local/bin/qualcarddb/ */
    if(strlen(cfg->realuser) >= PATHSIZE - 23 || strlen(cfg->pathuser) >= PATHSIZE - 23)
    {
        fprintf(stderr, "Real/User path filename overflow\n");
        exit(EXIT_FAILURE);
    }
    snprintf(cfg->cfgrealpath, PATHSIZE, "/home/%s/.config/qualcard", cfg->realuser);
    snprintf(cfg->cfguserpath, PATHSIZE, "/home/%s/.config/qualcard", cfg->pathuser);

    createcfgdir(cfg); /* /home/realuser/.config/qualcard/ */

    if(verb > 2)
    {
        printf("Binary path: %s\n", binpath);
        printf("Database path: %s/\n", cfg->dbpath);
        printf("Config path for writing: %s/\n", cfg->cfgrealpath);
        printf("Config path for reading: %s/\n", cfg->cfguserpath);
    }

    readdbfiles(cfg); /* update dbfiles[] vector from dbpath and cfgrealpath */
    if(!cfg->dbfsize)
    {
        printf("No databases found.\n");
        exit(EXIT_FAILURE);
    }

    readini(cfg); /* read INI file options */
    return;
}

/* read database from menu, set dbsize, dbasef and histf */
void menudb(tcfg *cfg)
{
    int i, dbnum;
    char *dbc;

    if(cfg->dbasef[0] == '\0')
    {
        do
        {
            if(verb > 1)
            {
                printf("Databases found:\n");
                for(i = 0; i < cfg->dbfsize; i++)
                    printf("(%d) %s\n", i + 1, cfg->dbfiles[i]);
            }

            dbnum = 1; /* default */
            if(cfg->dbfsize > 1)
            {
                printf("Choose a database: ");
                if(DBNUM != 0)
                {
                    dbnum = DBNUM; /* from command line */
                    printf("%d\n", DBNUM);
                }
                else
                    scanf("%d%*c", &dbnum); /* ignore '\n' */
            }
        }
        while(dbnum < 1 || dbnum > cfg->dbfsize);
        dbnum--;
        strncpy(cfg->dbasef, cfg->dbfiles[dbnum], STRSIZE); /* dbasef set */
    }
    cfg->QTDCARD = dbsize(cfg->dbasef);
    if(cfg->QTDCARD < 10)
    {
        printf("Error in database %s.\nMust have at least 10 questions::answers (lines).\n", cfg->dbasef);
        exit(EXIT_FAILURE);
    }

    dbc = dbcore(cfg->dbasef);
    /* ~/.config/qualcard/user-theme-front-verse.cf4 */
    if(strlen(cfg->cfgrealpath) + strlen(cfg->fileuser) + strlen(dbc) + strlen(EXTCF) >= STRSIZE)
    {
        fprintf(stderr, "History filename overflow\n");
        exit(EXIT_FAILURE);
    }
    snprintf(cfg->histf, STRSIZE, "%s/%s-%s%s", cfg->cfgrealpath, cfg->fileuser, dbc, EXTCF);
    if(verb > 2)
        printf("History file: %s\n", cfg->histf);

    return;
}

/* grab the core name of the file */
char *dbcore(char *s)
{
    static char dbc[PATHSIZE];
    char *dot;

    strncpy(dbc, filenopath(s), PATHSIZE);
    if((dot = strrchr(dbc, '.'))) /* find the dot */
        *dot = '\0'; /* delete from dot on */
    return dbc;
}

/* given an average, return how many days for next review */
int ave2day(double ave)
{
    if(ave <= SCOREF) /* G: review tomorrow */
        return 1;
    if(ave <= SCOREE) /* F: review in 2 days */
        return 2;
    if(ave <= SCORED) /* E: review in 3 days */
        return 3;
    if(ave <= SCOREC) /* D: review in 5 days */
        return 5;
    if(ave <= SCOREB) /* C: review in 7 days */
        return 7;
    if(ave <= SCOREA) /* B: review in 9 days */
        return 9;
    return 11; /* A: review in 11 days */
}

/* add days to a date */
int newdate(int oldd, int days)
{
    int ano, mes, dia;
    time_t tt; /* epochs */
    struct tm date = {0}; /* date fields */
    char snewd[DTSIZE];

    ano = oldd / 10000;
    oldd -= ano * 10000;
    mes = oldd / 100;
    oldd -= mes * 100;
    dia = oldd;
    date.tm_year = ano - 1900;
    date.tm_mon = mes - 1;
    date.tm_mday = dia + days; /* add the number of days */

    tt = mktime(&date);
    date = *gmtime(&tt);
    snprintf(snewd, DTSIZE, "%04d%02d%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
    oldd = (int)strtol(snewd, NULL, 10);
    return oldd;
}

/* return date in a pretty format */
char *prettydate(int oldd)
{
    static char *months[] = {"Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dec" };
    static char dd[12];
    int ano, mes, dia;

    ano = oldd / 10000;
    oldd -= ano * 10000;
    mes = oldd / 100;
    oldd -= mes * 100;
    dia = oldd;

    snprintf(dd, 12, "%02d-%s-%04d", dia, months[mes - 1], ano);
    return dd;
}

/* resize the memory block of a pointer or die */
void *reallocordie(void *ptr, size_t sz)
{
    void *tmp;
    if((tmp = realloc(ptr, sz)) == NULL)
    {
        printf("out of memory\n");
        exit(EXIT_FAILURE);
    }
    return tmp;
}

/* ---------------------------------------------------------------------- */
/* read INI file options */
void readini(tcfg *c)
{
    FILE *fp;
    char line[STROPT];
    char filename[STRSIZE];

    /* printf("(1) readini option[0] = %d option[1] = %d\n", c->option[0], c->option[1]); */
    snprintf(filename, STRSIZE, "%s/%s", c->cfgrealpath, CFGINI);
    if((fp = fopen(filename, "r")) != NULL) /* we've got a ini file! */
    {
        while(fgets(line, STROPT, fp))
            setoption(c->option, line);
        fclose(fp);
    }
    /* printf("(2) readini option[0] = %d option[1] = %d\n", c->option[0], c->option[1]); */
    return;
}


/* ---------------------------------------------------------------------- */
/* read history cf4 file to memory */
void readcfg(tcfg *c)
{
    int i;
    int card, date;
    double ave;
    FILE *fp;

    c->cfcard = c->cfdate = NULL;
    c->cfave = NULL;
    c->cfsize = 0; /* number of cards in history file */
    if((fp = fopen(c->histf, "r")) != NULL) /* we've got a file! */
    {
        c->session = getactime(fp); /* get accumulated time session */
        for(i = 0; 3 == fscanf(fp, "%d %d %lf\n", &card, &date, &ave); i++)
        {
            c->cfcard = (int *)reallocordie(c->cfcard, sizeof(int) * (i + 1));
            c->cfdate = (int *)reallocordie(c->cfdate, sizeof(int) * (i + 1));
            c->cfave = (double *)reallocordie(c->cfave, sizeof(double) * (i + 1));
            c->cfcard[i] = card;
            c->cfdate[i] = date;
            c->cfave[i] = ave;
            c->cfsize++;
        }
        fclose(fp);
        return;
    }

    if(verb >= 0)
        printf("No previous history. Starting new study.\n");
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
 * @deprecated This function will be deactivated
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
void readdbfiles(tcfg *c)
{
    DIR *dp;
    struct dirent *ep;
    char *dot = NULL;
    int len;
    int dois = 1; /* run twice, one for standard db path and other for history path */
    char fullname[PATHSIZE];

    c->dbfiles = NULL; /* risk of memory leak here: this line isn't needed */
    c->dbfsize = 0; /* number of database files on dbfiles[] vector */

    do
    {
        dp = opendir(dois ? c->dbpath : c->cfgrealpath);
        if(dp != NULL)
        {
            while((ep = readdir(dp))) /* while there is a file, get it */
            {
                if(!(dot = strrchr(ep->d_name, '.'))) /* grab extension */
                    continue;
                len = snprintf(fullname, PATHSIZE, "%s/%s", (dois ? c->dbpath : c->cfgrealpath), ep->d_name);
                if(len >= PATHSIZE)
                {
                    fprintf(stderr, "Ignoring file with too big name: %s\n", fullname);
                    continue;
                }
                if(!strcmp(dot, EXTDB)) /* achou db */
                {
                    c->dbfsize++;
                    c->dbfiles = (char **)reallocordie(c->dbfiles, sizeof(char *) * (c->dbfsize));
                    c->dbfiles[c->dbfsize - 1] = NULL;
                    c->dbfiles[c->dbfsize - 1] = (char *)reallocordie(c->dbfiles[c->dbfsize - 1], sizeof(char) * (len + 1));
                    strncpy(c->dbfiles[c->dbfsize - 1], fullname, len + 1);
                }
            }
            closedir(dp);
        }
        else
        {
            fprintf(stderr, "Error: %s\n", (dois ? c->dbpath : c->cfgrealpath));
            perror("Couldn't open the directory");
        }
        dois--;
    }
    while(dois >= 0);

    return;
}

/* drawn numbers from a list with no repetition */
int randnorep(int mode, int *n)
{
    struct listnum
    {
        int num;              /* the number */
        struct listnum *prox; /* next number */
    };
    static int listqtd = 0; /* how much numbers in the basket */
    static struct listnum *lcab = NULL; /* head of the list */
    struct listnum *ln = NULL, *lant = NULL; /* ln: list point to number. lant: the node before. */
    static struct listnum *lista = NULL;

    int j;
    int sort;

    if(lcab == NULL)
    {
        lcab = (struct listnum *) malloc(sizeof(struct listnum));
        lcab->prox = NULL;
        lista = lcab;
    }

    if(mode == FILLBASKET) /* fills the basket */
    {
        if(*n < 1)
            return BASKETERROR;
        /* deleting the old basket */
        lant = lcab->prox;
        while(lant != NULL)     /* delete previous experiment */
        {
            ln = lant->prox;
            free(lant);
            lant = ln;
        }
        lcab->prox = NULL;

        /* filling the new basket */
        listqtd = *n;
        ln = lcab;
        for(j = 0; j < listqtd; j++)
        {
            ln->num = j;
            ln->prox = (struct listnum *) malloc(sizeof(struct listnum));
            lant = ln;
            ln = ln->prox;
            ln->prox = NULL;
        }
        free(ln);
        lant->prox = NULL;

        return BASKETOK;
    }
    else
        if(mode == DRAWBASKET) /* Draw a number */
        {
            if(listqtd == 0)
                return BASKETEMPTY; /* empty basket */
            sort = randmm(0, listqtd);
            ln = lcab;
            lant = NULL;
            for(j = 0; j < sort; j++)
            {
                lant = ln;
                ln = ln->prox;
            }
            *n = ln->num;         /* number drawn */
            listqtd--;

            if(listqtd <= 0)        /* if no more, dont need to delete head */
                return BASKETOK;

            if(lant != NULL)        /* not the head */
                lant->prox = ln->prox;
            else                    /* it is the head */
            {
                lcab = lcab->prox;
                lista = lcab;
            }
            free(ln);

            return BASKETOK;
        }
        else
            if(mode == LISTBASKET)    /* LISTBASKET  */
            {
                /* Example: for(a=randnorep(LISTBASKET); a!=BASKETLISTED; a=randnorep(LISTBASKET)) ( */
                if(listqtd == 0)
                    return BASKETLISTED;    /* listed because empty basket */
                if(lista == NULL)
                {
                    lista = lcab;
                    return BASKETLISTED;    /* basket listed */
                }
                while(lista != NULL)
                {
                    *n = lista->num;
                    lista = lista->prox;
                    return BASKETOK;
                }
            }
            else
                if(mode == REMOVEBASKET) /* Remove a specific item */
                {
                    if(listqtd == 0)
                        return BASKETEMPTY; /* empty basket */
                    sort = *n;
                    ln = lcab;
                    lant = NULL;
                    while(ln != NULL)
                    {
                        if(ln->num == sort) /* number remove */
                            break;
                        lant = ln;
                        ln = ln->prox;
                    }
                    if(ln == NULL) /* number not found */
                        return BASKETERROR;

                    listqtd--;

                    if(listqtd <= 0)        /* if no more, dont need to delete head */
                        return BASKETOK;

                    if(lant != NULL)        /* not the head */
                        lant->prox = ln->prox;
                    else                    /* it is the head */
                    {
                        lcab = lcab->prox;
                        lista = lcab;
                    }
                    free(ln);

                    return BASKETOK;
                }
    return BASKETERROR; /* unknow mode */
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
    printf("%s v.%s - %s\n", "qualcard", VERSION, "Learn cards by Spaced Repetition Method");
    printf("\nUsage: %s [options]\n", "qualcard");
    printf("\nOptions:\n");
    printf("\t-h,  --help\n\t\tShow this help.\n");
    printf("\t-c,  --version\n\t\tShow version and copyright information.\n");
    printf("\t-v,  --verbose\n\t\tSet verbose level (cumulative).\n");
    printf("\t-q,  --quiet\n\t\tReduces verbose level (also cumulative).\n");
    printf("\t-s,  --status\n\t\tShows how many cards in need of a review in each database. Together with -n N, shows only the status of a single chosen database.\n");
    printf("\t-i,  --invert\n\t\tInvert presentation order (show first the back, then the front of the card).\n");
    printf("\t-u username,  --user username\n\t\tUse the username's profile. If not given, defaults to the current username.\n");
    printf("\t-p username,  --path username\n\t\tSet the username used for the config path (it is mandatory to use together with -s).\n");
    printf("\t-n N, --number N\n\t\tPick a database number to start right away. With -s, prints the status of only a specific database.\n");
    printf("\t-d file.ex4,  --database file.ex4\n\t\tUse the given database to practice.\n\t\tThe file must have a '.ex4' extension\n\t\tand its name is in the form 'theme-front-verse.ex4', where:\n\t\t\t* theme: the theme of the study.\n\t\t\t* front: the first side of the card, ex. question.\n\t\t\t* verse: the back side of the card, ex. answer.\n");
    /* add more options here */
    printf("\t-e OPTION=VALUE\n\t\t Set OPTION. Options are: order=[sort|random], score=[before|after]\n\t\t Example: qualcard -e order=sort -e score=after\n\t\t Options are saved into ~/.config/qualcard/qualcard.ini\n");
    printf("\nExit status:\n\t0 if ok.\n\t1 some error occurred.\n");
    printf("\nTodo:\n\tLong options not implemented yet.\n\tsort=random not implemented yet.\n");
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
    printf("%s - Version %s\n", "QualCard", VERSION);
    printf("\nCopyright (C) %d %s <%s>, GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>. This  is  free  software:  you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law. USE IT AS IT IS. The author takes no responsability to any damage this software may inflige in your data.\n\n", 2016, "Ruben Carlo Benante", "rcb@beco.cc");
    if(verb > 3) printf("copyr(): Verbose: %d\n", verb); /* -vvvv */
    exit(EXIT_FAILURE);
}

/**  @} */
/* End of GroupUnique */

/* ---------------------------------------------------------------------- */
/* vi: set ai et ts=4 sw=4 tw=0 wm=0 fo=croql : C config for Vim modeline */
/* Template by Dr. Beco <rcb at beco dot cc> Version 20160612.142044      */

