#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void display_file_info(const char *filename, const struct stat *file_info, int long_format) 
{
	if (long_format) 
	{
		printf((S_ISDIR(file_info->st_mode)) ? "d" : "-");
		printf((file_info->st_mode & S_IRUSR) ? "r" : "-");
		printf((file_info->st_mode & S_IWUSR) ? "w" : "-");
		printf((file_info->st_mode & S_IXUSR) ? "x" : "-");
		printf((file_info->st_mode & S_IRGRP) ? "r" : "-");
		printf((file_info->st_mode & S_IWGRP) ? "w" : "-");
		printf((file_info->st_mode & S_IXGRP) ? "x" : "-");
		printf((file_info->st_mode & S_IROTH) ? "r" : "-");
		printf((file_info->st_mode & S_IWOTH) ? "w" : "-");
		printf((file_info->st_mode & S_IXOTH) ? "x" : "-");
		printf(" ");
		printf("%ld ", (long)file_info->st_nlink);

		struct passwd *pwd = getpwuid(file_info->st_uid);
		if (pwd != NULL) 
		{
			printf("%s ", pwd->pw_name);
		}
	       	else 
		{
			printf("%d ", file_info->st_uid);
		}		

		struct group *grp = getgrgid(file_info->st_gid);
		if (grp != NULL) 
		{
			printf("%s ", grp->gr_name);
		}
	       	else
	       	{
			printf("%d ", file_info->st_gid);
		}

		printf("%8ld ", (long)file_info->st_size);

		char timebuf[80];
		struct tm *tm_info = localtime(&file_info->st_mtime);
		strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
		printf("%s ", timebuf);
	}
	printf("%s", filename);
	if (long_format && S_ISLNK(file_info->st_mode)) 
	{
		char link_target[1024];
		ssize_t len = readlink(filename, link_target, sizeof(link_target) - 1);
		if (len != -1) 
		{
			link_target[len] = '\0';
			printf(" -> %s", link_target);
		}
	}

	printf("\n");
}

void do_ls(const char *pathname, int long_format)
{
	struct stat file_info;
	if (stat(pathname, &file_info) == -1) 
	{
		perror(pathname);
		return;
	}
	if (S_ISDIR(file_info.st_mode)) 
	{
		DIR *dir_ptr = opendir(pathname);
		if (dir_ptr == NULL) 
		{			
			perror(pathname);
			return;
		}
		if (long_format) 
		{
			printf("total %ld\n", (long)file_info.st_blocks / 2);
		}	
		struct dirent *dirent_ptr;
		while ((dirent_ptr = readdir(dir_ptr)) != NULL) 
		{
			if (dirent_ptr->d_name[0] == '.') 
			{
				continue;
			}
			char fullpath[1024];
			snprintf(fullpath, sizeof(fullpath), "%s/%s", pathname, dirent_ptr->d_name);
			if (stat(fullpath, &file_info)) 
			{
				perror(fullpath);
				continue;
			}
			display_file_info(dirent_ptr->d_name, &file_info, long_format);
		}
		closedir(dir_ptr);
	} 
	else 
	{
		display_file_info(pathname, &file_info, long_format);
	}
}

int main(int argc, char *argv[]) 
{
	int long_format = 0;
	int opt;
	while ((opt = getopt(argc, argv, "l")) != -1) 
	{
		switch (opt) 
		{
			case 'l':
				long_format = 1;
				break;
			default:
				fprintf(stderr, "Usage: %s [-l] [file...]\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	if (optind >= argc) 
	{
		do_ls(".", long_format);
	} 
	else 
	{
		for (int i = optind; i < argc; i++) 
		{
			if (argc - optind > 1) 
			{
				printf("%s:\n", argv[i]);
			}
			do_ls(argv[i], long_format);
			if (i < argc - 1) 
			{
				printf("\n");
			}
		}
	}
	return 0;
}
