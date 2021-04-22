#!/usr/bin/env python3

""" server.py - Echo server for sending/receiving C-like structs via socket
References:
- Ctypes: https://docs.python.org/3/library/ctypes.html
- Sockets: https://docs.python.org/3/library/socket.html
"""

import socket
import sys
import random
from ctypes import *
import time


""" This class defines a C-like struct """
class Payload(Structure):
    _fields_ = [("id", c_uint32),
                ("counter", c_uint32),
                ("period", c_ulonglong)]


def main():
    PORT = 2300
    server_addr = ('localhost', PORT)
    ssock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Socket created")

    try:
        # bind the server socket and listen
        ssock.bind(server_addr)
        print("Bind done")
        ssock.listen(3)
        print("Server listening on port {:d}".format(PORT))
        # csock, client_address = ssock.accept()
        # print("Accepted connection from {:s}".format(client_address[0]))
        
        splitter = 0
        old_freq = 0
        while True:
            csock, client_address = ssock.accept()
            # print("Accepted connection from {:s}".format(client_address[0]))

            buff = csock.recv(512)
            # print(buff)
            while buff:
                # buff = csock.recv(512)
                # print("\nReceived {:d} bytes".format(len(buff)))
                payload_in = Payload.from_buffer_copy(buff)
                # print("Received contents id={:d}, counter={:d}, period={:d}".format(payload_in.id,
                #                                             payload_in.counter,
                #                                             payload_in.period))
                # print("Sending it back.. ", end='')
                period1 = payload_in.period
                seconds_p = (period1 * 2) * 0.000000001
                freq = 1 / seconds_p

                if (splitter % 2) == 0:
                    print("Half 1: {} Half 2: {} \n".format(old_freq, freq))
                else:
                    old_freq = freq
                    # print("Half 2: {} \n".format(freq))

                # print("Elapsed: {} \n".format(freq))
                # nsent = csock.send(payload_in)
                # print("Sent {:d} bytes".format(nsent))
                buff = csock.recv(512)
                splitter += 1
                # buff = b''
            # time.sleep(0.5)
            # else:
                # print("No data \n")

            # print("Closing connection to client")
            # print("----------------------------")
            # csock.close()

    except AttributeError as ae:
        print("Error creating the socket: {}".format(ae))
    except socket.error as se:
        print("Exception on socket: {}".format(se))
    except KeyboardInterrupt:
        ssock.close()
    finally:
        print("Closing socket")
        ssock.close()


if __name__ == "__main__":
    main()