Array=csvread('12_Wrangler_Solenoid_Core_Idle_1.csv',2,0);
Array2=csvread('12_Wrangler_AEM_Idle_1.csv',2,0);
col1 = Array(:, 1);
col2 = Array(:, 2);
col3 = Array(:, 3);
col4 = Array2(:, 1);
col5 = Array2(:, 2);
col6 = Array2(:, 3);
figure()
subplot(2,1,1)
plot(col1, col2)
findpeaks(col2,'MinPeakDistance',100,'Threshold',.05)
hold on;
plot(col4, col5)
%findpeaks(col5,'MinPeakDistance',50,'Threshold',.5)
%xlim([0 0.015])
ylim([1.7,2.4])
grid on
grid minor
subplot(2,1,2)
plot(col1,col3);
hold on;
%findpeaks(col3,'MinPeakDistance',100,'Threshold',1.5);
plot(col4,col6);
%findpeaks(col6,'MinPeakDistance',5,'Threshold',1);
ylim([-5,15])
grid on
grid minor