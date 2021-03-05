#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

void all_plus_read(int &mode_u_r,int &mode_g_r,int &mode_o_r){
    mode_u_r=4;
    mode_g_r=4;
    mode_o_r=4;
}

void all_plus_write(int &mode_u_w,int &mode_g_w,int &mode_o_w){
    mode_u_w=2;
    mode_g_w=2;
    mode_o_w=2;
}

void all_plus_execute(int &mode_u_x,int &mode_g_x,int &mode_o_x){
    mode_u_x=1;
    mode_g_x=1;
    mode_o_x=1;
}

void all_minus_read(int &mode_u_r,int &mode_g_r,int &mode_o_r){
    mode_u_r=0;
    mode_g_r=0;
    mode_o_r=0;
}

void all_minus_write(int &mode_u_w,int &mode_g_w,int &mode_o_w){
    mode_u_w=0;
    mode_g_w=0;
    mode_o_w=0;
}

void all_minus_execute(int &mode_u_x,int &mode_g_x,int &mode_o_x){
    mode_u_x=0;
    mode_g_x=0;
    mode_o_x=0;
}

void parse(char *p,char *f,int &mode_u_r,int &mode_u_w,int &mode_u_x,int &mode_g_r,int &mode_g_w,int &mode_g_x,int &mode_o_r,int &mode_o_w,int &mode_o_x){
     int size=strlen(p);
     struct stat fs;
     stat(f,&fs); //Gets current permission
       
       
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
       if(p[0]=='u'){
          int i=1;
          while(i!=size){
              if(p[i]=='+' || p[i]=='='){
                  i++;
                  if(p[i]=='r'){
                      mode_u_r=4;
                      i++;
                  }
                  if(p[i]=='w'){
                      mode_u_w=2;
                      i++;
                  }
                  if(p[i]=='x'){
                      mode_u_x=1;
                      i++;
                  }
                  
              }
              if(p[i]=='-'){
                  i++;
                  if(p[i]=='r'){
                      mode_u_r=0;
                      i++;
                  }
                  if(p[i]=='w'){
                      mode_u_w=2;
                      i++;

                  }
                  if(p[i]=='x'){
                      mode_u_x=1;
                      i++;

                  }
              }
          }
       }

       //Group
       else if(p[0]=='g'){
          int i=1;
          while(i!=size){
              if(p[i]=='+' || p[i]=='='){
                  i++;
                  if(p[i]=='r'){
                      mode_g_r=4;
                      i++;
                  }
                  if(p[i]=='w'){
                      mode_g_w=2;
                      i++;
                  }
                  if(p[i]=='x'){
                      mode_g_x=1;
                      i++;
                  }
                  
              }
              if(p[i]=='-'){
                  i++;
                  if(p[i]=='r'){
                      mode_g_r=4;
                      i++;
                  }
                  if(p[i]=='w'){
                      mode_g_w=2;
                      i++;

                  }
                  if(p[i]=='x'){
                      mode_g_x=1;
                      i++;

                  }
              }
          }
       }

       //Others
       else if(p[0]=='o'){
          int i=1;
          while(i!=size){
              if(p[i]=='+' || p[i]=='='){
                  i++;
                  if(p[i]=='r'){
                      mode_o_r=4;
                      i++;
                  }
                  if(p[i]=='w'){
                      mode_o_w=2;
                      i++;
                  }
                  if(p[i]=='x'){
                      mode_o_x=1;
                      i++;
                  }
                  
              }
              if(p[i]=='-'){
                  i++;
                  if(p[i]=='r'){
                      mode_o_r=4;
                      i++;
                  }
                  if(p[i]=='w'){
                      mode_o_w=2;
                      i++;

                  }
                  if(p[i]=='x'){
                      mode_o_x=1;
                      i++;

                  }
              }
          }
       }

       //All
       else if(p[0]=='a'){
          int i=1;
          while(i!=size){
              if(p[i]=='+' || p[i] == '='){
                  i++;
                  if(p[i]=='r'){
                      all_plus_read(mode_u_r,mode_g_r,mode_o_r);
                      i++;
                  }
                  if(p[i]=='w'){
                      all_plus_write(mode_u_w,mode_g_w,mode_o_w);
                      i++;
                  }
                  if(p[i]=='x'){
                      all_plus_execute(mode_u_x,mode_g_x,mode_o_x);
                      i++;
                  }
                  
              }
              if(p[i]=='-'){
                  i++;
                  if(p[i]=='r'){
                      all_minus_read(mode_u_r,mode_g_r,mode_o_r);
                      i++;
                  }
                  if(p[i]=='w'){
                      all_minus_write(mode_u_w,mode_g_w,mode_o_w);
                      i++;

                  }
                  if(p[i]=='x'){
                    all_minus_execute(mode_u_x,mode_g_x,mode_o_x);
                    i++;

                  }
              }
          }
       }

       //No letter
       else{
           int i=0;
           while(i!=size){
            while(i!=size){
              if(p[i]=='+' || p[i] == '='){
                  i++;
                  if(p[i]=='r'){
                      all_plus_read(mode_u_r,mode_g_r,mode_o_r);
                      i++;
                  }
                  if(p[i]=='w'){
                      all_plus_write(mode_u_w,mode_g_w,mode_o_w);
                      i++;
                  }
                  if(p[i]=='x'){
                      all_plus_execute(mode_u_x,mode_g_x,mode_o_x);
                      i++;
                  }
                  
              }
              if(p[i]=='-'){
                  i++;
                  if(p[i]=='r'){
                      all_minus_read(mode_u_r,mode_g_r,mode_o_r);
                      i++;
                  }
                  if(p[i]=='w'){
                      all_minus_write(mode_u_w,mode_g_w,mode_o_w);
                      i++;

                  }
                  if(p[i]=='x'){
                    all_minus_execute(mode_u_x,mode_g_x,mode_o_x);
                    i++;

                  }
              }
          }
       }
       }
}

