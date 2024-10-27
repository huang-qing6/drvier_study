#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

/** write ./hello_test /dev/xxx 100ask
 *  read ./hello_test /dev/xxx
*/
int main(int argc, char **argv){
    int fd;
    int len;
    char buf[100];

    if(argc < 2){
        printf("usage: \n");
        printf("%s <dev> [string]", argv[0]);
        return -1;
    }
    
    // open
    fd = open(argv[1], O_RDWR);
    if(fd < 0){
        printf("can not open file %s\n", argv[1]);
        return -1;
    }

    // write
    if(argc == 3){
        len = write(fd, argv[2], strlen(argv[2]));
        printf("write ret = %d", len);
    // read
    } else {
        len = read(fd, &buf, 100);
        buf[99] = '\0';
        printf("read str : %s\n", buf);
    }

    // close
    close(fd);
    return 0;
}