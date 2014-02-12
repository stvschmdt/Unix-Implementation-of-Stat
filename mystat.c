#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>

int main(int argc, char *argv[]){
	struct stat sb;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (stat(argv[1], &sb) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}
	printf("  File: \'%s\'\n", argv[1]);

	printf("  Size: %lld\t\t",

			(long long) sb.st_size);
	printf("Blocks: %lld\t",
			(long long) sb.st_blocks);
	printf("IO Block: %ld bytes\t",
			(long) sb.st_blksize);
	
	switch (sb.st_mode & S_IFMT) {
		case S_IRWXU:  printf("block device\n");            break;
		case S_IRUSR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:  printf("symlink\n");                 break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
	}

	printf("Device: %xh/%dd ", (long) sb.st_dev, (long) sb.st_dev);

	printf("Inode: %ld\t", (long) sb.st_ino);
	printf("Links: %ld\n",  (long) sb.st_nlink);

	int r, w, x, rg, wg, xg, ro, wo, xo;
	r = (sb.st_mode & S_IRWXU & S_IRUSR) ? 'r' : '-';
	w = (sb.st_mode & S_IRWXU & S_IWUSR) ? 'w' : '-';
	x = (sb.st_mode & S_IRWXU & S_IXUSR) ? 'x' : '-';
	rg = (sb.st_mode & S_IRWXG & S_IRGRP) ? 'r' : '-';
	wg = (sb.st_mode & S_IRWXG & S_IWGRP) ? 'w' : '-';
	xg = (sb.st_mode & S_IRWXG & S_IXGRP) ? 'x' : '-';
	ro = (sb.st_mode & S_IRWXO & S_IROTH) ? 'r' : '-';
	wo = (sb.st_mode & S_IRWXO & S_IWOTH) ? 'w' : '-';
	xo = (sb.st_mode & S_IRWXO & S_IXOTH) ? 'x' : '-';


	printf("Access: (%.4o/-%c%c%c%c%c%c%c%c%c)\t",
			(unsigned long) sb.st_mode, r,w,x,rg,wg,xg,ro,wo,xo);

	struct passwd *pwd;
	pwd = getpwuid(sb.st_uid);

	struct group *gp;
	gp = getgrgid(sb.st_gid);

	printf("Uid=(%ld/%s)   Gid=(%ld/%s)\n",
			(long) sb.st_uid, pwd->pw_name, (long) sb.st_gid, gp->gr_name);


	printf("Access: %s", asctime(gmtime(&sb.st_ctime)));
	printf("Modify: %s", asctime(gmtime(&sb.st_atime)));
	printf("Change: %s", asctime(gmtime(&sb.st_mtime)));

	exit(EXIT_SUCCESS);
}
