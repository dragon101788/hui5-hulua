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

function sleep(n)
   local t0 = os.clock()
   while os.clock() - t0 <= n do end
end

function proc_M109(mcu)

	while true
	do
		local ret = mcu:read();
		print("recv:",ret);
		if (ret == "ok") then
			return;
		end
		
		local t = {}
		for k, v in string.gmatch(ret, "(%w+):(%d+)") do
		    t[k]=v
		end
		for k, v in pairs(t) do
		    print(string.format("[%s][%s]",k, v))
		end
		wd_jct:command(string.format("%s",t["T"]));
	end
end


function proc_mcu_read(mcu)

	for ret in mcu:lines() do
		--print("recv:",ret);
		if (ret == "ok") then
			return;
		else
			print("return ",ret);
		end
	end
end

co = coroutine.create(function (path)

    print("hello");
    file = io.open(path,"r");

    if file == nil then
        print("can't open file",path);
        return 5;
    end
		
		os.execute("stty -F /dev/ttyS0 raw speed 115200");
		mcu = io.open("/dev/ttyS0","r+");
		if file == nil then
        cmd:command(string.format("can't connect MCU"));
        print(string.format("can't connect MCU"));
        return 5;
    end
    
    --mcuinfo = mcu:read();
    --print(mcuinfo);
    
    i=0;
    for lbuf in file:lines() do
        i=i+1;

        n = string.find(lbuf,";");
        if (n == nil) or ( n>1 ) then
            local line = string.sub(lbuf,0,n);
            
            if(line ~= "") then
            	line = string.gsub(line, "^%s*;*(.-)%s*;*$", "%1")
	            key = split(line," ")[1];
	            value = string.sub(line,string.len(key)+1);
	            value = string.gsub(value, "^%s*;*(.-)%s*;*$", "%1")
	            print(string.format("[%s]:[%s]:[%s]",line,key,value));
							
							cmd:command(line);
							--mcu:write(line .. "\r\n");
							--mcu:flush();
							local s = string.format("echo %s >/dev/ttyS0",line);
							print("send[",s,"]");
							os.execute(s);
							proc_mcu_read(mcu);
							--if (key == "M109")
							--then
							--		proc_M109(mcu);
							--else
							--	proc_mcu_read(mcu);
							--end
            end
            
            
						
        end
    end

    file:close();
end)




function main()
	print("this is main");
	coroutine.resume(co,"test.gcode")
end	
