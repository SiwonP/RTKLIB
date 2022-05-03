#ifndef STREAM_H
#define STREAM_H

#ifndef WIN32
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#define __USE_MISC
#ifndef CRTSCTS
#define CRTSCTS  020000000000
#endif
#include <errno.h>
#include <termios.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

/* constants -----------------------------------------------------------------*/

#define TINTACT             200         /* period for stream active (ms) */
#define SERIBUFFSIZE        4096        /* serial buffer size (bytes) */
#define TIMETAGH_LEN        64          /* time tag file header length */
#define MAXCLI              32          /* max client connection for tcp svr */
#define MAXSTATMSG          32          /* max length of status message */
#define DEFAULT_MEMBUF_SIZE 4096        /* default memory buffer size (bytes) */

#define NTRIP_AGENT         "RTKLIB/" VER_RTKLIB
#define NTRIP_CLI_PORT      2101        /* default ntrip-client connection port */
#define NTRIP_SVR_PORT      80          /* default ntrip-server connection port */
#define NTRIP_MAXRSP        32768       /* max size of ntrip response */
#define NTRIP_MAXSTR        256         /* max length of mountpoint string */
#define NTRIP_RSP_OK_CLI    "ICY 200 OK\r\n" /* ntrip response: client */
#define NTRIP_RSP_OK_SVR    "OK\r\n"    /* ntrip response: server */
#define NTRIP_RSP_SRCTBL    "SOURCETABLE 200 OK\r\n" /* ntrip response: source table */
#define NTRIP_RSP_TBLEND    "ENDSOURCETABLE"
#define NTRIP_RSP_HTTP      "HTTP/"     /* ntrip response: http */
#define NTRIP_RSP_ERROR     "ERROR"     /* ntrip response: error */
#define NTRIP_RSP_UNAUTH    "HTTP/1.0 401 Unauthorized\r\n"
#define NTRIP_RSP_ERR_PWD   "ERROR - Bad Pasword\r\n"
#define NTRIP_RSP_ERR_MNTP  "ERROR - Bad Mountpoint\r\n"

#define FTP_CMD             "wget"      /* ftp/http command */
#define FTP_TIMEOUT         30          /* ftp/http timeout (s) */

#define MIN(x,y)            ((x)<(y)?(x):(y))

/* macros --------------------------------------------------------------------*/

#ifdef WIN32
#define dev_t               HANDLE
#define socket_t            SOCKET
typedef int socklen_t;
#else
#define dev_t               int
#define socket_t            int
#define closesocket         close
#endif


/* type definition -----------------------------------------------------------*/

/**
 * @brief 
 * 
 */
typedef struct File {            /* file control type */
    FILE *fp;               /* file pointer */
    FILE *fp_tag;           /* file pointer of tag file */
    FILE *fp_tmp;           /* temporary file pointer for swap */
    FILE *fp_tag_tmp;       /* temporary file pointer of tag file for swap */
    char path[MAXSTRPATH];  /* file path */
    char openpath[MAXSTRPATH]; /* open file path */
    int mode;               /* file mode */
    int timetag;            /* time tag flag (0:off,1:on) */
    int repmode;            /* replay mode (0:master,1:slave) */
    int offset;             /* time offset (ms) for slave */
    int size_fpos;          /* file position size (bytes) */
    gtime_t time;           /* start time */
    gtime_t wtime;          /* write time */
    uint32_t tick;          /* start tick */
    uint32_t tick_f;        /* start tick in file */
    long fpos_n;            /* next file position */
    uint32_t tick_n;        /* next tick */
    double start;           /* start offset (s) */
    double speed;           /* replay speed (time factor) */
    double swapintv;        /* swap interval (hr) (0: no swap) */
    lock_t lock;            /* lock flag */
// } file_t;
} File_t;

/**
 * @brief 
 * 
 */
typedef struct TCP {            /* tcp control type */
    int state;              /* state (0:close,1:wait,2:connect) */
    char saddr[256];        /* address string */
    int port;               /* port */
    struct sockaddr_in addr; /* address resolved */
    socket_t sock;          /* socket descriptor */
    int tcon;               /* reconnect time (ms) (-1:never,0:now) */
    uint32_t tact;          /* data active tick */
    uint32_t tdis;          /* disconnect tick */
} TCP_t;

/**
 * @brief 
 * 
 */
typedef struct TCPServer { /* tcp server type */
    TCP_t svr;              /* tcp server control */
    TCP_t cli[MAXCLI];      /* tcp client controls */
} TCPServer_t;

/**
 * @brief 
 * 
 */
typedef struct TCPClient {            /* tcp cilent type */
    TCP_t svr;              /* tcp server control */
    int toinact;            /* inactive timeout (ms) (0:no timeout) */
    int tirecon;            /* reconnect interval (ms) (0:no reconnect) */
} TCPClient_t;

/**
 * @brief 
 * 
 */
