#include"common.h"

const char server_dir[] = "/home/user/xbc/www";
void *send_data(void* param);

int main()
{
	int server_sockfd, client_sockfd;
	socklen_t sin_size;
	struct sockaddr_in server_addr, client_addr;

	memset(&server_addr,0,sizeof(server_addr));
	//create socket 创建套接字
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket create error！\n");
		exit(1);
	}

	//set the socket's attributes
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(server_addr.sin_zero), 8);

	//create a link
	if (bind(server_sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind error！\n");
		exit(1);
	}

	//listening requests from clients
	if (listen(server_sockfd, BACKLOG) == -1) {
		perror("listen error！\n");
		exit(1);
	}

	//accept requests from clients,loop and wait.
	while (1) {
		sin_size = sizeof(client_addr);
		if ((client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_addr, &sin_size)) == -1) {
			perror("accept error!\n");
			continue;
		}
		cout<<"received a connection from "<<inet_ntoa(client_addr.sin_addr)<<endl;

		pthread_t id;
		pthread_create(&id,NULL,send_data,&client_sockfd);
		pthread_join(id,NULL);

		close(client_sockfd);
	}
	return 0;
}

void *send_data(void* param){
	int client_sockfd,len;
	char buf[BUF_SIZE],ext[50],temp[30],file[50];
	char output[BUF_SIZE],content_type[50];
	char *pa,*pb,*pc;
	struct stat st;
	FILE* fp;

	memset(buf,0,sizeof(buf));
	memset(ext,0,sizeof(ext));
	memset(temp,0,sizeof(temp));
	memset(file,0,sizeof(file));
	memset(output,0,sizeof(output));
	memset(content_type,0,sizeof(content_type));

	client_sockfd = *(int*)param;
	cout<<"Request stream:\n";

	while((len = recv(client_sockfd, buf, sizeof(buf), 0)) > 0){
		//cout<<buf;
		char tp[20];
		char* pp = strstr(buf,"\r\n");
		strncpy(tp,buf,pp-buf);
		cout<<"first line: "<<tp<<endl;

		if((pa=strstr(buf,"Accept: ")) != NULL){
			pb = strchr(pa+1,' ');
			pc = strchr(pa+1,',');
			strncpy(ext,pb+1,pc-pb-1);
		}
		cout<<"file extention: "<<ext<<endl;
		if(strncmp(ext,"image/gif",9) ==0 ){
			pc = strchr(pa+1,'\r');
			strncpy(content_type,pb+1,pc-pb-1);
		}else if(strncmp(ext,"image/png",9) ==0 ){
			pc = strchr(pa+1,'\r');
			strncpy(content_type,pb+1,pc-pb-1);
		}else {
			strcpy(content_type,"text/html");
		}

		if(strncmp(buf,"GET",3) == 0){
			strcpy(file,server_dir);
			pa = strchr(buf,' ');
			pb = strchr(pa+1,' ');
			strncpy(temp,pa+1,pb-pa-1);
			strcat(file,temp);
			cout<<"file path and name: "<<file<<endl;
			stat(file,&st);
			if(strcmp(temp,"/google.jpg")==0){
				strcpy(content_type, "image/jpeg");
			}
			sprintf(output,"HTTP/1.1 200 OK\r\n"
					"Server: HttpServer-by XBC/1.1\r\n"
					"ETag: W/6381-1144994988000\r\n"
					"Last-Modified: Fri, 14 Apr 2006 06:09:48 GMT\r\n"
					/*"Content-Type: %s\r\n"*/
					"Content-Length: %d bytes\r\n"
					"Date: Mon, 19 Jun 2011 07:21:09 GMT\r\n"
					"Connection: keep-alive\r\n\r\n"/*,content_type*/,(int)st.st_size);

			cout<<output<<endl;
			cout<<"Content-Type: "<<content_type<<endl;
			send(client_sockfd,output,sizeof(output),0);
			fp = fopen(file,"r");
			while(fread(buf,1,sizeof(buf),fp) > 0){
				send(client_sockfd,buf,sizeof(buf),0);
			}
			close(client_sockfd);
		}
		cout<<"========================================================="<<endl;
	}
	return NULL;
}
