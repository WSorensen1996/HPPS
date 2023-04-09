import random
from time import sleep
import matplotlib.pyplot as plt
from scipy.spatial.distance import euclidean
import pandas as pd 
# Define the length of each sub-list in the existing list

# stockPath = '/Users/williamsorensen/Desktop/KO.csv'
stockPath = '/Users/williamsorensen/Desktop/TSLA_1min_sample.txt'

data = pd.read_csv(stockPath)
data = data['Close'].pct_change().values[1:]


existing_list_length = int(len(data)*3/5) # månedes tid
existing_list = data[:existing_list_length]


sub_list_length = 30 # 30 time i 5 min
predict_size = 1
# Define the existing list as a flattened list
# existing_list = [random.uniform(0, 0.05) for _ in range(existing_list_length)]
# input_list = existing_list[-sub_list_length:]
input_list = list(data[existing_list_length:existing_list_length+sub_list_length])

subData = data[existing_list_length+sub_list_length:]

delta = 0.0875
SUCCES = 0
FAIL = 0
# while True:
for idx,new_float in enumerate(subData): 
    # Generate a new random float to add to the input list and remove the oldest float from the beginning
    # new_float = random.uniform(0, 0.05)
    input_list.append(new_float)
    if len(input_list)>sub_list_length: 
        input_list.pop(0)

    distances = []
    sublists = []
    for i in range(existing_list_length-sub_list_length):
        start = i
        end = i + sub_list_length
        sub_list = existing_list[start:end]

        _distance = euclidean(input_list, sub_list)
        sublists.append(existing_list[start:end+predict_size])
        distances.append(_distance)

    min_dist = min(distances)
    # print("Min: ", min_dist)
    # if min_dist < delta:
    closest_index = distances.index(min_dist)

    # plt.plot([x for x in range(sub_list_length)], sublists[closest_index][:sub_list_length],'-o', label="Closest interval")
    # plt.plot([x for x in range(sub_list_length-1, sub_list_length+predict_size )], sublists[closest_index][sub_list_length-1:],'-o', label="Predict interval")

    try: 
        input_list.append(subData[idx+1])
    except: 
        break

    # plt.plot([x for x in range(len(input_list))], input_list, label="Input list")
    
    P_fst,P_snd = sublists[closest_index][sub_list_length-1:]
    fst,snd = input_list[-2:]
    input_list = input_list[:-1]

    if P_fst<P_snd and fst<snd : 
        SUCCES +=1
    elif P_fst>P_snd and fst>snd : 
        SUCCES +=1
    else: 
        FAIL +=1

    # plt.show()


print(f'SUCCES: {SUCCES}, FAIL: {FAIL}')

## Total: 4237
## ALL APPLE:           SUCCES: 2342, FAIL: 1894 = 1,23
## ALL MSFT :           SUCCES: 3382, FAIL: 2755












## APPLE test: 
## Delta: 0.0875, SUCCES: 1114, FAIL: 978 = 1,139
## Delta: 0.085, SUCCES: 988, FAIL: 873 = 1,13
## Delta: 0.0825, SUCCES: 867, FAIL: 755 = 1,14
## Delta: 0.08 , SUCCES: 711, FAIL: 618 = 
## Delta: 0.0775, SUCCES: 507, FAIL: 474
## Delta: 0.075, SUCCES: 341, FAIL: 340

