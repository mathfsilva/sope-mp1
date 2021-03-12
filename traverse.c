#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ftw.h>
#include <stdio.h>

/*xmod(file_dir) //assuming -R option
  chmod(file_dir)
  if (file_dir is dir)
    for (fd in file_dir)
      if (fd is dir)
        fork()
        parent:
          waits for child
        child:
          exec xmod(fd2)
      else if (fd is file)
        chmod(fd2)
    end cycle
*/

int traverse(const char *dir_name)
{
  char path[1000];
  struct dirent *DIRECTORY;
  DIR *DP = opendir(dir_name);

  if (!DP)
  {
    //Couldn't open directory stream.
    return -1;
  }

  //readdir() returns NULL if we've reached the end.
  while ((DIRECTORY = readdir(DP)) != NULL)
  {
    if (strcmp(DIRECTORY->d_name, ".") != 0 && strcmp(DIRECTORY->d_name, "..") != 0)
    {
      //Distinguishing files and directories.
      if (DIRECTORY->d_type == DT_REG) // NÃO É PRECISO USAR O STAT, BASTA DIRECTORY->d_type == DT_REG
                                   // mas cuidado que não sendo regular não é necessariamente logo um diretório!!! ver man readdir
                                   // cuidado também com links simbólicos (mencionados no enunciado)
      {
        //When it's a file, we gotta change its permissions.
        printf("%s is a regular file.\n", DIRECTORY->d_name);
      }
      else if (DIRECTORY->d_type == DT_DIR) {
        //Construct new path, to keep traversal.
        printf ("%s is a directory.\n", DIRECTORY->d_name);
        strcpy(path, dir_name);
        strcat(path, "/");
        strcat(path, DIRECTORY->d_name);
        traverse(path);
    }

    }
  }

  closedir(DP);
}

int main()
{
  traverse("."); //probably need to change the arguments xD
}