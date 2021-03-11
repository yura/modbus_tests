/*========================================================
test2.c 7/23/97 - UNIX C program to read registers via gateway
  compile using
  cc -o test2 test2.c
  run using
  test2 198.202.138.73 1 2 3
==========================================================*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

/* global data */

int main(int argc, char **argv)
{
  char *ip_adrs;
  unsigned short unit;
  unsigned short reg_no;
  unsigned short num_regs;
  int s;
  int i;
  struct sockaddr_in server;
  fd_set fds;
  struct timeval tv;
  unsigned char obuf[261];
  unsigned char ibuf[261];

  if (argc<5)
  {
    std::cout << "usage: ./SocketTest ip_adrs unit reg_no num_regs" << std::endl << "eg ./SocketTest 192.168.0.99 1 4064 1" << std::endl;
    return 1;
  }

  /* confirm arguments */
  ip_adrs = argv[1];
  unit = atoi(argv[2]);
  reg_no = atoi(argv[3]);
  num_regs = atoi(argv[4]);
  printf("ip_adrs = %s unit = %d reg_no = %d num_regs = %d\n",
  ip_adrs, unit, reg_no, num_regs);

  /* establish connection to gateway on ASA standard port 502 */
  s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  server.sin_family = AF_INET;
  server.sin_port = htons(502); /* ASA standard port */
  server.sin_addr.s_addr = inet_addr(ip_adrs);

  i = connect(s, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
  if (i<0)
  {
    printf("connect - error %d\n",i);
    close(s);
    return 1;
  }

  FD_ZERO(&fds);
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  /* check ready to send */
  FD_SET(s, &fds);
  i = select(32, NULL, &fds, NULL, &tv);
  if(0)if (i<=0)
  {
    printf("select - error %d\n",i);
    close(s);
    return 1;
  }

// # [00][01][00][00][00][06][01][03][0f][e0][00][01]
// # msg = @transaction.to_word + "\0\0" + (pdu.size + 1).to_word + @uid.chr + pdu


  /* build MODBUS request */
  for (i=0;i<5;i++) obuf[i] = 0;
  obuf[5] = 6;
  obuf[6] = unit;
  obuf[7] = 3;
  obuf[8] = reg_no >> 8;
  obuf[9] = reg_no & 0xff;
  obuf[10] = num_regs >> 8;
  obuf[11] = num_regs & 0xff;

  /* send request */
  i = send(s, obuf, 12, 0);
  if (i<12)
  {
    printf("failed to send all 12 chars\n");
  }

  /* wait for response */
  FD_SET(s, &fds);
  i = select(32, &fds, NULL, NULL, &tv);
  if (i<=0)
  {
    printf("no TCP response received\n");
    close(s);
    return 1;
  }

  /* receive and analyze response */
  i = recv(s, ibuf, 261, 0);
  if (i<9)
  {
    if (i==0)
    {
      printf("unexpected close of connection at remote end\n");
    }
    else
    {
      printf("response was too short - %d chars\n", i);
    }
  }
  else if (ibuf[7] & 0x80)
  {
    printf("MODBUS exception response - type %d\n", ibuf[8]);
  }
  else if (i != (9+2*num_regs))
  {
    printf("incorrect response size is %d expected %d\n",i,(9+2*num_regs));
  }
  else
  {
    for (i=0;i<num_regs;i++)
    {
      unsigned short w = (ibuf[9+i+i]<<8) + ibuf[10+i+i];
      printf("word %d = %d\n", i, w);
    }
  }

  /* close down connection */
  close(s);

}
