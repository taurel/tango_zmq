tango_zmq
=========

Experiment ZMQ to replace CORBA in Tango CS

Prerequisites:
- Get Czmq from its web site (http://czmq.zeromq.org/) and compile it
- Get zmq as well (obvious)


There are only few files in this project:
 - clnt.c which simulates a client
 - svr.c which simulates a server
 - Makefile which has to be adapted to your setup

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

Work to be done or points to be analyzed (some are big tasks, some are small tasks)
========================================

- Define a Tango protocol (in ABNF) with at least:
	request id
	obj id (uniq in DS, in CS?)
	method name
	request type
	protocol release
	data

- Implement all Tango::DevVarXXXArray type
	Similar interface - Similar memory management

- Implement a CORBA::string_dup/string_free/string_alloc methods

- Implement a dummy (?) CORBA::Any object (used in Pogo generated code for commands) in order to provide compatibility

- omnithread library compatibility (C++11 ?)
	thread class (detached and undetached)
	oomni_mutex, omni_mutex_lock, omni_condition variable

- Manage TANGO_HOST definition like "orion:10000,orion:11000"

- Implement something like ORBtraceLevel, ORBtraceFile, ORBtraceTime, ORBtraceThreadId
  Is Tango logging adequate?

- Select a serialiation lib:
	No lib
	Google protbuf
	capnproto
	Messagepack
	
- Implement a kind of "is_a" method for all devices in order to manage interface changes with time

- Define a TOR (Tango Object Reference) stored in DB in order to build object connection
  How are we going to manage DS on host with several NIC boards?

- Select which kind of container used in DS in which we will store the device object pointer (vector, map,...)
  Choose the object_id coherent with choice

- Try to implement things in a way we could easily replace ZMQ by something else (nanomsg...)

- Can we write it using C++11 features?

- Is it possible to remove event heartbeat by using XPUB in server (instead of PUB) and monitor socket API in client?

- Possible to replace DevString (char *) with C++ string?

- Collocated calls?

- Device server with user event loop

- DS command line option to start DS on a specific port (like ORBendPoint)


	
