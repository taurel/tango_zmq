#include <czmq.h>


int main(int argc,char *argv[])
{
	if (argc != 2)
	{
		printf("Wrong number of argument\n");
		printf("Usage: %s <port number> \n",argv[0]);
		return -1;
	}

	zctx_t *ctx = zctx_new();

	int port_nb = atoi(argv[1]);

	void *worker = zsocket_new(ctx,ZMQ_ROUTER);
	zsocket_set_identity(worker,argv[1]);
	int rc = zsocket_bind(worker,"tcp://*:%s",argv[1]);
	if (rc == port_nb)
		printf("Server %s: Connected\n",argv[1]);
	else
		printf("Server %s: Error while connecting\n",argv[1]);

	printf("Server %s: Waiting for request\n",argv[1]);
	while (true)
	{
/*		zmsg_t *msg = zmsg_recv(worker);
		zmsg_dump(msg);*/
		zframe_t *ident = zframe_recv(worker);
		char *rec = zstr_recv(worker);
		if (rec == NULL)
			break;
//		printf("length of received messages: %d, %d\n",(int)zframe_size(ident),(int)strlen(rec));
		struct timeval now;
		gettimeofday(&now,NULL);
		printf("%ld sec %ld usec : Server %s: Received: %s\n",now.tv_sec,now.tv_usec,argv[1],rec);
		free(rec);

		if (port_nb == 9001)
			sleep(1);
		else
			sleep(1);

		struct timeval another_now;
		gettimeofday(&another_now,NULL);
		printf("%ld sec %ld usec: Server %s: Send reply\n",another_now.tv_sec,another_now.tv_usec,argv[1]);
		zframe_send(&ident,worker,ZFRAME_MORE);
		if (port_nb == 9001)
			zstr_send(worker,"A REPLY from server %d",port_nb);
		else
			zstr_send(worker,"A SYNC REPLY from server %d",port_nb);
		free(ident);
	}

	zctx_destroy(&ctx);

	return 0;
}


