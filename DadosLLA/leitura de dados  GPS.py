# -*- coding: utf-8 -*-
"""
Created on Fri Oct  6 17:17:03 2023

@author: renat
"""

import numpy as np
from numpy import random
import matplotlib.pyplot as plt

t = np.arange(0,40,0.05);
tgps = t;
#tgps = 0:0.5:100;
pi = np.pi();
wp = pi/2*0.25;
r0 = 10;

#pr = r0*[cos(wp*tgps);sin(wp*tgps);zeros(1,length(tgps))];
#vr = r0*wp*[-sin(wp*tgps);cos(wp*tgps);zeros(1,length(tgps))];
#ar = r0*wp*wp*[-cos(wp*t);-sin(wp*t);zeros(1,length(t))]; // Sem YPR
#ar = [0*ones(1,length(t));r0*wp/wp*ones(1,length(t));0*ones(1,length(t))];
#wr = [0;0;wp]*ones(1,length(t));
#YPRr = [-asin(wp*t);zeros(1,length(t));zeros(1,length(t))];


pr = np.array([2+0.025*tgps**2,np.zeros(len(tgps)),np.zeros(len(tgps))]);
vr = np.array([0.05*tgps,np.zeros(len(tgps)),np.zeros(len(tgps))]);
ar = np.array([0.05*np.ones(len(t)),np.zeros(len(t)),np.zeros(len(t))]);
#wr = [zeros(1,length(t));zeros(1,length(t));zeros(1,length(t))];
#YPRr = [pi/2*ones(1,length(t));zeros(1,length(t));zeros(1,length(t))];


#incertezas: a = 0.003; w = 0.002; YPR = 0.015*pi/180

pm = pr+random.normal(scale = 1e-3,size=pr.shape);
vm = vr+random.normal(scale = 4e-3,size=vr.shape);
am = ar+random.normal(scale = 0.03,size=vr.shape);
#wm = wr+rand(wr,"normal")*0.02;
#YPRm = YPRr+rand(YPRr,"normal")*(0.15*pi/180);

n = 9;

f0 = np.eye(n);

q = np.diag([1e-4,1e-4,1e-4,4e-4,4e-4,4e-4,1e-3,1e-3,1e-3]);
     
r = np.diag([1e-6,1e-6,1e-6,4e-5,4e-5,4e-5,1e-2,1e-2,1e-2]);

p0 = q.copy;

k = 1;

x = np.array([pm[:,0],vm[:,0],am[:,0]]).reshape((9,1));

x1 = x.copy;

p1 = p0.copy;
    
for i in range(len(t)):
    #k = i;
    dt = t[i+1]-t[i];
    if((t(i) == tgps(k))): #//sinal de GPS disponível
        #//if ((t(i) < 40 ) || (t(i) > 70 ))
        x = np.array([pm[:,0],vm[:,0],am[:,0]]).reshape((9,1));
        #//end
        k += 1;
    else: #// sinal de GPS indisponível
        x = np.array([x1[1:6,i],am[:,i]]).reshape((9,1));
    
    y[:,i] = x.copy;
    
    #Definições do sistema
    p1[1:6,1:6] += eye(6)*100*(t(i)-t(k));
    h = eye(9);
    f = f0;
    f[1:6,4:9] += eye(6)*dt; #//Velocidade
    #//f(1:3,7:9) = f(1:3,7:9) + eye(n-6,n-6)*dt*dt/2; //Aceleração
    g = zeros(n);
    #//mprintf("%d\n",i)
    
    [x1(:,i+1),p1,x,p]=kalm(y(:,i),x1(:,i),p1,f,g,h,q,r);
end

pe = x1(1:3,:);
ve = x1(4:6,:);
ae = x1(7:9,:);

figure;
plot(t,pe);

figure;
plot(t,ve);


figure;
plot(pe(1,1000:2000),pe(2,1000:2000));

figure;
plot(ve(1,1000:2000),ve(2,1000:2000));
