from datetime import datetime
import csv
import os.path


def event_handler_write(writer, utc_timestamps, calc_freq_both, calc_freq_1, calc_freq_2):
    for i in range(len(calc_freq_both)):
        writer.writerow([utc_timestamps[i], calc_freq_both[i], calc_freq_1[i], calc_freq_2[i]])
    print("Wrote {} rows\n".format(len(calc_freq_both)))


# utc_posix = datetime.now().timestamp()
# utc_timestamp = datetime.utcfromtimestamp(utc_posix).strftime('%Y-%m-%d %H:%M:%S.%f%z')

# print(utc_timestamp)



# with open(outfile, 'ab') as csvfile:
#     txt = '%s,%.6f,%.6f,%.6f\n' % ()
#     csvfile.write(txt)



outfile = 'data_.csv'
file_count = 0
file_check = True

while file_check:
    file_count += 1
    file_name = outfile.split("_")
    outfile = file_name[0] + "_" + str(file_count) + ".csv"
    file_check = os.path.isfile(outfile)


print("Writing to: {} \n".format(outfile))


csvfile = open(outfile, 'w')
writer = csv.writer(csvfile)
writer.writerow(["Timestamp UTC", "Freq Average Full", "Freq Average Half 1", "Freq Average Half 2"])
csvfile.close()


csvfile = open(outfile, 'a')
writer = csv.writer(csvfile)




utc_posix = datetime.now().timestamp()
utc_timestamp = datetime.utcfromtimestamp(utc_posix).strftime('%Y-%m-%d %H:%M:%S.%f%z')

list_times = []
list1 = []
list2 = []
list3 = []

for i in range(1,100):
    utc_posix = datetime.now().timestamp()
    utc_timestamp = datetime.utcfromtimestamp(utc_posix).strftime('%Y-%m-%d %H:%M:%S.%f%z')
    list_times.append(utc_timestamp)
    list1.append(i)
    list2.append(i*0.5)
    list3.append(i/5000)
    
event_handler_write(writer, list_times, list1, list2, list3)






csvfile.close()