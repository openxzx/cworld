/*
 * Copyright: 2019.07.23
 *
 * Author: Zhenxing Xu <xzxlnmail@163.com>
 *
 * This is free and unencumbered software released into the public domain.
 * For details see the UNLICENSE file at the root of the source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/un.h>

#define	N	64

struct cs_func {
	int (*init)(void);
	int (*run)(void);
	int (*stop)(void);
} cs_func_p;

pthread_t server_thread, client_thread;

/*
 UNIX域流式套接字服务器端流程如下:
 1. 创建UNIX域流式套接字; socket(AF_UNIX, SOCK_STREAM, 0)
 2. 填充本地信息结构体(服务器), struct sockaddr_un
 3. 绑定本地地址(服务器的地址信息), bind()
 4. 设置监听模式, listen()
 5. 接收客服端的连接请求, accept()
 6. 接收客户端的数据, recv()
 7. 发送数据给客户端, send()
 8. 关闭套接字, close()
 */
void *server(void *arg)
{
	char buf[N];

	int sockfd, connectfd;
	struct sockaddr_un serveraddr, clientaddr;
	socklen_t len = sizeof(clientaddr);

	/* Step 1: socket() */
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("fail to socket");
		return;
	}

	/* Step 2: struct sockaddr_un */
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, "mysocket");

	/* Step 3: bind() */
	if (bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
		perror("fail to bind");
		return;
	}

	/* Step 4: listen() */
	if (listen(sockfd, 5) < 0) {
		perror("fail to listen");
		return;
	}

	/* Step 5: accept() */
	if ((connectfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len)) < 0) {
		perror("fail to accept");
		return;
	}

	while (1) {
		/* Step 6: recv() */
		if (recv(connectfd, buf, N, 0) < 0) {
			perror("fail to recv");
			return;
		}

		if (strncmp(buf, "quit", 4) == 0)
			break;

		buf[strlen(buf) - 1] = '\0';
		printf("\033[31mserver buf: %s\033[0m\n", buf);

		/* Step 7: send() */
		if (send(connectfd, buf, N, 0) < 0) {
			perror("fail to send");
			return;
		}

		sleep(1);
	}

	/* Step 8: close() */
	close(sockfd);
}

/*
 UNIX域流式套接字客户端流程如下:
 1. 创建UNIX域流式套接字, socket(AF_UNIX, SOCK_STREAM, 0)
 2. 填充本地信息结构体(服务器), struct sockaddr_un
 3. 建立与服务器的连接, connect()
 4. 发送数据给服务器端, send()
 5. 接收服务器端的数据, recv()
 6. 关闭套接字, close()
 */
void *client(void *arg)
{
	char buf[N];

	int sockfd;
	struct sockaddr_un serveraddr;

	/* Step 1: socket() */
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sockfd < 0) {
	    perror("fail to sockfd");
	    return;
	}

	/* Step 2: struct sockaddr_un */
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, "mysocket");

	/* Step 3: connect() */
	if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
		perror("fail to connect");
		return;
	}

	while (1) {
		printf("\033[32mclient threads: \033[0m");
		fgets(buf, N, stdin);

		/* Step 4: send() */
		if (send(sockfd, buf, N, 0) < 0) {
			perror("fail to send");
			return;
		}

		if (strncmp(buf, "quit", 4) == 0)
			break;

		/* Step 5: recv() */
		if (recv(sockfd, buf, N, 0) < 0) {
			perror("fail to recv");
			return;
		}

		sleep(1);
	}

	/* Step 6: close() */
	close(sockfd);
}

int init(void)
{
	/* Create server thread */
	if (pthread_create(&server_thread, NULL, server, (void *)NULL) != 0) {
		printf("\033[31mCreate server thread failed!\033[0m\n");
		return 1;
	}

	/* Create client thread */
	if (pthread_create(&client_thread, NULL, client, (void *)NULL) != 0) {
		printf("\033[31mCreate client thread failed!\033[0m\n");
		return 1;
	}

	return 0;
}

int run(void)
{
	pthread_join(server_thread, NULL);
	pthread_join(client_thread, NULL);

	return 0;
}

int main(int argc, char *argv[])
{
	int ret;

	cs_func_p.init = init;
	cs_func_p.run = run;
	cs_func_p.stop = NULL;

	if (cs_func_p.init != NULL) {
		ret = cs_func_p.init();
		if (ret) {
			printf("\033[31mCreate threads failed, return: %d\033[0m\n", ret);
			return 0;
		}
	}

	if (cs_func_p.run != NULL) {
		cs_func_p.run();
	}

	if (cs_func_p.stop != NULL) {
		cs_func_p.stop();
	}

	return 0;
}