int xmod(int argc,char *argv[]){

    char *options;
    int mode;
    char mode_str[3]={'0','0','0'};
    int mode_u=0,mode_g=0,mode_o=0;
    int mode_u_r=0,mode_u_w=0,mode_u_x=0;
    int mode_g_r=0,mode_g_w=0,mode_g_x=0;
    int mode_o_r=0,mode_o_w=0,mode_o_x=0;
    if(argv[1][0]=='-' && (argv[1][1]=='c' | argv[1][1]=='R' | argv[1][1]=='v')){ //Or argc==4
       options=argv[1];
       printf("%s\n",options);
    }

    //Turn mode (when written in digits) to an octal number in order to call chmod function
   else if(isdigit(argv[1][0])){
        mode=strtol(argv[1],0,8);

        if(chmod(argv[argc - 1],mode)<0){
           printf("ERROR");
    }
   }
   else{
       /*We need to know the current permissions in place because here we 
       are just adding or deleting permissions to certain people, we
       don't change the current permissions for the other people.
       */
       parse(argv[1],argv[2],mode_u_r,mode_u_w,mode_u_x,mode_g_r,mode_g_w,mode_g_x,mode_o_r,mode_o_w,mode_o_x);

        mode_u=mode_u_r+mode_u_w+mode_u_x;
        mode_g=mode_g_r+mode_g_w+mode_g_x;
        mode_o=mode_o_r+mode_o_w+mode_o_x;

        mode_str[0]=mode_u+'0';
        mode_str[1]=mode_g+'0';
        mode_str[2]=mode_o+'0';

        mode=strtol(mode_str,0,8);
        printf("%s\n",mode_str);

        if(chmod(argv[argc - 1],mode)<0){
           printf("ERROR");
    } 
   }

   if(argc==4){ //In case we have options
       if(isdigit(argv[2][0])){
           mode=strtol(argv[1],0,8);

        if(chmod(argv[argc - 1],mode)<0){
           printf("ERROR");
    }
       }
       else{
       parse(argv[2],argv[3],mode_u_r,mode_u_w,mode_u_x,mode_g_r,mode_g_w,mode_g_x,mode_o_r,mode_o_w,mode_o_x);

        mode_u=mode_u_r+mode_u_w+mode_u_x;
        mode_g=mode_g_r+mode_g_w+mode_g_x;
        mode_o=mode_o_r+mode_o_w+mode_o_x;

        mode_str[0]=mode_u+'0';
        mode_str[1]=mode_g+'0';
        mode_str[2]=mode_o+'0';

        mode=strtol(mode_str,0,8);
        printf("%s\n",mode_str);

        if(chmod(argv[argc - 1],mode)<0){
           printf("ERROR");
    } 
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

void checkLog(char *envp[]){
    
    //Check if LOG_FILENAME was defined by user
    char *reg=secure_getenv("LOG_FILENAME");
    //printf("%s\n",reg);
   
    int fd;
    char const *text1="Holo Pat"; //Experiment

    if(access(reg,F_OK)==0){ //When file exists->Truncate it
        //printf("File exists\n");
        fd=open(reg,O_CREAT|O_TRUNC|O_WRONLY,0600);
        write(fd,text1,8);
        close(fd);
    }
    else{
        //If file doesn't exist->Create a new one
        //printf("File doesn't exist\n");
        fd = open(reg, O_CREAT | O_EXCL, 0644);
        write(fd,text1,8);
        close(fd);
    }
}

void checkSymlink(int argc, char *argv[]){
     //Symlink check.
    struct stat buffer;
    int t = lstat(argv[argc - 1], &buffer);
    if (S_ISLNK(buffer.st_mode))
    {
        char buf[PATH_MAX]; /* PATH_MAX incudes the \0 so +1 is not required */
        char *res = realpath(argv[argc - 1], buf);

        if (res == NULL)
        {
            perror("Couldn't find the real path of a symlink.");
        }
        else
        {
            //printf("Hihi, it worked!!!!\n");
            memcpy(argv[argc - 1], buf, strlen(buf));
            //printf("NOVO CAMINHO: %s\n", argv[argc - 1]);
        }
    }
}

int main(int argc,char *argv[],char *envp[]){ 
    if(argc<3){ //chmod options permissions file_name
                /*If no options are specified, chmod modifies the permissions of the file 
                specified by file name to the permissions specified by permissions.
                So it's possible to have only 3 arguments--->xmod, permissions, file_name*/
        printf("Not enough arguments\n");
        return 1;
    }

    checkLog(envp);
    checkSymlink(argc, argv);


    if(xmod(argc,argv)){
        return 1;
    }

    return 0;
}