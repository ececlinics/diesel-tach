%low pass test
clear
wr_sc=csvread('98_2500_AEM_Rev1000_Ch2_3.csv',4,0);

lpFilt = designfilt('lowpassiir','FilterOrder',8, ...
         'PassbandFrequency',35e3,'PassbandRipple',0.2, ...
         'SampleRate',400e3);

%a = [1 -0.2];
%b = 1;
%a_wr_sc = filter(b,a,wr_sc,[],2);
%figure()
%t = 0:length(wr_sc)-1;
%plot(t,a_wr_sc(1,:),'--',t,a_wr_sc(2,:),'-')
%legend('Original Data','Filtered Data');s

%plot(a_wr_sc(:,1),a_wr_sc(:,2));
%subplot(2,2,1);     
f_wr_sc = filter(lpFilt,wr_sc);
m_wr_sc = medfilt1(wr_sc,1);
subplot(2,2,2);
plot(wr_sc(:,1),wr_sc(:,2));
axis([0 0.05 -5000 17000])
grid on 
grid minor
subplot(2,2,3);
plot(f_wr_sc(:,1),f_wr_sc(:,2));
axis([0 0.05 -5000 17000])
grid on
grid minor
subplot(2,2,4);
plot(m_wr_sc(:,1),m_wr_sc(:,2));
axis([0 0.05 -5000 17000])
grid on
grid minor


