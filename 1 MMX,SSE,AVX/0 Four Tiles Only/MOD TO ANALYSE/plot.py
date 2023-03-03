import matplotlib.pyplot as plt
import numpy as np

f0 = open('Result0.txt', 'r')
f1 = open('Result1.txt', 'r')
f2 = open('Result2.txt', 'r')
f3 = open('Result3.txt', 'r')

size = 100


count = np.array(range(1,size+1), float)
float_0_scene0 = np.zeros(size)
float_0_scene1 = np.zeros(size)
float_0_scene2 = np.zeros(size)
float_0_scene3 = np.zeros(size)
float_0_scene4 = np.zeros(size)

float_1_scene0 = np.zeros(size)
float_1_scene1 = np.zeros(size)
float_1_scene2 = np.zeros(size)
float_1_scene3 = np.zeros(size)
float_1_scene4 = np.zeros(size)

float_2_scene0 = np.zeros(size)
float_2_scene1 = np.zeros(size)
float_2_scene2 = np.zeros(size)
float_2_scene3 = np.zeros(size)
float_2_scene4 = np.zeros(size)

float_3_scene0 = np.zeros(size)
float_3_scene1 = np.zeros(size)
float_3_scene2 = np.zeros(size)
float_3_scene3 = np.zeros(size)
float_3_scene4 = np.zeros(size)









''' Preparation scene 0 '''
string = f0.readline()
string = f0.readline()
string = f1.readline()
string = f1.readline()
string = f2.readline()
string = f2.readline()
string = f3.readline()
string = f3.readline()

''' Get floats scene 0 '''
for i in range(size):
    string = f0.readline()
    float_0_scene0[i] = float(string)
    
    string = f1.readline()
    float_1_scene0[i] = float(string)

    string = f2.readline()
    float_2_scene0[i] = float(string)

    string = f3.readline()
    float_3_scene0[i] = float(string)

''' Plot scene 0 '''
plt.figure(0)
plt.plot(count, float_0_scene0[0:size], 'r', label='Original')
plt.plot(count, float_1_scene0[0:size], 'y', label='MMX')
plt.plot(count, float_2_scene0[0:size], 'g', label='SSE')
plt.plot(count, float_3_scene0[0:size], 'b', label='AVX')
plt.xlabel('Commands')
plt.ylabel('Time')
plt.title('Scene 0')
plt.legend()
plt.show()












''' Preparation scene 1 '''
string = f0.readline()
string = f1.readline()
string = f2.readline()
string = f3.readline()

''' Get floats scene 1 '''
for i in range(size):
    string = f0.readline()
    float_0_scene1[i] = float(string)
    
    string = f1.readline()
    float_1_scene1[i] = float(string)

    string = f2.readline()
    float_2_scene1[i] = float(string)

    string = f3.readline()
    float_3_scene1[i] = float(string)

''' Plot scene 1 '''
plt.figure(1)
plt.plot(count, float_0_scene1[0:size], 'r',label='Original')
plt.plot(count, float_1_scene1[0:size], 'y',label='MMX')
plt.plot(count, float_2_scene1[0:size], 'g',label='SSE')
plt.plot(count, float_3_scene1[0:size], 'b',label='AVX')
plt.xlabel('Commands')
plt.ylabel('Time')
plt.title('Scene 1')
plt.legend()
plt.show()















''' Preparation scene 2 '''
string = f0.readline()
string = f1.readline()
string = f2.readline()
string = f3.readline()

''' Get floats scene 2 '''
for i in range(size):
    string = f0.readline()
    float_0_scene2[i] = float(string)
    
    string = f1.readline()
    float_1_scene2[i] = float(string)

    string = f2.readline()
    float_2_scene2[i] = float(string)

    string = f3.readline()
    float_3_scene2[i] = float(string)

''' Plot scene 2 '''
plt.figure(2)
plt.plot(count, float_0_scene2[0:size], 'r',label='Original')
plt.plot(count, float_1_scene2[0:size], 'y',label='MMX')
plt.plot(count, float_2_scene2[0:size], 'g',label='SSE')
plt.plot(count, float_3_scene2[0:size], 'b',label='AVX')
plt.xlabel('Commands')
plt.ylabel('Time')
plt.title('Scene 2')
plt.legend()
plt.show()











''' Preparation scene 3 '''
string = f0.readline()
string = f1.readline()
string = f2.readline()
string = f3.readline()

''' Get floats scene 3 '''
for i in range(size):
    string = f0.readline()
    float_0_scene3[i] = float(string)
    print(float_0_scene3[i])
    
    string = f1.readline()
    float_1_scene3[i] = float(string)

    string = f2.readline()
    float_2_scene3[i] = float(string)

    string = f3.readline()
    float_3_scene3[i] = float(string)

''' Plot scene 3 '''
plt.figure(3)
plt.plot(count, float_0_scene3[0:size], 'r',label='Original')
plt.plot(count, float_1_scene3[0:size], 'y',label='MMX')
plt.plot(count, float_2_scene3[0:size], 'g',label='SSE')
plt.plot(count, float_3_scene3[0:size], 'b',label='AVX')
plt.xlabel('Commands')
plt.ylabel('Time')
plt.title('Scene 3')
plt.legend()
plt.show()















''' Preparation scene 4 '''
string = f0.readline()
string = f1.readline()
string = f2.readline()
string = f3.readline()

''' Get floats scene 4 '''
for i in range(size):
    string = f0.readline()
    float_0_scene4[i] = float(string)
    
    string = f1.readline()
    float_1_scene4[i] = float(string)

    string = f2.readline()
    float_2_scene4[i] = float(string)

    string = f3.readline()
    float_3_scene4[i] = float(string)

''' Plot scene 4 '''
plt.figure(4)
plt.plot(count, float_0_scene4[0:size], 'r',label='Original')
plt.plot(count, float_1_scene4[0:size], 'y',label='MMX')
plt.plot(count, float_2_scene4[0:size], 'g',label='SSE')
plt.plot(count, float_3_scene4[0:size], 'b',label='AVX')
plt.xlabel('Commands')
plt.ylabel('Time')
plt.title('Scene 4')
plt.legend()
plt.show()







f0.close()
f1.close()
f2.close()
f3.close()
