--a = string.gsub("T:209.6 E:0 W:7","(%w+)%s(%w+)%s(%w+)","%1");
--local a = string.gsub("T:209.6 E:0 W:7","(%w+)%s(%w+)%s(W+)","%1");
--print(a());

t = {}
s = "T:209.6 E:0 W:7"
for k, v in string.gmatch(s, "(%w+):(%w+)%S") do
	t[k]=v
end
for k, v in pairs(t) do
	print(string.format("[%s]=[%s]",k, v))
end
