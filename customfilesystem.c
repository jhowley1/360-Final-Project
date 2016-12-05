//custom file system
//John Howley and Tim Rice
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs_level1.c"
#include "fs_level2.c"

SUPER *sp = NULL;
GD    *gp = NULL;
INODE *ip = NULL;
DIR   *dp = NULL;

MINODE minode[NMINODE] = {0};
PROC   proc[NPROC] = {0}, *running = NULL;
MINODE *root = NULL;

int dev = 0, imap = 0, bmap = 0;
int ninodes = 0, nblocks = 0;
int blk = 0, offset = 0;
int inodes_begin_block = 0, inodes_per_block = 0;
char buf[BLKSIZE] = { 0 };
char buf2[BLKSIZE] = { 0 };

int main(int argc, char *argv[], char *env[])
{
	int err = 0, i = 0;
	char input[256];
	char* splitInput[256];

	err = mount_root();
	
	if (!err)
	{
		printf ("Error mounting root");

		return 0;
	}

	while (1)
	{
		printf ("Please type a command: ");
		scanf ("%s", input);
		printf ("\n");

		while(i < 256)
		{
			splitInput[i] = NULL;

			i++;
		}

		i = 2;

		splitInput[0] = strtok(input, " ");

		splitInput[1] = strtok(NULL, " ");

		while (splitInput[i] = strtok(NULL, " "))
		{
			i++;
		}

		if (!strcmp(splitInput[0], "ls"))
		{
			if (splitInput[1])
			{
				ls(splitInput[1]);
			}
			else
			{
				ls("\n");
			}
		}
		else if (splitInput[1] && !strcmp(splitInput[0], "cd"))
		{
			cd(splitInput[1]);
		}
		else if (!strcmp(splitInput[0], "pwd"))
		{
			//pwd();
		}
		else if (splitInput[1] && !strcmp(splitInput[0], "mkdir"))
		{
			mkdir_fs(splitInput[1]);
		}
		else if (splitInput[1] && !strcmp(splitInput[0], "rmdir"))
		{
			rmdir_fs(splitInput[1]);
		}
		else if (splitInput[1] && !strcmp(splitInput[0], "creat"))
		{
			creat_fs(splitInput[1]);
		}
		else if (splitInput[1] && splitInput[2] && !strcmp(splitInput[0], "link"))
		{
			fs_link(splitInput[1], splitInput[2]);
		}
		else if (splitInput[1] && !strcmp(splitInput[0], "unlink"))
		{
			fs_unlink(splitInput[1]);
		}
		else if (splitInput[1] && splitInput[2] && !strcmp(splitInput[0], "symlink"))
		{
			fs_symlink(splitInput[1], splitInput[2]);
		}
		else if (!strcmp(splitInput[0], "readlink"))
		{
			fs_readlink();
		}
		else if (!strcmp(splitInput[0], "open") && splitInput[1]
&& splitInput[2])
		{
			fs_open(splitInput[1], splitInput[2]);
		}
		else if (!strcmp(splitInput[0], "close") && splitInput[1])
		{
			fs_close(splitInput[1]);
		}
		else if (!strcmp(splitInput[0], "lseek") && splitInput[1] && splitInput[2])
		{
			fs_lseek(splitInput[1], splitInput[2]);
		}
		else if (!strcmp(splitInput[0], "exit"))
		{
			break;
		}
		else
		{
			printf ("invalid command\n");
		}
	}
}

int mount_root()
{
	dev = open("diskimage", O_RDONLY);
	root = iget(dev, 2);

	printf("root\n");
	printf("dev: %d\n", root->dev);
	printf("ino: %d\n", root->ino);
	printf("refCount: %d\n", root->refCount);
	printf("dirty: %d\n", root->dirty);
	printf("mounted: %d\n", root->mounted);

	proc[0].cwd = iget(root->dev, 2);
	proc[1].cwd = iget(root->dev, 2);	

	running = &proc[0];

	if (!dev && !root && !running)
	{
		return 0;
	}

	return 1;
}
