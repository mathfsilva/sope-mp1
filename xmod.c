#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h>

int xmod(mode_t mode,char*path){

    return 0;
}

void output_permissions(mode_t m) //Prints current permissions
{
    putchar( m & S_IRUSR ? 'r' : '-' );
    putchar( m & S_IWUSR ? 'w' : '-' );
    putchar( m & S_IXUSR ? 'x' : '-' );
    putchar( m & S_IRGRP ? 'r' : '-' );
    putchar( m & S_IWGRP ? 'w' : '-' );
    putchar( m & S_IXGRP ? 'x' : '-' );
    putchar( m & S_IROTH ? 'r' : '-' );
    putchar( m & S_IWOTH ? 'w' : '-' );
    putchar( m & S_IXOTH ? 'x' : '-' );
    putchar('\n');
}

int main(int argc,char *argv[]){ //, char*env
    if(argc<3){ //chmod options permissions file_name
                /*If no options are specified, chmod modifies the permissions of the file 
                specified by file name to the permissions specified by permissions.
                So it's possible to have only 3 arguments--->xmod, permissions, file_name*/
        printf("Not enough arguments\n");
        return 1;
    }

    //Put code bellow in xmod()-probably?

    //Turn mode (when written in digits) to an octal number in order to call chmod function
    /*int mode=atoi(argv[1]);
    
    int mode_u = mode / 100;
    int mode_g = mode / 10 % 10;
    int mode_o = mode % 10;
    mode = (mode_u * 8 * 8) + (mode_g * 8) + mode_o; // Octal
    */
   if(isdigit(argv[1][0])){
        int mode=strtol(argv[1],0,8);

        if(chmod(argv[2],mode)<0){
           printf("ERROR");
    }
   }
   else{
       /*We need to know the current permissions in place because here we 
       are just adding or deleting permissions to certain people, we
       don't change the current permissions for the other people*/
       printf("Holo\n");
       if(argv[1][0]=='-'){ //if mode==-r--rw----
           printf("I do\n");
       }
       struct stat fs;
       stat(argv[2],&fs); //Gets current permission
       printf("Current permissions: \n");
       output_permissions(fs.st_mode); 
   }

   //Symbolic Link check
   /*
while ((ent = readdir(dir)) != NULL) {
  if (ent->d_type == DT_LNK) { 
            
  }
}   */
    
    return 0;



}