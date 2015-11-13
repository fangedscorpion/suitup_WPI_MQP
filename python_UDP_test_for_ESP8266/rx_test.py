import socket

UDP_IP = "192.168.1.10"
UDP_PORT = 80

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

print "Welcome to the RX test! Any data will be printed below that comes in for a UDP packet"

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    #charac = raw_input('Send: ')
    # if(charac == 'a'):
    #     print "Sending \"HI\" to the ESP8266"
    print "received message:", data