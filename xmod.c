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
       int mode_u_r=0,mode_u_w=0,mode_u_x=0;
       int mode_g_r=0,mode_g_w=0,mode_g_x=0;
       int mode_o_r=0,mode_o_w=0,mode_o_x=0;

       
       //User Permission
       if(fs.st_mode & S_IRUSR){
           mode_u_r+=4;
       }
       if(fs.st_mode & S_IWUSR){
           mode_u_w+=2;
       }
       if(fs.st_mode & S_IXUSR){
           mode_u_x+=1;
       }
       //Group Permission
        if(fs.st_mode & S_IRGRP){
           mode_g_r+=4;
       }
       if(fs.st_mode & S_IWGRP){
           mode_g_w+=2;
       }
       if(fs.st_mode & S_IXGRP){
           mode_g_x+=1;
       }

       //Others Permission
        if(fs.st_mode & S_IROTH){
           mode_o_r+=4;
       }
       if(fs.st_mode & S_IWOTH){
           mode_o_w+=2;
       }
       if(fs.st_mode & S_IXOTH){
           mode_o_x+=1;
       }

       //Let's consider letters option first (u+w)
       //User
       if(argv[1][0]=='u'){
          int i=1;
          while(i!=size){
              if(argv[1][i]=='+'){
                  i++;
                  if(argv[1][i]=='r'){
                      mode_u_r=4;
                      i++;
                  }
                  if(argv[1][i]=='w'){
                      mode_u_w=2;
                      i++;
                  }
                  if(argv[1][i]=='x'){
                      mode_u_x=1;
                      i++;
                  }
                  
              }
            //Check how to stop from taking permission off of stuff that wasn't to begin with
              if(argv[1][i]=='-'){
                  i++;
                  if(argv[1][i]=='r'){
                      mode_u_r=0;
                      i++;
                  }
                  if(argv[1][i]=='w'){
                      mode_u_w=2;
                      i++;

                  }
                  if(argv[1][i]=='x'){
                      mode_u_x=1;
                      i++;

                  }
              }

              if(argv[1][i]=='='){
                  mode_u_r=0,mode_u_w=0,mode_u_x=0;
                  i++;
                  if(argv[1][i]=='r'){
                      mode_u_r=4;
                      i++;

                  }
                  if(argv[1][i]=='w'){
                      mode_u_w=2;
                      i++;

                  }
                  if(argv[1][i]=='x'){
                      mode_u_x=1;
                      i++;

                  }
              }
          }
       }

       //Group
       if(argv[1][0]=='g'){
          int i=1;
          while(i!=size){
              if(argv[1][i]=='+'){
                  i++;
                  if(argv[1][i]=='r'){
                      mode_g_r=4;
                      i++;
                  }
                  if(argv[1][i]=='w'){
                      mode_g_w=2;
                      i++;
                  }
                  if(argv[1][i]=='x'){
                      mode_g_x=1;
                      i++;
                  }
                  
              }
            //Check how to stop from taking permission off of stuff that wasn't to begin with
              if(argv[1][i]=='-'){
                  i++;
                  if(argv[1][i]=='r'){
                      mode_g_r=4;
                      i++;
                  }
                  if(argv[1][i]=='w'){
                      mode_g_w=2;
                      i++;

                  }
                  if(argv[1][i]=='x'){
                      mode_g_x=1;
                      i++;

                  }
              }

              if(argv[1][i]=='='){
                  mode_g_r=0,mode_g_w=0,mode_g_x=0;
                  i++;
                  if(argv[1][i]=='r'){
                      mode_g_r=4;
                      i++;

                  }
                  if(argv[1][i]=='w'){
                      mode_g_w=2;
                      i++;

                  }
                  if(argv[1][i]=='x'){
                      mode_g_x=1;
                      i++;

                  }
              }
          }
       }

       //Others
       if(argv[1][0]=='o'){
          int i=1;
          while(i!=size){
              if(argv[1][i]=='+'){
                  i++;
                  if(argv[1][i]=='r'){
                      mode_o_r=4;
                      i++;
                  }
                  if(argv[1][i]=='w'){
                      mode_o_w=2;
                      i++;
                  }
                  if(argv[1][i]=='x'){
                      mode_o_x=1;
                      i++;
                  }
                  
              }
            //Check how to stop from taking permission off of stuff that wasn't to begin with
              if(argv[1][i]=='-'){
                  i++;
                  if(argv[1][i]=='r'){
                      mode_o_r=4;
                      i++;
                  }
                  if(argv[1][i]=='w'){
                      mode_o_w=2;
                      i++;

                  }
                  if(argv[1][i]=='x'){
                      mode_o_x=1;
                      i++;

                  }
              }

              if(argv[1][i]=='='){
                  mode_o_r=0,mode_o_w=0,mode_o_x=0;
                  i++;
                  if(argv[1][i]=='r'){
                      mode_o_r=4;
                      i++;

                  }
                  if(argv[1][i]=='w'){
                      mode_o_w=2;
                      i++;

                  }
                  if(argv[1][i]=='x'){
                      mode_o_x=1;
                      i++;

                  }
              }
          }
       }

       //All
       if(argv[1][0]=='a'){
          int i=1;
          while(i!=size){
              if(argv[1][i]=='+'){
                  i++;
                  if(argv[1][i]=='r'){
                      mode_u_r=4;
                      mode_g_r=4;
                      mode_o_r=4;
                      i++;
                  }
                  if(argv[1][i]=='w'){
                      mode_u_w=2;
                      mode_g_w=2;
                      mode_o_w=2;
                      i++;
                  }
                  if(argv[1][i]=='x'){
                      mode_u_x=1;
                      mode_g_x=1;
                      mode_o_x=1;
                      i++;
                  }
                  
              }
            //Check how to stop from taking permission off of stuff that wasn't to begin with
              if(argv[1][i]=='-'){
                  i++;
                  if(argv[1][i]=='r'){
                      mode_u_r=0;
                      mode_g_r=0;
                      mode_o_r=0;
                      i++;
                  }
                  if(argv[1][i]=='w'){
                      mode_u_w=0;
                      mode_g_w=0;
                      mode_o_w=0;
                      i++;

                  }
                  if(argv[1][i]=='x'){
                      mode_u_x=0;
                      mode_g_x=0;
                      mode_o_x=0;
                      i++;

                  }
              }

              if(argv[1][i]=='='){
                  mode_u_r=0,mode_u_w=0,mode_u_x=0,mode_g_r=0,mode_g_w=0,mode_g_x=0,mode_o_r=0,mode_o_w=0,mode_o_x=0;
                  i++;
                  if(argv[1][i]=='r'){
                      mode_u_r=4;
                      mode_g_r=4;
                      mode_o_r=4;                     
                      i++;

                  }
                  if(argv[1][i]=='w'){
                      mode_u_w=2;
                      mode_g_w=2;
                      mode_o_w=2;
                      i++;

                  }
                  if(argv[1][i]=='x'){
                      mode_u_x=1;
                      mode_g_x=1;
                      mode_o_x=1;
                      i++;

                  }
              }
          }
       }

       //No letter: starting with -
       if(argv[1][0]=='-'){
           int i=1;
           while(i!=size){
                if(argv[1][i]=='r'){
                      mode_u_r=0;
                      mode_g_r=0;
                      mode_o_r=0;
                      i++;
                  }
                  if(argv[1][i]=='w'){
                      mode_u_w=0;
                      mode_g_w=0;
                      mode_o_w=0;
                      i++;

                  }
                  if(argv[1][i]=='x'){
                      mode_u_x=0;
                      mode_g_x=0;
                      mode_o_x=0;
                      i++;

                  }
           }

       }

       //No letter starting with +
       if(argv[1][0]=='+'){
          int i=1;
          while(i!=size){
                  if(argv[1][i]=='r'){
                      mode_u_r=4;
                      mode_g_r=4;
                      mode_o_r=4;
                      i++;
                  }
                  if(argv[1][i]=='w'){
                      mode_u_w=2;
                      mode_g_w=2;
                      mode_o_w=2;
                      i++;
                  }
                  if(argv[1][i]=='x'){
                      mode_u_x=1;
                      mode_g_x=1;
                      mode_o_x=1;
                      i++;
                  }
                  
              }
       }

       //No letter starting with =
       if(argv[1][0]=='='){
           int i=1;
           while(i!=size){
               mode_u_r=0,mode_u_w=0,mode_u_x=0,mode_g_r=0,mode_g_w=0,mode_g_x=0,mode_o_r=0,mode_o_w=0,mode_o_x=0;
                  if(argv[1][i]=='r'){
                      mode_u_r=4;
                      mode_g_r=4;
                      mode_o_r=4;                     
                      i++;

                  }
                  if(argv[1][i]=='w'){
                      mode_u_w=2;
                      mode_g_w=2;
                      mode_o_w=2;
                      i++;

                  }
                  if(argv[1][i]=='x'){
                      mode_u_x=1;
                      mode_g_x=1;
                      mode_o_x=1;
                      i++;

                  }
              }
           }


        mode_u=mode_u_r+mode_u_w+mode_u_x;
        mode_g=mode_g_r+mode_g_w+mode_g_x;
        mode_o=mode_o_r+mode_o_w+mode_o_x;



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