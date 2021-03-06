#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <dirent.h>
#include <unistd.h>

/** Source Used to study
 * http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html
 * http://www.gnu.org/software/libc/manual/html_node/Testing-File-Type.html
 * http://www.tutorialspoint.com/unix_system_calls/stat.htm
 * https://www.codingunit.com/c-tutorial-how-to-use-time-and-date-in-c
 * http://www.geeksforgeeks.org/sprintf-in-c/
*/


/* 5 points */

/**
 * This function prints the file type:
 * '-' for a regular file	'd' for a directory	'b' for a block special
 * 'c' for a character special	'p' for a FIFO(pipe)	'l' for a symbolic link
 * 's' for a socket		'?' for anything else
 * The file type is encoded in the st_mode member of the stat structure
 */
void printtype(struct stat st) // original one use *
{
    switch (st.st_mode & S_IFMT) {
        case S_IFBLK:  
            printf("b");            
            break;
        case S_IFCHR:  
            printf("c");        
            break;
        case S_IFDIR:  
            printf("d");               
            break;
        case S_IFIFO:  
            printf("p");               
            break;
        case S_IFLNK:  
            printf("l");                 
            break;
        case S_IFREG:  
            printf("-");            
            break;
        case S_IFSOCK: 
            printf("s");                  
            break;
        default:      
             printf("?");                
             break;
    }
}

/**
 * This prints the permissions for the file or directory in 'ls' format, i.e.:
 *   rwxrwxrwx
 */
void printperm(mode_t mode)
{

    printf( (mode & S_IRUSR) ? "r" : "-");
    printf( (mode & S_IWUSR) ? "w" : "-");
    printf( (mode & S_IXUSR) ? "x" : "-");
    printf( (mode & S_IRGRP) ? "r" : "-");
    printf( (mode & S_IWGRP) ? "w" : "-");
    printf( (mode & S_IXGRP) ? "x" : "-");
    printf( (mode & S_IROTH) ? "r" : "-");
    printf( (mode & S_IWOTH) ? "w" : "-");
    printf( (mode & S_IXOTH) ? "x" : "-");

}

/**
 * Prints the uid or username.  Use getpwuid() to get the username from the uid.
 * If the user is not found, just print the uid.  Format usernames or uids to
 * 8 characters in width.
 */
void printuser(uid_t uid)
{   
    struct group *grp = getpwuid(uid);
    if (grp != 0)
        printf(" %8s", grp->gr_name);
    else
        printf(" %8ld",(long) uid);



}

/**
 * Prints the gid or group name. Use getgrgid() to get the group name from the gid.
 * If the group is not found, just print the gid.  Format groupnames or gids to
 * 8 characters in width.
 */
void printgroup(gid_t gid)
{
    struct passwd *pwd = getgrgid(gid);
    if (pwd != 0)
        printf(" %8s", pwd->pw_name);
    else
        printf(" %8d", gid);

}



/**
 * Prints the time (Mmm DD HH:MM YYYY).  Use localtime() to convert the time_t
 * value (gotten from the st.st_mtime field)
 */
void printtime(time_t t)
{
    char * months_names[12] = {"Jan", "Feb", "Mar", "Apr", "May","Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char timeCoonverted[100];
    struct tm *tmInfo;
    tmInfo = localtime(&t);

    sprintf(timeCoonverted, " %s %2d %2d:%02d %d", months_names[tmInfo->tm_mon], tmInfo->tm_mday,
                                              tmInfo->tm_hour, tmInfo->tm_min, tmInfo->tm_year + 1900);
    printf("%s", timeCoonverted);
  //o;  printf(" Test: %d ",tmInfo->tm_mon);
    

}

// Prints the size in bytes of the file
void printSize(struct stat st)
{
    printf("  %8ld", st.st_size);


}

void printFileName(char *name)
{
    printf(" %s", name);
}

void printHardLinks(struct stat st)
{

    printf(" %2ld",st.st_nlink);
}

/**
 * This function lists a single file.  If the file cannot be stat()'ed, then
 * print that you cannot stat it (with the files name) and return.
 * Otherwise use the above functions to print all the information about the
 * file using the information from the stat() system call.
 * To use stat() given path and name, you must combine path and name into one
 * file path, use sprintf() into a character buffer to do this.
 * The order in which things should be printed, is:
 * 1) file-type 2) user permissions 3) group permissions 4) other permissions
 * 5) number of hard links 6) user-name/uid 7) group-name/gid 8) size in bytes
 * 9) last modification time 10) file-name
 * Just like with the ls command.
 */
void list(char *path, char *name)
{
    struct stat st;
    char pathName[100];
    sprintf(pathName,"%s/%s",path,name);
    if(stat(pathName,&st) < 0){    
        printf("File %s cannot be stat it\n", name);
        exit(1);
    }

    printtype(st);
    printperm(st.st_mode);
    printHardLinks(st);
    printuser(st.st_uid);
    printgroup(st.st_gid);
    printSize(st);
    printtime(st.st_mtime);
    printFileName(name);
    printf("\n");
    

}

/**
 * Use scandir() to read a list of files from the given directory path, sort
 * the files using the alphasort method. An example of how to use scandir() can
 * be found in its "man" page, which should be quite illuminating.
 */
void listdir(char *path)
{
    struct dirent **nameFiles;
    int numbOfFiles;

    numbOfFiles = scandir(path, &nameFiles, NULL, alphasort);
    if (numbOfFiles < 0)
       printf("Error opening files in directory\n");
    else {
        for (int i = 0; i < numbOfFiles; i++)
        {
            /// call list for each file in the directory
           list(path,nameFiles[i]->d_name);
           free(nameFiles[i]);
        }

       free(nameFiles);
    }


}

/**
 * Main should use listdir on "." (the current directory) if no arguments are
 * given, otherwise use listdir on each argv[i], i >= 1
 */
int main(int argc, char **argv)
{
     if(argc <2){ // list everething on current directory  
        
        listdir(".");

        return 0; // end program

    }

    for (int i = 1; i < argc; i++){ // get all arguments from argc
        
        listdir(argv[i]);
    }
 

    return 0;
}







