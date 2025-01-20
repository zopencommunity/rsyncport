#ifndef STATS_ZOS_H_
#define STATS_ZOS_H_
/*
 * This is the same as the z/OS system header except
 * that the mode_t variables have been marked as unsigned int
 * since we are working internally in rsync with mode_t being a Linux
 * mode_t and not a z/OS mode_t (i.e. 16 bits and not 32 bits)
 */
 
  #if defined(__IBM_METAL__) 
 
    #error Language Environment standard C headers \
cannot be used when METAL option is used. \
Correct your header search path.
 
  #endif /* __IBM_METAL__ */
 
  #ifdef __cplusplus
  extern "C" {
  #endif
 
 
  #ifndef __EDC_LE
    #define __EDC_LE 0x10000000
  #endif
 
  #if __TARGET_LIB__ >= __EDC_LE
 
    #if !defined(__features_h)  || defined(__inc_features)
      #include <features.h>
    #endif
 
    #ifndef __modes
       #include <sys/modes.h>
    #endif
 
    #ifndef __types
       #include <sys/types.h>
    #endif
 
    #ifdef __FX
 
     #ifdef __AE_BIMODAL_F
       #pragma map(____chattr_a,    "\174\174A00123")
       #pragma map(____chattr_e,    "\174\174CHATTR")
       #pragma map(____lchattr_a,   "\174\174A00414")
       #pragma map(____lchattr_e,   "\174\174LCHATT")
     #endif  /* __AE_BIMODAL_F */
 
     #ifdef __NATIVE_ASCII_F
       #pragma map(__chattr,  "\174\174A00123")
       #pragma map(__lchattr, "\174\174A00414")
     #endif
 
     struct file_tag {                /* File Tag Attributes         */
       unsigned short ft_ccsid;       /*   Character Set Id          */
       unsigned int   ft_txtflag :1;  /*   Pure Text Flag            */
       unsigned int   ft_deferred:1;  /*   Defer Until 1st Write     */
       unsigned int   ft_rsvflags:14; /*   Reserved Flags            */
     };
     #define FT_UNTAGGED 0x0000
     #define FT_BINARY   0xFFFF
 
    /*
     *
     * f_attributes - File Attributes Structure for __chattr() and
     * and __fchattr(), and __lchattr(), C mapping of LFS BPXYATT.
     *
     */
     struct f_attributes {
 
      char   att_id[4];        /* Eye-catcher="ATT "                */
      short  att_version;      /* Version of this structure         */
      char   att_res01[2];     /* (reserved)                        */
 
      int    att_modechg:1,    /* X'80' 1=Change to mode indicated  */
             att_ownerchg:1,   /* X'40' 1=Change to Owner indicated */
             att_setgen:1,     /* X'20' 1=Set General Attributes    */
             att_trunc:1,      /* X'10' 1=Truncate Size             */
             att_atimechg:1,   /* X'08' 1=Change the Atime          */
             att_atimetod:1,   /* X'04' 1=Change Atime to Cur. Time */
             att_mtimechg:1,   /* X'02' 1=Change the Mtime          */
             att_mtimetod:1,   /* X'01' 1=Change Mtime to Cur. Time */
 
             att_maaudit:1,    /* X'80' 1=Modify auditor audit info */
             att_muaudit:1,    /* X'40' 1=Modify user audit info    */
             att_ctimechg:1,   /* X'20' 1=Change the Ctime          */
             att_ctimetod:1,   /* X'10' 1=Change Ctime to Cur. Time */
             att_reftimechg:1, /* X'08' 1=Change the RefTime        */
             att_reftimetod:1, /* X'04' 1=Change RefTime to Cur.Time*/
             att_filefmtchg:1, /* X'02' 1=Change File Format        */
             att_flgreserv1:1, /* X'01' (reserved flag bits)        */
 
             att_flgreserv2:1, /* X'80' (reserved flag bits)        */
             att_filetagchg:1, /* X'40' 1=Change File Tag           */
          #if __EDC_TARGET >= 0x41050000
             att_lp64times:1,  /* X'20' 1=Use 64-bit time values    */
             att_seclabelchg:1,/* X'10' 1=Change Seclabel           */
             att_flgreserv4:12;/*       (reserved flag bits)        */
          #else
             att_flgreserv3:14;/*       (reserved flag bits)        */
          #endif /* __EDC_TARGET >= 0x41050000 */
      unsigned int att_mode;   /* z/OS File Mode - 32 bits and not 16*/
      int    att_uid;          /* User ID of the owner of the file  */
      int    att_gid;          /* Group ID of the Group of the file */
 
      int    att_opaquemask:24,  /* (reserved for ADSTAR use)       */
             att_visblmaskres:3, /* (reserved for visible mask use) */
             att_sharelibmask:1, /* X'10' 1=Shared Library Mask     */
             att_noshareasmask:1,/* X'08' 1=No Shareas Flag Mask    */
             att_apfauthmask:1,  /* X'04' 1=APF Authorized Flag Mask*/
             att_progctlmask:1,  /* X'02' 1=Prog. Control Flag Mask */
             att_visblmskrmain:1;/* (reserved flag mask bit)        */
 
      int    att_opaque:24,      /* (reserved for ADSTAR use)       */
             att_visibleres:3,   /* (reserved for visible flag use  */
             att_sharelib:1,     /* X'10' 1=Shared Library Flag     */
             att_noshareas:1,    /* X'08' 1=No Shareas Flag         */
             att_apfauth:1,      /* X'04' 1=APF Authorized Flag     */
             att_progctl:1,      /* X'02' 1=Program Controlled Flag */
             att_visblrmain:1;   /* (reserved flag mask bit)        */
 
     #if !defined(__LF) && !defined(_LP64) /* if not large files,   */
                                 /* and not 64bit                   */
      int    att_size_h;         /*   first word of file size       */
     #endif /*not __LF and _64*/ /* end _LARGE_FILES *not* defined, */
                                 /* and not 64bit                   */
      off_t  att_size;           /* doubleword/second word file size*/
 
      #ifdef  _LP64
      char   att_atime_31[4];    /* Time of last access             */
      char   att_mtime_31[4];    /* Time of last data modification  */
      #else
      time_t att_atime;          /* Time of last access             */
      time_t att_mtime;          /* Time of last data modification  */
      #endif  /*  _LP64  */
 
      int    att_auditoraudit;   /* Area for auditor audit info     */
      int    att_useraudit;      /* Area for user audit info        */
 
      #ifdef  _LP64
      char   att_ctime_31[4];    /* Time of last file statuse change*/
      char   att_reftime_31[4];  /* Reference Time                  */
      #else
      time_t att_ctime;          /* Time of last file statuse change*/
      time_t att_reftime;        /* Reference Time                  */
      #endif  /*  _LP64  */
 
      char   att_filefmt;        /* File Format                     */
      char   att_res02[3];       /*  (reserved for expansion))      */
      struct file_tag att_filetag; /* File Tag                      */
      char   att_res03[8];       /*  (reserved for expansion)       */
 
    #ifdef  _LP64
      time_t att_atime;          /* Time of last access             */
      time_t att_mtime;          /* Time of last data modification  */
      time_t att_ctime;          /* Time of last file statuse change*/
      time_t att_reftime;        /* Reference Time                  */
    #endif  /*  _LP64  */
 
    #if (__EDC_TARGET >= 0x41050000) && !defined(_LP64)
      char   att_res04[32];      /*  (reserved for expansion)       */
    #endif /* __EDC_TARGET >= 0x41050000 && !defined(_LP64) */
 
    #if __EDC_TARGET >= 0x41050000
      char   att_seclabel[8];    /*  Security Label                 */
      char   att_res05[8];       /*  (reserved for expansion)       */
    #endif /* __EDC_TARGET >= 0x41050000 */
 
     };
     typedef struct f_attributes attrib_t;
 
     __new4102(int,__chattr,(char*,attrib_t*,int));
     __new4102(int,__fchattr,(int,attrib_t*,int));
     __new4105(int,__lchattr,(char*,attrib_t*,int));
 
     #ifdef __AE_BIMODAL_F
        __new4102(int,____chattr_a,(char*,attrib_t*,int));
        __new4102(int,____chattr_e,(char*,attrib_t*,int));
        __new4105(int,____lchattr_a,(char*,attrib_t*,int));
        __new4105(int,____lchattr_e,(char*,attrib_t*,int));
     #endif /* __AE_BIMODAL_F */
 
 
    #endif /* __FX */
 
   #ifdef __DOT1
    /*
     *
     *   POSIX.1
     *
     */
 
     #if __EDC_TARGET >= 0X22090000
       #ifndef __sys_mntent
         #include <sys/mntent.h>
       #endif
     #endif /* __EDC_TARGET >= 0X22090000*/
 
     #define S_MAJOR(x) ( ( ((x)>>16) & 0x0000FFFF))
     #define S_MINOR(x) ( ( (x) & 0x0000FFFF))
 
     struct stat {
           char         st_eye[4];     /* constant eye catcher        */
           unsigned short st_length;   /* Length of this struct       */
           unsigned short st_version;  /* version number              */
 
           unsigned int st_mode ;      /* z/OS File mode              */
           ino_t        st_ino  ;      /* File serial number          */
           dev_t        st_dev  ;      /* File Device ID              */
           nlink_t      st_nlink;      /* Number of links             */
           uid_t        st_uid  ;      /* User ID of the owner        */
           gid_t        st_gid  ;      /* Group ID of the group       */
           #if ( !defined(__LF) && !defined(_LP64) )
            int         st_size_h;     /*                             */
           #endif /* !__LF && !_LP64     */
           off_t        st_size ;      /* Size of the file            */
           #ifdef  _LP64
           char         st_atime_31[4];
           char         st_mtime_31[4];
           char         st_ctime_31[4];
           #else
           time_t       st_atime;      /* Time last access            */
           time_t       st_mtime;      /* Time last data modification */
           time_t       st_ctime;      /* Time last file status change*/
           #endif  /*  _LP64  */
 
           #ifdef __XPG4
             dev_t        st_rdev;     /* Major and Minor numbers
                                        *  for character special
                                        *   file
                                        */
           #else
             rdev_t       st_rdev;
           #endif
 
           unsigned int st_auditoraudit; /* Auditor audit info        */
           unsigned int st_useraudit;    /* User audit info           */
           #if ( defined(_LP64) || defined(__U98) || \
                 defined(__SUSV3_POSIX) ) 
             blksize_t    st_blksize;    /* File Block Size           */
           #elif  defined(__UU)
             long         st_blksize;    /* File Block Size           */
           #else
             int          st_blksize;
           #endif
           #ifdef  _LP64
           char         st_createtime_31[4];
           #else
           time_t       st_createtime;   /* File Creation Time        */
           #endif  /*  _LP64  */
           char         st_auditid[16];  /* File ID for auditing      */
           char         st_rsrvd1[4];    /* Reserved for expansion    */
           #ifdef __FX
             struct file_tag st_tag;     /* File Tag                  */
             char       st_charsetid[8]; /* Obsolete code char set ID */
           #else
           char         st_charsetid[12];/* Coded character set ID    */
           #endif /* __FX */
           #ifdef __LF
             long long  st_blocks;       /* Number of I/O blocks      */
           #elif defined(_LP64)
             blkcnt_t  st_blocks;
           #else
             int        st_blocks_h;
             #if defined(__U98) || defined(__SUSV3_POSIX) 
                blkcnt_t   st_blocks;
             #elif  __UU
                long    st_blocks;       /* Number of I/O blocks      */
             #else
                unsigned int st_blocks;
             #endif
           #endif /*  __LF */
           unsigned int st_genvalue;     /* general attribute values  */
           #ifdef  _LP64
           char         st_reftime_31[4];
           #else
           time_t       st_reftime;      /* reference time            */
           #endif  /*  _LP64  */
           char         st_fid[8];       /* file id                   */
           char         st_filefmt;      /* file format               */
           #if __EDC_TARGET >= 0x41030000
           char         st_fspflag2;     /* ACL support               */
           char         st_rsrvd2[2];    /* Reserved for expansion    */
           #else
           char         st_rsrvd2[3];    /* Reserved for expansion    */
           #endif /* __EDC_TARGET >= 0x41030000 */
           int          st_ctimemsec;    /* Micro-Sec of full Ctime   */
           #if __EDC_TARGET >= 0x41050000
           char         st_seclabel[8];  /* security label            */
           char         st_rsrvd3[4];    /* Reserved for expansion    */
           #else
           char         st_rsrvd3[12];   /* Reserved for expansion    */
           #endif /* __EDC_TARGET >= 0x41050000 */
           #ifdef  _LP64
           char         st_rsrvd4[4];    /* Alignment                 */
           time_t       st_atime;      /* Time last access            */
           time_t       st_mtime;      /* Time last data modification */
           time_t       st_ctime;      /* Time last file status change*/
           time_t       st_createtime;   /* File Creation Time        */
           time_t       st_reftime;      /* reference time            */
           char         st_rsrvd5[24];
           #endif  /*  _LP64  */
     };
 
    /*
     *
     * st_genvalue bits
     *
     */
     #define __ST_EXT_SYMLINK  0x01        /* external symbolic link  */
     #define __ST_PROG_CTL     0x02
     #define __ST_APF_AUTH     0x04
     #define __ST_NO_SHAREAS   0x08
 
     #if __EDC_TARGET >= 0X22090000
       #define __ST_SHARE_LIB    0x10
     #endif /* __EDC_TARGET >= 0X22090000*/
 
     /*
      * Constants for st_filefmt field of stat
      */
     #define S_FFNA       0       /* Not specified  */
     #define S_FFBINARY   1       /* Binary data    */
     #define S_FFNL       2       /* New Line       */
     #define S_FFCR       3       /* Carrage Return */
     #define S_FFLF       4       /* Line Feed      */
     #define S_FFCRLF     5       /* CR & LF        */
     #define S_FFLFCR     6       /* LF & CR        */
     #define S_FFCRNL     7       /* CR & NL        */ 
     #if __EDC_TARGET >= __EDC_LE410C 
     #define S_FFRECORD   8       /* Records        */ 
     #endif /* __EDC_TARGET >= __EDC_LE410C */ 
 
     #if __EDC_TARGET >= 0x41030000
     /*
      * Constants for st_fspflag2 field of stat
      *
      * Note:
      * File Model ACLs are also known as Default ACLs.
      * Directory Model ACLs are known as Directory Default ACLs.
      */
     #define S_ACCESSACL 0x80  /* Access ACL exists     */
     #define S_FMODELACL 0x40  /* File Model ACL exists */
     #define S_DMODELACL 0x20  /* Dir  Model ACL exists */
     #endif /* __EDC_TARGET >= 0x41030000 */
 
     /*
      *
      * function prototypes
      *
      */
     #ifdef __AE_BIMODAL_F
 
        #pragma map (__chmod_a,      "\174\174A00129")
        #pragma map (__chmod_e,      "\174\174CHMOD")
        #pragma map (__mkdir_a,      "\174\174A00130")
        #pragma map (__mkdir_e,      "\174\174MKDIR")
        #pragma map (__stat_a,       "\174\174A00131")
        #pragma map (__stat_e,       "\174\174STAT")
        #pragma map (__mkfifo_a,     "\174\174A00133")
        #pragma map (__mkfifo_e,     "\174\174MKFIFO")
        #pragma map (____open_stat_a,  "\174\174A00134")
        #pragma map (____open_stat_e,  "\174\174OPEN\174S")
 
     #endif /* __AE_BIMODAL_F */
 
     #ifdef __NATIVE_ASCII_F
       #pragma map (chmod,       "\174\174A00129")
       #pragma map (mkdir,       "\174\174A00130")
       #pragma map (mkfifo,      "\174\174A00133")
       #pragma map (__open_stat, "\174\174A00134")
       #ifdef __LF
         #pragma map (stat,      "\174\174A00132")
         #pragma map (fstat,     "\174\174FSTA\174O")
       #else
         #pragma map (stat,      "\174\174A00131")
       #endif /* __LF */
     #else
       #ifdef __LIBASCII_F
         #pragma map (chmod,    "\174\174CHMOD\174")
         #pragma map (mkdir,    "\174\174MKDIR\174")
         #pragma map (stat,     "\174\174STAT\174")
       #else
         #ifdef __LF
           #pragma map (fstat, "\174\174FSTA\174O")
           #pragma map (stat,  "\174\174STAT\174O")
         #endif /*__LF         */
       #endif  /* __LIBASCII_F */
       #pragma map (__open_stat, "\174\174OPEN\174S")
     #endif   /* __NATIVE_ASCII_F */
 
     #ifdef _NO_PROTO
       int chmod();
       int fstat();
       int mkdir();
       int mkfifo();
       int stat();
       unsigned int umask();
       int __open_stat();
     #else
       int chmod(const char *, unsigned int);
       int fstat(int, struct stat *);
       int mkdir(const char *, unsigned int);
       int mkfifo(const char *, unsigned int);
       int stat(const char * __restrict__,
                struct stat * __restrict__); 
       unsigned int umask(unsigned int);
       int __open_stat(const char *, int, unsigned int, struct stat *);
     #endif /* _NO_PROTO */
 
     #ifdef __AE_BIMODAL_F
       __new4102(int,__chmod_a,(const char *, unsigned int));
       __new4102(int,__chmod_e,(const char *, unsigned int));
       __new4102(int,__mkdir_a,(const char *, unsigned int));
       __new4102(int,__mkdir_e,(const char *, unsigned int));
       __new4102(int,__stat_a,(const char *, struct stat *));
       __new4102(int,__stat_e,(const char *, struct stat *));
       __new4102(int,____open_stat_a,
                       (const char *, int, unsigned int, struct stat *));
       __new4102(int,____open_stat_e,
                       (const char *, int, unsigned int, struct stat *));
       __new4102(int,__mkfifo_a,(const char *, unsigned int));
       __new4102(int,__mkfifo_e,(const char *, unsigned int));
     #endif /* __AE_BIMODAL_F */
 
   #endif  /* __DOT1 */
 
 
 
   #if defined(__DOT1A) || defined(__UU)
    /*
     *
     * POSIX.1a
     *
     */
 
     #ifdef __AE_BIMODAL_F
 
        #pragma map (__lstat_a,    "\174\174A00135")
        #pragma map (__lstat_e,    "\174\174LSTAT")
 
     #endif /* __AE_BIMODAL_F */
 
     #ifdef __NATIVE_ASCII_F
       #ifdef __LF
         #pragma map (lstat, "\174\174A00136")
       #else
         #pragma map (lstat, "\174\174A00135")
       #endif /*__LF         */
     #else
       #ifdef __LF
         #pragma map (lstat, "\174\174LSTA\174O")
       #endif /*__LF         */
     #endif /* __NATIVE_ASCII_F */
 
     /*
      *
      * function prototypes
      *
      */
 
 
     #ifdef _NO_PROTO
       int fchmod();
       int lstat();
     #else
       int fchmod(int, unsigned int);
       int lstat(const char * __restrict__,
                 struct stat * __restrict__); 
     #endif /* _NO_PROTO */
 
     #ifdef __AE_BIMODAL_F
       __new4102(int,__lstat_a,(const char *, struct stat *));
       __new4102(int,__lstat_e,(const char *, struct stat *));
     #endif /* __AE_BIMODAL_F */
 
 
   #endif /* __DOT1A or __UU */
 
 
 
   #ifdef _OPEN_SYS
 
    /*
     *
     * Extensions not allowed to be made visible under any
     * of the standard feature test macros.
     *
     */
 
    /*
     * Bit constants for mount() and unmount() flags
     */
 
     #define MTM_RDONLY      0x80000000 /* Mount file set read-only   */
     #define MTM_RDWR        0x40000000 /* Mount file set read/write  */
     #define MTM_UMOUNT      0x10000000 /* This is an unmount request */
     #define MTM_IMMED       0x08000000 /* Force unmount              */
     #define MTM_FORCE       0x04000000 /* Force unmount, possible loss
                                         *   of data
                                         */
     #define MTM_DRAIN       0x02000000 /* Drain all requests and wait
                                         *   for unmount to complete
                                         */
     #define MTM_RESET       0x01000000 /* Forget previous unmount    */
     #if __EDC_TARGET >= __EDC_LE410B 
     #define MTM_SAMEMODE    0x00100000 /* Remount in same mode  @D3A*/
     #endif /* __EDC_TARGET >= __EDC_LE410B */ 
     #define MTM_UNQSEFORCE  0x00040000 /* Force unquiesce request,
                                         *   even if the requester is
                                         *   not the process that made
                                         *   the quiesce request
                                         */
     #define MTM_NOSUID      0x00000400 /* Dont allow setuid          */
     #define MTM_SYNCHONLY   0x00000200 /* Mount must be completed
                                         *   synchronously.  That is,
                                         *   mount() must not return +1.
                                         */
     #define MTM_REMOUNT     0x00000100 /* Change attributes of mounted
                                         * file system
                                         */
 
     #if __EDC_TARGET >= 0X22070000
     #define MTM_NOSECURITY  0x00000080 /* Security checks will not
                                         * be enforced for files in
                                         * this file system
                                         */
     #endif /* __EDC_TARGET >= 0X22070000*/
 
    /*
     * Bit constants for chaudit() input value
     */
 
     #define AUDTREADFAIL  0x02000000   /* failing read accesses     */
     #define AUDTREADSUCC  0x01000000   /* successful read accesses  */
     #define AUDTWRITEFAIL 0x00020000   /* failing write accesses    */
     #define AUDTWRITESUCC 0x00010000   /* successful write accesses */
     #define AUDTEXECFAIL  0x00000200   /* failing exec or search    */
     #define AUDTEXECSUCC  0x00000100   /* successful exec or search */
 
     /*
      *
      * values for chaudit() options parameter.
      *
      */
     #define AUDT_USER     0x00         /* user audit option         */
     #define AUDT_AUDITOR  0x01         /* auditor audit option      */
 
     #if __EDC_TARGET >= 0x41050000
 
     /*
      * Constants for __writedown() writedownop:
      */
     #define __WD_QUERY       0          /* query write-down      */
     #define __WD_ACTIVATE    1          /* activate write-down   */
     #define __WD_INACTIVATE  2          /* in activate write-down*/
     #define __WD_RESET       3          /* reset write-down      */
     /*
      * Constants for __writedown() writedownscope:
      */
     #define __WD_SCOPE_AS    1          /* scope is process      */
     #define __WD_SCOPE_THD   2          /* scope is thread       */
     /*
      * Constants for __writedown() returned Int value when
      * doing a __writedown() query operation indicating the
      * the current setting of the writedown privelege:
     */
     #define __WD_IS_INACTIVE 0          /* write-down is inactive*/
     #define __WD_IS_ACTIVE   1          /* write-down is active  */
 
     #endif /* __EDC_TARGET >= 0x41050000 */
 
     /*
      *
      * function prototypes
      *
      */
 
     #ifdef __AE_BIMODAL_F
 
        #pragma map (__mknod_a,      "\174\174A00137")
        #pragma map (__mknod_e,      "\174\174MKNOD")
        #pragma map (__mount_a,      "\174\174A00138")
        #pragma map (__mount_e,      "\174\174MOUNT")
        #pragma map (__umount_a,     "\174\174A00139")
        #pragma map (__umount_e,     "\174\174UMOUNT")
        #pragma map (__chaudit_a,    "\174\174A00141")
        #pragma map (__chaudit_e,    "\174\174CHAUDI")
 
     #endif /* __AE_BIMODAL_F */
 
     #ifdef __NATIVE_ASCII_F
        #pragma map(mknod,   "\174\174A00137")
        #pragma map(mount,   "\174\174A00138")
        #pragma map(umount,  "\174\174A00139")
        #pragma map(chaudit, "\174\174A00141")
     #else
 
       #ifdef __LIBASCII_F
         #ifndef __UU
           #pragma map (mknod,    "\174\174MKNOD\174")
         #endif
       #endif /* __LIBASCII_F*/
 
       #if (__EDC_TARGET >= 0X22090000) || \
           (!defined(_NO_NEW_FUNC_CHECK))
         #ifndef _NO_PRAGMA
           #pragma map (__mount, "\174\174MOUN2")
         #endif /* _NO_PRAGMA */
       #endif /* __EDC_TARGET >= 0X22090000*/
 
     #endif /* __NATIVE_ASCII_F */
 
     #ifdef _NO_PROTO
       int chaudit();
       int fchaudit();
       #ifndef __UU
         int mknod();
       #endif
       int mount();
       int umount();
     #else
       int chaudit(const char *, unsigned int, unsigned int);
       int fchaudit(int, unsigned int, unsigned int);
       #ifndef __UU
         int mknod(const char *, unsigned int, rdev_t);
       #endif
       int mount(const char *, char *, char *, mtm_t, int, char *);
       int umount(const char *, mtm_t);
     #endif /* _NO_PROTO */
 
     __new4105(int, __writedown, (int , int ));
 
     __new29 (int, __mount, (struct mnte2 *, char *));
 
     #ifdef __AE_BIMODAL_F
       #ifndef __UU
         __new4102(int,__mknod_a,(const char *, unsigned int, rdev_t));
         __new4102(int,__mknod_e,(const char *, unsigned int, rdev_t));
       #endif
       __new4102(int,__chaudit_a,
                 (const char *, unsigned int, unsigned int));
       __new4102(int,__chaudit_e,
                 (const char *, unsigned int, unsigned int));
       __new4102(int,__mount_a,
                 (const char *, char *, char *, mtm_t, int, char *));
       __new4102(int,__mount_e,
                 (const char *, char *, char *, mtm_t, int, char *));
       __new4102(int,__umount_a,(const char *, mtm_t));
       __new4102(int,__umount_e,(const char *, mtm_t));
     #endif /* __AE_BIMODAL_F */
 
   #endif  /* _OPEN_SYS */
 
   #ifdef __UU
    /*
     *
     * X/Open Issue 4 Version 2 (XPG4 Extended)
     *
     */
 
     /*
      *
      * function prototypes
      *
      */
     #ifdef __AE_BIMODAL_F
       #pragma map (__mknod_a,      "\174\174A00137")
       #pragma map (__mknod_e,      "\174\174MKNOD")
     #endif /* __AE_BIMODAL_F */
 
     #ifdef __NATIVE_ASCII_F
       #pragma map(mknod,   "\174\174A00137")
     #endif /* __NATIVE_ASCII_F */
 
     #ifdef __LIBASCII_F
       #pragma map (mknod,    "\174\174MKNOD\174")
     #endif /* __LIBASCII_F*/
 
     #ifdef _NO_PROTO
       int mknod();
     #else
       int mknod(const char *, unsigned int, dev_t);
     #endif /* _NO_PROTO */
 
     #ifdef __AE_BIMODAL_F
       __new4102(int,__mknod_a,(const char *, unsigned int, rdev_t));
       __new4102(int,__mknod_e,(const char *, unsigned int, rdev_t));
     #endif /* __AE_BIMODAL_F */
 
   #endif /* __UU  */
 
  #endif  /* __TARGET_LIB__ >= __EDC_LE */
 
 
  #ifdef __cplusplus
  }
  #endif
 
                   #pragma checkout(resume)
                   ??=endif /* __stat */
