/**
   C++ client example using sockets
*/
#include<iostream>    //cout
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent
#include"tcp_client.h"

using namespace std;

/**
   TCP Client class
*/
tcp_client::tcp_client(unsigned int debug)
{
  debug = debug;
  sock = -1;
  port = 0;
  address = "";
}

/**
   Connect to a host on a certain port number
*/
bool tcp_client::conn(string address , int port)
{
  //create socket if it is not already created
  if(sock == -1)
    {
      //Create socket
      sock = socket(AF_INET , SOCK_STREAM , 0);
      if (sock == -1)
	{
	  if(debug==1) cout<<"Could not create socket\n";
	}
      
      if(debug==1) cout<<"Socket created\n";
    }
  else    {   /* OK , nothing */  }
  
  //setup address structure
  // if(inet_addr(address.c_str()) < 0) //==-1
  //     {
  struct hostent *he;
  struct in_addr **addr_list;
  
  //resolve the hostname, its not an ip address
  if ( (he = gethostbyname( address.c_str() ) ) == NULL)
    {
      //gethostbyname failed
      //herror("gethostbyname");
      if(debug==1) cout<<"Failed to resolve hostname (gethostbyname)\n";
      return false;
    }
  
  //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
  addr_list = (struct in_addr **) he->h_addr_list;
  
  for(int i = 0; addr_list[i] != NULL; i++)
    {
      //strcpy(ip , inet_ntoa(*addr_list[i]) );
      server.sin_addr = *addr_list[i];
      
      if(debug==1) cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<endl;
      
      break;
    }
  //  }
  
  //plain ip address
  // else
  //   {
  //     server.sin_addr.s_addr = inet_addr( address.c_str() );
  //   }
  
  server.sin_family = AF_INET;
  server.sin_port = htons( port );
  
  //Connect to remote server
  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
      if(debug==1) cout<<"connect failed (connect)\n";
      return false;
    }
  
  if(debug==1) cout<<"Connected\n";
  return true;
}

/**
   Send data to the connected host
*/
bool tcp_client::send_data(string data)
{
  //Send some data
  if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
    {
      if(debug==1) cout<<"Send failed\n";
      return false;
    }
  
  if(debug==1) cout<<" Data send\n";  
  return true;
}

/**
   Receive data from the connected host
*/
string tcp_client::receive(int size=512)
{
  char buffer[size];
  string reply;
  
  //Receive a reply from the server
  if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
    {
      if(debug==1) cout<<"recv failed\n";
    }
  
  reply = buffer;
  return reply;
}

