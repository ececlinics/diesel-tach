%filter settings
window=180;

%read csv
raw = csvread('12_Wrangler_AEM_Idle600_2.csv',23,0);
raw(:,2)=raw(:,2)/1000;

%digitize
rawbinary = raw;
for i = 1:length(rawbinary(:,2))
    if(rawbinary(i,2)>1.6)
        rawbinary(i,2)=1;
    else
       rawbinary(i,2)=0;
    end   
end

%custom filter
filtbinary = rawbinary;
filtbinary(:,2) = 0;
history = zeros(1,window);
windex = 1;
for i = 1:length(rawbinary(:,2))
    %Populate Window
    history(windex) = rawbinary(i,2);
    
    %output
    if(sum(history)>(window/2))
        filtbinary(i,2)=1;
    else
        filtbinary(i,2)=0;
    end
    
    %Window Index
    if (windex==window)
        windex=1;
    else
        windex = windex+1;
    end
end

%plot
figure(1);
subplot(3,1,1);
plot(raw(:,1),raw(:,2));

subplot(3,1,2);
plot(rawbinary(:,1),rawbinary(:,2));
ylim([-.2 1.2]);

subplot(3,1,3);
plot(filtbinary(:,1),filtbinary(:,2));
ylim([-.2 1.2]);


