

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>	// for white space characters and for the fanction: "isspace"
#include <time.h>	// for mktime

#define LOCNAMEF 0
#define DATEF 1
#define TOTMISSF 2
#define DEADWF 3
#define DEADMF 4
#define DEADCF 5
#define CAUSEF 6
#define LOCDSCRF 7
#define COORDF 8
#define URLF 9

// alias read_line() as getline() if O.S. is WINDOWS else (hopefully linux)
// no alias, use the original getline()
#ifdef WIN32
#define IS_WIN_OS 1
    #define getline(x, y, z) read_line (x, y, z)
#elif _WIN32
#define IS_WIN_OS 1
	#define getline(x, y, z) read_line (x, y, z)
#elif _WINDOWS
#define IS_WIN_OS 1
	#define getline(x, y, z) read_line (x, y, z)
#else
#define IS_WIN_OS 0
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(__WINDOWS__)
//define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #else
      //define something for Windows (32-bit only)
   #endif

   #define NEWLINE '\n'

#elif __APPLE__

#define NEWLINE '\r'

#elif __MACH__

#define NEWLINE '\r'

#elif __linux__
#define NEWLINE '\n'

#elif linux
#define NEWLINE '\n'

#elif __linux
   #define NEWLINE '\n'

#elif __unix__ // all unices not caught above
    // Unix
   #define NEWLINE '\n'

#elif__unix // all unices not caught above
    // Unix
   #define NEWLINE '\n'

#elif defined(_POSIX_VERSION)
    // POSIX
   #define NEWLINE '\n'
#else
	//  "Unknown compiler"
   #define NEWLINE '\n'

#endif

typedef struct
{
    int year;
    int month;
    int day;
}date;



typedef struct
{
    double latitude;    //double for more PRECISION
    double longitude;   //double for more PRECISION
}location;

typedef struct CODL
{
    char *cause_death;
    struct CODL *next;
} COD;

typedef struct incList
{
    char *location;
    date reported;
    int total_missing;
    int total_dead_women;
    int total_dead_men;
    int total_dead_children;
    char *cause_death;
    char *loc_descr;
    location coordinates;
    char *URL;
    struct incList *next;		// for Linked List of Incidents !!!
    int node_counter;			// usefull for debugging

}incident;



void mypause() ;




void freeNode( incident *Node ) ;

incident *delete_first_node (incident *last, int *node_counter) ;

incident *delete_last_node (incident *last, int *node_counter) ;

incident * add_node (incident *last, incident *new_Inc) ;


/* ~~~~~~~~~~~~  read a line as string from stream ~~~~~~~~~~~~
size_t ::  if the compiler is 32 bit then  a typedef for unsigned int
		   if the compiler is 64 bit then  a typedef for unsigned long long.
		   The size_t data type is never negative.
ssize_t ::  [-1,0,size_t]

i dilwsi twn parametrwn opws tis getline() oste na mporei na ginei alias sto define !
*/
ssize_t read_line(char **line, size_t *bufsize, FILE *stream ) ;
//~~~~~~~~~~~~~~ left trims white space characters ~~~~~~~~~~~~~~~
void l_trim(char *instr) ;
//~~~~~~~~~~~~~~~ right trims white space characters~~~~~~~~~~~~~~~~
char* r_trim(char *instr) ;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~CONCERTING THE STRING TO LOWER CASE~~~~~~~~~~~~~~~~~~~~~~~~
void lower_case(char *instr) ;

//~~~~~~~~~~~~~~~returns a new string converted to lower case~~~~~~~~~~~~~~~~~~~~~~~
char *str_lower_case(char *instr) ;

/* ~~~~~~~~~~~~~~~~~~~~~~~~print a string with sepcified len right padded wish spaces~~~~~~~~~~~~~~~~~
	called by :  varius prints (printab, findclr, ...)
*/
void strsprint( const char *instr, char *outstr, int maxlen);

/* parses user input. separates command string from parameter.
  calls strtok()
*/
void parse_user_input( char * userInput, char ** command, char **parameter);

/* splits a delimited line to fields array of strings . char fields delimiter is parametric
   called by : str_splitdatenum() */
int str_split(char * line, char *fields[] , int fieldcount, char delim ) ;

/* ~~~~~~~~ splits a date string 17/04/2020 to passed array of ints {d,m,y} ~~~~~~~~~~~~
   returns    : 3 if okey, 0 if error.
   calls     : str_split()
   called by : printab()
*/
int str_splitdatenum( char * datestr, int daten[3]) ;

/* ~~~~~~~~~~~~~~~~~~~~~ splits a csv line to fields array of strings ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int csvlinetofields(char * line, char **fields, int fieldcount ) ;

//~~~~~~~~ prints Field Titles line with specific length to FILE* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void title_line( /*char *line,*/ size_t maxl, char *delim, int maxlocname, int maxcause, int maxlocdescr, int maxurl, FILE *fp );

//~~~~~~~~ prints formatted record line with specific length to FILE* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void frmt_inc_line( incident * currInc, /* char *line,*/ size_t maxl, char *delim, int maxlocname, int maxcause, int maxlocdescr, int maxurl, FILE *fp);

/* counts how many times a character is present in a string */
size_t count_char_instr(const char *str, const char check) ;

/* counts how many times a substring needle character is present in a string haystack */
int get_substr_count(const char * haystack, const char *needle) ;

/* ~~~~~~~~~ check if date is valid date, e.g. 31/04/2020 is not valid date ~~~~~~~~~~~~~~ */
int isValidDate(int yy, int mm, int dd) ;

/* ~~~~~~~~~~~ returns true if is a valid url else false,0 ~~~~~~~~~~~~~ */
int isValidUrl( char *url ) ;


/* ~~~~~~~~~~~~crt_incident_node  creates a new incident node from line data~~~~~~~~~~~~~~~~~~~~
  used by :: load() and new_incident() to
  1) parse the csv line,
  2) validate data and,
  3) create a new incident node and fill it with the data if all okey ***
  calls   :: csvlinetofields()
*/
incident * crt_incident_node( char *param ) ;

/* =========== program commands =============================================================== */

/*  command newIncident: creates a new incident from command line and adds it to incident list
example:
newIncident Mediterranean;12/03/2020;5;2;0;1;Drowning,Asphyxiation;Off the coast of Larache, Morocco;35.189860435378, -6.173145606543; https://archive.ph/gB4Vs
*/
incident * new_incident( incident *last, char *param, int* node_counter, int *maxlocname, int * maxcause, int * maxlocdescr, int * maxurl);

/* *** command load: loads lines of specified csv file, parses, validates, creates new node, adds node to the end of list *** */
incident * load( incident *last, char *param, int* node_counter, int *maxlocname, int * maxcause, int * maxlocdescr, int * maxurl);

/* commands findC/findL/findR : */
void findlcr( incident *last, char *param, char findtype, int maxlocname, int maxcause, int maxlocdescr, int maxurl) ;
/* commands printA/printB : */
void printab( incident *last, char *param, char direction, int maxlocname, int maxcause, int maxlocdescr, int maxurl );

/* ~~~~~~~~~~~~~~~~~~ command save <out_file>:  ~~~~~~~~~~~~~~~~~~~ */
void save( incident *last, char *param, int maxlocname, int maxcause, int maxlocdescr, int maxurl );

/* command count missing|men|women|children */
void count_incidents( incident *last, char *param) ;

/* used by printReverse(). is the recursive function */
void printReverseRecur(incident *last, incident *currInc, int maxlocname, int maxcause, int maxlocdescr, int maxurl) ;

/* command printreverse: */
void printReverse(incident *last, incident *currInc, int maxlocname, int maxcause, int maxlocdescr, int maxurl) ;

/* ========== my extra commands ====================== */
/* command help: */
void onlinehelp();

/* print some nodes in list based on number. for debugging */
void print_list (incident *last, int lastcount);

/* print top nodes in list. for debugging */
void print_top (incident *last, char *param);

/* prints the content of node. for debugging  , command: dnode n */
void print_node (incident *last, char *param);


