#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <pthread.h>

#define SIZE 100

using namespace std;
#include <deque>

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

typedef struct ForwardingTable_tag
{
  int destination_id [SIZE];
  int next_neighbor [SIZE];
  int size;
} ForwardingTable;

typedef struct Neighbor_fdmap_tag
{
  int neighbor [SIZE];
  int fd [SIZE];
  int size;
} Neighbor_fdmap;

typedef struct SyncQ_teg {
  std::deque <DataPkt> queue;
  pthread_mutex_t lock;
  pthread_cond_t empty;
  pthread_cond_t full;
} SyncQ;

int neighbors = 0, id;
Neighbor_fdmap nebor_map;
ForwardingTable fwd_table = { { 1, 2, 3},
                              { 2, 2, 3}, 
                              3 };
SyncQ packetq;

void parse_packet (DataPkt *packet, int fd);
void dequeue_packet (DataPkt *packet);
void enqueue_packet (const DataPkt *packet);
void *receiver_function (void *);
void *sender_function (void *);
int get_neighbor_fd (int dest);
void handshake(int, int); 
void error(char *msg);
void cleanup ();

int main(int argc, char *argv[])
{ 
  int sockfd, newsockfd, portno; 
  struct sockaddr_in serv_addr, cli_addr; 
  unsigned int clilen;
  int i;
  pthread_t receiver_thread;
  pthread_attr_t attr;
    
  if (argc < 4) 
  { 
    fprintf(stderr,"ERROR: Invalid # of arguments: Usage: program <ID> <port> <# of neighbors>\n");
    exit(1); 
  } 
  
  id = atoi (argv[1]);
  portno = atoi(argv[2]); 
  neighbors = atoi (argv[3]);

  sockfd = socket(AF_INET, SOCK_STREAM, 0); 

  if (sockfd < 0) 
    error("ERROR opening socket"); 
  
  bzero((char *) &serv_addr, sizeof(serv_addr)); 

  serv_addr.sin_family = AF_INET; 
  serv_addr.sin_addr.s_addr = INADDR_ANY; 
  serv_addr.sin_port = htons(portno); 

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    error("ERROR on binding"); 
  
  listen(sockfd, 5); 
  clilen = sizeof(cli_addr); 
  
  for (i = 0; i < neighbors; ++i)
  { 
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); 
    if (newsockfd < 0) 
      error("ERROR on accept"); 
    handshake (i, newsockfd); 
  } 
  
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_mutex_init (&packetq.lock, NULL);
  pthread_cond_init (&packetq.empty, NULL);
  pthread_cond_init (&packetq.full, NULL);
  
  pthread_create (&receiver_thread, &attr, receiver_function, 0);
  
  sender_function (0);  
  
  
  pthread_attr_destroy(&attr); 
  pthread_cond_destroy (&packetq.empty);
  pthread_cond_destroy (&packetq.full);
  pthread_mutex_destroy (&packetq.lock);
  cleanup ();
  return 0; 
}

void parse_packet (DataPkt *packet, int fd)
{

}

void *receiver_function (void *)
{
    struct timeval tv;
    int i, max = 0;
    fd_set readfds;  //set of file descriptors

    tv.tv_sec = 1; 
    tv.tv_usec = 500000; 

    while (1)
    {
      FD_ZERO(&readfds); 
      for (i = 0; i < nebor_map.size; ++i)
      {
        FD_SET(nebor_map.fd[i], &readfds); 
        if (nebor_map.fd[i] > max)
          max = nebor_map.fd[i];
      }

      select(max+1, &readfds, NULL, NULL, NULL); 
      for (i = 0; i < nebor_map.size; ++i)
      {
        if (FD_ISSET(nebor_map.fd[i], &readfds)) 
        {
          printf("Data was received from ID = %d.\n", nebor_map.neighbor[i]);
          DataPkt packet;
          parse_packet (&packet, nebor_map.fd[i]);
          enqueue_packet (&packet);
        }
      }
    }
    
    return 0;
}

void *sender_function (void *)
{
  while (1)
  {
    DataPkt packet;
    dequeue_packet (&packet);
     
  }
    
  return 0;
}

void dequeue_packet (DataPkt *packet)
{
    pthread_mutex_lock (&packetq.lock);
    
    if (packetq.queue.size () == 0)
      pthread_cond_wait (&packetq.empty, &packetq.lock);
    
    *packet = packetq.queue.front ();
    packetq.queue.pop_front();
    
    if (packetq.queue.size() == SIZE - 1)
      pthread_cond_signal (&packetq.full); 
    
    pthread_mutex_unlock (&packetq.lock);
}

void enqueue_packet (const DataPkt *packet)
{
    pthread_mutex_lock (&packetq.lock);
    
    if (packetq.queue.size () == SIZE)
      pthread_cond_wait (&packetq.full, &packetq.lock);
    
    packetq.queue.push_back (*packet);
    
    if (packetq.queue.size() == 1)
      pthread_cond_signal (&packetq.empty); 
    
    pthread_mutex_unlock (&packetq.lock);
}

void handshake (int i, int sock)
{ 
  int n, neighbor = 0; //char buffer[256]; 
  char buffer [SIZE];

  // fill up the packet 
  bzero(buffer, sizeof(buffer)); 
  n = read(sock, &buffer, sizeof(buffer)); 

  if (n < 0) 
    error("ERROR reading from socket"); 
  
  printf("Here is the message %s\n",buffer); 
  sscanf (buffer, "HELLO_FROM %d", &neighbor);
  nebor_map.neighbor[i] = neighbor;
  nebor_map.fd[i] = sock;
  nebor_map.size = i+1;
  
  bzero(buffer, sizeof(buffer)); 
  sprintf (buffer, "HELLO_FROM %d",id);
  
  n = write(sock, buffer, strlen (buffer)); 
  if (n < 0) 
    error("ERROR writing to socket");
}

void error(char *msg)
{ 
  perror(msg); 
  exit(1);
}

void cleanup()
{
  int i;
  for (i = 0;i < nebor_map.size; ++i)
  {
    close (nebor_map.fd[i]);
  }
}

int get_neighbor_fd (int dest)
{
  int i;
  for (i = 0; i < nebor_map.size; ++i)
  {
    if (dest == nebor_map.neighbor[i])
      return nebor_map.fd [i];
  }
  error ("ERROR: Invalid data or corrupt data structures");
  return -1;
}

