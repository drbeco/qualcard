/***************************************************************************
 *   qualcard.c                               Version 1.1.20160411.205959  *
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
#include <dirent.h> /* Defines directory entries */
#include <assert.h> /* Verify assumptions with assert */

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
#define DTSIZE 9 /**< String with yyyymmdd */
#define PEREXEC 10 /**< How many cards presented per round (execution) */
#define NEWEXEC 1 /**< Minimum number of new cards presented per round */

/* ---------------------------------------------------------------------- */
/* globals */

static int verb=0; /**< verbose level, global within the file */
static int SUMMA=0; /* summary only */

typedef struct scfg /* configuration data struct */
{
    int QTDCARD; /* database size */
    int today; /* yyyymmdd */
    char user[STRSIZE]; /* user name */
    char dbasef[STRSIZE], config[STRSIZE]; /* filenames: database and configuration */
    int *cfcard, *cfdate; /* card num, last date */
    float *cfave; /* card average */
    int cfsize; /* size of config file */
    char **dbfiles; /* char dbfiles[100][256];*/
    int dbfsize; /* first dimension / lines */
} tcfg; /* configuration data type */

/* ---------------------------------------------------------------------- */
/* prototypes */

void help(void); /* print some help */
void copyr(void); /* print version and copyright information */
void qualcard_init(tcfg *cfg); /* global initialization function */
void summary(tcfg *c); /* how many cards to review */
int dbsize(char *dbname); /* database size */
double randmm(double min, double max); /* drawn a number from [min, max[ */
int newcard(tcfg c); //, char *card); /* drawn a new card */
int ave2day(float ave); /* given an average, return how many days */
int newdate(int oldd, int days); /* add days to a date */
char *prettydate(int somedate); /* return date in a pretty format */
void readcfg(tcfg *c); /* read config file */
void *reallocordie(void *ptr, size_t sz); /* resize the memory block of a pointer or die */
void readdbfiles(tcfg *c); /* read files from directory and create a dynamic vector of strings */
char *theme(char *file); /* take the theme from a database file name */
void select10cards(tcfg *c, int tencards[10][2]); /* select 10 cards (old or new) to be presented */
void sortmemo(tcfg *c); /* prioritary (old) comes first (selection sort) */
void getcard(tcfg c, int cardnum, char *card); /* given a card number, get it from file */
char *cardfront(char *card); /* get the front of the card */
char *cardback(char *card); /* get the back of the card */
void save2memo(tcfg *c, int i, int card, int score); /* save new or update old card */
void save2file(tcfg c); /* save updated cards in memory to config file */
void cfanalyses(char *cfname, int *view, int *learn); /* analyses a history file */

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
    int opt; /* return from getopt() and user options */
    tcfg c={0};
    int i; /* index, auxiliary */
    char card[STRSIZE]; /* card drawn */
    int newd;
    int tencards[10][2]; /* ten cards, index in memory (-1 if new), line in file */
    int again=1; /* while some card score presented is still zero */

    IFDEBUG("Starting optarg loop...\n");

    /* getopt() configured options:
     *        -h            help
     *        -c            copyright & version
     *        -v            verbose++
     *        -q            quiet (verbose--)
     *        -s            status of reviews
     *        -u username   set the username (default: whoami)
     *        -d database   set the database to use (default: ask)
     */
    opterr = 0;
    while((opt = getopt(argc, argv, "hcvqsu:d:")) != EOF)
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
            case 'q':
                verb--;
                break;
            case 's':
                SUMMA=1;
                break;
            case 'u':
                strcpy(c.user, optarg);
                break;
            case 'd':
                strcpy(c.dbasef, optarg);
                break;
            case '?':  /* exit */
            default:
                printf("Type\n\t$man %s\nor\n\t$%s -h\nfor help.\n\n", argv[0], argv[0]);
                return EXIT_FAILURE;
        }

    if(verb>1)
        printf("Verbose level set at: %d\n", verb);

    qualcard_init(&c); /* initialization function */

    if(SUMMA)
    {
        summary(&c); /* how many cards to review */
        exit(EXIT_SUCCESS);
    }

    printf("\nDataBase file: %s (%d cards)\n", c.dbasef, c.QTDCARD);
    printf("History  file: %s\n\n", c.config);

    readcfg(&c);
    select10cards(&c, tencards);

    while(again)
    {
        again=0;
        for(i=0; i<10; i++)
        {
            if(tencards[i][0]==-2) /* already presented and ok */
                continue;
            getcard(c, tencards[i][1], card); /* tencards[i][1]=line number in file */
            printf("----------------------------------------\n");
            if(tencards[i][0]==-1) /* new card? */
                printf("Card %d (new card)\n", tencards[i][1]+1);
            else
            {
                newd=newdate(c.cfdate[tencards[i][0]], ave2day(c.cfave[tencards[i][0]]));
                printf("Card %d (revision date %s)\n", tencards[i][1]+1, prettydate(newd));
            }

            printf("%s\n\n", cardfront(card));

            printf("Press <ENTER> to see the back of the card\n");
            do opt=getchar(); while(opt!='\n');
            printf("%s\n", cardback(card));

            do
            {
                printf("Your self-evaluation (from 0 to 5) is: ");
                scanf("%d%*c", &opt); /* discard the '\n'. Better use fgets() */
            } while(opt<0 || opt>5);
            if(opt==0)
                again=1;
            else
            {
                save2memo(&c, tencards[i][0], tencards[i][1], opt); /* tencards[i][0]=index in memory */
                tencards[i][0]=-2; /* presented and ok */
            }
        }
    }
    printf("----------------------------------------\n");

    save2file(c);

    printf("\nLive as if you were to die tomorrow. Learn as if you were to live forever. (Mahatma Gandhi)\n");
    printf("Thank you for using QualCard version %s.\n\n", VERSION);
    return EXIT_SUCCESS;
}

