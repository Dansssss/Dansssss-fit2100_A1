#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{
	int fd, fsize; 
	char *buffer,*filename;
	struct stat st;
	filename = "./logfile.txt";
	
	//copy to source directory
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		write(fd,"Unable to open read file!\n", 100);
		return 2;
	}
	else
	{
		stat(filename, &st);
		fsize = st.st_size;
		
		buffer=(char *)malloc((1+fsize)*sizeof(char));
		
		read(fd,buffer,fsize);
	
        close(fd); 
			
		fd = open("newfile.txt", O_WRONLY|O_CREAT);
		if (fd < 0)
		{
			write(fd,"Can't open newfile!\n'", 100);
			return 2;	
		}
		else
		{
			write(fd, buffer, fsize);
		}
			
		close(fd);
		free(buffer);
		exit(0);
	}
		
	
	
	
	
	
	
	
	
	
	
	
	//if (is_file_exist_stats(dest) == 1){
		
	
}
