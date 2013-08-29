tango_zmq
=========

Experiment ZMQ to replace CORBA in Tango CS

Prerequisites:
- Get Czmq from its web site (http://czmq.zeromq.org/) and compile it


There are only two files in this project:
 - clnt.c which simulates a client
 - svr.c which simulates a server

To run the example, start three server processes
- One with 9000 (port number) as argument
- Another one with 9001 as argument
- The last one with 9002 as argument

Then start the client which does the following:
- Connect the client router ZMQ socket used for synchronous communication to the three servers
- Connect the client router ZMQ socket used for asynchronous communication to the three servers
- Send two messages to server 9001 asynchronously. Each request sent to server 9001 needs 1 second
to be executed
- Send one synchronous message to server 9000 with a timeout of 3 seconds. Each request sent to server
9000 or 9002 needs 4 seconds to be executed. Therefore, this request timed out
- Send one synchronous message to server 9002 with a timeout of 5 seconds.
- Read the asynchronous requests answers
