program listDigl;

{$APPTYPE CONSOLE}

uses
  SysUtils;

Type
  node=^_node;
  _node=record
    x:integer;
    r:node;
  end;
  listD=object
    first,last:node;
    constructor listDNew;
    procedure pop;
    procedure push(_x:integer);
    function peek:integer;
  end;
constructor listD.listDNew;
begin
  new(first);
  last:=first;
end;

procedure listD.pop;
var
  temp:node;
begin
  if last<>first then
  begin
    temp:=last;
    last:=last^.r;
    dispose(temp);
  end;

end;

procedure listD.push(_x:integer);
var
  temp:node;
begin
  new(temp);
  temp^.r:=nil;
  temp^.x:=0;
  first^.x:=_x;
  first^.r:=temp;
  first:=temp;
end;

function listD.peek:integer;
begin
  peek:=last^.x;
end;

var
  obj:listD;
  i:integer;
begin
  obj.listDNew;
  for i:=1 to 100 do
    obj.push(i);
  for i:=1 to 100 do
  begin
    writeln(obj.peek);
    obj.pop;
  end;
readln;

  { TODO -oUser -cConsole Main : Insert code here }
end.
