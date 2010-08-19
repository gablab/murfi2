% psychoTest(dataName, roiName)
function psychoTest(dataName, roiName)

  if(nargin ~= 2)
    error('usage: psychoTest(dataName, roiName)');
  end
  
  DEBUG_smallScreen = true;
  
  oldSyncLevel = Screen('Preference', 'SkipSyncTests', 2);
  Screen('Preference','Verbosity',0);
  Screen('Preference', 'VisualDebugLevel', 0);

  DisableKeysForKbCheck([175]);
  ListenChar(2);
  
  myScr = max(Screen('Screens'));

  if(DEBUG_smallScreen)
    [w,rect] = Screen('OpenWindow',myScr,128,[0 100 800 600]);
  else
    [w,rect] = Screen('OpenWindow',myScr,128);
  end
  
  % start the infoclient
  try
    [stat host] = system('hostname');
    infoclient('start',15003,host(1:end-1),15002);
  catch 
    shutdown();
    error('failed to start the infoclient');
  end
  
  infoclient('add',dataName,roiName);

  DrawFormattedText(w,'waiting...','center','center');
  Screen('Flip',w);

  % run a loop forever
  bCont = true;
  while bCont
    
    newdat = infoclient('check');
    
    str = [];
    while(~isempty(newdat))
      str = [makeDataString(newdat(end)) '\n' str];
      %str = num2str(newdat(end).value);
      newdat(end) = [];            
    end
    if(~isempty(str))
      DrawFormattedText(w,str,'center','center');
      Screen('Flip',w);      
    end

    [keyIsDown, secs, keyCode] = KbCheck;
    if(keyIsDown)
      sKey = KbName(find(keyCode));

      if(~isempty(sKey))
	sKey = sKey(1);
	if(strcmp(sKey, 'q'))
	  bCont = false;
	end
      end
    end

    WaitSecs(0.01);
  end

  shutdown();
  
return;

function str = makeDataString(data)

  timestr = num2str(data.time);
  time_msec = str2num(timestr(end-5:end));
  val_msec = 10^6 * (data.value - fix(data.value));
  delay = time_msec - val_msec;
  
  str = sprintf('%s:%s tr=%d time=%s value=%2.6f delay=%ss', ...
		data.dataName, data.roiName, data.tr, ...
		timestr, data.value, num2str(delay/(10^6)));
  str = sprintf('%s: %s tr=%d time=%s value=%2.6f', ...
		data.dataName, ...
		data.roiName, ...
                data.tr, ...
		timestr, ...
		data.value);
		%num2str(delay/(10^6)));
		
  
return

function shutdown()
  sca;
  infoclient('stop');
return