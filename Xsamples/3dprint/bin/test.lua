function split(str, split_char)
	local sub_str_tab = {};

	while (true) do
		local pos = string.find(str, split_char);
		if (not pos) then
			local size_t = #sub_str_tab
			table.insert(sub_str_tab,size_t+1,str);
			break;
		end

		local sub_str = string.sub(str, 1, pos - 1);
		local size_t = #sub_str_tab
		table.insert(sub_str_tab,size_t+1,sub_str);
		local t = string.len(str);
		str = string.sub(str, pos + 1, t);
	end
	return sub_str_tab;
end


function trim (s) 
	  return (string.gsub(s, "^%s*(.-)%s*$*;", "%1")) 
end 


co = coroutine.create(function (path)

	print("hello");
	file = io.open(path,"r");

	if file == nil then
		print("can't open file",path);
		return 5;
	end

	i=0;
	for lbuf in file:lines() do
		i=i+1;
		
		n = string.find(lbuf,";");
		if (n == nil) or ( n>1 ) then
			line = string.sub(lbuf,0,n)
			line = string.gsub(line, "^%s*;*(.-)%s*;*$", "%1")
			key = split(line," ")[1];
			value = string.sub(line,string.len(key)+1);
			value = string.gsub(value, "^%s*;*(.-)%s*;*$", "%1")
			print(string.format("[%s]:[%s]:[%s]",line,key,value));
			
		end
		if key == "M114" then

		end

	end

	file:close();
end)


coroutine.resume(co,"test.gcode")
