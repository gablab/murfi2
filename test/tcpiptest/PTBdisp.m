%oldSyncLevel = Screen('Preference', 'SkipSyncTests', 2);
%oldLevel = Screen('Preference','Verbosity',4);

myScr = max(Screen('Screens'));

[w,rect] = Screen('OpenWindow',myScr,128,[0 0 500 500]);


for i = 1:5
    
text = TCPget('18.188.71.203',15000);

DrawFormattedText(w,text,'center','center');
Screen('Flip',w);
WaitSecs(2);
end

sca

