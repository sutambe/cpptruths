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
#include <signal.h>

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

typedef struct PktParse_tag
{
  int remaining, collected;
  DataPkt packet;  
} PktParse;

typedef struct SyncQ_teg {
  std::deque <DataPkt> queue;
  pthread_mutex_t lock;
  pthread_cond_t empty;
  pthread_cond_t full;
} SyncQ;

int neighbors = 0, self_id;
Neighbor_fdmap nebor_map;
ForwardingTable fwd_table = { { 1, 3},
                              { 1, 3}, 
                              2 };
SyncQ packetq;
PktParse packet_buffer [SIZE];
int stop = 0;

void signal_handler (int signal);
int establish_connections (int portno, int neighbors);
int find_neighbor_fd (int neighbor);
int find_next_hop (int dest_id);
int process_packet (const DataPkt *packet);
int consume_packet (const DataPkt *packet);
int forward_packet (const DataPkt *packet);
void init_packet_buffer ();
int parse_packet (int fd);
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
  int portno, neighbors;

  pthread_t receiver_thread;
  pthread_attr_t attr;
    
  if (argc < 4) 
  { 
    fprintf(stderr,"ERROR: Invalid # of arguments: Usage: program <ID> <port> <# of neighbors>\n");
    exit(1); 
  } 
  
  self_id = atoi (argv[1]);
  portno = atoi(argv[2]); 
  neighbors = atoi (argv[3]);

  //signal (SIGINT, signal_handler);
  establish_connections (portno, neighbors);  
  init_packet_buffer ();
  
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

int parse_packet (int fd)
{
  PktParse *pkt_parse = &packet_buffer[fd];
  int nbytes = 0;
  nbytes = read (fd, &pkt_parse->packet + pkt_parse->collected, pkt_parse->remaining);
  
  if (nbytes == -1 && pkt_parse->remaining != 0)
  {
      error ("Connection closed before complete packet was received");
      return -1;
  }
  
  pkt_parse->collected += nbytes;
  pkt_parse->remaining -= nbytes;

  if (pkt_parse->remaining == 0)
  {
    pkt_parse->collected = 0;
    pkt_parse->remaining = sizeof (DataPkt);
    return 1; // received a complete packet
  }
  else
    return 0; // packet is still remaining
}

void *receiver_function (void *)
{
    struct timeval tv;
    int i, max = 0;
    fd_set readfds;  //set of file descriptors
/*    sigset_t sigmask;
    sigemptyset (&sigmask);
    sigaddset (&sigmask, SIGINT);
*/
    tv.tv_sec = 1; 
    tv.tv_usec = 500000; 
    while (!stop)
    {
      FD_ZERO(&readfds); 
      for (i = 0; i < nebor_map.size; ++i)
      {
        FD_SET(nebor_map.fd[i], &readfds); 
        if (nebor_map.fd[i] > max)
          max = nebor_map.fd[i];
      }

      //pselect(max+1, &readfds, NULL, NULL, NULL, &sigmask); 
      select(max+1, &readfds, NULL, NULL, NULL);
      for (i = 0; i < nebor_map.size; ++i)
      {
        if (FD_ISSET(nebor_map.fd[i], &readfds)) 
        {
          int fd = nebor_map.fd[i];
          //printf("Data was received from ID = %d.\n", nebor_map.neighbor[i]);
          if (1 == parse_packet (fd)) /// received a complete packet.
            enqueue_packet (&packet_buffer[fd].packet);
        }
      }
    }
    
    return 0;
}

void *sender_function (void *)
{
  while (!stop)
  {
    DataPkt packet;
    dequeue_packet (&packet);
    process_packet (&packet);     
  }
    
  return 0;
}

int process_packet (const DataPkt *packet)
{
  if (packet->DestID == self_id)
    consume_packet (packet);
  else
    forward_packet (packet);
  return 0;
}

int consume_packet (const DataPkt *packet)
{
  printf ("Received a packet for me(%d): %s, from %d.\n",self_id,
      packet->datamsg, packet->SourceID);
  return 0;
}

int forward_packet (const DataPkt *packet)
{
  int neighbor = 0, fd = 0;
  fprintf (stdout, "Got packet for %d.\n", packet->DestID);
  neighbor = find_next_hop (packet->DestID);
  fprintf (stdout, "Forwarding packet to the next hop %d.\n", neighbor);
  fd = find_neighbor_fd (neighbor);
  write (fd, packet, sizeof (DataPkt)); 
  return 0;
}

int find_neighbor_fd (int neighbor)
{
  int fd = 0, i = 0;
  for (i = 0;i < nebor_map.size;++i)
  {
    if (nebor_map.neighbor[i] == neighbor)
    {
      fd = nebor_map.fd[i];
      break;
    }
  }
  if (i >= nebor_map.size)
  {
    error ("Incorrect neighbor value passed.");
    return -1;
  }
  else
    return fd;
}

int find_next_hop (int dest_id)
{
  int neighbor = 0, i = 0;
  for (i = 0;i < fwd_table.size;++i)
  {
    if (fwd_table.destination_id[i] == dest_id)
    {
      neighbor = fwd_table.next_neighbor[i];
      break;
    }
  }
  if (i >= fwd_table.size)
  {
    error ("Incorrect dest_id passed.");
    return -1;
  }
  else
    return neighbor;
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
  
  sscanf (buffer, "HELLO_FROM %d", &neighbor);
  printf("Here is the handshake from %d:%s:\n",neighbor, buffer); 
  nebor_map.size = i+1;
  nebor_map.neighbor[i] = neighbor;
  nebor_map.fd[i] = sock;
  
  bzero(buffer, sizeof(buffer)); 
  sprintf (buffer, "HELLO_FROM %d",self_id);
  
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

void init_packet_buffer ()
{
  int i;
  for (i = 0;i < SIZE;++i)
  {
    packet_buffer[i].remaining = sizeof(DataPkt);
    packet_buffer[i].collected = 0;
    bzero((void *) &packet_buffer[i].packet, sizeof(DataPkt)); 
  }
}

int establish_connections (int portno, int neighbors)
{  
  int sockfd, newsockfd; 
  struct sockaddr_in serv_addr, cli_addr; 
  unsigned int clilen;
  int i;

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
 
  printf ("Waiting for %d neighbors to connect.\n",neighbors);
  for (i = 0; i < neighbors; ++i)
  { 
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); 
    if (newsockfd < 0) 
      error("ERROR on accept"); 
    handshake (i, newsockfd); 
  } 
  printf ("%d neighbors are connected. Now waiting for packets..\n",neighbors);
  return  0;
}

void signal_handler (int signal)
{
  fprintf (stderr, "Received SIGINT. Stopping...\n");
  stop = 1;
}