int main()
{

    int node_counter = 0;

    incident *last = NULL;

    int userExit = 0;
    char *userInput = NULL;
    size_t bufflen = 128;

    int maxlocname_len =0;
    int maxcause_len   =0;
    int maxlocdescr_len=0;
    int maxurl_len     =0;

    char *command = NULL;
    char *parameter = NULL;
    ssize_t l = 0;

    while (!userExit)
    {
        printf("\n\n =================================");
        printf("\n ******* Missing Migrants ********");
        printf("\n Total Incidents=%d", node_counter );
        printf("\n ---------------------------------");
        printf("\n Ender your command (type `exit` to finish or `help` for help)\n$>");
        l = getline(&userInput, &bufflen, stdin);
        // l = read_line(&userInput, &bufflen, stdin);
        if (l < 4)
        {
            printf("\n ERROR *** The command you gave is wrong. Please try again\n");
            continue;
        }

        userInput[l-1] = 0;		// trwo ton teleytatoio xaratkrira \n

        l_trim( r_trim(userInput) );

        command = NULL;
        parameter = NULL;
        parse_user_input( userInput, &command, &parameter);
        if (command == NULL || command[0] == 0){

            printf("\n ERROR *** Wrong command.\n");
            continue;
        }

        if (parameter !=NULL)
            l_trim( r_trim( parameter) ) ;

        lower_case(command);


        printf("\n User Command=[%s] with param=[%s]", command, parameter);

        if ( strcmp(command,"exit") == 0 )
        {
            printf("\n Goodbye.");
            userExit = 1; // !!!!!!!
            // exit(0); // exits at end to free command and parameter
        }
        else if ( strcmp(command,"newincident") == 0 )
        {
            if(parameter == NULL){
                printf("\n\n NOTICE: newincident command syntax is `newincident <csv line of incident data>` .... Try again.");
            } else {
                last = new_incident( last, parameter, &node_counter, &maxlocname_len, &maxcause_len, &maxlocdescr_len, &maxurl_len);
                // last = new_incident( last, parameter, &node_counter);
                // mypause();
            }
        }
        else if ( strcmp(command,"count") == 0 )
        {
            if(parameter == NULL){
                printf("\n\n NOTICE: count command syntax is `count missing|men|women|children` .... Try again.");
            } else {

                count_incidents( last, parameter);
                // mypause();
            }
        }
        else if ( strcmp(command,"printa") == 0 )
        {
            if(parameter == NULL){
                printf("\n\n NOTICE: printa command syntax is `printa 01/01/2020` (after date) .... Try again.");
            } else {
                printab( last, parameter, 'A', maxlocname_len, maxcause_len, maxlocdescr_len, maxurl_len );
            }

        }
        else if ( strcmp(command,"printb") == 0 )
        {
            if(parameter == NULL){
                printf("\n\n NOTICE: printb command syntax is `printb 01/01/2020` (before date) .... Try again.");
            } else {
                printab( last, parameter, 'B', maxlocname_len, maxcause_len, maxlocdescr_len, maxurl_len );
            }

        }
        else if ( strcmp(command,"findl") == 0 )
        {
            if(parameter == NULL){
                printf("\n\n NOTICE: findL command syntax is `findL <part_of_location>` .... Try again.");
            } else {
                findlcr( last, parameter, 'L', maxlocname_len, maxcause_len, maxlocdescr_len, maxurl_len);
            }
        }
        else if ( strcmp(command,"findc") == 0 )
        {
            if(parameter == NULL){
                printf("\n\n NOTICE: findC command syntax is `findC <part_of_Causeofdeath>` .... Try again.");
            } else {
                findlcr( last, parameter,'C', maxlocname_len, maxcause_len, maxlocdescr_len, maxurl_len);
            }
        }
        else if ( strcmp(command,"findr") == 0 )
        {
            if(parameter == NULL){
                printf("\n\n NOTICE: findR command syntax is `findL <part_of_Region>` .... Try again.");
            } else {
                findlcr( last, parameter, 'R', maxlocname_len, maxcause_len, maxlocdescr_len, maxurl_len);
            }
        }
        else if ( strcmp(command,"deleten") == 0 )
        {
            last = delete_last_node(last, &node_counter);
            // mypause();
        }
        else if ( strcmp(command,"deleteo") == 0 )
        {
            last = delete_first_node(last, &node_counter);
            // mypause();
        }
        else if ( strcmp(command,"save") == 0 )
        {
            if(parameter == NULL){
                printf("\n\n NOTICE: save command syntax is `save <file to save>` .... Try again.");
            } else {
                save( last, parameter, maxlocname_len, maxcause_len, maxlocdescr_len, maxurl_len);
                // mypause();
            }

        }
        else if ( strcmp(command,"load" ) == 0 )
        {
            if(parameter == NULL){
                printf("\n\n NOTICE: load command syntax is `load <file to load>` .... Try again.");
            } else {
                last=load(last, parameter, &node_counter, &maxlocname_len, &maxcause_len, &maxlocdescr_len, &maxurl_len);
                // mypause();
            }
        }

        else if ( strcmp(command,"help") == 0 )
        {
            onlinehelp();
        }

        else if ( strcmp(command,"printreverse") == 0 ) {
            printReverse(last, last->next, maxlocname_len, maxcause_len, maxlocdescr_len, maxurl_len)	;
        }

        else if ( strcmp(command,"pall") == 0 )		// print all nodes
        {
            print_list(last, node_counter);
        }
        else if ( strcmp(command,"ptop") == 0 )	// print top n nodes contents
        {

            if (parameter !=NULL)
                print_top(last, parameter);
            else
                printf("\n Error. ptop num_of_nodes \n");
        }
        else if ( strcmp(command,"dnode") == 0 )	// dump node # contents
        {
            if (parameter !=NULL)
                print_node (last, parameter);
            else
                printf("\n Error. pnode counter_no \n");
        }
        else
            printf("\n *** Unknown command ***. Try again");

        free(command);
        free(parameter);
    }

//    if(command != NULL) {
//        free(command);
//        command = NULL;
//    }
//    if(parameter != NULL) {
//        free(parameter);
//        parameter = NULL ;
//    }

    if(userInput != NULL) {
        free(userInput);
        userInput = NULL;
    }
    while( last != NULL ) {
        last = delete_last_node(last, &node_counter);
    }
    printf("\n Program Finished.\n");

    return 0;
}

/* =================== functions =========================================== */

void mypause() {
    printf("\n Press ENTER to continue...");
    // char dummy;
    //dummy =
    getchar( );
}



/* ========= Diaxeirisi kombwn kai listas ================ */

void freeNode( incident *Node ){
    if(Node->location != NULL) free(Node->location);
    if(Node->cause_death != NULL) free(Node->cause_death);
    if(Node->loc_descr != NULL) free(Node->loc_descr);
    if(Node->URL != NULL) free(Node->URL);
    Node->next = NULL;
    free(Node);
}


incident *delete_first_node (incident *last, int *node_counter)
{

    if (last == NULL)
    {
        printf("\nList is empty\n");
        return NULL;
    }
    if (last == last->next)//only one node
    {
        printf("\n.........Is only one node..\n");
        freeNode(last);
        (*node_counter)--;
        return NULL;
    }
    incident *head_node;
    head_node = last->next;
    last->next = head_node->next;
    freeNode(head_node);
    (*node_counter)--;
    printf("\n Oldest Node deleted.\n");
    return last ;


}

incident *delete_last_node (incident *last, int *node_counter)
{
    incident *temp; //   node_counter; // topic counter

    if (last == NULL)
    {
        printf("List is empty\n");
        return NULL;
    }

    if (last == last->next)//only one node
    {
        freeNode(last);
        (*node_counter)--;
        return NULL;
    }

    temp = last->next; // node_counter = first node

    while (temp->next != last)
    {
        temp = temp->next;
    }
    temp->next = last->next;

    freeNode(last);
    (*node_counter)--;
    printf("\n Newest Node deleted.\n");
    return temp;

}

//
incident * add_node (incident *last, incident *new_Inc) //creating list
{

    if( new_Inc == NULL) {
        printf("\n\n  add_node:: **** ERROR*** new Node is NULL.\n");
        return last;

    }
    if (last == NULL)// list is empty creating first and last node
    {
        last = new_Inc;
        last->next = last;
    }
    else
    {
        new_Inc->next = last->next;
        last->next = new_Inc;
    }
    return new_Inc;
}

/* ========== genikes ==================================================== */



/* ~~~~~~~~~~~~  read a line as string from stream ~~~~~~~~~~~~
size_t ::  if the compiler is 32 bit then  a typedef for unsigned int
		   if the compiler is 64 bit then  a typedef for unsigned long long.
		   The size_t data type is never negative.
ssize_t ::  [-1,0,size_t]

i dilwsi twn parametrwn opws tis getline() oste na mporei na ginei alias sto define !
*/
ssize_t read_line(char **line, size_t *bufsize, FILE *stream ) {

    size_t pos = 0;
    int c;
    size_t n = 128;

    if(stream==NULL || line == NULL) return -1;

    c = getc(stream);
    if (c == EOF) {		// empty file !
        return -1;
    }

    if (*line == NULL) {		// called without malloced => malloc here
        *line = malloc(n);
        if (*line == NULL) {
            return -1;
        }
    }
    pos=0;
    while(c != EOF) {
        if(pos+1>=n) {
            n += 128 / 2 ;
            char *buf = realloc(*line, n);
            if(buf == NULL) {
                return -1;
            }
            *line = buf;
        }
        ((unsigned char *)(*line))[pos] = c;
        pos++;
        if( c == NEWLINE) {
            break;
        }
        c = getc(stream);
    }
    ((unsigned char *)(*line))[pos] = 0;
    return pos;

}