/* save new or update old card */
void save2memo(tcfg *c, int i, int card, int score)
{
    if(i==-1) /* new memory block */
    {
        c->cfsize++;
        c->cfcard=(int *)reallocordie(c->cfcard, sizeof(int)*c->cfsize);
        c->cfdate=(int *)reallocordie(c->cfdate, sizeof(int)*c->cfsize);
        c->cfave=(float *)reallocordie(c->cfave, sizeof(float)*c->cfsize);
        c->cfcard[c->cfsize-1]=card;
        c->cfdate[c->cfsize-1]=c->today;
        c->cfave[c->cfsize-1]=(float)score; /* first score */
        return;
    }

    assert(c->cfcard[i] == card); /* if c->cfcard[i] != card; then error; */
    c->cfdate[i] = c->today;
    c->cfave[i] = (c->cfave[i] + (float)score)/2.0;
    return;
}

/* save updated cards in memory to config file */
void save2file(tcfg c)
{
    int i;
    FILE *fp;

    if((fp=fopen(c.config, "w"))!=NULL) /* create from scratch */
    {
        for(i=0; i<c.cfsize; i++)
            fprintf(fp, "%d %d %f\n", c.cfcard[i], c.cfdate[i], c.cfave[i]);

        fclose(fp);
    }
    else
        fprintf(stderr, "save2file(): can't open config file for writing.\n");
    return; /* leu do arquivo para os vetores */
}


/* get the front of the card */
char *cardfront(char *card)
{
    static char front[STRSIZE];
    char *colon;

    strcpy(front, card);
    if((colon=strchr(front, ':'))) /* find the colon */
        *colon='\0'; /* delete from : on */
    return front;
}

/* get the back of the card */
char *cardback(char *card)
{
    static char back[STRSIZE];
    char *colon;

    if((colon=strchr(card, ':'))) /* find the colon */
        colon++; /* next char starts the back */
    else
        colon=card; /* no colon? copy all */
    strcpy(back, colon);
    return back;
}

