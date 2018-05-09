#include "gt.h"

size_t writen(int fd,void *ptr,size_t n)
{
    size_t nleft = n;
    size_t nwriten = 0;
    char *p = (char *)ptr;

    while(nleft > 0)
    {
        nwriten = write(fd,p,n);
        if(nwriten <= 0 && errno == EINTR)
            nwriten = 0;
        else
            return -1;
        p += nwriten;
        nleft -= nwriten;
    }
    return n;
}