//~~~~~~~~~~~~~~ left trims white space characters ~~~~~~~~~~~~~~~
void l_trim(char *instr)
{

    if (instr == NULL)
        return ;
    //if (instr[0] == 0)
    //	return instr;// empty string

    size_t l = strlen(instr);
    size_t pos = 0;

    while ( pos<l && instr[pos] !=0 && isspace(instr[pos])  )
    {
        pos++;
    }

    if(pos==l) {
        // all white spaces ?
        instr[0] = 0;
        return ;
    } else {

        int i;
        for(i=pos;i<l;i++){
            instr[i-pos]=instr[i];
        }
        instr[i-pos]=0;
        return ;
    }
}

//~~~~~~~~~~~~~~~ right trims white space characters~~~~~~~~~~~~~~~~
char* r_trim(char *instr)
{

    if (instr == NULL)
        return NULL;
    if (instr[0] == 0)
        return instr;// empty string

    size_t  l =  strlen(instr)-1;

    while( *instr !=0 && isspace( instr[l]) && l >=0 )
    {
        instr[l]=0;
        l--;
    }

    return instr;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~CONVERTING THE STRING TO LOWER CASE ~~~~~~~~~~~~~~~~~~~~~~~~
void lower_case(char *instr)
{
    int next = -1;
    while ( next+1 <= strlen(instr) )
    {
        next++;
        instr[next] = tolower( instr[next] );
    }
}

//~~~~~~~~~~~~~~~ returns a new string converted to lower case from passed string ~~~~~~~~~~~~~~~~~~~~~~~
char *str_lower_case(char *instr)
{
    if(instr==NULL) return NULL;

    size_t l = strlen(instr);
    char * lstr = (char *) malloc( l +1 );

    strcpy(lstr, instr);
    int next = -1;
    while ( next+1 <= strlen(lstr) )
    {
        next++;
        lstr[next] = tolower( lstr[next] );
    }
    lstr[l] = 0 ;
    return lstr;
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~print a string with specified len right padded wish spaces~~~~~~~~~~~~~~~~~
	called by :  varius prints (printab, findclr, ...)
*/
void strsprint( const char *instr, char *outstr, int maxlen){

    int ilen = strlen(instr);

    int i;
    for(i=0;i<=maxlen;i++){
        outstr[i]=' ';
    }
    outstr[maxlen+1]=0;

    if(ilen > maxlen) {
        for(i=0;i<=maxlen;i++)
            outstr[i]=instr[i];
    } else {
        for(i=0;i<ilen;i++)
            outstr[i]=instr[i];
    }
    outstr[maxlen+1]=0;
    return; // 	outstr;
}


/* parses user input. separates command string from parameter.
  calls strtok()
*/
void parse_user_input( char * userInput, char ** command, char **parameter) {

    *command   = NULL;
    *parameter = NULL;
    size_t totlen = strlen(userInput);

    char *copy_of_userInput;//we keep backup for userInput because strtok breaks the string
    copy_of_userInput =  (char*) malloc( (totlen+1) * sizeof(char) );
    strcpy(copy_of_userInput, userInput);
    copy_of_userInput[totlen] = 0;

    char * token = strtok(userInput, " ");  // extract 1st token  which is the command
    if(token == NULL) {
        printf("\n ERROR Empty command !");
        free(copy_of_userInput);
        copy_of_userInput = NULL;
        return ;				// bad command, empty
    }

    char * _cmd = malloc( (strlen(token)+1) * sizeof(char) );
    strcpy(_cmd,token);
    _cmd[strlen(token)]=0;
    size_t lc = strlen(_cmd);

    *command = malloc( (lc+1) * sizeof(char) );
    strcpy(*command,_cmd);

    free(_cmd);
    _cmd=NULL;

    if( token != NULL ) { // command was not null, 	// try 2o token , i parametros

        if ( totlen > lc )
        {
            *parameter = malloc( (totlen-lc+1) * sizeof(char) );
            size_t i;
            for(i=lc;i<totlen;i++){
                (*parameter)[i-lc] = copy_of_userInput[i];
            }
            (*parameter)[i-lc] = 0;
        }
    }

    while( token != NULL ) token = strtok(NULL, " ");	// clean-up token and restore userInput

    free(copy_of_userInput);
    copy_of_userInput = NULL;

    return ;



}

/* splits a delimited line to fields array of strings . char fields delimiter is parametric
   called by : str_splitdatenum()
*/
int str_split(char * line, char *fields[] , int fieldcount, char delim ){
    // printf("\n  ---- starting str_split ---------------------- \n");
    // int in_double_quote = 0;
    size_t l = strlen(line);
    int n = 0;
    int field_count = 0;
    int startp = 0, endp = 0;
    while ( line[endp] != 0 )		// while not the end of the string ...
    {
        if ( line[endp] == delim )		// eftasa se delimiter ;
        {

            n = endp - startp;

            if( field_count < fieldcount ) {
                fields[field_count] = (char*) malloc(n+1);
                if (fields[field_count] == NULL)
                {
                    printf("\n System canot allocate memory");
                    return -1;
                }

                int i;
                for(i=startp;i<endp;i++) {
                    fields[field_count][i-startp]=line[i];
                }
                fields[field_count][i-startp]=0;

                // printf("\n fields[%d]={%s}",field_count, fields[field_count]);
            }
            field_count++;

            if ( endp + 1 <=  l )		// An ending pointer+1  einai mesa sto string, didladi den exei perasei to telos toy
            {
                startp = endp + 1;		// metatopizw ton starting pointer   mia thesi meta apo to teleytaio delimiter pou brika
                endp = startp;			// metatopizw kai ton ending pointer mia thesi meta apo to teleytaio delimiter pou brika
                //  gia na arxisei xana to psazimo gia ton epomeno delimiter !
            }

        }
        else 					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            endp++;

    }

    if (startp < endp)
    {
        n = endp - startp;

        if( field_count < fieldcount ) {
            fields[field_count] = (char*) malloc(n+1);
            if (fields[field_count] == NULL)			// last field of csv line left because line does not end with  ; .
            {
                printf("\n System canot allocate memory");
                return -1;
            }
            int i;
            for(i=startp;i<endp;i++) {
                fields[field_count][i-startp]=line[i];
            }
            fields[field_count][i-startp]=0;

            // printf("\n fields[%d]={%s}",field_count, fields[field_count]);
        }
        field_count++;
    }
    // printf("\n  ---- str_split End ---------------------- \n");
    // fflush(stdout);

    return field_count;

}

/* ~~~~~~~~ splits a date string 17/04/2020 to passed array of ints {d,m,y} ~~~~~~~~~~~~
   returns    : 3 if okey, 0 if error.
   calls     : str_split()
   called by : printab()
*/
int str_splitdatenum( char * datestr, int daten[3]) {
    char *datef[3];
    l_trim(r_trim(datestr));
    int i;
    for ( i=0; i<3; i++) {
        datef[i] = NULL; // 0;
        daten[i] = 0;
    }
    int n = str_split (datestr, datef, 3, '/' );
    if(n!=3){
        printf("(splitdate ERROR: Bad Date, fields=%d)",n);
        return n;
    } else {
        // printf(" \n -- debug create_incident: DAYstr=[%s], MONTHstr=[%s], YEARstr=[%s]", datef[0], datef[1], datef[2]);
        int nd=0, nm=0, ny=0;
        if( datef[0]==NULL)  nd = 0;
        else {
            int scresult = sscanf(datef[0],"%d",&nd);
            if(scresult==EOF || scresult==-1) nd = 0;
        }
        if( datef[1]==NULL)  nm = 0;
        else {
            int scresult = sscanf(datef[1],"%d",&nm);
            if(scresult==EOF || scresult==-1) nm = 0;
        }
        if( datef[2]==NULL)  ny = 0;
        else {
            int scresult = sscanf(datef[2],"%d",&ny);
            if(scresult==EOF || scresult==-1) ny = 0;
        }
        if(ny==0 || nm == 0 || nd == 0) {
            printf("(splitdate ERROR: Bad Date)");
            return 0;
        }
        daten[0]=nd;
        daten[1]=nm;
        daten[2]=ny;

        if(datef[0]!=NULL) free(datef[0]);
        if(datef[1]!=NULL) free(datef[1]);
        if(datef[2]!=NULL) free(datef[2]);

        return 3;
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~ splits a csv line to fields array of strings ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int csvlinetofields(char * line, char **fields, int fieldcount ){
    // printf("\n  ---- starting function csvlinetofields ---------------------- \n");
    int in_double_quote = 0;
    size_t l = strlen(line);

    int n ;
    int field_count = 0;
    size_t startp=0, endp=0 ;
    while ( line[endp] != 0 )		// while not the end of the string ...
    {

        if ( line[endp] == ';' && in_double_quote == 0)		// eftasa se delimiter xwris na eimai anamesa se double-quote ;
        {
            n = endp - startp;

            if( field_count < fieldcount ) {
                // version 2:
                fields[field_count] = (char*) realloc(fields[field_count], (n+1)*sizeof(char));
                int i;
                for(i=startp;i<endp;i++) {
                    if( line[i]>31 && line[i]<127 )	fields[field_count][i-startp]=line[i];
                    else 	fields[field_count][i-startp]=32;
                }
                fields[field_count][i-startp]=0;

            }

            field_count++;
            if ( endp + 1 <= l )		// An ending pointer+1  einai mesa sto string, diladi den exei perasei to telos toy
            {
                startp = endp + 1;		// metatopizw ton starting pointer   mia thesi meta apo to teleytaio delimiter pou brika
                endp = startp;			// metatopizw kai ton ending pointer mia thesi meta apo to teleytaio delimiter pou brika
                //  gia na arxisei xana to psazimo gia ton epomeno delimiter !
            }

        }
        else if ( line[endp] == '"') {

            in_double_quote = !in_double_quote ;	// an oxi in_double_quote tote in_double_quote=true else in_double_quote=false

            endp++;
        }

        else 					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            endp++;

    }

    // teleiosan oi xaraktires tou string sto psaximo gia delimiter alla exoun meinei akataxwritoi, to teleytaio pedio tis grammis
    if (startp < endp)
    {

        n = endp - startp;

        if( field_count < fieldcount ) {
            fields[field_count] =  (char*) realloc(fields[field_count], (n+1)*sizeof(char));
            int i;
            for(i=startp;i<endp;i++) {
                if( line[i]>31 && line[i]<127 )	fields[field_count][i-startp]=line[i];
                else fields[field_count][i-startp]=32;
            }
            fields[field_count][i-startp]=0;
        }
        field_count++;
    }

    return field_count;

}

//~~~~~~~~ prints Field Titles line with specific length to FILE* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void title_line( /*char *line,*/ size_t maxl, char *delim, int maxlocname, int maxcause, int maxlocdescr, int maxurl, FILE *fp ){

    fprintf(fp, "AA    ");
    fprintf(fp, "%s",delim);

    char locname[maxlocname+1];
    strsprint("LOCATION", locname, maxlocname);

    fprintf(fp, "%s",locname);
    fprintf(fp, "%s",delim);

    char cdate[11];
    strsprint("DATE", cdate, 10);
    cdate[10]=0;
    fprintf(fp, "%s",cdate);
    fprintf(fp, "%s",delim);

    fprintf(fp, "MIS%sDW %sDM %sDC %s",delim,delim,delim,delim);

    char cause[maxcause+1];
    strsprint("CAUSE OF DEATH", cause, maxcause) ;

    fprintf(fp, "%s",cause);
    fprintf(fp, "%s",delim);

    char locdescr[maxlocdescr+1];
    strsprint("LOCATION DESCR.", locdescr, maxlocdescr) ;

    fprintf(fp, "%s",locdescr);
    fprintf(fp, "%s",delim);

    char coords[18+1+18+1];
    strsprint("COORDINATES", coords, 18+1+18) ;

    fprintf(fp, "%s",coords);
    fprintf(fp, "%s",delim);

    char url[maxurl+1];
    strsprint("URL", url, maxurl) ;

    fprintf(fp, "%s",url);

    fprintf(fp, "\n");

    return ;
}

//~~~~~~~~ prints formatted record line with specific length to FILE* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void frmt_inc_line( incident * currInc, /* char *line,*/ size_t maxl, char *delim, int maxlocname, int maxcause, int maxlocdescr, int maxurl, FILE *fp){

    fprintf(fp, "%.6d", currInc->node_counter);
    fprintf(fp, "%s", delim);

    char locname[maxlocname+1];
    if( currInc->location!=NULL && currInc->location[0] != 0 ) strsprint(currInc->location, locname, maxlocname);
    else strsprint("?", locname, maxlocname);
    fprintf(fp, "%s", locname);
    fprintf(fp, "%s", delim);

    fprintf(fp, "%.2d/%.2d/%.4d", currInc->reported.day,currInc->reported.month,currInc->reported.year);
    fprintf(fp, "%s", delim);

    fprintf(fp, "%.3d", currInc->total_missing);
    fprintf(fp, "%s", delim);

    fprintf(fp, "%.3d", currInc->total_dead_women);
    fprintf(fp, "%s", delim);

    fprintf(fp, "%.3d", currInc->total_dead_men);
    fprintf(fp, "%s", delim);

    fprintf(fp, "%.3d", currInc->total_dead_children);
    fprintf(fp, "%s", delim);

    char cause[maxcause+1];
    if( currInc->cause_death!=NULL && currInc->cause_death[0] != 0 ) strsprint(currInc->cause_death, cause, maxcause);
    else  strsprint("?", cause, maxcause);
    fprintf(fp, "%s", cause);
    fprintf(fp, "%s", delim);

    char locdescr[maxlocdescr+1];
    if( currInc->loc_descr!=NULL && currInc->loc_descr[0] != 0 ) strsprint(currInc->loc_descr, locdescr, maxlocdescr);
    else  strsprint("?", locdescr, maxlocdescr);
    fprintf(fp, "%s", locdescr);
    fprintf(fp, "%s", delim);

    fprintf(fp, "%+18.12f,%+18.12f",currInc->coordinates.latitude, currInc->coordinates.longitude);
    fprintf(fp, "%s", delim);


    if( currInc->URL!=NULL && currInc->URL[0] != 0 ) fprintf(fp, "%s", currInc->URL);
    //else  strsprint("?", url, maxurl);

    fprintf(fp, "\n");


    return ;


}

/* counts how many times a character is present in a string */
size_t count_char_instr(const char *str, const char check) {

    size_t count = 0;

    size_t pos =0;
    while(str[pos] != '\0')
    {
        if(str[pos] == check)
            count++;
        pos++;
    }
    return count;
}

/* counts how many times a substring needle character is present in a string haystack */
int get_substr_count(const char * haystack, const char *needle)
{
    int count = 0;
    const char *tmp = haystack;
    while( ( tmp = strstr( tmp, needle) ) ){
        ++count;
        tmp += strlen(needle);
    }
    return count;
}

/* ~~~~~~~~~ check if date is valid date, e.g. 31/04/2020 is not valid date ~~~~~~~~~~~~~~ */
int isValidDate(int yy, int mm, int dd) {

    // PREPARE TM TO CHECK IT IS A VALID DATE, I.E.  31/04/2020 IS NOT VALID, mktime MODIFIES IT TO 01/05/2020
    struct tm  ltm ;
    ltm.tm_year= yy - 1900;	/* The number of years since 1900   */
    ltm.tm_mon = mm - 1;	/* month, range 0 to 11             */
    ltm.tm_mday= dd;		/* day of the month, range 1 to 31  */
    ltm.tm_hour = 0;
    ltm.tm_min = 0;
    ltm.tm_sec = 0;
    ltm.tm_isdst = -1;
    int ret = mktime(&ltm);
    if(ret == -1){
        return 0;
    }
    if(ltm.tm_year ==  yy- 1900 && ltm.tm_mon == mm - 1  && ltm.tm_mday == dd) return 1;
    else return 0;
}



/* ~~~~~~~~~~~ returns true if is a valid url else false,0 ~~~~~~~~~~~~~ */
int isValidUrl( char *url ) {
    int isValid = 1;
    // http://in.gr (minimum url) or  https://www.in.gr
    size_t n = count_char_instr(url, ':');
    if(n < 1) {				// den exei `:` katholou => lathos
        isValid = 0;
        // printf(" **ERROR,  no `:` in url !");
        return isValid;
    }
    n = count_char_instr(url, '/');		// ligotera apo 2 `/` => lathos
    if(n < 2) {
        isValid = 0;
        // printf(" **ERROR,  less then 2 `/` in url !");
        return isValid;
    }
    n = count_char_instr(url, '.');		// den exei katholou `.` => lathos
    if(n < 1 ) {
        isValid = 0;
        // printf(" **ERROR,  no `.` in url at all !");
        return isValid;
    }
    char *lurl;

    // https://archive.ph/gB4Vs
    lurl = (char *)malloc( (strlen(url)+1)*sizeof(char));
    strcpy(lurl, url);
    lurl[strlen(url)]=0;

    lower_case(lurl);

    /* if( get_substr_count(lurl, "http:") > 1 || get_substr_count(lurl, "https:") > 1 )	// perissotera apo 1 `http:` i `https:` ara lathos
    {
        isValid = 0;
        free(lurl);
        // printf(" **ERROR,  protocol defined more than one time in url !");
        return isValid;
    } */

    // https://archive.ph/gB4Vs => splits by `:` to, [0]:`https` (protocol) , [1]:`//archive.ph/gB4Vs` ypoloipo

    char *ufields[2];
    ufields[0]=NULL;  ufields[0]=0;
    ufields[1]=NULL;  ufields[1]=0;
    int nf = str_split(lurl, ufields , 2, ':' );
    if(nf < 2) {								// den xoristike me ton `:` => lathos
        // printf(" **ERROR,  could not break url into protocol and rest !");
        isValid = 0;
        return isValid;
    }

    if(strcmp(ufields[0],"http")!=0 && strcmp(ufields[0],"https")!=0 ) {	// den yparxei http or https before the :  => lathos
        // printf(" **ERROR,  Bad protocol, not http or https !");
        isValid = 0;
    }
    else if(ufields[1][0]!='/' && ufields[1][1]!='/' ) {	// den exei `//` meta apo to prwto `:` => lathos
        isValid = 0;
        // printf(" **ERROR,  Bad Fullpath (/ missing from start).");
    }
    else if( strlen(ufields[1])< 6 ) {	// `//i.gr`
        isValid = 0;
        printf(" **ERROR,  Very Short Fullpath < 6 chars.");
    }
    else if( ufields[1][2]==' ') {	// `//i.gr`
        isValid = 0;
        printf(" **ERROR, Fullpath starts with space ! ");
    }
    // replace ta prwta `//` me `keno` gia na xrisimopoisw to `/` ws delimiter gia ta epomena split me tin strtok
    ufields[1][0]= 32;  // space
    ufields[1][1]= 32; // space

    // to `  archive.ph/gB4Vs` spaei me basei to `/` se `  archive.ph` kai `gB4Vs` kai an eixe kai allo `/` tha eixe kai 3o meros
    char *dom = strtok(ufields[1],"/");
    if(dom){
        // if space in dom ? .. blepe parakatw
        size_t l = strlen(dom);
        char *domain = (char *)malloc( (l+1)*sizeof(char));
        strcpy( domain, dom);
        domain[l]=0;

        l_trim(r_trim(domain));
        size_t dl = strlen(domain);

        size_t vdl = strspn(domain, "abcdefghijklmnopqrstuvwxyz0123456789-._");
        // str1 : string to be scanned, str2 : string containing the characters to match
        // Return Value : the number of characters in the initial segment of str1 which consist only of characters from str2.
        // edw an exei keno mesa sto domain part tote theoreitai in-valid , lathos
        if(vdl != dl){
            // printf(" **ERROR,  Invalid Characters in Domain Part.");
            isValid = 0;
        } else {
            char *domsuffix ; // = NULL ;
            domsuffix = strrchr(domain,'.') ;	// psaxnw `.` apo to telos pros tin arxi
            if(domsuffix == NULL){
                // printf(" **ERROR,  Bad Domain Suffix NULL.");
                isValid = 0;
            } else {
                // printf("\n ... isvalidurl :: Domain Suffix=%s",domsuffix);
                size_t dsfl = strlen(domsuffix);
                if(dsfl<3 || dsfl > 5) {
                    // printf(" **ERROR,  Bad Domain Suffix len (=%lu).",dsfl);
                    isValid = 0;
                }
            }
        }
        while(dom !=NULL) {		// loop to clean dom and reset ufields[1] pointer to original position because strtok messes it !
            dom = strtok(NULL,"/");
        }
        free(domain);
    } else {
        isValid = 0;
    }


    if(dom != NULL) { free(dom); dom = NULL; }
    if(ufields[0]!=NULL) { free(ufields[0]); ufields[0]=NULL ; }
    if(ufields[1]!=NULL) { free(ufields[1]); ufields[1]=NULL ; }

    free(lurl);
    lurl=NULL;
    // printf(", finished, isValid=%d.",isValid);
    return isValid;

}

/* ~~~~~~~~~~~~crt_incident_node  creates a new incident node from line data~~~~~~~~~~~~~~~~~~~~
  used by :: load() and new_incident() to
  1) parse the csv line,
  2) validate data and,
  3) create a new incident node and fill it with the data if all okey ***
  calls   :: csvlinetofields()
*/
incident * crt_incident_node( char *param ) {

    // the array of field string where the fields of the line will be placed in their order. we know that csv line has 10 fields
    char ** fields = (char **) malloc( 10 * sizeof(char*));
    if( fields==NULL )
    {
        printf("(*ERROR*: mem alloc for fields table failed)");
        return NULL;
    }
    int i;

    // initialize to NULL so we know which fields were not parsed (missing) from line.
    for ( i=0; i<10; i++){
        fields[i] = NULL;
    }

    // parse csv line to fields array
    int number_of_fields = csvlinetofields( param, fields, 10 );

    if(number_of_fields < 7 ) {
        printf("(*ERROR*: less than 7 fields parsed)");
        for ( i=0; i<10; i++)
            if( fields[i]!=NULL ) free(fields[i]);
        free(fields);
        return NULL;
    }

    // create new incList node
    struct incList *newincident = malloc( sizeof(struct incList) );
    if( newincident==NULL) {
        printf("(*ERROR*: Malloc for NewInicdent)");
        for ( i=0; i<10; i++) if( fields[i]!=NULL ) free(fields[i]);
        free(fields);
        return NULL;
    }

/* *** incident fields validation **** */
    int isValid = 1;
    int total_missing=0;
    int total_dead_women=0;
    int total_dead_men=0;
    int total_dead_children=0;
    size_t lname = 0;
    size_t lcause = 0;
    size_t lldescr = 0;
    size_t lurl = 0;
    int totalincsize = 0;



    if( fields[LOCNAMEF]!=NULL && fields[LOCNAMEF][0]!=0) {
        l_trim(r_trim(fields[LOCNAMEF]));
        lname = strlen(fields[LOCNAMEF]);
        totalincsize+=lname;
        newincident->location = (char *)malloc((strlen(fields[LOCNAMEF])+1)*sizeof(char));
        if(newincident->location == NULL ) {
            printf("(*ERROR*: Malloc for NewInicdent->location)");
            isValid = 0;
        }
        else {
            strcpy(newincident->location, fields[LOCNAMEF]);
            newincident->location[strlen(fields[LOCNAMEF])]=0;
        }
    } else {
        printf("(*ERROR*: Missing LocationName)");
        isValid = 0;
    }

    if( fields[DATEF]!=NULL && fields[DATEF][0]!=0 && strlen(fields[DATEF]) >= 10) {

        int d = 0;
        int m = 0;
        int y = 0;
        int sresult = sscanf(fields[1], "%d/%d/%d", &d, &m, &y);
        if (sresult == EOF || sresult != 3) {
            printf("(*ERROR*: Bad date 2)");
            isValid = 0;
            // return NULL;
        }
        else
        {
            if (d > 0 && d <= 31)
                newincident->reported.day = d;
            else {
                printf("(*ERROR*: Bad Day %d)", d);
                isValid = 0;
                // return NULL;
            }
            if (m > 0 && m <= 12)
                newincident->reported.month = m;
            else {
                printf("(*ERROR*: Bad Month %d)",m);
                isValid = 0;
                // return NULL;
            }
            if (y >= 1900 && y <2100)
                newincident->reported.year = y;
            else {
                printf("(*ERROR*: Bad Year) %d",y);
                isValid = 0;
                // return NULL;
            }
            if( isValidDate( y, m, d) != 1 ) {
                printf("(*ERROR*: In-Valid Date) %d/%d/%d",d,m,y);
                isValid = 0;
                // return NULL;
            }

        }

    } else {
        printf("(*ERROR*: Missing or Bad Date)");
        isValid = 0;
    }

    if( fields[TOTMISSF]!=NULL && fields[TOTMISSF][0]!=0) {
        int scresult = sscanf(fields[TOTMISSF],"%d",&total_missing);
        if(scresult==EOF || scresult==-1) {
            printf("(*ERROR*:  Bad TotalMissing)");
            isValid = 0;
        } else {
            newincident->total_missing = total_missing;
        }
    } else {
        printf("(*ERROR*: Missing TotalMissing)");
        isValid = 0;
    }

    if( fields[DEADWF]!=NULL && fields[DEADWF][0]!=0) {
        int scresult = sscanf(fields[DEADWF],"%d",&total_dead_women);
        if(scresult==EOF || scresult==-1) {
            printf("(*ERROR*: Bad WomenDeaths)");
            isValid = 0;
        } else {
            newincident->total_dead_women = total_dead_women;
        }
    } else {
        printf("(*ERROR*: Missing WomenDeaths)");
        isValid = 0;
    }

    if( fields[DEADMF]!=NULL && fields[DEADMF][0]!=0) {
        int scresult = sscanf(fields[DEADMF],"%d",&total_dead_men);
        if(scresult==EOF || scresult==-1) {
            printf("(*ERROR*: Bad MenDeaths)");
            isValid = 0;
        } else {
            newincident->total_dead_men = total_dead_men;
        }
    } else {
        printf("(*ERROR*: Missing MenDeaths)");
        isValid = 0;
    }

    if( fields[DEADCF]!=NULL && fields[DEADCF][0]!=0) {
        int scresult = sscanf(fields[DEADCF],"%d",&total_dead_children);
        if(scresult==EOF || scresult==-1) {
            printf("(*ERROR*: Bad ChildrenDeaths)");
            isValid = 0;
        } else {
            newincident->total_dead_children = total_dead_children;
        }
    } else {
        printf("(*ERROR*: Missing ChildrenDeaths)");
        isValid = 0;
    }

    if( fields[CAUSEF]!=NULL && fields[CAUSEF][0]!=0) {
        l_trim(r_trim(fields[CAUSEF]));
        lcause = strlen(fields[CAUSEF]);
        totalincsize+=lcause;
        newincident->cause_death = (char *)malloc((strlen(fields[CAUSEF])+1)*sizeof(char));
        if(newincident->cause_death == NULL ) {
            printf("(*ERROR*: Malloc for NewInicdent->cause_death)");
            isValid = 0;
        }
        else {
            strcpy(newincident->cause_death, fields[CAUSEF]);
            newincident->cause_death[strlen(fields[CAUSEF])]=0;
        }

    } else {
        printf("(WARNING: Missing CauseofDeath)");
        // newincident->cause_death = NULL;
        newincident->cause_death = malloc(1);
        newincident->cause_death[0]=0;
    }

    if( fields[LOCDSCRF]!=NULL && fields[LOCDSCRF][0]!=0) {
        l_trim(r_trim(fields[LOCDSCRF]));
        lldescr = strlen(fields[LOCDSCRF]);
        totalincsize+=lldescr;
        newincident->loc_descr = (char *)malloc((strlen(fields[LOCDSCRF])+1)*sizeof(char));
        if(newincident->loc_descr == NULL )
        {
            printf("(*ERROR*: Malloc for NewInicdent->loc_descr)");
            isValid = 0;
        } else {
            strcpy(newincident->loc_descr, fields[LOCDSCRF]);
            newincident->loc_descr[strlen(fields[LOCDSCRF])]=0;
        }
    } else {
        printf("(WARNING: Missing Locationdescription)");
        // newincident->loc_descr=NULL;
        newincident->loc_descr = malloc(1);
        newincident->loc_descr[0]=0;
    }

    if( fields[COORDF]!=NULL && fields[COORDF][0]!=0 && strlen(fields[8])>=29) {
        char coords[strlen(fields[8])+1];
        strcpy(coords,fields[8]);
        coords[strlen(fields[8])]=0;
        l_trim(r_trim(coords));
        float lat=0;
        float longi=0;
        int sresult = sscanf(coords, "%f, %f",&lat, &longi);; // sscanf(fields[8], "%f, %f",&lat, &longi);
        if(sresult == EOF || sresult != 2) {
            printf("(*ERROR*: Bad coordinates 2)");
            isValid = 0;
            // return NULL;
        }
        else {
            if (lat >= -90 && lat <= 90) newincident->coordinates.latitude = lat;
            else {
                printf("(*ERROR*: Bad latitude)");
                isValid = 0;
                // return NULL;
            }
            if (longi >= -180 && longi <= 180) newincident->coordinates.longitude = longi;
            else {
                printf("(*ERROR*: Bad longitude)");
                isValid = 0;
                // return NULL;
            }
        }
    } else {
        printf("(*ERROR*: Missing Coordinates)");
        isValid = 0;
    }

    if( fields[URLF]!=NULL && fields[URLF][0]!=0) {

        l_trim(r_trim(fields[URLF]));
        // fields[URLF][l]	= 0;
        lurl = strlen(fields[URLF]);
        totalincsize+=lurl;
        newincident->URL = (char *)malloc((strlen(fields[URLF])+1)*sizeof(char));
        if(newincident->URL == NULL )
        {
            printf("(*ERROR*: Malloc for NewInicdent->URL)");
            isValid = 0;
        } else {
            strcpy(newincident->URL, fields[URLF]);
            newincident->URL[strlen(fields[URLF])]=0;

            // TODO: Validation if url...
            if( isValidUrl( newincident->URL ) != 1) {
                printf("(*ERROR*: URL is In-Valid)");
                // mypause();
                isValid = 0;
            }
        }
    } else {
        newincident->URL = malloc(1);
        newincident->URL[0]=0;
    }

    for ( i=0; i<10; i++) if( fields[i]!=NULL ) free(fields[i]);
    free(fields);

    if(isValid == 0) {

        freeNode( newincident );
        return NULL;

    } else {
        return newincident;
    }

}

/* =========== program commands =============================================================== */

/*  command newIncident: creates a new incident from command line and adds it to incident list
example:
newIncident Mediterranean;12/03/2020;5;2;0;1;Drowning,Asphyxiation;Off the coast of Larache, Morocco;35.189860435378, -6.173145606543; https://archive.ph/gB4Vs
*/
incident * new_incident( incident *last, char *param, int* node_counter, int *maxlocname, int * maxcause, int * maxlocdescr, int * maxurl){
    int max_locname =(*maxlocname);
    int max_cause   =(*maxcause);
    int max_locdescr=(*maxlocdescr);
    int max_url     =(*maxurl);
    printf("\n  Command NewIncident ...");
    if(param==NULL || *param == 0) {
        printf(" *** ERROR * No parameter (incident csv data) provided ! \n");
        return last;
    }

    if(strlen(param)< 27 ) {
        printf("*** ERROR * Bad parameter provided. Incident csv data too short !\n ");
        return last;
    }

    l_trim(r_trim(param));			// clears whitespace chars from start and end of string (kai toys cr, lf )
    incident * newIncident = crt_incident_node( param );	// call crt_incident_node to validate and create a new incident node
    if(newIncident != NULL) {
        (*node_counter)++;
        newIncident->node_counter = *node_counter;
        last = add_node (last, newIncident) ;			// call add_node to add new node last in list
        printf(". Success.\n");
        if( newIncident->location!=NULL && newIncident->location[0]!=0 && strlen(newIncident->location) > max_locname )
            max_locname=strlen(newIncident->location);
        if( newIncident->cause_death!=NULL && newIncident->cause_death[0]!=0 && strlen(newIncident->cause_death) > max_cause)
            max_cause=strlen(newIncident->cause_death);
        if( newIncident->loc_descr!=NULL && newIncident->loc_descr[0]!=0 && strlen(newIncident->loc_descr) > max_locdescr)
            max_locdescr=strlen(newIncident->loc_descr);
        if( newIncident->URL!=NULL && newIncident->URL[0]!=0 && strlen(newIncident->URL) > max_url )
            max_url=strlen(newIncident->URL);

        (*maxlocname) =max_locname;
        (*maxcause)   =max_cause;
        (*maxlocdescr)=max_locdescr;
        (*maxurl)     =max_url;

    } else {
        printf(" *** FAILED.\n");
    }
    return last;

}



/* *** command load: loads lines of specified csv file, parses, validates, creates new node, adds node to the end of list *** */
incident * load( incident *last, char *param, int* node_counter, int *maxlocname, int * maxcause, int * maxlocdescr, int * maxurl){

    int max_locname =(*maxlocname);
    int max_cause   =(*maxcause);
    int max_locdescr=(*maxlocdescr);
    int max_url     =(*maxurl);

    printf("\n Load file: ");
    FILE *file;
    file = fopen(param, "r");
    if(file==NULL) {
        printf(" ERROR. Error opening file {%s} for reading!",param);
        return last;

    }
    // writelog( " Success." , 0, 1 );
    printf("\n");
    size_t line_buf_size = 0;
    char *line =NULL;                // required set to null
    ssize_t line_size = getline(&line, &line_buf_size, file);	// read 1st line
    // ssize_t line_size = read_line(&line, &line_buf_size, file) ;	// read 1st line

    int count_lines=0;
    int rejected_lines = 0;
    while (line_size >= 0)
    {
        l_trim(r_trim(line));		// trim left/right line also removes ending CR/LF
        count_lines++;
        printf("#%d", count_lines);
        incident * newIncident = crt_incident_node( line );  	// call crt_incident_node to validate data and create a new incident node from line fields
        if(newIncident != NULL) {

            (*node_counter)++;
            newIncident->node_counter = *node_counter;			// increment node_counter by one
            last = add_node (last, newIncident) ;			// call add_node to add new node last in list

            // calculate max field sizes. we need them for justified printings
            if( newIncident->location!=NULL && newIncident->location[0]!=0 && strlen(newIncident->location) > max_locname )
                max_locname=strlen(newIncident->location);
            if( newIncident->cause_death!=NULL && newIncident->cause_death[0]!=0 && strlen(newIncident->cause_death) > max_cause)
                max_cause=strlen(newIncident->cause_death);
            if( newIncident->loc_descr!=NULL && newIncident->loc_descr[0]!=0 && strlen(newIncident->loc_descr) > max_locdescr)
                max_locdescr=strlen(newIncident->loc_descr);
            if( newIncident->URL!=NULL && newIncident->URL[0]!=0 && strlen(newIncident->URL) > max_url )
                max_url=strlen(newIncident->URL);
            (*maxlocname) =max_locname;
            (*maxcause)   =max_cause;
            (*maxlocdescr)=max_locdescr;
            (*maxurl)     =max_url;

            printf(".");

        } else {
            rejected_lines++;			// new incident node from line fields creation failed. propably bad data.
            printf(" *** FAILED.|");

        }

        line_size = getline(&line, &line_buf_size, file);	// read next line
        // line_size = read_line(&line, &line_buf_size, file);	// read next line

    }
    fclose(file);

    free(line);

    printf("\n\n Load file: Finished. Processed %d lines. Rejected %d lines.",count_lines,rejected_lines);

    return last;
}


/* commands findC/findL/findR : */
void findlcr( incident *last, char *param, char findtype, int maxlocname, int maxcause, int maxlocdescr, int maxurl) {
    char strTitle[20];
    if(findtype=='L') { strcpy(strTitle,"Location"); strTitle[strlen("Location")]=0; }
    else if(findtype=='C') { strcpy(strTitle,"Cause of Death"); strTitle[strlen("Cause of Death")]=0; }
    else if(findtype=='R') { strcpy(strTitle,"Region"); strTitle[strlen("Region")]=0; }

    if(last == NULL){
        printf("\n * Searching %s: Empty List \n",strTitle);
        return;
    }

    char *delim = "|";
    size_t ldel = strlen(delim);
    size_t maxl= 6+ldel + maxlocname+ldel +10+ldel +3+ldel +3+ldel +3+ldel +
                 maxcause+ldel +maxlocdescr+ldel + 18+1+18+ldel +maxurl + 1;

    char * searchstr = str_lower_case(param);

    int totals = 0;
    int nfound = 0;
    int lFound = 0;

    printf("\n Searching for %s matching `%s` ...\n", strTitle, searchstr);

    printf(" AA; Location; Date; totMiss; DeadWomen; DeadMen; DeadChildren; Cause; Loc_descr; Coords; url\n");
    printf(" ============================================================================================\n");

    //title_line( line, maxl, delim, maxlocname, maxcause, maxlocdescr, maxurl );
    //printf("%s\n",line);

    incident * temp = last->next;
    do {
        totals++;
        lFound = 0;
        if( param[0]=='*' ) lFound=1;
        else {
            if(findtype=='L' &&  temp->location!= NULL && temp->location[0]!=0) {
                char * strtarget = str_lower_case(temp->location);
                char *s = strstr(strtarget, searchstr);
                if(s != NULL )
                    lFound=1;
                free(strtarget);
            }
            else if(findtype=='C' && temp->cause_death!= NULL && temp->cause_death[0]!=0) {
                char * strtarget = str_lower_case(temp->cause_death);
                char *s = strstr(strtarget, searchstr);
                if(s != NULL )
                    lFound=1;
                free(strtarget);
            }
            else if(findtype=='R' && temp->loc_descr!= NULL && temp->loc_descr[0]!=0) {
                char * strtarget = str_lower_case(temp->loc_descr);
                char *s = strstr(strtarget, searchstr);
                if(s != NULL )
                    lFound=1;
                free(strtarget);
            }
        }
        if(lFound==1) {
            nfound++;
            frmt_inc_line( temp, maxl, delim, maxlocname, maxcause, maxlocdescr, maxurl, stdout );
        }
        temp = temp->next;
    } while(temp != last->next);
    printf("\n======================================================");
    printf("\n ---- Found %d of %d searching `%s` for `%s` ---- ",nfound,totals,strTitle, searchstr);

    free(searchstr);
    return;

}

/* commands printA/printB : */
void printab( incident *last, char *param, char direction, int maxlocname, int maxcause, int maxlocdescr, int maxurl ){
    printf("\n ======= Incident List. Incidents ");
    if( direction == 'A' ) printf("Is or After date %s ========",param);
    else printf("IS or Before date %s ========",param);
    int daten[3]={0,0,0};
    int i = str_splitdatenum( param, daten );
    if(i != 3){
        printf("\n  ** ERROR ** Bad Date given .");
        return;
    }
    if (last == NULL){
        printf("\n The list is empty.\n");
        return;
    }
    printf("\n");

    incident *temp = last->next;//ksekinaw apo ton 1o
    int toPrint=0;
    char sdate[11];
    int totals = 0;
    int nfound = 0;
    char *delim = "|";
    size_t ldel = strlen(delim);
    size_t maxl= 6+ldel + maxlocname+ldel +10+ldel +3+ldel +3+ldel +3+ldel +
                 maxcause+ldel +maxlocdescr+ldel + 18+1+18+ldel +maxurl + 1;
    printf(" AA; Location; Date; totMiss; DeadWomen; DeadMen; DeadChildren; Cause; Loc_descr; Coords; url\n");
    printf(" ============================================================================================\n");

    do{
        strcpy(sdate,"00/00/0000");
        sdate[10]=0;
        toPrint=0;
        totals++;
        if( direction == 'A' ) {
            if( temp->reported.year > daten[2] ) toPrint=1;
            else if( temp->reported.year < daten[2] ) toPrint=0;
            else if( temp->reported.month > daten[1] ) toPrint=1;
            else if( temp->reported.month < daten[1] ) toPrint=0;
            else if( temp->reported.day >= daten[0] ) toPrint=1;
            else if( temp->reported.day < daten[0] ) toPrint=0;

        } else {
            if( temp->reported.year < daten[2] ) toPrint=1;
            else if( temp->reported.year > daten[2] ) toPrint=0;
            else if( temp->reported.month < daten[1] ) toPrint=1;
            else if( temp->reported.month > daten[1] ) toPrint=0;
            else if( temp->reported.day <= daten[0] ) toPrint=1;
            else if( temp->reported.day > daten[0] ) toPrint=0;
        }
        if( toPrint==1 ) {
            nfound++;
            // sprintf(sdate,"%.2d/%.2d/%.4d",temp->reported.day,temp->reported.month,temp->reported.year);
            // printf("\n node_counter=%d, sdate=%s, temp=%p, temp->next=%p.",
            //	temp->node_counter,
            //	sdate,
            //	temp,
            //	temp->next );
            frmt_inc_line( temp, maxl, delim, maxlocname, maxcause, maxlocdescr, maxurl, stdout );
        }
        temp = temp->next;

    }while(temp != last->next);//ews otou ftasw ston 1o xana

    if( direction == 'A' ) printf("\n === Incident List End Found %d of %d On or After date %s ========",nfound,totals,param);
    else printf("\n === Incident List End Found %d of %d On or Before date %s ========",nfound,totals,param);

    return;
}

/* ~~~~~~~~~~~~~~~~~~ command save <out_file>:  ~~~~~~~~~~~~~~~~~~~ */
void save( incident *last, char *param, int maxlocname, int maxcause, int maxlocdescr, int maxurl ){
    printf("\n Save:: saving to file ");
    if (last == NULL){
        printf("\n The list is empty.");
        return;
    }
    int totals = 0;
    FILE *file;
    char *filename;

    filename = (char *)malloc((strlen(param)+1) * sizeof(char));
    if(filename==NULL){
        printf("\n *** ERROR malloc for filename falied !");
        return ;
    }
    int i;
    for(i=0;i<strlen(param);i++){
        if(param[i]=='\\') filename[i]='/';
        else filename[i]=param[i];
    }
    filename[i]='\0';
    printf("%s ...",filename);

    file = fopen(filename, "w");
    if (file) {
        char *delim = ";";
        size_t ldel = strlen(delim);
        size_t maxl= 6+ldel + maxlocname+ldel +10+ldel +3+ldel +3+ldel +3+ldel +
                     maxcause+ldel +maxlocdescr+ldel + 18+1+18+ldel +maxurl + 1;

        title_line( maxl, ";", maxlocname, maxcause, maxlocdescr, maxurl, file );

        incident * temp = last->next;

        do {
            frmt_inc_line( temp, maxl, delim, maxlocname, maxcause, maxlocdescr, maxurl, file );

            totals++;
            temp = temp->next;

        } while(temp != last->next);

        fclose(file);

        printf("\n Finished. Total %d incidents were written to file.",totals);
    } else {
        printf("\n *** ERROR writing to file %s",filename);
    }
    free(filename);

}

/* command count missing|men|women|children */
void count_incidents( incident *last, char *param){

    char *strTitle;
    if ( strcmp(param,"missing") == 0 ) {
        strTitle="missing";
    }
    else if ( strcmp(param,"men") == 0 ) {
        strTitle="men";
    }
    else if ( strcmp(param,"women") == 0 ) {
        strTitle="women";
    }
    else if ( strcmp(param,"children") == 0 ) {
        strTitle="children";
    } else {
        printf("\n *** ERROR *** count, bad parameter, enter missing or men or women or children ");
        return;
    }
    if(last == NULL){
        printf("\n * Count %s: Empty List .",strTitle);
        return;
    }


    int cnt_missing=0;
    int cnt_men=0;
    int cnt_women=0;
    int cnt_children=0;

    incident * temp = last->next;
    do {
        cnt_missing  += temp->total_missing;
        cnt_women    += temp->total_dead_women;
        cnt_men      += temp->total_dead_men;
        cnt_children += temp->total_dead_children;
        temp = temp->next;
    } while(temp != last->next);

    if ( strcmp(param,"missing") == 0 ) {
        printf("\n  Count %s . Total = %d .", strTitle, cnt_missing);
    }
    else if ( strcmp(param,"men") == 0 ) {
        printf("\n  Count %s . Total = %d .", strTitle, cnt_men);
    }
    else if ( strcmp(param,"women") == 0 ) {
        printf("\n  Count %s . Total = %d .", strTitle, cnt_women);
    }
    else if ( strcmp(param,"children") == 0 ) {
        printf("\n  Count %s . Total = %d .", strTitle, cnt_children);
    }

}

/* used by printReverse(). is the recursive function */
void printReverseRecur(incident *last, incident *currInc, int maxlocname, int maxcause, int maxlocdescr, int maxurl) {

    char *delim = "|";
    size_t ldel = strlen(delim);
    size_t maxl= 6+ldel + maxlocname+ldel +10+ldel +3+ldel +3+ldel +3+ldel +
                 maxcause+ldel +maxlocdescr+ldel + 18+1+18+ldel +maxurl + 1;
    if( currInc == last ) {
        frmt_inc_line( currInc, /*line,*/ maxl, delim, maxlocname, maxcause, maxlocdescr, maxurl, stdout );
    }
    else {
        printReverseRecur(last, currInc->next, maxlocname, maxcause, maxlocdescr, maxurl);
        frmt_inc_line( currInc, /*line,*/ maxl, delim, maxlocname, maxcause, maxlocdescr, maxurl, stdout );
    }
    return;
}

/* command printreverse: */
void printReverse(incident *last, incident *currInc, int maxlocname, int maxcause, int maxlocdescr, int maxurl) {

    if(last == NULL) {
        printf("\n printReverse:: Empty List.\n");
        return;
    }
    printf("\n AA; Location; Date; totMiss; DeadWomen; DeadMen; DeadChildren; Cause; Loc_descr; Coords; url");
    printf("\n======================================================");
    printReverseRecur(last, currInc, maxlocname, maxcause, maxlocdescr, maxurl);
    printf("\n========= Print Reverse Finished. ======================");

}


/* ========== my extra commands ====================== */
/* command help: */
void onlinehelp(){
    printf("\n List of Commands and their parameters:");
    printf("\n ----------------------------------------");
    printf("\n exit                            : Exit program.");
    printf("\n newIncident <incident_csv_data> : add new incident by command.");
    printf("\n       Must be followed by csv data line as in example:");
    printf("\n       newIncident Mediterranean;12/03/2020;5;2;0;1;Drowning...");
    printf("\n load <file_path>                : loads incident from specified csv file.");
    printf("\n count missing|men|women|children: counts totals according to parameter.");
    printf("\n findL <part_of_Location>        : find Location");
    printf("\n findC <part_of_CauseOfDeath>    : find Cause of death");
    printf("\n findR <part_of_Region>          : find Region");
    printf("\n *** NOTE finds matches start or part. Character * means all. case-ins.");
    printf("\n printA <date>                   : print all incidents After date given.");
    printf("\n                                   e.g. printA 01/01/2020");
    printf("\n printB <date>                   : print all incidents Before date given.");
    printf("\n                                   e.g. printB 01/01/2020");
    printf("\n deleteN                         : deletes the Newest added incident.");
    printf("\n deleteO                         : deletes the Oldest added incident.");
    printf("\n save <file_path>                : saves incidents to a formatted text file.");
    printf("\n *** NOTE: commands are NOT Case-sensitive.");

}

/* print some nodes in list based on number. for debugging */
void print_list (incident *last, int lastcount)
{
    printf("\n\n print_list ---- Printing List ------ ");
    if (last == NULL){
        printf("\n ***** The list is empty ***** \n");
        return;
    }

    incident *tmp = last->next;	// ksekinaw apo ton 1o
    do{

        printf("\n %.6d, %s, %d/%d/%d (address=%p, next=%p)",
               tmp->node_counter,
               tmp->location,
               tmp->reported.day,
               tmp->reported.month,
               tmp->reported.year,
               tmp, tmp->next);
        if(lastcount == tmp->node_counter) break;
        tmp = tmp->next;

    }while(tmp != last->next); //ews otou ftasw ston 1o

    printf("\n ---- Printing List End ------\n");

}

/* print top nodes in list. for debugging */
void print_top (incident *last, char *param)
{
    printf("\n\n print_list ---- Printing List ------ ");
    if (last == NULL){
        printf("\n ***** The list is empty ***** \n");
        return;
    }

    int lastcount = 0;
    int sresult = sscanf(param, "%d",&lastcount);
    if(sresult == EOF || sresult != 2) {
        printf("\n *** ERROR Bad Number of Incidents given. \n");
    }


    incident *tmp = last->next;	// ksekinaw apo ton 1o
    do{

        printf("\n %.6d, %s, %d/%d/%d (address=%p, next=%p)",
               tmp->node_counter,
               tmp->location,
               tmp->reported.day,
               tmp->reported.month,
               tmp->reported.year,
               tmp, tmp->next);
        if(lastcount == tmp->node_counter) break;
        tmp = tmp->next;

    }while(tmp != last->next); //ews otou ftasw ston 1o

    printf("\n ---- Printing List End ------\n");

}


/* prints the content of node. for debugging  , command: dnode n */
void print_node (incident *last, char *param)
{
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n print_list ---- Dump Node ------      ");
    if (last == NULL){
        printf("\n ***** The list is empty ***** \n");
        return;
    }
    int nodecount=0;
    int sresult = sscanf(param, "%d",&nodecount);
    if(sresult == EOF || sresult != 1) {
        printf("\n *** ERROR Bad Node Number given. \n");
        return;
    }
    incident *tmp = last->next;//ksekinaw apo ton 1o
    do{
        if(nodecount == tmp->node_counter) {
            printf("\n cntr: %d",tmp->node_counter);
            printf("\n loc : %s",tmp->location);
            printf("\n Day : %d",tmp->reported.day);
            printf("\n mon : %d",tmp->reported.month);
            printf("\n Year: %d",tmp->reported.year);
            printf("\n Miss: %d",tmp->total_missing);
            printf("\n DWm : %d",tmp->total_dead_women);
            printf("\n DMn : %d",tmp->total_dead_men);
            printf("\n DCh : %d",tmp->total_dead_children);
            printf("\n caus: %s",tmp->cause_death);
            printf("\n des : %s",tmp->loc_descr);
            printf("\n lat : %f",tmp->coordinates.latitude);
            printf("\n long: %f",tmp->coordinates.longitude);
            printf("\n url : %s",tmp->URL);
            printf("\n node address=%p, node->next address=%p",tmp, tmp->next);
            break;
        }
        tmp = tmp->next;

    }while(tmp != last->next);//ews otou ftasw sto 1o xana

    printf("\n ---- Dump Node End ------\n");
}