/* prioritary (old) comes first (selection sort) */
void sortmemo(tcfg *c)
{
    int i, j, iux;
    int ki, kj;
    float fux;
    if(c->cfsize<2)
        return;

    for(i=0; i<c->cfsize-1; i++)
        for(j=i+1; j<c->cfsize; j++)
        {
            ki=newdate(c->cfdate[i], ave2day(c->cfave[i]));
            kj=newdate(c->cfdate[j], ave2day(c->cfave[j]));
            if(ki>kj) /* ki is after, invert */
            {
                /* swap cards number */
                iux=c->cfcard[i];
                c->cfcard[i]=c->cfcard[j];
                c->cfcard[j]=iux;
                /* swap cards presented date */
                iux=c->cfdate[i];
                c->cfdate[i]=c->cfdate[j];
                c->cfdate[j]=iux;
                /* swap cards average */
                fux=c->cfave[i];
                c->cfave[i]=c->cfave[j];
                c->cfave[j]=fux;
            }
        }
}

/* select 10 cards (old or new) to be presented */
void select10cards(tcfg *c, int tencards[10][2])
{
    int i;

    sortmemo(c);
    for(i=0; i<9 && i<c->cfsize; i++) /* nine olds if possible */
    {
        if(newdate(c->cfdate[i], ave2day(c->cfave[i]))>c->today) /* just until today */
            break;
        tencards[i][0]=i; /* take the first nine or less */
        tencards[i][1]=c->cfcard[i]; /* file line */
    }
    for(; i<10; i++)
    {
        tencards[i][0]=-1; /* not in memory */
        tencards[i][1]=newcard(*c); /* new not in memory */
    }
}

/* database size */
int dbsize(char *dbname)
{
    char line[TAMLINE];
    int qtdl=0;
    FILE *fp;

    if(!(fp=fopen(dbname,"r")))
    {
        printf("Fail to open database %s.\n", dbname);
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, 0 ); /* linha 0 */
    do
    {
        fgets(line, TAMLINE, fp);
        qtdl++;
    }while(!feof(fp));
    fclose(fp);
    return --qtdl;
}

/* history analises */
void cfanalyses(char *cfname, int *view, int *learn)
{
    int card, date;
    float ave;
    FILE *fp;

    *view=0;
    *learn=0;
    if(!(fp=fopen(cfname,"r")))
        return;

    while(3 == fscanf(fp, "%d %d %f\n", &card, &date, &ave))
    {
        (*view)++;
        if(ave>=4.5)
            (*learn)++;
    }
    fclose(fp);
    return;
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
void summary(tcfg *cfg)
{
    int i, view=0, learn=0;
    char *dot, dbcore[STRSIZE];

    /* English: total 1500, viewed 300, learned 12 */

    for(i=0; i<cfg->dbfsize; i++)
    {

        strcpy(cfg->dbasef, cfg->dbfiles[i]);
        strcpy(dbcore, cfg->dbasef);
        if((dot=strrchr(dbcore, '.'))) /* find the dot */
            *dot='\0'; /* delete from dot on */
        sprintf(cfg->config, "%s-%s%s", cfg->user, dbcore, EXTCF);
        cfg->QTDCARD=dbsize(cfg->dbasef);

        cfanalyses(cfg->config, &view, &learn);
        printf("%s: total %d, viewed %d, learned %d\n", theme(cfg->dbfiles[i]), cfg->QTDCARD, view, learn);
    }
    return;
}

/* take the theme out of a file name */
char *theme(char *file)
{
    char *dash;
    static char theme[STRSIZE];

    strcpy(theme, file);
    if((dash=strchr(theme, '-')))
        *dash='\0';

    return theme;
}

/* Sorteia um cartao novo que esta no arquivo e nunca foi visto */
int newcard(tcfg c) //, char *card)
{
    int l, i, total, novo;

    total=c.QTDCARD-c.cfsize;
    novo=1;
    l=(int)randmm(0.0, total); /* [= 0, total-1 =] */

    while(novo)
    {
        novo=0;
        for(i=0; i<c.cfsize; i++) /* talvez ordenar por card num? */
            if(l==c.cfcard[i])
            {
                l = (l+1)%c.QTDCARD; /* advance l to the next avaiable */
                novo=1;
            }
    }
    return l; /* card line number */
}

/* given a card number, get it from file */
void getcard(tcfg c, int cardnum, char *card)
{
    char line[TAMLINE];
    FILE *fp;
    int i;

    if(!(fp=fopen(c.dbasef,"r")))
    {
        printf("Fail to open database %s.\n", c.dbasef);
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0, 0);
    for(i=0; i<=cardnum; i++)
        fgets(line, TAMLINE, fp);
    strcpy(card, line);

    fclose(fp);
    return;
}

/**
 * @brief Drawn a float number in the given interval [min, max[
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
    s /= ((float)RAND_MAX);
    s *= (max-min);
    s += min;
    return s;
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
void qualcard_init(tcfg *cfg) //(char *td, char *db, char *cf)
{
    IFDEBUG("qualcard_init()");

    time_t lt;
    struct tm *timeptr;
    int i, dbnum;
    char dbcore[STRSIZE];
    char *dot;
    char stoday[DTSIZE];

    lt=time(NULL);
    timeptr=localtime(&lt);
    sprintf(stoday, "%04d%02d%02d", 1900 + timeptr->tm_year, 1 + timeptr->tm_mon, timeptr->tm_mday);
    cfg->today=(int)strtol(stoday, NULL, 10);

    srand((unsigned)time(&lt)); /* new unknow seed */

    if(cfg->user[0]=='\0')
        strcpy(cfg->user, getenv("USER"));

    printf("QualCard v.%s - Spaced Repetition\n", VERSION);
    if(verb>0)
    {
        printf("%s, ", cfg->user);
        printf("today is %s\n", prettydate(cfg->today));
    }

    readdbfiles(cfg);
    if(!cfg->dbfsize)
    {
        printf("No databases found.\n");
        exit(EXIT_FAILURE);
    }

    if(SUMMA)
        return;
    if(cfg->dbasef[0]=='\0')
    {
        do
        {
            printf("Databases found:\n");
            for(i=0; i<cfg->dbfsize; i++)
                printf("(%d) %s\n", i+1, cfg->dbfiles[i]);

            printf("Choose a database: ");
            scanf("%d%*c", &dbnum); /* ignore '\n' */
        } while(dbnum<1 || dbnum>cfg->dbfsize);
        dbnum--;
        strcpy(cfg->dbasef, cfg->dbfiles[dbnum]);
    }

    strcpy(dbcore, cfg->dbasef);
    if((dot=strrchr(dbcore, '.'))) /* find the dot */
        *dot='\0'; /* delete from dot on */
    sprintf(cfg->config, "%s-%s%s", cfg->user, dbcore, EXTCF);
    cfg->QTDCARD=dbsize(cfg->dbasef);

    return;
}

