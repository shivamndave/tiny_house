#This is some Python code that generates real time dummy data to be pushed to server
#This data meant to model a realistic situation of when a power surge occurs
#uses fourier series to model in a realistic manner
#it also produces a image of the fuction to see if the user is satisfied by the scale of the image
#scaling can be done by editing parameters of the fs(x) function

import numpy as np
import matplotlib.pyplot as plt
import numpy
from astropy.io import ascii
%matplotlib inline 
def fs(x):
    sum1 = 0 
    for n in np.arange(1,100):
        L = 15.0
        n = 2.0*n-1.0
        sum1 = (1/n)*np.sin(n*1.3*np.pi*x/L)+sum1
    return .35*(4/np.pi)*sum1+59.55
     
def fs1(x):
    return np.exp((x-15.0)/28.0)+58.17+.035*np.sin(x)
x1 = np.arange(0,34,.08333)
y1 = fs(x1)
plt.title("Dummy Data")
plt.xlabel("minutes")
plt.ylabel("HZ")
plt.plot(x1,np.piecewise(x1,[x1<=15.0,x1>15.0,x1>30.3],[fs,fs1,fs]))
plt.grid(b=True, which='major', color='g', linestyle='--')
plt.savefig("Test2")
Hz = np.piecewise(x1,[x1<=11.0,x1>11.0,x1>30.3], [fs,fs1,fs])
ascii.write([x1,Hz], 'DummyData1.dat', names = ["Hz", "Minutes"])