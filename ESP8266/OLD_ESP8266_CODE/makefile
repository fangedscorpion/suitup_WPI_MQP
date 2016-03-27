all: tcpUdpserver tcpUdpClient echoClient echoServer udpServer

tcpUdpserver: tcpUdpserver.c
	gcc -o tcpUdpserver tcpUdpserver.c	

tcpUdpClient: tcpUdpClient.c
	gcc -o tcpUdpClient tcpUdpClient.c

echoClient: echoClient.c
	gcc -o echoClient echoClient.c

echoServer: echoServer.c
	gcc -o echoServer echoServer.c

udpServer: udpServer.c
	gcc -o udpServer udpServer.c


clean:
	rm tcpUdpserver tcpUdpClient echoServer echoClient udpServer