/* given an average, return how many days */
int ave2day(float ave)
{
    if(ave<0.1)
        return 0; /* review today BUG */
    if(ave<2.0)
        return 1; /* review tomorrow */
    if(ave<3.25)
        return 3; /* review in three days */
    if(ave<4.50)
        return 5; /* review in five days */
    return 7; /* review in a week */
}

/* add days to a date */
int newdate(int oldd, int days)
{
    int ano, mes, dia;
    time_t timer; /* epochs */
    struct tm date={0}; /* campos da data */
    char snewd[DTSIZE];

    ano = oldd/10000; /* 20160410/10000=2016.0410 */
    oldd -= ano*10000;
    mes = oldd/100; /* 0410/100=04.10 */
    oldd -= mes*100;
    dia = oldd;

    date.tm_year = ano-1900;
    date.tm_mon = mes-1;
    date.tm_mday = dia + days; /* add the number of days */

    timer = mktime(&date);
    date = *gmtime(&timer); /* atribuir valor, nao o ponteiro */
    sprintf(snewd, "%04d%02d%02d",date.tm_year+1900,date.tm_mon+1,date.tm_mday);
    oldd=(int)strtol(snewd, NULL, 10);
    return oldd;
}

/* return date in a pretty format */
char *prettydate(int oldd)
{
    static char* months[] = {"Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dec" };
    static char dd[12];
    int ano, mes, dia;

    ano = oldd/10000; /* 20160410/10000=2016.0410 */
    oldd -= ano*10000;
    mes = oldd/100; /* 0410/100=04.10 */
    oldd -= mes*100;
    dia = oldd;

    sprintf(dd, "%02d-%s-%04d", dia, months[mes-1], ano);
    return dd;
}

