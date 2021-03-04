#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h>
#include <cstring>
#include <string.h>

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

    char *options;
    int mode;
    if(argv[1][0]=='-' && (argv[1][1]=='c' | argv[1][1]=='R' | argv[1][1]=='v')){ //Or argc==4
       options=argv[1];
       printf("%s\n",options);
    }

    //Turn mode (when written in digits) to an octal number in order to call chmod function
   else if(isdigit(argv[1][0])){
        mode=strtol(argv[1],0,8);

        if(chmod(argv[2],mode)<0){
           printf("ERROR");
    }
   }
   else{
       /*We need to know the current permissions in place because here we 
       are just adding or deleting permissions to certain people, we
       don't change the current permissions for the other people.
       */

       int size=strlen(argv[1]);
       printf("%d\n",size);
       struct stat fs;
       stat(argv[2],&fs); //Gets current permission
       int mode_u=0,mode_g=0,mode_o=0;
       
       //User Permission
       if(fs.st_mode & S_IRUSR){
           mode_u+=4;
       }
       if(fs.st_mode & S_IWUSR){
           mode_u+=2;
       }
       if(fs.st_mode & S_IXUSR){
           mode_u+=1;
       }
       //Group Permission
        if(fs.st_mode & S_IRGRP){
           mode_g+=4;
       }
       if(fs.st_mode & S_IWGRP){
           mode_g+=2;
       }
       if(fs.st_mode & S_IXGRP){
           mode_g+=1;
       }

       //Others Permission
        if(fs.st_mode & S_IROTH){
           mode_o+=4;
       }
       if(fs.st_mode & S_IWOTH){
           mode_o+=2;
       }
       if(fs.st_mode & S_IXOTH){
           mode_o+=1;
       }

       /*Let's consider letters option first (u+w)
       if(argv[1][0]=='u'){
          int i=1;
          while(i!=size-1){
              if(argv[1][i]=='+'){
                  i++;
                  if(argv[1][i]=='r'){
                      mode_u+=4;
                  }
                  if(argv[1][i]=='w'){
                      mode_u+=2;
                  }
                  if(argv[1][i]=='x'){
                      mode_u+=1;
                  }
                  i++;
              }

              if(argv[1][i]=='-'){
                  i++;
                  if(argv[1][i]=='r'){
                      mode_u-=4;
                  }
                  if(argv[1][i]=='w'){
                      mode_u-=2;
                  }
                  if(argv[1][i]=='x'){
                      mode_u-=1;
                  }
                  i++;
              }

              if(argv[1][i]=='='){
                  mode_u=0;
                  i++;
                  if(argv[1][i]=='r'){
                      mode_u+=4;
                  }
                  if(argv[1][i]=='w'){
                      mode_u+=2;
                  }
                  if(argv[1][i]=='x'){
                      mode_u+=1;
                  }
                  i++;
              }
          }
       }
*/





        char mode_str[3]={'0','0','0'};
        mode_str[0]=mode_u+'0';
        mode_str[1]=mode_g+'0';
        mode_str[2]=mode_o+'0';

        mode=strtol(mode_str,0,8);
        printf("%s\n",mode_str);

        if(chmod(argv[2],mode)<0){
           printf("ERROR");
    }




       

      
   }
   //Symbolic Link check
   /*
while ((ent = readdir(dir)) != NULL) {
  if (ent->d_type == DT_LNK) { 
            
  }
}   */
    
    return 0;



}