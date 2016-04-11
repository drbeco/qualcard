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
#include <dirent.h> /* Defines directory entries */

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
#define DTSIZE 9 /**< String with yyyymmdd */
#define PEREXEC 10 /**< How many cards presented per round (execution) */
#define NEWEXEC 1 /**< Minimum number of new cards presented per round */

/* ---------------------------------------------------------------------- */
/* globals */

static int verb=0; /**< verbose level, global within the file */

typedef struct scfg /* configuration data struct */
{
    int QTDCARD; /* database size */
    int today; /* yyyymmdd */
    char user[STRSIZE]; /* user name */
    char dbasef[STRSIZE], config[STRSIZE]; /* filenames: database and configuration */
//     FILE *fdb; /* database file pointer */
//     FILE *fcf; /* config file pointer */
    int *cfcard, *cfdate; /* card num, last date */
    float *cfave; /* card average */
    int cfsize; /* size of config file */
    char **dbfiles; /* char dbfiles[100][256];*/
    //char *dbfiles[0]; / * char dbfiles[100][256];*/
    int dbfsize; /* first dimension / lines */
} tcfg; /* configuration data type */

/* ---------------------------------------------------------------------- */
/* prototypes */

void help(void); /* print some help */
void copyr(void); /* print version and copyright information */
void qualcard_init(tcfg *cfg); /* global initialization function */
void summary(tcfg c); /* how many cards to review */
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
    int dbnum; /* database number chosen */
    int SUMMA=0; /* summary only */
    time_t lt;
    struct tm *timeptr;
    /*int QTDCARD; / * database size * /
    char today[14]; / * yyyymmdd * /
    char dbasef[STRSIZE], config[STRSIZE], user[STRSIZE]; */
    tcfg c={0};
    /* FILE *fdb=NULL, *fcf=NULL; */
    int l; /* line drawn */
    int i; /* index, auxiliary */
    char card[STRSIZE]; /* card drawn */
//     char newd[DTSIZE]; /* 20140608'\0' */
    int newd;
//     int ntoday; /* today as an integer yyyymmdd */
    int tencards[10][2]; /* ten cards, index in memory (-1 if new), line in file */


    IFDEBUG("Starting optarg loop...\n");

    /* getopt() configured options:
     *        -h            help
     *        -c            copyright & version
     *        -v            verbose
     *        -s            status of reviews
     *        -u username   set the username (default: whoami)
     *        -d database   set the database to use (default: ask)
     */
    opterr = 0;
    while((opt = getopt(argc, argv, "vhcsu:d:")) != EOF)
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
            case 'd':
                strcpy(c.dbasef, optarg);
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


    printf("Number of cards: %d\n", c.QTDCARD);
    printf("DB: %s\n", c.dbasef);
    printf("CF: %s\n", c.config);

//     for(i=0; i<10; i++)
//     {
//         l=newcard(c, card);
//         printf("Card %4d: %s", l+1, card);
//     }

//     if(c.fdb)
//         fclose(c.fdb); / * database file * /


//     printf("readcfg()...\n");
    readcfg(&c);
//     printf("c.cfsize=%d\n", c.cfsize);
//     for(i=0; i<c.cfsize; i++)
//         printf("card: %d, date: %d, ave: %f\n", c.cfcard[i], c.cfdate[i], c.cfave[i]);

    /*ntoday=newdate(c.today, 0, newd);*/

//     ntoday=(int)strtol(c.today, NULL, 10);

    select10cards(&c, tencards);

    for(i=0; i<10; i++)
    {
        getcard(c, tencards[i][1], card);
        if(tencards[i][0]==-1) /* new card? */
            printf("Card %d (new card)\n", tencards[i][1]);
        else
        {
            newd=newdate(c.cfdate[tencards[i][0]], ave2day(c.cfave[tencards[i][0]]));
            printf("Card %d (revision date %s)\n", tencards[i][1], prettydate(newd));
        }
        printf("%s\n", card);
    }

//     if(c.fcf)
//         fclose(c.fcf); /* config file */

    return EXIT_SUCCESS;
}

/* prioritary (old) comes first (selection sort) */
void sortmemo(tcfg *c)
{
//     int *cfcard, *cfdate; / * card num, last date * /
//     float *cfave; / * card average * /
//     int cfsize; / * size of config file * /
    int i, j, iux;
    int ki, kj;
    float fux;
    if(c->cfsize<2)
        return;

//     printf("before sort\n");
//     for(i=0; i<c->cfsize; i++)
//     {
//         ki=newdate(c->cfdate[i], ave2day(c->cfave[i]));
//         printf("%d %d %f - (+%d) %d\n", c->cfcard[i], c->cfdate[i], c->cfave[i], ave2day(c->cfave[i]), ki);
//     }

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
//     printf("\nafter sort\n");
//     for(i=0; i<c->cfsize; i++)
//     {
//         ki=newdate(c->cfdate[i], ave2day(c->cfave[i]));
//         printf("%d %d %f - (+%d) %d\n", c->cfcard[i], c->cfdate[i], c->cfave[i], ave2day(c->cfave[i]), ki);
//     }
//     printf("\n");
}