typedef struct Serial {            /* serial control type */
    dev_t dev;              /* serial device */
    int error;              /* error state */
#ifdef WIN32
    int state,wp,rp;        /* state,write/read pointer */
    int buffsize;           /* write buffer size (bytes) */
    HANDLE thread;          /* write thread */
    lock_t lock;            /* lock flag */
    uint8_t *buff;          /* write buffer */
#endif
    TCPServer_t *tcpsvr;       /* tcp server for received stream */
} Serial_t;

/**
 * @brief 
 * 
 */
typedef struct NTrip {            /* ntrip control type */
    int state;              /* state (0:close,1:wait,2:connect) */
    int type;               /* type (0:server,1:client) */
    int nb;                 /* response buffer size */
    char url[MAXSTRPATH];   /* url for proxy */
    char mntpnt[256];       /* mountpoint */
    char user[256];         /* user */
    char passwd[256];       /* password */
    char str[NTRIP_MAXSTR]; /* mountpoint string for server */
    uint8_t buff[NTRIP_MAXRSP]; /* response buffer */
    TCPClient_t *tcp;          /* tcp client */
} ntrip_t;

/**
 * @brief 
 * 
 */
typedef struct NTripConnection {            /* ntrip client/server connection type */
    int state;              /* state (0:close,1:connect) */
    char mntpnt[256];       /* mountpoint */
    char str[NTRIP_MAXSTR]; /* mountpoint string for server */
    int nb;                 /* request buffer size */
    uint8_t buff[NTRIP_MAXRSP]; /* request buffer */
} ntripc_con_t;

/**
 * @brief 
 * 
 */
typedef struct NTripCaster {            /* ntrip caster control type */
    int state;              /* state (0:close,1:wait,2:connect) */
    int type;               /* type (0:server,1:client) */
    char mntpnt[256];       /* mountpoint */
    char user[256];         /* user */
    char passwd[256];       /* password */
    char srctbl[NTRIP_MAXSTR]; /* source table */
    TCPServer_t *tcp;          /* tcp server */
    ntripc_con_t con[MAXCLI]; /* ntrip client/server connections */
} ntripc_t;

/**
 * @struct UDP_t
 * @brief 
 * 
 */
typedef struct UDP {            /* udp type */
    int state;              /* state (0:close,1:open) */
    int type;               /* type (0:server,1:client) */
    int port;               /* port */
    char saddr[256];        /* address (server:filter,client:server) */
    struct sockaddr_in addr; /* address resolved */
    socket_t sock;          /* socket descriptor */
} UDP_t;

/**
 * @brief 
 * 
 */
typedef struct FTP {            /* ftp download control type */
    int state;              /* state (0:close,1:download,2:complete,3:error) */
    int proto;              /* protocol (0:ftp,1:http) */
    int error;              /* error code (0:no error,1-10:wget error, */
                            /*            11:no temp dir,12:uncompact error) */
    char addr[1024];        /* download address */
    char file[1024];        /* download file path */
    char user[256];         /* user for ftp */
    char passwd[256];       /* password for ftp */
    char local[1024];       /* local file path */
    int topts[4];           /* time options {poff,tint,toff,tretry} (s) */
    gtime_t tnext;          /* next retry time (gpst) */
    thread_t thread;        /* download thread */
} FTP_t;

/**
 * @brief 
 * 
 */
typedef struct MemoryBuffer {            /* memory buffer type */
    int state,wp,rp;        /* state,write/read pointer */
    int bufsize;            /* buffer size (bytes) */
    lock_t lock;            /* lock flag */
    uint8_t *buf;           /* write buffer */
} MemoryBuffer_t;

/**
 * @brief 
 * 
 * @param path 
 * @param mode 
 * @param msg 
 * @return Serial_t* 
 */
Serial_t *openserial(const char *path, int mode, char *msg);

/**
 * @brief 
 * 
 * @param serial 
 */
void closeserial(Serial_t *serial);

/**
 * @brief 
 * 
 * @param serial 
 * @param buff 
 * @param n 
 * @param msg 
 * @return int 
 */
int readserial(Serial_t *serial, uint8_t *buff, int n, char *msg);

/**
 * @brief 
 * 
 * @param serial 
 * @param buff 
 * @param n 
 * @param msg 
 * @return int 
 */
int writeserial(Serial_t *serial, uint8_t *buff, int n, char *msg);

/**
 * @brief 
 * 
 * @param sock 
 * @param msg 
 * @return int 
 */
int setsock(socket_t sock, char *msg);

/**
 * @brief 
 * 
 * @param sock 
 * @param addr 
 * @param len 
 * @return socket_t 
 */
socket_t accept_nb(socket_t sock, struct sockaddr *addr, socklen_t *len);


int connect_nb(socket_t sock, struct sockaddr *addr, socklen_t len);


int recv_nb(socket_t sock, uint8_t *buff, int n);

#endif