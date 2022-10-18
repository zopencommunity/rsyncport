/*       
 * Replacement header to provide Linux 'mode' values for rsync on z/OS
 */      
     #ifndef __modesh
       #define __modesh 1
      	
       #ifdef __DOT1
      	/*
         *
         * POSIX.1 
         *         
 	 */
                                          /* octal values */
         #define S_ISUID  0x0800          /*	 04000    */
         #define S_ISGID  0x0400          /*	 02000    */
         #define S_ISVTX  0x0200          /*	 01000    */

         #define S_IRUSR  0x0100          /*	 00400    */
         #define S_IWUSR  0x0080          /*	 00200    */
         #define S_IXUSR  0x0040          /*	 00100    */
         #define S_IRWXU  0x01C0          /*	 00700    */

         #define S_IRGRP  0x0020          /*	 00040    */
         #define S_IWGRP  0x0010          /*	 00020    */
         #define S_IXGRP  0x0008          /*	 00010    */
         #define S_IRWXG  0x0038          /*	 00070    */

         #define S_IROTH  0x0004          /*	 00004    */
         #define S_IWOTH  0x0002          /*	 00002    */
         #define S_IXOTH  0x0001          /*	 00001    */
         #define S_IRWXO  0x0007          /*	 00007    */

         /*
          *
          * These values not required by POSIX.
          *
          */

         #ifdef _ALL_SOURCE
           #define S_IREAD   S_IRUSR
           #define S_IWRITE  S_IWUSR
           #define S_IEXEC   S_IXUSR
 	 #endif

 	/*
 	 * The following are Linux values, not z/OS
 	 */
        #  define S_IFMT       0170000   /* [XSI] type of file mask */
 	#  define S_IFIFO      0010000   /* [XSI] named pipe (fifo) */
 	#  define S_IFCHR      0020000   /* [XSI] character special */
 	#  define S_IFDIR      0040000   /* [XSI] directory */
 	#  define S_IFBLK      0060000   /* [XSI] block special */
 	#  define S_IFREG      0100000   /* [XSI] regular */
 	#  define S_IFLNK      0120000   /* [XSI] symbolic link */
 	#  define S_IFSOCK     0140000   /* [XSI] socket */

 	 /* the macros themselves  */
 	 #define S_ISBLK(m)  ( ( (m) & S_IFMT)==S_IFBLK )
 	 #define S_ISDIR(m)  ( ( (m) & S_IFMT)==S_IFDIR )
 	 #define S_ISCHR(m)  ( ( (m) & S_IFMT)==S_IFCHR )
 	 #define S_ISFIFO(m) ( ( (m) & S_IFMT)==S_IFIFO)
 	 #define S_ISREG(m)  ( ( (m) & S_IFMT)==S_IFREG )
 	 #define S_ISLNK(m)  ( ( (m) & S_IFMT)==S_IFLNK )
 	 #define S_ISSOCK(m) ( ( (m) & S_IFMT)==S_IFSOCK)
      #endif
    #endif
