#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <string.h>
#include <errno.h>




static unsigned short DEFAULT_PORT=8888;

static void Usage(const char *msg, const char *progname)
{
  if( msg != NULL) {
    printf("%s\n", msg);
  }
  printf("Usage:\n"
	 "\tFor the server mode: %s\n"
	 "or\n"
	 "\tFor the client mode: %s -c\n",
	 progname, progname);
  exit(1);
}

static void runServer(void)
{
  printf("Run server\n");

  // Create listening socket
  int listeningSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // Set listening address (127.0.0.1) and port
  struct sockaddr_in addr;
  memset( &addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(DEFAULT_PORT);
  addr.sin_addr.s_addr = INADDR_ANY;

  int on = 1;
  setsockopt(listeningSock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));

  int ret;
  ret = bind(listeningSock, (struct sockaddr*) &addr, sizeof(addr));

  if(ret != 0)	{
    printf( "Bind failed with errno %d (%s)\n",
	    errno, strerror(errno));
    exit(1);
  }
  
  ret = listen(listeningSock, 10);
  if(ret != 0) {
    printf( "Listen failed with errno %d (%s)\n",
	    errno, strerror(errno));

    exit(1);
  }

  socklen_t addrlen = sizeof(addr);
  int sockfd;

  while( (sockfd = accept( listeningSock, 
			   (struct sockaddr *)&addr, 
			   &addrlen)) >= 0) {
    char *addrStr = inet_ntoa(addr.sin_addr);

    printf("Accepted connection from [%s]\n", addrStr);

    char readBuf[256];
    printf("%s:%d - before read\n", __FUNCTION__, __LINE__);
    ssize_t nb = read(sockfd, readBuf, sizeof(readBuf));

    // We expect to receive a string. So, if there is a newline at the
    // end - terminate the string at the newline.
    char *eol = memchr( readBuf, '\r', sizeof(readBuf));
    if(eol != NULL ||
       (eol=memchr(readBuf, '\n', sizeof(readBuf))) != NULL)
      *eol = '\0';
    
    printf("%s:%d - got nb=%zd\n", __FUNCTION__, __LINE__, nb);

    if( nb <= 1) {
      printf("%s:%d - error reading, errno=%d (%s)\n", __FUNCTION__, __LINE__, errno, strerror(errno));
    } else {
      printf("%s:%d - Received msg [%s]\n", __FUNCTION__, __LINE__, readBuf);
      char reply[256];
      snprintf(reply, sizeof(reply)-1, "%s - acknowledged", readBuf);
      write( sockfd, reply, strlen(reply)+1);
    }
    close(sockfd);
  }
}

static void runClient(void)
{
  printf("Run client\n");
  printf("Enter msg: "); fflush(stdout);

  // Create client socket
  int clientFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if( clientFd < 0) {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in dst_addr;
  memset( &dst_addr, 0, sizeof(dst_addr));
  dst_addr.sin_family = AF_INET;
  dst_addr.sin_port = htons(DEFAULT_PORT);
  dst_addr.sin_addr.s_addr = INADDR_ANY;

  if( connect(clientFd, (struct sockaddr *)&dst_addr, sizeof(dst_addr)) != 0) {
    perror("connect");
    return;
  }

  char msg[256];
  fgets( msg, sizeof(msg)-1, stdin);

  // Clear the newline
  char *eol;
  if( (eol=strchr( msg, '\r')) != NULL ||
      (eol=strchr( msg, '\n')) != NULL) {
    *eol = '\0';
  }

  if( write( clientFd, msg, strlen(msg)+1) != (int)strlen(msg)+1) {
    perror("write");
  } else {
    char reply[256];
    ssize_t nb = read( clientFd, reply, sizeof(reply));
    if( nb <= 0) {
      perror("read");
    } else {
      printf("Received reply: [%s]\n", reply);
    }
  }

  close(clientFd);
  return;
}

int main(int argc, char *argv[])
{
  int arg;
  int serverMode=1; // By default run in server mode

  // Parse command line
  while( (arg=getopt(argc, argv, "c")) != EOF) {
    switch( arg) {
    case 'c':
      serverMode = 0;
      break;
    default:
      Usage("Unknown argument", argv[0]);
      exit(1);
      // Notreached
      break;
    }
  }

  if( serverMode) {
    runServer();
    // Notreached
    return(1);
  } else {
    runClient();
  }
  return(0);
}
