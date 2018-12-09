import matplotlib.pyplot as plt
import math as m
def deg2rad(_deg):
	return ((_deg/180)*m.pi)
def rad2deg(_rad):
	return ((_rad/m.pi)*180)
sine1=[]
sine2=[]
sine3=[]
i=0
while(i < 360):
	sine1.append((m.sin(deg2rad(i))))
	sine2.append(m.sin(deg2rad(i+120)))
	sine3.append(m.sin(deg2rad(i+240)))
	i=i+1

print(sine1)
print(sine2)
print(sine3)
plt.plot(sine1)
plt.figure()
plt.plot(sine2)
plt.figure()
plt.plot(sine3)
plt.show()
