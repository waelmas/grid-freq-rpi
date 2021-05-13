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
from datetime import datetime
import csv
import os.path

""" This class defines a C-like struct """
class Payload(Structure):
    _fields_ = [("id", c_uint32),
                ("counter", c_uint32),
                ("period", c_ulonglong)]

# csv batch writer
def event_handler_write(writer, batch_size, utc_timestamps, calc_freq_both, calc_freq_1, calc_freq_2, max_freq_list, min_freq_list, skipped_list):
    for i in range(batch_size):
        if calc_freq_both[i] != None:
            # print([utc_timestamps[i], calc_freq_both[i], calc_freq_1[i], calc_freq_2[i]])
            writer.writerow([utc_timestamps[i], calc_freq_both[i], calc_freq_1[i], calc_freq_2[i], max_freq_list[i], min_freq_list[i], skipped_list[i]])
    print("Wrote {} rows\n".format(len(calc_freq_both)))


def main():
    outfile = 'data/data_.csv'
    file_count = 0
    file_check = True

    #  Using this to create a new file if older versions exist and keep linking of sequence
    while file_check:
        file_count += 1
        file_name = outfile.split("_")
        outfile = file_name[0] + "_" + str(file_count) + ".csv"
        file_check = os.path.isfile(outfile)


    print("Writing to: {} \n".format(outfile))


    csvfile = open(outfile, 'w')
    writer = csv.writer(csvfile)
    writer.writerow(["Timestamp UTC", "Freq Average Full", "Freq Average Half 1", "Freq Average Half 2", "Max Freq", "Min Freq", "Skipped Count"])
    csvfile.close()

    PORT = 2300
    server_addr = ('localhost', PORT)
    ssock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Socket created")

    csvfile = open(outfile, 'a')
    writer = csv.writer(csvfile)

    try:
        # bind the server socket and listen
        ssock.bind(server_addr)
        print("Bind done")
        ssock.listen(3)
        print("Server listening on port {:d}".format(PORT))

        
        splitter = 0
        old_freq = 0

        batch_size = 450

        # To optimize Python garbage collector, instead of appending to lists we init them then change values
        calc_freq_both = [None] * (batch_size + 1)
        calc_freq_1 = [None] * (batch_size + 1)
        calc_freq_2 = [None] * (batch_size + 1)
        list_times = [None] * (batch_size + 1)
        max_freq_list = [None] * (batch_size + 1)
        min_freq_list = [None] * (batch_size + 1)
        skipped_list = [None] * (batch_size + 1)

        # We cannot use the garbage collctor trick here as we cannot know how large this could be now
        # In the future we can use an estimation with a safety and test
        #  starting with one value to avoid division by zero when measurement is started or stopped
        freq_list_1 = [50.0]
        freq_list_2 = [50.0]

        rows_count = 1
        total_time = 0
        skipped = 0

        seconds_counter = 0




        while True:
            csock, client_address = ssock.accept()


            buff = csock.recv(512)

            while buff:

                payload_in = Payload.from_buffer_copy(buff)

                period1 = payload_in.period
                seconds_p = (period1 * 2) * 0.000000001
                freq = 1 / seconds_p

                total_time = total_time + (period1 * 0.000000001)

                if (splitter % 2) == 0:
                    # adding try except as the first time freq_average_1 will not have been assigned
                    if seconds_counter >= 30:
                        # Exclude partial measurement if it's a lot different from the previous second average
                        if freq < freq_average_1-0.1 or freq > freq_average_1+0.1:
                            # Wanna exclude this but also count them
                            skipped += 1
                        else:
                            freq_list_1.append(freq)
                    else:
                        freq_list_1.append(freq)
                    
                else:
                    if seconds_counter >= 30:
                        if freq < freq_average_2-0.1 or freq > freq_average_2+0.1:
                            # Wanna exclude this but also count them
                            skipped += 1
                        else:                            
                            freq_list_2.append(freq)
                            old_freq = freq
                    else:
                        freq_list_2.append(freq)
                        old_freq = freq



                buff = csock.recv(512)
                splitter += 1
                if total_time >= 1.0000000001:
                    # get timestamp before other operations to be as close as possible to the source time
                    utc_posix = datetime.now().timestamp()
                    utc_timestamp = datetime.utcfromtimestamp(utc_posix).strftime('%Y-%m-%d %H:%M:%S.%f%z')

                    freq_average_2 = sum(freq_list_2) / len(freq_list_2)
                    freq_average_1 = sum(freq_list_1) / len(freq_list_1)

                    max_freq = max(max(freq_list_1), max(freq_list_2))
                    min_freq = min(min(freq_list_1), min(freq_list_2))

                    
                    freq_average_both = (sum(freq_list_1) + sum(freq_list_2)) / (len(freq_list_1) + len(freq_list_2))
                    # print("Average 1: {} Average 2: {} \n".format(freq_average_1, freq_average_2))
                    print("Totoal Average: {} \n".format(freq_average_both))
                    # print("Moment reading: {}".format(freq))
                    # print("Time: {} \n".format(total_time))

                    
                    list_times[rows_count] = utc_timestamp
                    calc_freq_both[rows_count] = freq_average_both
                    calc_freq_1[rows_count] = freq_average_1
                    calc_freq_2[rows_count] = freq_average_2
                    max_freq_list[rows_count] = max_freq
                    min_freq_list[rows_count] = min_freq
                    skipped_list[rows_count] = skipped

                    freq_list_1 = [50.0]
                    freq_list_2 = [50.0]
                    skipped = 0

                    total_time = 0

                    seconds_counter += 1

                    
                    if rows_count >= batch_size:
                        event_handler_write(writer, batch_size, list_times, calc_freq_both, calc_freq_1, calc_freq_2, max_freq_list, min_freq_list, skipped_list)
                        rows_count = 1
                        list_times = [None] * (batch_size + 1)
                        calc_freq_both = [None] * (batch_size + 1)
                        calc_freq_1 = [None] * (batch_size + 1)
                        calc_freq_2 = [None] * (batch_size + 1)
                        max_freq_list = [None] * (batch_size + 1)
                        min_freq_list = [None] * (batch_size + 1)
                        skipped_list = [None] * (batch_size + 1)

                    rows_count += 1

            csvfile.close()

    except AttributeError as ae:
        print("Error creating the socket: {}".format(ae))
        csvfile.close()
    except socket.error as se:
        print("Exception on socket: {}".format(se))
        csvfile.close()
    except KeyboardInterrupt:
        ssock.close()
        csvfile.close()
        
    finally:
        print("Closing socket")
        ssock.close()
        csvfile.close()


if __name__ == "__main__":
    main()