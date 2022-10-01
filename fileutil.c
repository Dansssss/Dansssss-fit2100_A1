/*	
	name: Danlan Chen
	ID: 29920965
	start date:	Sept.8th
	last modified date: Sept. 14th (I have applied for extension)
	
	This program receives user's command which will include the filename and maybe arguments.
	It mainly copies and moves file to the specified destination directory. 
	And when error occurs, it shows different error messages.
	
	Only upon arguments including -d, -M, -F are not given, the program output the content of the specified file.
	If user does not specify a file, it output the contents of logfile.txt.
	But once user write arguments including -d, -M, -F, it will do the copy/move work preferentially.
	Upon successful completion of copying or moving, it outputs the successful message.
		 

*/

#include <string.h>
#include <getopt.h>				//this is for getopt(), references and comments are given when using that function afterwards
								//reference code is mainly from https://www.cnblogs.com/qingergege/p/5914218.html
								//		https://blog.csdn.net/mamong/article/details/20648575
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


//This function checks if the file exists in the destination directory.
//It return 1 if it does, otherwise returns 0
int is_file_exist_stats(const char *path)
{
	struct stat stats;

	stat(path, &stats);		
	
	if (stats.st_mode & F_OK)		//check the mode for file existence; F_OK reference: http://blog.sina.com.cn/s/blog_6a1837e90100uh5d.html
		return 1;

	return 0;
}

int main(int argc, char *argv[])
{
	//check for -d existence
	int d = 0;		
	//the target file path
	char *dest = NULL;		
	//check for -F existence
	int F = 0;		
	//check for -M existence
	int M = 0;		
	//struct about the file
	struct stat st;
	//fd points to the source file
	//fd_w points to the destination file
	//fsize is the length
	int fd, fd_w, fsize;
	//path of the source file
	char *filename;
	
	int opt;
	
	//this means we deal with command containing -d -F -M
	while ((opt = getopt(argc, argv, ":d:F:M")) != -1)       
	{
		//printf("opt: %c  optarg: %s  optopt: %c  optind: %d\n",(char)opt,optarg,(char)optopt,optind);
		switch (opt)
		{
		case 'F':			//if -F is given
			F = 1;
			break;
		case 'M':			//if -M is given
			M = 1;
			break;
		case 'd':			//if -d is given
			d = 1;		
			dest = optarg;			//this specifies the target file path
			break;
		case ':':
			write(2, "Invalid argument:immediately after -d,a directory path was expected", 100);
			return 1;
		case '?':
			write(2, "Invalid argument!", 50);
			return 1;
		}
	}

	//if -d, -F, -M are nor not given, program output the contents
	if (d == 0 && F == 0 && M == 0)
	{
		//if source file is specified, contents of source file would be output
		if (argc > 1)
		{
			filename = (char *)malloc(sizeof(char) * strlen(argv[1]));
			strcpy(filename, argv[1]);				//make filename be what user specifies
			fd = open(filename, O_RDONLY);
			if (fd < 0)
			{
				write(2, "file does not exist", 50);
				close(fd);
				return 1;
			}
			stat(filename, &st);
			fsize = st.st_size;
			char buf[fsize];
			read(fd, buf, fsize);
			write(1, buf, fsize);

			close(fd);
			return 0;
		}
		//user does not specify the source file, output the contents of logfile.txt
		else
		{
			filename = (char *)malloc(sizeof(char) * 14);
			filename = "./logfile.txt";

			fd = open(filename, O_RDONLY);
			if (fd < 0)
			{
				write(2, "file does not exist", 40);
				close(fd);
				return 1;
			}
			stat(filename, &st);
			fsize = st.st_size;
			char buf[fsize];
			read(fd, buf, fsize);
			write(1, buf, fsize);

			close(fd);
			return 0;
		}
	}
	
	//This is for case where arguments are given
	else
	{
		//if there is no -d command
		if (d == 0)
		{
			write(2, "Invalid argument!", 50);
			return 1;
		}
		//if there is -d command
		else			
		{
			//no source file is specified, the filename is logfile.txt
			filename = (char *)malloc(sizeof(char) * 14);
			filename = "./logfile.txt";

			//open the source file
			fd = open(filename, O_RDONLY);
			//if it cannot be open
			if (fd < 0)
			{
				write(2,"sourcefile does not exist", 50);
				close(fd);
				return 1;
			}
			
			//there is destination file
			if (is_file_exist_stats(dest) == 1)				
			{
				// -d -M are given, -F is not, program print error message
				if (F == 0)						
				{
					write(2, "file already exists\n", 100);
					return 2;
				}
				//otherwise, force open the destination directory
				fd_w = open(dest, O_WRONLY);
				//fail to open destination directory 
				if (fd_w < 0)
				{
					write(2, "creat destfile wrong\n", 50);
					close(fd_w);
					return 2;
				}
			}
			//there is no destination file
			else 
			{
				fd_w = open(dest, O_WRONLY | O_CREAT);
				if (fd_w < 0)
				{
					write(2, "creat destfile wrong\n", 50);
					close(fd_w);
					return 2;
				}
			}
			
			//write contents of source file into destination file
			stat(filename, &st);
			fsize = st.st_size;
			char buf[fsize];
			read(fd, buf, fsize);
			write(fd_w, buf, fsize);

			if (F == 1)
			{
				if (M == 1)
				{
					unlink(filename);		//delect the source file, this is easy. reference: https://blog.csdn.net/judgejames/article/details/83749669
					close(fd);
					close(fd_w);
					write(1, "force move successfully\n", 40);
					return 0;
				}
				//-M is not given, program does not delete the source file, which is copy function
				else
				{
					close(fd);
					close(fd_w);
					write(1, "force copy successfully\n", 40);
					return 0;
				}
			}
			//-F argument is not given
			else
			{
				//and -M is given, delete the source file
				if (M == 1)
				{
					unlink(filename);
					close(fd);
					close(fd_w);
					write(1, "move successfully\n", 30);
					return 0;
				}
				//do the copy job
				else
				{
					close(fd);
					close(fd_w);
					write(1, "copy successfully\n", 30);
					return 0;
				}
			}
		}
	}
	



}
