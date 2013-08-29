#include <czmq.h>

static int connect_socket(void *sock)
{
	int ret = 0;

	int rc = zsocket_connect(sock,"tcp://localhost:9000");
	if (rc == 0)
		printf("Client: Socket connected to server 9000\n");
	else
	{
		printf("Client: Error while connecting to server 9000\n");
		ret = -1;
	}

	rc = zsocket_connect(sock,"tcp://localhost:9001");
	if (rc == 0)
		printf("Client: Socket connected to server 9001\n");
	else
	{
		printf("Client: Error while connecting to server 9001\n");
		ret = -1;
	}

	rc = zsocket_connect(sock,"tcp://localhost:9002");
	if (rc == 0)
		printf("Client: Socket connected to server 9002\n");
	else
	{
		printf("Client: Error while connecting to server 9002\n");
		ret = -1;
	}

	return ret;
}

static void send_synchronous(void *sock,char *worker_thread,int to)
{
	zmq_pollitem_t items[] = {
		{sock,0,ZMQ_POLLIN,0}
	};

	zstr_sendm(sock,worker_thread);
	zstr_send(sock,"SYNC HELLO WORLD");
	struct timeval now;
	gettimeofday(&now,NULL);
	printf("%ld sec %ld usec : Sync message sent to server %s\n",now.tv_sec,now.tv_usec,worker_thread);

	int rc = zmq_poll(items,1,to * 1000);
	if (rc == -1)
		printf("Error returned by zmq_poll\n");
	else if (rc == 0)
	{
		struct timeval now;
		gettimeofday(&now,NULL);
		printf("%ld sec %ld usec : Timeout waiting for sync request\n",now.tv_sec,now.tv_usec);
		rc = zsocket_disconnect(sock,"tcp://localhost:%s",worker_thread);
		if (rc == -1)
			printf("Error while disconnecting after timeout\n");
		rc = zsocket_connect(sock,"tcp://localhost:%s",worker_thread);
		if (rc == -1)
			printf("Error while connecting after timeout\n");
	}
	else
	{
		if (items[0].revents & ZMQ_POLLIN)
		{
			char *ident = zstr_recv(sock);
			free(ident);
			char *reply = zstr_recv(sock);
			struct timeval now;
			gettimeofday(&now,NULL);
			printf("%ld sec %ld usec: Sync reply: %s\n",now.tv_sec,now.tv_usec,reply);
			free(reply);
		}
		else
			printf("Strange return value from zmq_poll!!!\n");
	}
}


int main(int argc,char *argv[])
{
	zctx_t *ctx = zctx_new();

	void *async = zsocket_new(ctx,ZMQ_ROUTER);
	if (connect_socket(async) == -1)
	{
		printf("Error while connecting async socket\n");
		return -1;
	}

	void *sync = zsocket_new(ctx,ZMQ_ROUTER);
	if (connect_socket(sync) == -1)
	{
		printf("Error while connecting sync socket\n");
		return -1;
	}

	printf("\n");
	int ctr;
	for (ctr = 0;ctr < 2;ctr++)
	{
		zstr_sendm(async,"9001");
		zstr_send(async,"ASYNC HELLO WORLD");
		struct timeval now;
		gettimeofday(&now,NULL);
		printf("%ld sec %ld usec : Async message sent to svr 9001\n",now.tv_sec,now.tv_usec);
	}

	send_synchronous(sync,"9000",3);

	send_synchronous(sync,"9002",5);

	while (true)
	{
		char *ident = zstr_recv(async);
		free(ident);
		char *reply = zstr_recv(async);
		if (reply == NULL)
			break;
		struct timeval now;
		gettimeofday(&now,NULL);
		printf("%ld sec %ld usec : Reply: %s\n",now.tv_sec,now.tv_usec,reply);
		free(reply);
	}

	zctx_destroy(&ctx);	
	return 0;
}

