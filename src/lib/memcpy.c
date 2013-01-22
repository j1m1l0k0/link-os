void memcpy(void *dst, void *src, unsigned int size)
{
    unsigned char *d = dst;
    unsigned char *s = src;
    while(size--){
        *d++ = *s++;
    }
}

/* needed by gcc */
void bcopy(void *dst, void *src, unsigned int size)
{
    memcpy(dst,src,size);
}

void memset(void *dst, char c, unsigned int size)
{
    unsigned char *d = dst;
    while(size--){
        *d++ = c;
    }
}
