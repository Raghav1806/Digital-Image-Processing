function dispstat1(TXT,varargin)
% Prints overwritable message to the command line. 
% Options available:
% (1) 'init': It can overwrite previous outputs on the command line.
% (2) 'keepthis': The message would be persistent (non-overwritable).
% (3) 'keepprev': New message shall start with a new line.
% (4) 'timestamp': current time hh:mm:ss will be appended to the beginning.

keepthis = 0;
keepprev = 0;
timestamp = 0;
init = 0;

if ~isstr(TXT)
    return
end

persistent prevCharCnt;
if isempty(prevCharCnt)
    prevCharCnt = 0;
end

if nargin == 0
    return
elseif nargin > 1
    for i = 2:nargin
        eval([varargin{i-1} '=1;']);
    end
end

if isempty(TXT) && timestamp == 0
    return
end

if timestamp == 1
    c = clock;
    txtTimeStamp = sprintf('%02d:%02d:%02d', c(4), c(5), round(c(6)));
else
    txtTimeStamp = '';
end

if keepprev == 1
    prevCharCnt = 0;
end

TXT = strrep(TXT,'%','%%');
TXT = strrep(TXT,'\','\\');
TXT = [txtTimeStamp TXT '\n'];

fprintf([repmat('\b',1, prevCharCnt) TXT]);

nof_extra = length(strfind(TXT,'%%'));
nof_extra = nof_extra + length(strfind(TXT,'\\'));
nof_extra = nof_extra + length(strfind(TXT,'\n'));

prevCharCnt = length(TXT) - nof_extra; %-1 is for \n

if keepthis == 1
    prevCharCnt = 0;
end

end