/* resize the memory block of a pointer or die */
void *reallocordie(void *ptr, size_t sz)
{
    void *tmp;
    if((tmp=realloc(ptr, sz))==NULL)
    {
        printf("out of memory\n");
        exit(EXIT_FAILURE);
    }
    return tmp;
}

/* read config file to memory */
void readcfg(tcfg *c)
{
    int i;
    int card, date;
    float ave;
    FILE *fp;

    c->cfcard=c->cfdate=NULL;
    c->cfave=NULL;
    c->cfsize=0;
    if((fp=fopen(c->config, "r"))!=NULL) /* we've got a file! */
    {
        for(i=0; 3 == fscanf(fp, "%d %d %f\n", &card, &date, &ave); i++)
        {
            c->cfcard=(int *)reallocordie(c->cfcard, sizeof(int)*(i+1));
            c->cfdate=(int *)reallocordie(c->cfdate, sizeof(int)*(i+1));
            c->cfave=(float *)reallocordie(c->cfave, sizeof(float)*(i+1));
            c->cfcard[i]=card;
            c->cfdate[i]=date;
            c->cfave[i]=ave;
            c->cfsize++;
        }
        fclose(fp);
    }
    else
        printf("No previous history. Starting new study.\n");
    return; /* leu do arquivo para os vetores */
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
    char *dot=NULL;
    int len;

    c->dbfiles=NULL; /* risk of memory leak here: this line isn't needed */
    c->dbfsize=0;
    dp = opendir("./");
    if(dp != NULL)
    {
        while((ep=readdir(dp))) /* while there is a file, get it */
        {
            if(!(dot=strrchr(ep->d_name, '.'))) /* grab extension */
                continue;
            len=strlen(ep->d_name);
            if(len>(STRSIZE-1))
            {
                perror("Ignoring file with too big name");
                continue;
            }
            if(!strcmp(dot, ".ex4")) /* achou db */
            {
                c->dbfsize++;
                c->dbfiles=(char **)reallocordie(c->dbfiles, sizeof(char *)*(c->dbfsize));
                c->dbfiles[c->dbfsize-1] = NULL;
                c->dbfiles[c->dbfsize-1] = (char *)reallocordie(c->dbfiles[c->dbfsize-1], sizeof(char)*(len+1));
                strncpy(c->dbfiles[c->dbfsize-1], ep->d_name, len+1);
            }
        }
        closedir(dp);
    }
    else
        perror ("Couldn't open the directory");

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
    printf("%s v.%s - %s\n", "qualcard", VERSION, "Learn cards by Spaced Repetition Method");
    printf("\nUsage: %s [options]\n", "qualcard");
    printf("\nOptions:\n");
    printf("\t-h,  --help\n\t\tShow this help.\n");
    printf("\t-c,  --version\n\t\tShow version and copyright information.\n");
    printf("\t-v,  --verbose\n\t\tSet verbose level (cumulative).\n");
    printf("\t-q,  --quiet\n\t\tReduces verbose level (also cumulative).\n");
    printf("\t-s,  --status\n\t\tShow how many cards needs review in each database.\n");
    printf("\t-u username,  --user username\n\t\tUse the username's profile.\n");
    printf("\t-d file.ex4,  --database file.ex4\n\t\tUse the given database to practice.\n\t\tThe file must have a '.ex4' extension\n\t\tand its name is in the form 'theme-question-answer.ex4', where:\n\t\t\t* theme: the theme of the study.\n\t\t\t* question: the first side of the card.\n\t\t\t* answer: the back side of the card.\n");
    /* add more options here */
    printf("\nExit status:\n\t0 if ok.\n\t1 some error occurred.\n");
    printf("\nTodo:\n\tLong options not implemented yet.\n");
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
    if(verb>3) printf("copyr(): Verbose: %d\n", verb); /* -vvvv */
        exit(EXIT_FAILURE);
}

/**  @} */
/* End of GroupUnique */
/* ---------------------------------------------------------------------- */
/* vi: set ai et ts=2 sw=2 tw=0 wm=0 fo=croql : C config for Vim modeline */
/* Template by Dr. Beco <rcb at beco dot cc> Version 20150619.231433      */
