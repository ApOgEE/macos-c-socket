/*

Author: M. Fauzilkamil Zainuddin

*/

#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define PORT 80
#define USERAGENT "ApOgEE Mojave Socket Client v1.1"

char *build_get_query(char *, char *);

int main(int argc, char *argv[])
{
	
	struct hostent *hp;
	struct in_addr ip_addr;
	char *getquery;
	char buf[BUFSIZ+1];
	int tmpres;
	int sock = 0;
	struct sockaddr_in serv_addr; 
	char *htmlcontent;
	char *strpage;


	printf("%s\n", "This is ApOgEE Socket Client Example");
	printf("%s\n\n", "====================================");

	/* Verify a "hostname" parameter was supplied */
	if (argc < 2 )
	{
		printf("Please enter hostname. Example:\n %s coderstalk.blogspot.com\n", argv[0]);
		exit(EXIT_FAILURE);		
	}

	/* call gethostbyname() with a host name. gethostbyname() returns a */
	/* pointer to a hostent struct or NULL.                             */
	hp = gethostbyname(argv[1]);

	if (!hp) {
		printf("%s was not resolved\n",argv[1]);
		exit(EXIT_FAILURE);
	}

	/* move h_addr to ip_addr. This enables conversion to a form        */
	/* suitable for printing with the inet_ntoa() function.             */
	ip_addr = *(struct in_addr *)(hp->h_addr);
	printf("Hostname: %s, was resolved to: %s\n", argv[1],inet_ntoa(ip_addr));

	if (argc < 3)
	{
		strpage = "/";
	} else {
		strpage = argv[2];
	}

	// build the query
	getquery = build_get_query(argv[1], strpage);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    }

	// clear serv_addr 
	memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, inet_ntoa(ip_addr), &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    send(sock , getquery , strlen(getquery) , 0 ); 

    printf("\nQuery sent \n%s\n", getquery); 

	// clear buffer
	memset(buf, 0, sizeof(buf));

	tmpres = recv(sock, buf, BUFSIZ, 0);
	if(tmpres > 0) {


		htmlcontent = buf;
		printf("%s\n", htmlcontent);

		memset(buf, 0, tmpres);
		
	}
	printf("tmpres: %d\n", tmpres);
	free(getquery);

	exit(EXIT_SUCCESS);
}

char *build_get_query(char *host, char *page)
{
	char *query;
	char *getpage = page;
	char *tpl = "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
	if(getpage[0] == '/'){
		getpage = getpage + 1;
		fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
	}
	// -5 is to consider the %s %s %s in tpl and the ending \0
	query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
	sprintf(query, tpl, getpage, host, USERAGENT);
	return query;
}