// int ave2day(float ave)
// tudo newdate(char *oldd, int days, char *newd)


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
        /* return 0; */
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
    int i;

    for(i=0; i<c.dbfsize; i++)
    {
        printf("%s: you have ", theme(c.dbfiles[i]));
        printf("%d cards to review today.\n", rand()%21);
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



/*
 * le um novo cartao aleatorio do arquivo
 * retorna o numero da linha lida (de 0 a FIM-1)
 * e a string lida
 */
int newcard(tcfg c) //, char *card)
{
    int l, i, total, novo;
    char line[TAMLINE];
//     FILE *fp;


//     if(!(fp=fopen(c.dbasef,"r")))
//     {
//         printf("Fail to open database %s.\n", c.dbasef);
//         exit(EXIT_FAILURE);
        /* return 0; */
//     }

    total=c.QTDCARD-c.cfsize;
    novo=1;

    l=(int)randmm(0.0, total); /* [= 0, QTDPAD-1 =] */

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

//     fseek(fp, 0, 0);
//     for(i=0; i<=l; i++)
//         fgets(line, TAMLINE, fp);
//     strcpy(card, line);

//     fclose(fp);
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
        /* return 0; */
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
    /* initialization */

    time_t lt;
    struct tm *timeptr;
    int i, dbnum;
    /* const char *user=getenv("USER"); */
//     const char *dbcore="english-word-definition-test";
    char dbcore[STRSIZE];
    char *dot;
    FILE *fdb;
    char stoday[DTSIZE];

    lt=time(NULL);
    timeptr=localtime(&lt);
    sprintf(stoday, "%04d%02d%02d", 1900 + timeptr->tm_year, 1 + timeptr->tm_mon, timeptr->tm_mday);
    cfg->today=(int)strtol(stoday, NULL, 10);

    srand((unsigned)time(&lt));

    if(cfg->user[0]=='\0')
        strcpy(cfg->user, getenv("USER"));

    printf("QualCard v.%s - Spaced Repetition\n", VERSION);
    printf("%s, ", cfg->user);
    printf("today is %s\n", prettydate(cfg->today));

    readdbfiles(cfg);
    if(!cfg->dbfsize)
    {
        printf("No databases found.\n");
        exit(EXIT_FAILURE);
    }

    if(cfg->dbasef[0]=='\0')
    {
        do
        {
            printf("Databases found:\n");
            for(i=0; i<cfg->dbfsize; i++)
                printf("(%d) %s\n", i+1, cfg->dbfiles[i]);

            printf("Choose a database: ");
            scanf("%d", &dbnum);
        } while(dbnum<1 || dbnum>cfg->dbfsize);
        dbnum--;
        strcpy(cfg->dbasef, cfg->dbfiles[dbnum]);
    }

    // sprintf(cfg->dbasef, "%s%s", dbcore, EXTDB);
    // strcpy(db, "english-word-definition.ex4");
    strcpy(dbcore, cfg->dbasef);
    if((dot=strrchr(dbcore, '.'))) /* find the dot */
        *dot='\0'; /* delete from dot on */

//     printf("core: %s\n", dbcore);

    sprintf(cfg->config, "%s-%s%s", cfg->user, dbcore, EXTCF);
    // strcpy(cf, "beco-english-word-definition-test.cf4");


    cfg->QTDCARD=dbsize(cfg->dbasef);

    return;
}

/* given an average, return how many days */
int ave2day(float ave)
{
    if(ave<0.1)
        return 0; /* review today */
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

    /*char str[LEN];
    snprintf(str, LEN, "%d", 42);*/
    /* char sdata[MAXSDATA]; / * data no formato escolhido */

//     tudo=(int)strtol(oldd, NULL, 10);

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
    /*printf("Local is %s\n",asctime(localtime(&timer))); */
    /*printf("UTC is %s\n",asctime(gmtime(&timer))); */
    /*strftime(sdata, sizeof(sdata), "%F %H:%M", &date);*/
    sprintf(snewd, "%04d%02d%02d",date.tm_year+1900,date.tm_mon+1,date.tm_mday);
    oldd=(int)strtol(snewd, NULL, 10);
    /*printf("%s + %d = %s\n", olddate, days, newd); */
    return oldd;
}

/* return date in a pretty format */
char *prettydate(int oldd)
{
    static char* months[] = {"Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dec" };
    static char dd[12];
    int ano, mes, dia;

//     tudo=(int)strtol(oldd, NULL, 10);

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

    /* if(access(c->config, F_OK)!=-1) / * config file exists */
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
        fprintf(stderr, "readcfg(): can't open config file.\n");
    return; /* leu do arquivo para os vetores */
}


/* todo:
 * writecfg
 * choosedb
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
//             puts(ep->d_name);
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
//                 printf("c->dbfsize:%d\n", c->dbfsize);
                c->dbfiles=(char **)reallocordie(c->dbfiles, sizeof(char *)*(c->dbfsize));
//                 printf("vai %s\n", ep->d_name);
//                 printf("c->dbfiles=%p, c->dbfiles[0]=%p [1]=%p\n", c->dbfiles, c->dbfiles[0], c->dbfiles[1]);
//                c->dbfiles=(char **)malloc(sizeof(char *)*c->dbfsize);
//                *(c->dbfiles+i)=(char *)malloc(10);

                c->dbfiles[c->dbfsize-1] = NULL;
                c->dbfiles[c->dbfsize-1] = (char *)reallocordie(c->dbfiles[c->dbfsize-1], sizeof(char)*(len+1));

//                 c->dbfiles[c->dbfsize-1] = (char *)reallocordie(*(c->dbfiles + c->dbfsize - 1), sizeof(char)*(len+1));
                strncpy(c->dbfiles[c->dbfsize-1], ep->d_name, len+1);
//                 printf("c->dbfiles[%d]=%p '%s'\n", c->dbfsize-1, c->dbfiles[c->dbfsize-1], c->dbfiles[c->dbfsize-1]);
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
    printf("\nUsage: %s [-h|-v]\n", "qualcard");
    printf("\nOptions:\n");
    printf("\t-h,  --help\n\t\tShow this help.\n");
    printf("\t-c,  --version\n\t\tShow version and copyright information.\n");
    printf("\t-v,  --verbose\n\t\tSet verbose level (cumulative).\n");
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
