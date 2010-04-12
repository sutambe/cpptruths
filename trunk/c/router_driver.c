#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFLEN 100
#define SIZE 100

typedef struct dataPkt_tag 
{
  uint16_t PacketID ;
  uint16_t ProtocolID ;
  uint16_t SourceID ;
  uint16_t DestID ;
  uint16_t Pktlength ;
  uint16_t TTL ;
  char datamsg[SIZE];
} DataPkt;

int sd = 0, selfid = 0;
int destination = 0;

int read_packet ();
int send_packet (int packet_id);
void handshake (void);
void error(char *msg);
void * reader(void *);
void establish_connection (const char *router, int port);
int main (int argc, char **argv)
{
  int n, port;
  char buffer[BUFLEN]={ 0 }, *router=0;
  pthread_attr_t reader_thread_attr;
  pthread_t reader_thread;

  if (argc != 5)
    {
      fprintf(stderr, "Usage: %s <selfid> <router hostname> <router port> <dest-id>\n",argv[0]);
      exit(1);
    }
  
  selfid = atoi (argv[1]);
  router=argv[2];
  port=atoi(argv[3]);
  destination = atoi (argv[4]);
  
  establish_connection (router, port);
  handshake ();

  pthread_attr_init(&reader_thread_attr);
  pthread_attr_setdetachstate(&reader_thread_attr,PTHREAD_CREATE_DETACHED);
  pthread_create(&reader_thread,&reader_thread_attr,reader,0);

  n = 0;
  while(1)
  {
      fgets (buffer, BUFLEN,stdin);
      //sscanf ("%s",buffer);
      if(strstr(buffer,"quit"))
      {
        fprintf(stderr,"Quitting..\n");
        break;
      }
      
      if(strstr(buffer,"send"))
      {
        //while (1)
        if (0 != send_packet (n++))
            break;
      }
  }

  pthread_cancel(reader_thread);
  pthread_attr_destroy(&reader_thread_attr);
  
  printf("Connection closed.\n");
  close(sd);
  return 0;

}

int send_packet (int packet_id)
{
  DataPkt packet;
  int nbytes = 0; 
  
  bzero(&packet, sizeof(DataPkt)); 
  packet.PacketID = packet_id;
  packet.ProtocolID = 9999;
  packet.SourceID = selfid;
  packet.DestID = destination;  
  packet.Pktlength = sizeof (DataPkt);
  packet.TTL = 9999;
  strcpy (packet.datamsg, "Hang to your dreams!!!");
  
  if ((nbytes = write (sd, &packet, sizeof (DataPkt)) == -1)) 
  {
      perror ("Can't write to the router socket: ");
      return -1;
  }
  //printf("Sent %d bytes.\n",nbytes);
  return 0;
}

int read_packet ()
{
  DataPkt *packet = 0;
  char buffer[sizeof(DataPkt)];
  int n=0, collected = 0, remaining = sizeof(DataPkt);

  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,0);
  while((n=read (sd, buffer + collected, remaining)) > 0)
  {
    if (n == -1 && remaining != 0)
    {
        error ("Connection closed before complete packet was received");
    }
    
    collected += n;
    remaining -= n;
    
    if (remaining == 0)
      break;
  }
  
  packet = (DataPkt *) buffer;
  printf("Received packet id = %d from src = %d: data = %s\n",
          packet->PacketID, packet->SourceID, packet->datamsg);
  bzero(buffer, sizeof(DataPkt)); 
  
  return 0;
}

void * reader (void *arg)
{
  while (1)
  {
    read_packet();
  }
}

void establish_connection (const char *router, int port)
{
  struct sockaddr_in server;
  struct hostent *hp=0;
  
  /* Socket Creation */
  if((sd = socket(AF_INET, SOCK_STREAM, 0))== -1)
    {
      fprintf(stderr, "Cannot create socket\n");
      exit(1);
    }
  bzero((char *)&server,sizeof(struct sockaddr_in));
  server.sin_family=AF_INET;
  server.sin_port=htons(port);
  if((hp=gethostbyname(router))==NULL)
  {
  	fprintf(stderr,"Can't get server's address.\n");
	exit(1);
  }
  bcopy(hp->h_addr,(char *)&server.sin_addr,hp->h_length);
  if(connect(sd,(struct sockaddr *)&server,sizeof(server))==-1)
  {
  	fprintf(stderr,"Can't connect to router %s on port %d.\n",router, port);
	exit(1);
  }
  printf("Connected to the router on %s.\n",router);
}


void error(char *msg)
{ 
  perror(msg); 
  exit(1);
}

void handshake (void)
{
  int n, neighbor;
  char buffer [SIZE];
  bzero(buffer, sizeof(buffer)); 
  sprintf (buffer, "HELLO_FROM %d",selfid);
  
  n = write(sd, buffer, strlen (buffer)); 
  if (n < 0) 
    error("ERROR writing to socket");
  
  bzero(buffer, sizeof(buffer)); 
  n = read(sd, &buffer, sizeof(buffer)); 

  if (n < 0) 
    error("ERROR reading from socket"); 
  
  sscanf (buffer,"HELLO_FROM %d",&neighbor);
  printf("Here is the handshake from %d:%s:\n",neighbor, buffer); 
}

