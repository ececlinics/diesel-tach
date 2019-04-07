%% import4mat.m
% *Import* CSV data and for*(4)mat* as cells.
% 
% Updated 3-14-19
% Author: Damon Boorstein

clear all
close all
clc

folders = ["04_Ford_F250","12_Jeep_Wrangler","98_Dodge_Ram","98_GMC_2500"];
F250 = [];
Wrangler = [];
Ram = [];
GMC = [];

for i = 1:length(folders)
    listing = dir(folders(i));
    for j = 3:length(listing)
        fulldir = strcat(listing(i).folder,"\",listing(j).name);
        [filepath,name,ext] = fileparts(fulldir);
        clear filepath
        if ext == ".csv"
            switch folders(i)
                case folders(1)
                    importF250 = csvread(fulldir,4,1);
                    headers = genheaders(importF250,folders(i),name);
                    F250 = [F250 [headers;importF250]];
                case folders(2)
                    importWrangler = csvread(fulldir,3,0);
                    headers = genheaders(importWrangler,folders(i),name);
                    Wrangler = [Wrangler [headers;importWrangler]];
                case folders(3)
                    importRam = csvread(fulldir,4,1);
                    headers = genheaders(importRam,folders(i),name);
                    Ram = [Ram [headers;importRam]];
                case folders(4)
                    import2500 = csvread(fulldir,4,1);
                    headers = genheaders(import2500,folders(i),name);
                    GMC = [GMC [headers;import2500]];
                otherwise
                    errunkfile = "unknown file.";
                    error(errormsg);
            end
        else
            warnbadext = "bad extension = ";
            warnmsg = strcat(warnbadext,ext);
            warning(warnmsg);
        end
    end
end

clearvars -except F250 Wrangler Ram GMC

function headers = genheaders(S,folderName,filename)
    name1 = strrep(filename,'_',' ');
    match = strsplit(folderName,'_');
    name2 = strtrim(erase(name1,match));
    [~,cols] = size(S);
    headers = repmat(name2,1,cols);
end