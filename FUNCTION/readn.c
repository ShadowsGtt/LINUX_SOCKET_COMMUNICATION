#include "gt.h"

size_t readn(int fd,void *ptr,size_t n)
{
    char *p = (char *)ptr;
    size_t nleft = n;
    size_t nread = 0;

    while(nleft > 0)
    {
        nread = read(fd,p,nleft);
        if(nread == -1)
        {
            if(errno == EINTR)
                nread = 0;
            else
                return -1;
        }
        if(nread == 0)
            break;
        nleft -= nread;
        p += nread;
    }
    return (n-nleft);
}

